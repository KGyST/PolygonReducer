#include "SubPolygon.hpp"
#include "PolygonReducer.template.hpp"

using namespace Geometry;

namespace S {
    SubPolygon::SubPolygon(Array<API_Coord>* coords, Array<API_PolyArc>* pars, Array<UInt32>* vertexIDs) {
        m_segments = *new Array<Segment*>;

        Sector midPerpPrev;
        Coord centerPrev(0, 0);
        Coord centerThis(0, 0);
        UINT firstIdx = 0;
        //double aPrev, aThis;

        for(UInt16 i = 1; i < coords->GetSize(); i++)
        {
            Segment s((*coords)[i-1], (*coords)[i]);

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
    }

    using namespace Geometry;

    SubPolygon::~SubPolygon() {}
}