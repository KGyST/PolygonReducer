#pragma once
#define ACExtension
#include "ACAPinc.h"					// also includes APIdefs.h
#include "../APICommon.h"
#include <APIdefs.h>

#ifndef UTILS_HPP
#define UTILS_HPP

void TrackPoly(const API_Polygon* poly, const API_ElementMemo* memo);

bool IsItPolygon(API_Neig p_neig);

API_Guid NeigToAPIGuid(API_Neig p_neig);

API_ElementMemo ConvertToMemos(API_Neig p_neig);

API_Coord** ConvertToCoords(API_ElementMemo p_memo);

#endif // !UTILS_HPP
