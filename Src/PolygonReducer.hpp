#pragma once
#ifndef POLYGON_REDUCER_HPP
#define POLYGON_REDUCER_HPP
#endif // !POLYGON_REDUCER_HPP

#include "RS.hpp"
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "basicgeometry.h"
#include "APICommon.h"

#define ACExtension

#include "PolygonReducer_Resource.h"
#include "InfoboxPanel.hpp"
#include <string>

extern  int GetPointNumber(void);

template <class inT>
bool ReturnTrue(inT p_inObj);

template bool ReturnTrue< UInt32>(UInt32);
template bool ReturnTrue< Int32>(Int32);
template bool ReturnTrue< API_PolyArc>(API_PolyArc);

template <class T>
T ConvertToTheSame(T p_obj);

template UInt32 ConvertToTheSame< UInt32>(UInt32);
template Int32 ConvertToTheSame< Int32>(Int32);
template API_PolyArc ConvertToTheSame< API_PolyArc>(API_PolyArc);

template <class inT, class outT>
GSErrCode ConvertToGSArray(
    inT**               p_neigs,
    GS::Array<outT>*    resultArray,
    bool                (*funcFilter)(inT)      = ReturnTrue<inT>,
    outT                (*funcConverter)(inT)   = ConvertToTheSame
);

template <class T>
GS::Array<T>* ArraySlice(GS::Array<T> p_array, UInt32 start, UInt32 end);

template GS::Array<UInt32>* ArraySlice(GS::Array<UInt32>, UInt32, UInt32);
template GS::Array<API_Coord>* ArraySlice(GS::Array<API_Coord>, UInt32, UInt32);

//-----------------------------------------

