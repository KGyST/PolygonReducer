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
		Array(const Type** p_neigs);
		Type** ToNeigs() const;

		Array &Slice(GS::UIndex start, GS::UIndex end) const;
	};
}

#endif // !S_ARRAY_HPP