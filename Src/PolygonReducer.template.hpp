#pragma once
#ifndef POLYGON_REDUCER_TEMPLATE_HPP
#define POLYGON_REDUCER_TEMPLATE_HPP

#if ACVER == 27
#include	"AC27.hpp"
#endif

// To be removed ------------------------------
namespace PolygonReducer {
    template <class i_T>
    bool ReturnTrue(i_T const &i_inObj)
    {
        return true;
    }

    template <class T>
    T ConvertToTheSame(T const &i_obj)
    {
        return i_obj;
    }

///To be removed ------------------------------

    template <class T>
    GS::Array<T>* ArraySlice(GS::Array<T> p_array, UInt32 start, UInt32 end) {
        GS::Array<T>* result = new GS::Array<T>();
        UInt32 _size = p_array.GetSize();

        if (end > _size) end = _size;

        for (UInt32 i = start; i < end; i++)
        {
            result->Push(p_array[i]);
        }

        return result;
    }

    template <class i_T, class o_T>
    GSErrCode ConvertToGSArray(
        i_T** i_neigs,
        GS::Array<o_T>* o_resultArray,
        bool (*funcFilter)(i_T const &) = ReturnTrue<i_T>,
        o_T (*funcConverter)(i_T const &) = ConvertToTheSame<i_T>
    )
        // FIXME nameless functions as default parameters
    {
        UInt32 nSel = BMGetHandleSize((GSHandle)i_neigs) / sizeof(i_T);

        i_T _an;

        try {
            for (UInt32 ii = 0; ii < nSel; ++ii) {
                _an = (*i_neigs)[ii];

                if (funcFilter(_an))
                    o_resultArray->Push(funcConverter(_an));
            }
        }
        catch (...) {
            return 1;
        }

        return 0;
    }

    template <class i_T, class o_T>
    GSErrCode ConvertToGSArray(
        GS::Array<i_T>* i_neigs,
        GS::Array<o_T>* o_resultArray,
        bool (*i_funcFilter)(i_T const &) = ReturnTrue<i_T>,
        o_T (*i_funcConverter)(i_T const &) = ConvertToTheSame<i_T>
    )
    {
        try {
            for (i_T _neig: *i_neigs) {
                if (i_funcFilter(_neig))
                    o_resultArray->Push(i_funcConverter(_neig));
            }
        }
        catch (...) {
            return 1;
        }

        return 0;
    }
}
#endif // !POLYGON_REDUCER_TEMPLATE_HPP
