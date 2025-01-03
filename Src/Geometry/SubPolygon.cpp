#include "SubPolygon.hpp"
#include "../PolygonReducer.template.hpp"

using namespace Geometry;

namespace S {
    SubPolygon::SubPolygon(Array<API_Coord>* coords, Array<API_PolyArc>* pars, Array<UInt32>* vertexIDs)
        //m_segments({}) 
    {
        Coord *centerPrev = NULL;
        Coord pointPrev = (*coords)[0], pointThis;

        Coord centerThis;
        UINT firstIdx = 0;
        double eps = 0, radEps = 0;

        const Segment s((*coords)[0], (*coords)[1]);
        Sector midPerpPrev = s.MidPerp().toSector();

        for(UInt16 i = 1; i < coords->GetSize(); i++)
        {
            pointThis = (*coords)[i];
            Segment s(pointPrev, pointThis);

            const Sector midPerpThis = s.MidPerp().toSector();

            XLinesEps(midPerpPrev, midPerpThis, &centerThis, eps, radEps);

            if (centerPrev != NULL && centerThis != *centerPrev)
            {
                if (i > firstIdx + 1)
                {
                    Segment newSeg(pointPrev, pointThis);

                    //(*segments)[firstIdx]->SetArc(0, centerPrev);
                    //TODO
                }

                firstIdx = i;
            }

            *centerPrev = centerThis;
            midPerpPrev = midPerpThis;
        }
    }

    using namespace Geometry;

    SubPolygon::~SubPolygon() {}
}