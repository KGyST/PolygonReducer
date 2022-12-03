#include "Algorithms.hpp"
#include "Polygon.hpp"
#include "../PolygonReducer.template.hpp"

#include "Algorithms.hpp"


namespace S {
    Polygon::Polygon(const API_ElementMemo* p_memo)
        : m_subpolys()
        , m_segments()
    {
        //Segment _segment;

        Array<Int32> _pends(p_memo->pends);
        Array<API_Coord> _coords(p_memo->coords);
        Array<API_PolyArc> _parcs(p_memo->parcs);
        Array<UInt32> _vertexIDs(p_memo->vertexIDs);
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
            SubPolygon* _sp = new SubPolygon;
            Segment* _sPrev=nullptr;
            Segment* _segment = nullptr;

            for (UInt32 j = (UInt32)_pends[i - 1] + 1; j < (UInt32)_pends[i]; j++)
            {
                API_Coord c1(_coords[j]);
                API_Coord c2(_coords[j + 1]);
                _segment = new Segment(_idx++, _vertexIDs[j], _vertexIDs[j + 1], c1, c2);
                if (_archTable.ContainsKey(j))
                {
                    _segment->SetArc(_archTable[j]);
                }

                if (_sPrev)
                {
                    _segment->SetPrev(_sPrev);
                    _sPrev->SetNext(_segment);
                }

                m_segments.Push(_segment);
                _sp->m_segments.Push(_segment);

                _sPrev = _segment;
            }

            if (_segment)
            {
                m_segments[0]->SetPrev(_segment);
                _segment->SetNext(m_segments[0]);
            }

            //auto _iLast = sp.m_segments.GetSize() - 1;
            //sp.m_segments[0]->SetPrev(sp.m_segments[_iLast]);
            //sp.m_segments[_iLast]->SetNext(sp.m_segments[0]);

            //S_SubPoly sp2(sp);
            //GS::Sort(sp2.m_segments.Begin(), sp2.m_segments.End(), [](S::Segment* s1, S::Segment* s2) -> bool {return s1->GetLength() > s2->GetLength(); });
            //sp2.m_segments.Sort(0, sp2.m_segments.GetSize(), [](S::Segment* s1, S::Segment* s2) -> bool {return s1->GetLength() > s2->GetLength(); });

            //for each (auto s in sp2.m_segments)
            //{
            //    auto l = s->GetLength() ;
            //    l = l + 1;
            //}

            m_subpolys.Push(_sp);
        }
    }


    Polygon::~Polygon() {
        for each (Segment* _seg in m_segments)
            delete _seg;

        for each (SubPolygon * _sp in m_subpolys)
            delete _sp;
    }


    std::string Polygon::getGDLcode() {
        using namespace std;

        string result = "";

        for each (Segment* _s in m_segments)
        {
            result += _s->ToString();
        }

        return result;
    }


    API_ElementMemo Polygon::getMemo()
    {
        Array<API_Coord> _coords;
        Array<API_PolyArc> _parcs;
        Array<Int32> _pends;
        Array<UInt32> _vertIDs;

        API_Coord* _ac = new API_Coord(Coord(m_isPolygon ? 0.00 : -1.00, 0.00).ToAPICoord());    //1st coord special
        _coords.Push(*_ac);
        delete _ac;

        UInt32 maxId = 0/*, id = 0*/;
        _pends.Push(0);
        _vertIDs.Push(0);

        for each (SubPolygon* sp in m_subpolys)
        {
            _coords.Push(sp->m_segments[0]->GetStart()->ToAPICoord());
            UInt32 iFirstSegmentIdx = ++maxId;
            //UInt32 iFirstSegmentIdx = sp->m_segments[0]->GetStartIdx();
            _vertIDs.Push(iFirstSegmentIdx);
            //maxId = sp->m_segments[0]->GetStartIdx() > maxId ? sp->m_segments[0]->GetStartIdx() : maxId;
            
            for each (Segment* _segment in sp->m_segments)
            {
                //++id;

                if  (   _segment->GetAng() < -EPS
                    ||  _segment->GetAng() >  EPS)
                {
                    API_PolyArc _arc;
                    _arc.arcAngle = _segment->GetAng();
                    _arc.begIndex = maxId;
                    //_arc.begIndex = _segment->GetStartIdx();
                    _arc.endIndex = maxId + 1;
                    //_arc.endIndex = _segment->GetEndIdx();
                    _parcs.Push(_arc);
                }

                _vertIDs.Push(maxId++);
                //_vertIDs.Push(_segment->GetEndIdx() );
                //maxId = _segment->GetEndIdx() > maxId ? _segment->GetEndIdx() : maxId;
                _coords.Push(_segment->GetEnd()->ToAPICoord());
            }

            _pends.Push(maxId);
            //_pends.Push(id+1);

            //_vertIDs.Push(iFirstSegmentIdx);
            //_coords.DeleteLast();
            //_coords.Push(_coords[iFirstSegmentIdx]);
        }

        _vertIDs[0] = maxId;

        API_ElementMemo resultMemo;
        BNZeroMemory(&resultMemo, sizeof(API_ElementMemo));

        resultMemo.coords = _coords.ToNeigs();
        resultMemo.parcs = _parcs.ToNeigs();
        resultMemo.pends = _pends.ToNeigs();
        resultMemo.vertexIDs = _vertIDs.ToNeigs();

        return resultMemo;
    }


    void Polygon::removeShortestEdge()
    {
        Array <Segment*> newSegments(m_segments);

        GS::Sort(m_segments.Begin(), m_segments.End(), [](Segment* s1, Segment* s2) -> bool {return s1->GetLength() < s2->GetLength(); });

        Segment* shortestSegment = m_segments[0];
        Segment* _prevSeg = shortestSegment->GetPrev();
        Segment* _nextSeg = shortestSegment->GetNext();

        intersectSegments( _prevSeg, _nextSeg);

        _prevSeg->SetEndIdx(shortestSegment->GetEndIdx() );
        _nextSeg->SetStartIdx(shortestSegment->GetEndIdx());

        GS::Sort(newSegments.Begin(), newSegments.End(), [](Segment* s1, Segment* s2) -> bool {return s1->GetIdx() < s2->GetIdx(); });

        removeSegment(shortestSegment);
    }


    void Polygon::intersectSegments(Segment * io_prev, Segment * io_next)
    {
        using namespace Geometry;

        Sector lin1 = SetSector(io_prev->GetStart()->ToCoord(), io_prev->GetEnd()->ToCoord());
        Sector lin2 = SetSector(io_next->GetStart()->ToCoord(), io_next->GetEnd()->ToCoord());

        ::Coord xc(0, 0);

        double eps = 0, radEps = 0;

        XLinesEps(lin1, lin2, &xc, eps, radEps);

        io_prev->SetEnd(xc);
        io_next->SetStart(xc);
        io_prev->SetNext(io_next);
        io_next->SetPrev(io_prev);
    } 


    void Polygon::setPointCount(const unsigned int i_count)
    {
        for (UINT i = getPointCount(); i > i_count; i--)
        {
            removeShortestEdge();
        }
    }


    USize Polygon::getPointCount()
    {
        return m_segments.GetSize();
    }


    void Polygon::removeSegment(Segment* i_segment)
    {
        m_segments.DeleteAll(i_segment);
        for each (SubPolygon* _subPoly in m_subpolys)
        {
            _subPoly->RemoveSegment(i_segment);
        }

        delete i_segment;
    }


    // Only for testning
    void Polygon::MoveAllPoints()
    {
        for each (SubPolygon* _sp in m_subpolys)
        {
            for each (Segment * _seg in _sp->m_segments)
            {
                Coord _s(*_seg->GetStart());
                _seg->SetStart(Coord(_s.GetX() + 1.00, _s.GetY() + 1.00));
                Coord _e(*_seg->GetEnd());
                _seg->SetEnd(Coord(_e.GetX() + 1.00, _e.GetY() + 1.00));
            }
        }
    }

    // --------------- TODO ---------------

    // Writing in relevant memo as original user data if not present already
    void SetUserdata(){}
}
