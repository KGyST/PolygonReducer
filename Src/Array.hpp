#pragma once

#ifndef S_ARRAY_HPP
#define S_ARRAY_HPP

#define ACExtension
#include "ACAPinc.h"					// also includes APIdefs.h

namespace S {
	template <class Type>
	class Array : public GS::Array<Type>
	{
	public:
		//typedef GS::RandomContainerIterator<typename Array<Type>>		 Iterator;

		Array(const Type* const* p_neigs, UINT p_startIdx = 0);
		Array(GS::Array<Type>) {} ;
		Array() {} ;
		~Array() {};
		Type** ToNeigs() const;

		Array<Type> &Slice(GS::UIndex start, GS::UIndex end) const;
		//Array<Type>* Sort(void (*funcSort)());

		typename GS::Array<Type>::ConstIterator begin(void);
		typename GS::Array<Type>::ConstIterator end(void);
	};
}

//---------------------------------------------------------------------------

template <class Type>
S::Array<Type>::Array(const Type* const* p_neigs, UINT p_startIdx)
{
	UInt32 nSel = BMGetHandleSize((GSHandle)p_neigs) / sizeof(Type);
	UINT startIdx = p_startIdx <= nSel ? p_startIdx : nSel;
	
	//IncreaseCapacity(nSel);

	Type _an;

	for (UINT i = startIdx; i < nSel; ++i) {
		_an = (*p_neigs)[i];

		this->Push(_an);
	}
}

//template <class Type>
//S::Array<Type>::Array(GS::Array<Type> a)
//{
//	S::Array<Type> *_ = &((S::Array<Type>)a);
//	this = _;
//}

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
inline typename GS::Array<Type>::ConstIterator begin(void)
{
	return GS::Array<Type>::ConstIterator(*this, 0);
}

template <class Type>
inline typename GS::Array<Type>::ConstIterator end(void)
{
	return GS::Array<Type>::ConstIterator(*this, size);
}

#endif // S_ARRAY_HPP