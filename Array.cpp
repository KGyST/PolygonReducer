#include "Src/Array.hpp"



template <class Type>
Type** S::Array<Type>::ToNeigs() const
{
    Type** neigs = BMAllocateHandle(this->GetSize());
    UINT i = 0;

    if (neigs != NULL)
    {
        for (auto item : this)
        {
            neigs[i++] = item;
        }
    }

    return neigs;
}

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

