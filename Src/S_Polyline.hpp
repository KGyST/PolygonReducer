#pragma once
#ifndef S_POLYLINE_HPP
#define S_POLYLINE_HPP
#endif // !S_POLYLINE_HPP

#define ACExtension
#include "ACAPinc.h"					// also includes APIdefs.h
#include "S_Segment.hpp"

class S_Polyline
{
private:
    GS::Array <S_Segment>* segments;

public:
    S_Polyline(GS::Array<API_Coord>* coords, GS::Array<API_PolyArc>* pars, GS::Array<UInt32>* vertexIDs);
    ~S_Polyline();
};