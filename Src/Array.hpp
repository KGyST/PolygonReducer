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
		//typedef GS::RandomContainerIterator<GS::Array<Type>>		 Iterator;

		Array(const Type* const*);
		Array(GS::Array<Type>) {} ;
		Array() {} ;
		~Array() {};
		Type* ToNeigs() const;

		Array<Type> &Slice(GS::UIndex start, GS::UIndex end) const;
		//TODO Sort() 

		//Iterator begin();
		//Iterator end();
	};
}

//---------------------------------------------------------------------------

template <class Type>
S::Array<Type>::Array(const Type* const* p_neigs)
{
	UInt32 nSel = BMGetHandleSize((GSHandle)p_neigs) / sizeof(Type);

	//IncreaseCapacity(nSel);

	Type _an;

	for (UINT i = 0; i < nSel; ++i) {
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
Type* S::Array<Type>::ToNeigs() const
{
    Type* neigs = reinterpret_cast<Type*> (BMAllocateHandle(this->GetSize(), ALLOCATE_ACTION, 0));
    if (neigs != NULL)
    {
        for (UINT i = 0; i < this->GetSize(); i++)
        {
            neigs[i] = (Type) (*this)[i];
        }
    }

    return neigs;
}

//template <class Type>
//S::Array<Type>::Iterator S::Array<Type>::begin()
//{
//	//return GS::RandomContainerIterator<S::Array<Type>>(*this, 0);
//	return GS::Array<Type>.Begin();
//}

#endif // S_ARRAY_HPP