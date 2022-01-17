#include "S_Polyline.hpp"
#include "PolygonReducer.template.hpp"

using namespace Geometry;

namespace PolygonReducer {
    S_Polyline::S_Polyline(GS::Array<API_Coord>* coords, GS::Array<API_PolyArc>* pars, GS::Array<UInt32>* vertexIDs) {
        m_segments = new GS::Array<S::Segment*>;

        Sector midPerpPrev;
        Coord centerPrev(0, 0);
        Coord centerThis(0, 0);
        UINT firstIdx = 0;
        //double aPrev, aThis;

        for(UInt16 i = 1; i < coords->GetSize(); i++)
        {
            S::Segment s((*coords)[i-1], (*coords)[i]);

            Coord* _halfPoint = new Coord((s.GetStart()->GetX() + s.GetEnd()->GetX())/2, (s.GetStart()->GetY() + s.GetEnd()->GetY()) / 2);
            Coord* _start = new Coord(s.GetStart()->GetX(), s.GetStart()->GetY());
            Coord _rotEnd = RotCoord(_halfPoint, _start, 1.00, 0.00);
            double eps = 0, radEps = 0;

            Sector midPerpThis = SetSector(*_halfPoint, _rotEnd);
            XLinesEps(midPerpPrev, midPerpThis, &centerThis, eps, radEps);

            if (centerThis != centerPrev)
            {
                if (i > firstIdx + 1)
                {
                    //(*segments)[firstIdx]->SetArc(0, centerPrev);
                    //TODO
                }


                firstIdx = i;
            }
            centerPrev = centerThis;
            midPerpPrev = midPerpThis;
        }

        UNUSED_PARAMETER(coords);
        UNUSED_PARAMETER(pars);
        UNUSED_PARAMETER(vertexIDs);
        //UNUSED_PARAMETER(segments);
    }

    using namespace Geometry;

    S_Polyline::~S_Polyline() {}
}