#pragma once

#ifndef S_ARRAY_HPP
#define S_ARRAY_HPP

#define ACExtension
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include <type_traits>
#include <optional>

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

		Array<Type> Slice(int start, int end, int step) const;
		Array<Type> Slice(int start, int end) const {return Slice(start, end, 1);}
		Array<Type> Slice(int start = 0) const { return Slice(start, this->GetSize() );}
		void sort(GS::UIndex start, GS::UIndex end, bool (*funcSort)());
		std::optional<int> IndexOf(const Type& item) const;

		typename S::Array<Type>::ConstIterator begin(void) const { return ConstIterator(*this, 0); }
		typename S::Array<Type>::ConstIterator end(void) const { return ConstIterator(*this, this->GetSize()); }
	};

	template <class Type>
	class CircularArray : public Array<Type>
	{
	public:
		Type operator[](int) const;
		// FIXME Rotate method
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

template <class Type>
S::Array<Type> S::Array<Type>::Slice(int start, int end, int step) const
{
	int size = GetSize();

	if (start < 0)
		start = size + start;
	if (end < 0)
		end = size + end;

	if (start < 0) start = 0;
	if (end > size) end = size;
	if (end < start) end = start;

	S::Array<Type> resultArray{};

	for (int i = start; i < end; i += step)
	{
		resultArray.Push((*this)[i]);
	}

	return resultArray;
}

template <class Type>
std::optional<int> S::Array<Type>::IndexOf(const Type& item) const {
	for (int i = 0; i < this->GetSize(); ++i) {
		if ((*this)[i] == item) {
			return i;
		}
	}
	return std::nullopt;
}

template <class Type>
Type S::CircularArray<Type>::operator[](int i_index) const
{
	int size = this->GetSize();
	if (size == 0)
		throw std::out_of_range("CircularArray is empty");

	i_index = ((i_index % size) + size) % size;

	return GS::Array<Type>::operator[](i_index);
}

#endif // S_ARRAY_HPP

