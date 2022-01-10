#include "Src/Array.hpp"

template <class Type>
S::Array<Type>::Array(const Type** p_neigs)
{
    UInt32 nSel = BMGetHandleSize((GSHandle)p_neigs) / sizeof(Type);

    Type _an;

    try {
        for (UInt32 ii = 0; ii < nSel; ++ii) {
            _an = (*p_neigs)[ii];

            this->Push(_an);
        }
    }
}

template <class Type>
Type** S::Array<Type>::ToNeigs() const
{
}

template <class Type>
S::Array<Type>& S::Array<Type>::Slice(GS::UIndex start, GS::UIndex end) const
{
}

