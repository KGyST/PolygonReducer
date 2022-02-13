#include "Array.hpp"

template <class Type>
S::Array<Type>& S::Array<Type>::Slice(GS::UIndex start, GS::UIndex end) const
{
    S::Array<Type> resultArray{};

    for (UINT i = start; i < end; i++)
    {
        resultArray.Push(this[i]);
    }

    return &resultArray;
}

//template <class Type>
//S::Array<Type>* Sort(void (*funcSort)())
//{
//}

template <class Type>
GS::RandomContainerIterator<S::Array<Type>> begin()
{
    return Begin();
}

//template <class Type>
//GS::RandomContainerIterator<S::Array<Type>> end()
//{
//    return GS::RandomContainerIterator<S::Array<Type>>(*this, size);
//}