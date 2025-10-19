#include "APIEnvir.h"

#include "Algorithms.hpp"
#include "Polygon.hpp"
#include "../PolygonReducer.template.hpp"
#include <stdexcept>
#include <algorithm>
#include <boost/format.hpp>

namespace S {
	// Internal helper functions

    void _swap(Polygon& a, Polygon& b) noexcept {
        using std::swap;
        swap(a.m_subpolys, b.m_subpolys);
        swap(a.m_segments, b.m_segments);
        swap(a.m_isPolygon, b.m_isPolygon);
        //swap(a.m_pointCount, b.m_pointCount);
    }

	// Constructors, destructors, assignment operators

    Polygon::Polygon(const Polygon& other)
        : m_pointCount(other.m_pointCount)
        , m_isPolygon(other.m_isPolygon)
        , m_subpolys()
        , m_segments() {
        GS::HashTable<Segment* , int> _sMap{};
        int _iDx = 0;

        for (auto s: other.m_segments)
        {
            m_segments.Push(new Segment(*s));
            _sMap.Add(s, _iDx++);
        }

        for (SubPolygon* sp : other.m_subpolys)
        {
            SubPolygon* _nsp = new SubPolygon(*sp);
            _nsp->m_segments.Clear();
            m_subpolys.Push(_nsp);
            for (auto s : sp->m_segments)
            {
                int _segIdx = _sMap[s];
                Segment *_seg = m_segments[_segIdx];
                _nsp->m_segments.Push(_seg);
                _sMap.Add(_seg, _segIdx);
            }
            for (auto s : _nsp->m_segments)
            {
                int _segIdx = _sMap[s];
                Segment* _seg = m_segments[_segIdx];
                int _prevIdx = _sMap[s->GetPrev()];
                int _nextIdx = _sMap[s->GetNext()];
                _seg->SetPrev(m_segments[_prevIdx]);
                _seg->SetNext(m_segments[_nextIdx]);
			}
        }
    }

    Polygon& Polygon::operator=(const Polygon& other) {
        if (this != &other) {
            Polygon temp(other);
            _swap(*this, temp);
        }
        return *this;
    }

	// // FIXME this should be a good check if a polygon can be reproduced from another's memo
    //Polygon::Polygon(const Polygon& other)
    //{
    //    API_ElementMemo memo;
    //    BNZeroMemory(&memo, sizeof(API_ElementMemo));

    //    other.getMemo(memo);

    //    Polygon rebuilt(&memo);
    //    *this = std::move(rebuilt);

    //    ACAPI_DisposeElemMemoHdls(&memo);
    //}


    Polygon::Polygon()
        : m_pointCount(0)
        , m_subpolys()
        , m_segments()
        , m_isPolygon(true) {}


    Polygon::Polygon(const API_Guid* p_guid)
        : Polygon([&] {
        API_ElementMemo memo{};
        GSErrCode err = ACAPI_Element_GetMemo(*p_guid, &memo);
        if (err != NoError)
            throw std::runtime_error("Cannot get memo");
        return memo;
            }()) {}


    Polygon::Polygon(const API_Neig* p_neig)
        : Polygon([&] {
        API_ElementMemo memo{};

        GSErrCode err = ACAPI_Element_GetMemo(p_neig->guid, &memo);
        if (err == NoError)
            throw std::runtime_error("Cannot get memo");

        return memo;
            } ()) {}


