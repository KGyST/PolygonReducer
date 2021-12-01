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

template <class T>
T ConvertToTheSame(T p_obj);

template <class inT, class outT>
GSErrCode ConvertToGSArray(
    inT**               p_neigs,
    GS::Array<outT>*    resultArray,
    bool                (*funcFilter)(inT)      = ReturnTrue<inT>,
    outT                (*funcConverter)(inT)   = ConvertToTheSame
);

template <class T>
GS::Array<T>* ArraySlice(GS::Array<T> p_array, UInt32 start, UInt32 end);

//-----------------------------------------

