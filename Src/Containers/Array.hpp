#pragma once

#ifndef S_ARRAY_HPP
#define S_ARRAY_HPP

#define ACExtension
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include <type_traits>

namespace S {
	template <class Type>
	class Array : public GS::Array<Type>
	{
	public:
		//typedef GS::RandomContainerIterator<typename Array<Type>>		 Iterator;

		Array(const Type* const* p_neigs, UINT p_startIdx = 0);
		//TODO Array(const Type* const* p_neigs, bool (*funcFilter)(inT), outT(*funcConverter)(inT));
		Array(const GS::Array<Type>& other)
			: GS::Array<Type>(other) {};
		Array(const S::Array<Type>& other)
			: GS::Array<Type>(other) {};
		Array() = default;
		~Array() = default;
		Type** ToNeigs() const;

		Array<Type> &slice(GS::UIndex start, GS::UIndex end) const;
		void sort(GS::UIndex start, GS::UIndex end, bool (*funcSort)());

		typename S::Array<Type>::ConstIterator begin(void) const { return ConstIterator(*this, 0); }
		typename S::Array<Type>::ConstIterator end(void) const { return ConstIterator(*this, this->GetSize()); }
	};
}

//---------------------------------------------------------------------------

template <class Type>
S::Array<Type>::Array(const Type* const* p_neigs, UINT p_startIdx)
{
	UInt32 nSel = BMGetHandleSize((GSHandle)p_neigs) / sizeof(Type);
	UINT startIdx = p_startIdx <= nSel ? p_startIdx : nSel;
	
	SetCapacity(nSel);

	for (UINT i = startIdx; i < nSel; ++i) {
		Type _an = (*p_neigs)[i];

		if constexpr (std::is_pointer_v<Type>) {
			this->Push(new std::remove_pointer_t<Type>(*_an));
		}
		else {
			this->Push(_an);
		}
	}
}


template <class Type>
Type** S::Array<Type>::ToNeigs() const
{
	// ChatGPT suggested change: use GSHandle directly instead of UINT
	UINT _size = this->GetSize();
	GSHandle hdl = BMAllocateHandle(_size * sizeof(Type), ALLOCATE_CLEAR, 0);
	if (!hdl)
		return nullptr;

	Type* raw = reinterpret_cast<Type*>(*hdl);
	for (UINT i = 0; i < _size; i++)
		raw[i] = (*this)[i];

	return reinterpret_cast<Type**>(hdl);
}


#endif // S_ARRAY_HPP

