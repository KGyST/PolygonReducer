#include "Algorithms.hpp"
//#include "GSRoot.hpp"
#include "S_Polygon.hpp"
#include "PolygonReducer.template.hpp"

#include "Algorithms.hpp"


namespace PolygonReducer {
    S_Polygon::S_Polygon(const API_ElementMemo* p_memo)
        : m_subpolys()
        , m_segments()
    {
        S::Segment _segment;

        S::Array<Int32> _pends(p_memo->pends);
        S::Array<API_Coord> _coords(p_memo->coords);
        S::Array<API_PolyArc> _parcs(p_memo->parcs);
        S::Array<UInt32> _vertexIDs(p_memo->vertexIDs);
        GS::HashTable<UInt32, double> _archTable;

        //for (auto _parc : _parcs)
        for each (API_PolyArc _parc in _parcs)
        {
            _archTable.Add(_parc.begIndex, _parc.arcAngle);
        }

        int _idx = 0;

        if  (   _coords[0].x - EPS < 0.00
            &&  _coords[0].x + EPS > 0.00)
            m_isPolygon = true;
        else
            m_isPolygon = false;    // Polyline

        for (UInt32 i = 1; i < _pends.GetSize(); i++)
        {
            S_SubPoly sp;

            for (UInt32 j = (UInt32)_pends[i - 1] + 1; j < (UInt32)_pends[i]; j++)
            {
                API_Coord c1(_coords[j]);
                API_Coord c2(_coords[j + 1]);
                S::Segment* _segment = new S::Segment(_idx++, _vertexIDs[j], _vertexIDs[j + 1], c1, c2);
                if (_archTable.ContainsKey(j))
                {
                    _segment->SetArc(_archTable[j]);
                }

                m_segments.Push(_segment);
                sp.m_segments.Push(_segment);
            }

            S_SubPoly sp2(sp);
            GS::Sort(sp2.m_segments.Begin(), sp2.m_segments.End(), [](S::Segment* s1, S::Segment* s2) -> bool {return s1->GetLength() > s2->GetLength(); });
            sp2.m_segments.Sort(0, sp2.m_segments.GetSize(), [](S::Segment* s1, S::Segment* s2) -> bool {return s1->GetLength() > s2->GetLength(); });
            for each (auto s in sp2.m_segments)
            {
                auto l = s->GetLength() ;
                l = l + 1;
                UNUSED_VARIABLE(l);
            }

            m_subpolys.Push(sp);
        }
    }


    S_Polygon::~S_Polygon() {
    }


    std::string S_Polygon::getGDLcode() {
        using namespace std;

        string result = "";

        for (UInt16 i = 0; i < m_segments.GetSize(); i++)
        {
            S::Segment* _s = m_segments[i];
            result += _s->ToString();
        }

        return result;
    }


    API_ElementMemo S_Polygon::getMemo()
    {
        S::Array<API_Coord> _coords;
        S::Array<API_PolyArc> _parcs;
        S::Array<Int32> _pends;
        S::Array<UInt32> _vertIDs;

        API_Coord _ac = *new API_Coord(S::Coord(m_isPolygon ? 0.00 : -1.00, 0.00).ToAPICoord());
        _coords.Push(_ac);

        UInt32 maxId = 0;
        _pends.Push(0);
        _vertIDs.Push(0);

        for (UInt32 i = 0; i < m_subpolys.GetSize(); i++)
        {
            S_SubPoly sp = m_subpolys[i];
            _coords.Push(sp.m_segments[0]->GetStart()->ToAPICoord());
            _vertIDs.Push(sp.m_segments[0]->GetStartIdx());

            for (UInt32 j = 0; j < sp.m_segments.GetSize(); j++)
            {
                maxId++;

                if (sp.m_segments[j]->GetAng() < -EPS
                    || sp.m_segments[j]->GetAng() > EPS)
                {
                    API_PolyArc _arc;
                    _arc.arcAngle = sp.m_segments[j]->GetAng();
                    _arc.begIndex = maxId;
                    _arc.endIndex = maxId + 1;
                    _parcs.Push(_arc);
                }

                _coords.Push(sp.m_segments[j]->GetEnd()->ToAPICoord());
                _vertIDs.Push(sp.m_segments[j]->GetEndIdx());
            }

            _pends.Push(++maxId);
        }

        _vertIDs[0] = (UInt32)maxId;

        API_ElementMemo resultMemo;
        BNZeroMemory(&resultMemo, sizeof(API_ElementMemo));

        resultMemo.coords = _coords.ToNeigs();
        resultMemo.parcs = _parcs.ToNeigs();
        resultMemo.pends = _pends.ToNeigs();
        resultMemo.vertexIDs = _vertIDs.ToNeigs();

        return resultMemo;
    }


    void S_Polygon::setPointCount(const int i_count)
    {
        UNUSED_PARAMETER(i_count);
    }


    USize S_Polygon::getPointCount()
    {
        return m_segments.GetSize();
    }


    void S_Polygon::MoveAllPoints()
    {
        for (UINT i = 0; i < m_segments.GetSize(); i++)
        {
            S::Coord _s(*m_segments[i]->GetStart());
            m_segments[i]->SetStart(S::Coord(_s.GetX() + 1.00, _s.GetY() + 1.00));
            S::Coord _e(*m_segments[i]->GetEnd());
            m_segments[i]->SetEnd(S::Coord(_e.GetX() + 1.00, _e.GetY() + 1.00));
        }
    }
}
