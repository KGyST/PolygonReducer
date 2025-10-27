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
        swap(a.m_isPolygon, b.m_isPolygon);
    }

	// Constructors, destructors, assignment operators

    // Main ctor
    Polygon::Polygon(const API_ElementMemo* p_memo)
        : m_subpolys()
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

        if (_coords[0].x - EPS < 0.00
            && _coords[0].x + EPS > 0.00)
            m_isPolygon = true;
        else
            // Polyline
            m_isPolygon = false;

        for (UInt32 i = 1; i < _pends.GetSize(); ++i)
        {
            SubPolygon* _sp = new SubPolygon;
            Segment* _sPrev = nullptr;
            Segment* _segment = nullptr;

            for (UInt32 j = (UInt32)_pends[i - 1] + 1; j < (UInt32)_pends[i]; ++j)
            {
                API_Coord c1(_coords[j]);
                API_Coord c2(_coords[j + 1]);
                _segment = new Segment(_idx++, _vertexIDs[j], _vertexIDs[j + 1], c1, c2);

                if (_archTable.ContainsKey(j))
                {
                    _segment->SetAng(_archTable[j]);
                }

                if (_sPrev)
                {
                    _segment->SetPrev(_sPrev);
                    _sPrev->SetNext(_segment);
                }

                _sp->m_segments.Push(_segment);

                _sPrev = _segment;
            }

            if (_segment)
            {
                _sp->m_segments[0]->SetPrev(_segment);
                _segment->SetNext(_sp->m_segments[0]);
            }

            _sp->m_isHole = true;
            _sp->Preprocess();
            m_subpolys.Push(_sp);
        }

        m_subpolys[0]->m_isHole = false;
    }

    Polygon::Polygon(const Polygon& other)
        : m_pointCount(other.m_pointCount)
        , m_isPolygon(other.m_isPolygon)
    {
        for (auto _sp : other.m_subpolys)
        {
            m_subpolys.Push(new SubPolygon(*_sp));
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
    // 
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

    Polygon::~Polygon() {
        for (SubPolygon* _sp: m_subpolys)
            delete _sp;
    } 

	// Accessors, mutators and other methods

    std::string Polygon::getGDLcode() const
    {
        using namespace std;

        string result = "";

        for (SubPolygon* _sp: m_subpolys)
        {
            result += _sp->ToString();
		}

        //for each (Segment* _s in m_segments)
        //{
        //    result += _s->ToString();
        //}

        return result;
    }

    void Polygon::GetMemo(API_ElementMemo& io_memo) const
    {
        Array<API_Coord> _coords;
        Array<API_PolyArc> _parcs;
        Array<Int32> _pends;
        Array<UInt32> _vertIDs;

        //1st coord special
        _coords.Push(API_Coord(Coord(m_isPolygon ? 0.00 : -1.00, 0.00).ToAPICoord())); 

        UInt32 maxId = 0;
        _pends.Push(0);
        _vertIDs.Push(0);

        for (SubPolygon* sp: m_subpolys)
        {
            _coords.Push(sp->m_segments[0]->GetStart()->ToAPICoord());
            UInt32 iFirstSegmentIdx = ++maxId;
            _vertIDs.Push(iFirstSegmentIdx);
            
            for (Segment* _segment: sp->m_segments)
            {
                if  (   _segment->GetCenter())
                {
                    API_PolyArc _arc;
                    _arc.arcAngle = *_segment->GetAng();
                    _arc.begIndex = maxId;
                    _arc.endIndex = maxId + 1;
                    _parcs.Push(_arc);
                }

                _vertIDs.Push(maxId++);
                _coords.Push(_segment->GetEnd()->ToAPICoord());
            }

            _pends.Push(maxId);
        }

        _vertIDs[0] = maxId;

        //API_ElementMemo resultMemo;
        //BNZeroMemory(&resultMemo, sizeof(API_ElementMemo));

        io_memo.coords = _coords.ToNeigs();
        io_memo.parcs = _parcs.ToNeigs();
        io_memo.pends = _pends.ToNeigs();
        io_memo.vertexIDs = _vertIDs.ToNeigs();
    }

    double Polygon::GetShortestEdgeLength() const
    // Length of the shortest edge
    {
        if (m_subpolys.GetSize() == 0)
            return 0.00;

        double _minLength = DBL_MAX;

        for (UINT i = 1; i < m_subpolys.GetSize(); ++i)
        {
            double _len = m_subpolys[i]->GetShortestEdgeLength();
            if (_len < _minLength)
                _minLength = _len;
		}

		return _minLength;
    }

    SubPolygon* Polygon::getSPolyHavingShortestSegment() const
    {
        SubPolygon* _resultSP = nullptr;

        for (SubPolygon* _sp : m_subpolys)
        {
            if (_resultSP == nullptr || _sp->getShortestSegment()->GetLength() < _resultSP->getShortestSegment()->GetLength())
                _resultSP = _sp;
        }

		return _resultSP;
    }

    void Polygon::removeShortestSegment()
    {
        SubPolygon* SPhavingShortestSegment = getSPolyHavingShortestSegment();
		SPhavingShortestSegment->removeShortestSegment();

        if (!SPhavingShortestSegment->isValid())
        {
            m_subpolys.DeleteAll(SPhavingShortestSegment);

            logger.Log(GS::UniString("Removing SubPoly: ") + SPhavingShortestSegment->ToString());
		}
    }

    void Polygon::intersectSegments(Segment * io_prev, Segment * io_next)
    {
        using namespace Geometry;

        Sector lin1{ io_prev->GetStart()->ToCoord(), io_prev->GetEnd()->ToCoord() };
        Sector lin2{ io_next->GetStart()->ToCoord(), io_next->GetEnd()->ToCoord() };

        ::Coord xc(0, 0);

        double eps = 0, radEps = 0;

        if (XLinesEps(lin1, lin2, &xc, eps, radEps))
        {
            io_prev->SetEnd(xc);
            io_next->SetStart(xc);
            io_prev->SetNext(io_next);
            io_next->SetPrev(io_prev);
        }
    } 

    void Polygon::setPointCount(unsigned int i_count)
    {
        for (UINT i = getPointCount(); i > i_count; --i)
        {
            removeShortestSegment();
        }
    }

    USize Polygon::getPointCount() const
    {
		USize count = 0;

        for (auto sp : m_subpolys)
        {
            count += sp->m_segments.GetSize();
        }

		return count;
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

        GetMemo(mem);

        poly.nCoords = BMGetHandleSize((GSHandle)mem.coords) / sizeof(API_Coord) - 1;
        poly.nSubPolys = BMGetHandleSize((GSHandle)mem.pends) / sizeof(Int32) - 1;
        poly.nArcs = BMGetHandleSize((GSHandle)mem.parcs) / sizeof(API_PolyArc);

		return poly;
    }
}

