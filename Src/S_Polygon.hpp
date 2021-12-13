#pragma once
#ifndef S_POLYGON_HPP
#define S_POLYGON_HPP

#define ACExtension
#include "ACAPinc.h"					// also includes APIdefs.h
#include "S_Polyline.hpp"
#include "S_Segment.hpp"
#include "PolygonReducer.hpp"
#include <string>

namespace PolygonReducer {

    class S_Polygon
    {
    private:
        GS::Array <S_Polyline*>* m_polylines;
        GS::Array <S_Segment*>*  m_segments;

        int m_pointCount;

    public:
        S_Polygon(const API_ElementMemo* p_memo);
        ~S_Polygon();

        API_ElementMemo* getResultMemo();
        API_ElementMemo* getOriginalMemo();
        std::string getGDLcode();

        void setPointCount(const int i_count);
    };
}
#endif // !S_POLYGON_HPP