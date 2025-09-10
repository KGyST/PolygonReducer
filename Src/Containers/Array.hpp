#pragma once

#ifndef S_ARRAY_HPP
#define S_ARRAY_HPP

#define ACExtension
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h

namespace S {
	template <class Type>
	class Array : public GS::Array<Type>
	{
	public:
		//typedef GS::RandomContainerIterator<typename Array<Type>>		 Iterator;

		Array(const Type* const* p_neigs, UINT p_startIdx = 0);
		//TODO Array(const Type* const* p_neigs, bool (*funcFilter)(inT), outT(*funcConverter)(inT));
		Array(GS::Array<Type>) {} ;
		//Array(S::Array<Type>) {} ;
		Array() {} ;
		~Array() {};
		Type** ToNeigs() const;

		Array<Type> &Slice(GS::UIndex start, GS::UIndex end) const;
		void Sort(GS::UIndex start, GS::UIndex end, bool (*funcSort)());

		typename S::Array<Type>::ConstIterator begin(void);
		typename S::Array<Type>::ConstIterator end(void);
	};
}

//---------------------------------------------------------------------------

template <class Type>
S::Array<Type>::Array(const Type* const* p_neigs, UINT p_startIdx)
{
	UInt32 nSel = BMGetHandleSize((GSHandle)p_neigs) / sizeof(Type);
	UINT startIdx = p_startIdx <= nSel ? p_startIdx : nSel;
	
	SetCapacity(nSel);

	Type _an;

	for (UINT i = startIdx; i < nSel; ++i) {
		_an = (*p_neigs)[i];

		this->Push(_an);
	}
}

template <class Type>
Type** S::Array<Type>::ToNeigs() const
{
	UINT _size = this->GetSize();
    Type** neigs = reinterpret_cast<Type**> (BMAllocateHandle(_size * sizeof(Type), ALLOCATE_CLEAR, 0));
    if (neigs != NULL)
    {
        for (UINT i = 0; i < this->GetSize(); i++)
        {
            (*neigs)[i] = (Type)(*this)[i];
        }
    }

    return neigs;
}

template <class Type>
inline typename S::Array<Type>::ConstIterator S::Array<Type>::begin(void)
{
	return ConstIterator(*this, 0);
}

template <class Type>
inline typename S::Array<Type>::ConstIterator S::Array<Type>::end(void)
{
	return ConstIterator(*this, this->GetSize() );
}


#endif // S_ARRAY_HPP