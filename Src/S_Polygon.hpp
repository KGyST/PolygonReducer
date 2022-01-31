#pragma once
#ifndef S_POLYGON_HPP
#define S_POLYGON_HPP

#define ACExtension
#include "ACAPinc.h"					// also includes APIdefs.h
#include "S_Polyline.hpp"
#include "Segment.hpp"
#include "PolygonReducer.hpp"
#include "Array.hpp"
#include <string>

namespace PolygonReducer {

    class S_Polygon
    {
    private:
        GS::Array <S_Polyline*> m_polylines;

        UINT m_pointCount;

    public:
        S_Polygon(const API_ElementMemo* p_memo);
        ~S_Polygon();

        void updateMemo(API_ElementMemo* o_memo);   //??WTF
        std::string getGDLcode();
        API_ElementMemo getMemo();

        void setPointCount(const int i_count);
        void setupArcs(void);

        GS::Array <S::Segment> m_segments;
    };
}
#endif // !S_POLYGON_HPP