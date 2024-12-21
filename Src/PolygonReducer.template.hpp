#pragma once
#ifndef POLYGON_REDUCER_TEMPLATE_HPP
#define POLYGON_REDUCER_TEMPLATE_HPP

// To be removed ------------------------------
namespace PolygonReducer {

    template <class inT>
    bool ReturnTrue(inT p_inObj)
    {
        return true;
    }

    template <class T>
    T ConvertToTheSame(T p_obj)
    {
        return p_obj;
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

    template <class inT, class outT>
    GSErrCode ConvertToGSArray(
        inT** p_neigs,
        GS::Array<outT>* resultArray,
        bool (*funcFilter)(inT) = ReturnTrue<inT>,
        outT (*funcConverter)(inT) = ConvertToTheSame
    )
        // FIXME nameless functions as default parameters
    {
        UInt32 nSel = BMGetHandleSize((GSHandle)p_neigs) / sizeof(inT);

        inT _an;

        try {
            for (UInt32 ii = 0; ii < nSel; ++ii) {
                _an = (*p_neigs)[ii];

                if (funcFilter(_an))
                    resultArray->Push(funcConverter(_an));
            }
        }
        catch (...) {
            return 1;
        }

        return 0;
    }

    template <class inT, class outT>
    GSErrCode ConvertToGSArray(
        GS::Array<inT>* p_neigs,
        GS::Array<outT>* resultArray,
        bool (*funcFilter)(inT) = ReturnTrue<inT>,
        outT (*funcConverter)(inT) = ConvertToTheSame
    )
    {
        try {
            for each(inT _neig in p_neigs) {
                if (funcFilter(_neig))
                    resultArray->Push(funcConverter(_neig);
            }
        }
        catch (...) {
            return 1;
        }

        return 0;
    }
}
#endif // !POLYGON_REDUCER_TEMPLATE_HPP
