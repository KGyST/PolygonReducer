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
		Array(Type**);
		~Array() {};
		Type** ToNeigs() const;

		Array<Type> &Slice(GS::UIndex start, GS::UIndex end) const;
	};
}

template <class Type>
S::Array<Type>::Array(Type** p_neigs)
{
	UInt32 nSel = BMGetHandleSize((GSHandle)p_neigs) / sizeof(Type);

	//IncreaseCapacity(nSel);

	Type _an;

	for (UINT i = 0; i < nSel; ++i) {
		_an = (*p_neigs)[i];

		this->Push(_an);
	}
}

#endif // !S_ARRAY_HPP