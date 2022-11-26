#include "SubPolygon.hpp"
#include "PolygonReducer.template.hpp"

#include "GenArc2DData.h"

using namespace Geometry;

namespace S {
    //SubPolygon::SubPolygon(Array<API_Coord>* coords, Array<API_PolyArc>* parcs, Array<UInt32>* vertexIDs) {
    SubPolygon::SubPolygon
        (UInt32                 i_iStart
        ,UInt32                 i_iEnd
        ,Array<API_Coord>&      i_coords
        ,Array<API_PolyArc>&    i_parcs
        ,Array<UInt32>&         i_vertexIDs
        ,Array<Segment*>&       o_segments)
    {
        m_segments = *new Array<Segment*>;

        double aPrev, aThis;
        UInt16 _idx = 0;
        Segment* _sPrev = nullptr;
        Segment* _segment = nullptr;
        GS::HashTable<UInt32, double> _arcTable;

        //for (auto _parc : _parcs)
        for each (API_PolyArc _parc in i_parcs)
        {
            _arcTable.Add(_parc.begIndex, _parc.arcAngle);
        }

        for (UInt32 i = i_iStart + 1; i < i_iEnd + 1; i++)
        {
            API_Coord c1(i_coords[i]);
            API_Coord c2(i_coords[i + 1]);
            _segment = new Segment(_idx++, i_vertexIDs[i], i_vertexIDs[i + 1], c1, c2);

            if (_arcTable.ContainsKey(i))
            {
                _segment->SetArc(_arcTable[i]);
            }

            if (_sPrev)
            {
                _segment->SetPrev(_sPrev);
                _sPrev->SetNext(_segment);
            }

            m_segments.Push(_segment);
            o_segments.Push(_segment);

            _sPrev = _segment;
        }

        if (_segment)
        {
            m_segments[0]->SetPrev(_segment);
            _segment->SetNext(m_segments[0]);
        }

        CreateArcsFromPolys();
    }

    SubPolygon::~SubPolygon() {}

    // TODO---------------------------------------------

    void SubPolygon::CreateArcsFromPolys() 
    {
        Sector midPerpThis, midPerpPrev;
        Coord centerPrev, centerThis;
        ::Coord _c;
        UINT firstIdx = 0;
        Segment* segPrev = nullptr;
        Segment* segFirst = nullptr;
        double eps = 0, radEps = 0;

        Segment* s;
        midPerpPrev = m_segments[0]->MidPerp().ToSector();
        midPerpThis = m_segments[1]->MidPerp().ToSector();
        XLinesEps(midPerpPrev, midPerpThis, &_c, eps, radEps);
        centerPrev = Coord(_c);
        midPerpPrev = midPerpThis;
        segPrev = m_segments[1];

        for (UInt16 _i = 2; _i <= m_segments.GetSize(); _i++ )
        {
            s = m_segments[_i];
            midPerpThis = s->MidPerp().ToSector();
            XLinesEps(midPerpPrev, midPerpThis, &_c, eps, radEps);
            centerThis = Coord(_c);

            if (centerThis != centerPrev)
            {
                if (segFirst != nullptr && segFirst != segPrev)
                {
                    double orifi = 1.00, _angle;
                    long arcFlag;
                    Int32 ind = 0;
                    ::Coord _1 = segFirst->GetStart()->ToCoord()   ;
                    ::Coord _2 = s->GetEnd()->ToCoord();

                    GetViewAngle(&centerPrev, &_1, &_2,
                        orifi, ind, &_angle);

                    s->SetArc(_angle, centerPrev);
                }

                firstIdx = s->GetIdx();
                centerPrev = centerThis;
                segFirst = s;
            }
            
            midPerpPrev = midPerpThis;
            segPrev = s;
        }
    }

    void SubPolygon::CreatePolysFromArcs() {}
}

