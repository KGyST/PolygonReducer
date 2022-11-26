#pragma once
#define ACExtension
#include "ACAPinc.h"					// also includes APIdefs.h
#include "APICommon.h"
#include <APIdefs.h>

#ifndef S_UTILS_HPP
#define S_UTILS_HPP

void TrackPoly(const API_Polygon* poly, const API_ElementMemo* memo);

bool IsItPolygon(API_Neig p_neig);

API_Guid NeigToAPIGuid(API_Neig p_neig);

API_ElementMemo ConvertToMemos(API_Neig p_neig);

API_Coord** ConvertToCoords(API_ElementMemo p_memo);

__declspec(dllexport) bool boolTest();
#endif // !S_UTILS_HPP
