#include "S_Polyline.hpp"
#include "PolygonReducer.template.hpp"

using namespace Geometry;

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

    using namespace Geometry;

    //S_Polyline::S_Polyline(S_Polygon *p_owner, int p_iStart, int p_iEnd)
    //{
    //    segments = ArraySlice<S_Segment*>(*(p_owner->m_segments), p_iStart, p_iEnd);

    //    Sector prevParallel;
    //    Coord centerPrev, centerThis;
    //    UINT firstIdx;
    //    double aPrev, aThis;

    //    for(UInt16 i = 0; i < segments->GetSize(); i++)
    //    {
    //        S_Segment* s = (*segments)[i];

    //        Coord* _halfPoint = new Coord((s->getStart()->x + s->getEnd()->x)/2, (s->getStart()->y + s->getEnd()->y) / 2);
    //        Coord* _start = new Coord(s->getStart());
    //        Coord _rotEnd = RotCoord(_halfPoint, _start, 0.00, 1.00);
    //        double eps = 0, radEps = 0;
    //        Coord centerThis(0, 0);

    //        Sector thisParallel = SetSector(*_start, _rotEnd);
    //        XLinesEps(prevParallel, thisParallel, &centerThis, eps, radEps);

    //        if (centerThis != centerPrev)
    //        {
    //            if (i > firstIdx + 1)
    //            {
    //                (*segments)[firstIdx]->SetArc(0, centerPrev);
    //                //TODO
    //            }

    //            centerPrev = centerThis;
    //            firstIdx = i;
    //        }
    //    }
    //}

    S_Polyline::~S_Polyline() {}
}