#pragma once
#ifndef S_POLYGON_HPP
#define S_POLYGON_HPP
#endif // !S_POLYGON_HPP

#define ACExtension
#include "ACAPinc.h"					// also includes APIdefs.h
#include "S_Polyline.hpp"
#include "S_Segment.hpp"
#include "PolygonReducer.hpp"

//template <class T>
//extern GS::Array<T>* ArraySlice(GS::Array<T> p_array, int start, int end);
//
//template <class inT>
//extern bool ReturnTrue(inT p_inObj);
//
//template <class T>
//extern GS::Array<T>* ArraySlice(GS::Array<T> p_array, UInt32 start, UInt32 end);

class S_Polygon
{
private:
    GS::Array <S_Polyline>* polylines;
    GS::Array <S_Segment>* segments;

    int m_pointCount;

public:
    S_Polygon(const API_ElementMemo* p_memo);
    ~S_Polygon();

    API_ElementMemo* getResultMemo();
    API_ElementMemo* getOriginalMemo();
    //string getGDLcode();

    void setPointCount(const int i_count);
};