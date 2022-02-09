#pragma once
#ifndef S_POLYGON_HPP
#define S_POLYGON_HPP

#define ACExtension
#include "ACAPinc.h"					            // also includes APIdefs.h
#include "S_SubPoly.hpp"
#include "Segment.hpp"
#include "PolygonReducer.hpp"
#include "Array.hpp"
#include <string>

namespace PolygonReducer {

    class S_Polygon
    {
    private:
        UINT m_pointCount;

        void SetUserdata();

    public:
        S_Polygon(const API_ElementMemo* p_memo);
        ~S_Polygon();

        std::string getGDLcode();
        API_ElementMemo getMemo();

        void setPointCount(const int i_count);
        USize getPointCount();

        void MoveAllPoints();

        bool m_isPolygon;                           //false if polyline


        GS::Array <S_SubPoly> m_subpolys;
        GS::Array <S::Segment*> m_segments;
    };
}
#endif // !S_POLYGON_HPP