    Polygon::Polygon(const API_ElementMemo* p_memo)
        : m_subpolys()
        , m_segments()
        , m_pointCount(0)
    {
        Array<Int32> _pends(p_memo->pends);
        Array<API_Coord> _coords(p_memo->coords);
        Array<API_PolyArc> _parcs(p_memo->parcs);
        Array<UInt32> _vertexIDs(p_memo->vertexIDs);
        GS::HashTable<UInt32, double> _archTable;

        for (API_PolyArc _parc : _parcs)
        {
            _archTable.Add(_parc.begIndex, _parc.arcAngle);
        }

        int _idx = 0;

        if  (   _coords[0].x - EPS < 0.00
            &&  _coords[0].x + EPS > 0.00)
            m_isPolygon = true;
        else
            m_isPolygon = false;    // Polyline

        for (UInt32 i = 1; i < _pends.GetSize(); ++i)
        {
            SubPolygon* _sp = new SubPolygon;
            Segment* _sPrev=nullptr;
            Segment* _segment = nullptr;

            for (UInt32 j = (UInt32)_pends[i - 1] + 1; j < (UInt32)_pends[i]; ++j)
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
                _sp->m_segments[0]->SetPrev(_segment);
                _segment->SetNext(_sp->m_segments[0]);
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


	// Accessors, mutators and other methods


    std::string Polygon::getGDLcode() const
    {
        using namespace std;

        string result = "";

        for each (Segment* _s in m_segments)
        {
            result += _s->ToString();
        }

        return result;
    }


    void Polygon::getMemo(API_ElementMemo& io_memo) const
    {
        Array<API_Coord> _coords;
        Array<API_PolyArc> _parcs;
        Array<Int32> _pends;
        Array<UInt32> _vertIDs;

        _coords.Push(API_Coord(Coord(m_isPolygon ? 0.00 : -1.00, 0.00).ToAPICoord()));  //1st coord special

        UInt32 maxId = 0/*, id = 0*/;
        _pends.Push(0);
        _vertIDs.Push(0);

        for (SubPolygon* sp: m_subpolys)
        {
            _coords.Push(sp->m_segments[0]->GetStart()->ToAPICoord());
            UInt32 iFirstSegmentIdx = ++maxId;
            //UInt32 iFirstSegmentIdx = sp->m_segments[0]->GetStartIdx();
            _vertIDs.Push(iFirstSegmentIdx);
            //maxId = sp->m_segments[0]->GetStartIdx() > maxId ? sp->m_segments[0]->GetStartIdx() : maxId;
            
            for (Segment* _segment: sp->m_segments)
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

        //API_ElementMemo resultMemo;
        //BNZeroMemory(&resultMemo, sizeof(API_ElementMemo));

        io_memo.coords = _coords.ToNeigs();
        io_memo.parcs = _parcs.ToNeigs();
        io_memo.pends = _pends.ToNeigs();
        io_memo.vertexIDs = _vertIDs.ToNeigs();
    }


    double Polygon::getShortestEdgeLength() const
    // Length of the shortest edge
    {
        if (m_segments.GetSize() == 0)
            return 0.00;
        double _minLength = m_segments[0]->GetLength();
        for (Segment* _seg: m_segments)
        {
            double _len = _seg->GetLength();
            if (_len < _minLength)
                _minLength = _len;
        }
		return _minLength;
    }


    void Polygon::removeShortestEdge()
    {
        GS::Sort(m_segments.Begin(), m_segments.End(), [](Segment* s1, Segment* s2) -> bool {return s1->GetLength() < s2->GetLength(); });

        Segment* shortestSegment = m_segments[0];
        Segment* _prevSeg = shortestSegment->GetPrev();
        Segment* _nextSeg = shortestSegment->GetNext();

		logger.Log(GS::UniString("Removing segment: ") + shortestSegment->ToString() + GS::UniString("Length:") + str(boost::format("%-.2f ") % shortestSegment->GetLength()));

        intersectSegments( _prevSeg, _nextSeg);

        _prevSeg->SetEndIdx(shortestSegment->GetEndIdx());
        _nextSeg->SetStartIdx(shortestSegment->GetEndIdx());

        GS::Sort(m_segments.Begin(), m_segments.End(), [](Segment* s1, Segment* s2) -> bool {return s1->GetIdx() < s2->GetIdx(); });

        removeSegment(shortestSegment);

        //delete shortestSegment;
    }


    void Polygon::intersectSegments(Segment * io_prev, Segment * io_next)
    {
        using namespace Geometry;

        Sector lin1{ io_prev->GetStart()->ToCoord(), io_prev->GetEnd()->ToCoord() };
        Sector lin2{ io_next->GetStart()->ToCoord(), io_next->GetEnd()->ToCoord() };

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

        //delete i_segment;
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


    API_Polygon Polygon::toPoly() const
    { 
        API_ElementMemo mem{};
        API_Polygon poly{};

        getMemo(mem);

        poly.nCoords = BMGetHandleSize((GSHandle)mem.coords) / sizeof(API_Coord) - 1;
        poly.nSubPolys = BMGetHandleSize((GSHandle)mem.pends) / sizeof(Int32) - 1;
        poly.nArcs = BMGetHandleSize((GSHandle)mem.parcs) / sizeof(API_PolyArc);

		return poly;
    }
}

