#include "S_Polyline.hpp"
namespace PolygonReducer {

    S_Polyline::S_Polyline(GS::Array<API_Coord>* coords, GS::Array<API_PolyArc>* pars, GS::Array<UInt32>* vertexIDs) {
        GS::Array <S_Segment>* segments = new GS::Array <S_Segment>();

        for (UInt32 i = 0; i < coords->GetSize() - 1; i++)
        {

        }

        UNUSED_PARAMETER(coords);
        UNUSED_PARAMETER(pars);
        UNUSED_PARAMETER(vertexIDs);
        UNUSED_PARAMETER(segments);
    }
    S_Polyline::~S_Polyline() {}
}