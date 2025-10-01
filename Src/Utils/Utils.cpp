#include "Utils.hpp"

void TrackPoly(const API_Polygon* i_poly, const API_ElementMemo * i_memo)
{
    int         j, i, begInd, endInd, arcInd;
    API_Coord   begC, endC;

    for (j = 1; j <= i_poly->nSubPolys; j++) {
        begInd = (*i_memo->pends)[j - 1] + 1;
        endInd = (*i_memo->pends)[j];
        for (i = begInd; i < endInd; i++) {
            begC = (*i_memo->coords)[i];
            endC = (*i_memo->coords)[i + 1];
            arcInd = FindArc(*i_memo->parcs, i_poly->nArcs, i);
        }

        WriteReport_Alert(&"Teszt"[0]);
    }
}

bool IsItPolygon(API_Neig const &i_neig)
{
    if  (    i_neig.neigID == APINeig_PolyLine
        ||   i_neig.neigID == APINeig_PolyLineOn
        ||   i_neig.neigID == APINeig_Hatch
        ||   i_neig.neigID == APINeig_HatchOn
        )
        return true;

    return false;
}

bool IsItPolygon(API_Element * const &i_elem)
{
    if  (   i_elem->header.type.typeID == API_PolyLineID
        ||  i_elem->header.type.typeID == API_HatchID
        )
        return true;

    return false;
}

API_Guid NeigToAPIGuid(API_Neig const &i_neig)
{
    return i_neig.guid;
}

API_ElementMemo ConvertToMemos(API_Neig const &i_neig)
{
    API_Guid _guid = i_neig.guid;
    API_ElementMemo _memo;

    GSErrCode err = ACAPI_Element_GetMemo(_guid, &_memo);

    return _memo;
}

API_Coord** ConvertToCoords(API_ElementMemo const &i_memo)
{
    return i_memo.coords;
}