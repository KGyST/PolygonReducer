#include "Array.hpp"

template <class Type>
S::Array<Type>& S::Array<Type>::slice(GS::UIndex start, GS::UIndex end) const
{
    S::Array<Type> resultArray{};

    for (UINT i = start; i < end; ++i)
    {
        resultArray.Push(this[i]);
    }

    return &resultArray;
}

template <class Type>
void S::Array<Type>::sort(GS::UIndex start, GS::UIndex end, bool (*funcSort)())
{
    auto _start = ConstIterator(*this, start);
    auto _end = ConstIterator(*this, end);
    GS::Sort(_start, _end, funcSort);
}

