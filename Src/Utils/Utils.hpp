#pragma once
#define ACExtension
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "../AC27/APICommon.h"
#include <APIdefs.h>

#ifndef UTILS_HPP
#define UTILS_HPP

void TrackPoly(const API_Polygon*, const API_ElementMemo*);

bool IsItPolygon(API_Neig const&);
bool IsItPolygon(API_Element* const&);

API_Guid NeigToAPIGuid(API_Neig const&);

API_ElementMemo ConvertToMemos(API_Neig const &);

API_Coord** ConvertToCoords(API_ElementMemo const&);

#endif // !UTILS_HPP
