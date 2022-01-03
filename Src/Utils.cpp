#include "Utils.hpp"
#include "APICommon.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include <math.h>
#include <stdlib.h>

void TrackPoly(const API_Polygon* poly, const API_ElementMemo* memo)
{
    int         j, i, begInd, endInd, arcInd;
    API_Coord   begC, endC;

    for (j = 1; j <= poly->nSubPolys; j++) {
        begInd = (*memo->pends)[j - 1] + 1;
        endInd = (*memo->pends)[j];
        for (i = begInd; i < endInd; i++) {
            begC = (*memo->coords)[i];
            endC = (*memo->coords)[i + 1];
            arcInd = FindArc(*memo->parcs, poly->nArcs, i);
        }

        WriteReport_Alert(&"Teszt"[0]);
    }
}

bool IsItPolygon(API_Neig p_neig)
{
    if (p_neig.neigID == APINeig_PolyLine
        || p_neig.neigID == APINeig_PolyLineOn
        || p_neig.neigID == APINeig_Hatch
        || p_neig.neigID == APINeig_HatchOn
        )
        return true;

    return false;
}

API_Guid NeigToAPIGuid(API_Neig p_neig)
{
    return p_neig.guid;
}

API_ElementMemo ConvertToMemos(API_Neig p_neig)
{
    API_Guid _guid = p_neig.guid;
    API_ElementMemo _memo;

    GSErrCode err = ACAPI_Element_GetMemo(_guid, &_memo);

    UNUSED_VARIABLE(err);

    return _memo;
}

API_Coord** ConvertToCoords(API_ElementMemo p_memo)
{
    return p_memo.coords;
}




