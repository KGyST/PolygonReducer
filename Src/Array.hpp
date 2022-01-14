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
		//typedef ConstRandomContainerIterator<S::Array>  ConstIterator;

		//typedef RandomContainerReverseIterator<S::Array>		ReverseIterator;
		//typedef ConstRandomContainerReverseIterator<S::Array>	ConstReverseIterator;

		Array(const Type* const*);														//TODO const Type**
		~Array() {};
		Type** ToNeigs() const;

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
//S::Array<Type>::Iterator S::Array<Type>::begin()
//{
//	//return GS::RandomContainerIterator<S::Array<Type>>(*this, 0);
//	return GS::Array<Type>.Begin();
//}

#endif // !S_ARRAY_HPP