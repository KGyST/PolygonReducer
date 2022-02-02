#include "S_Polygon.hpp"
#include "PolygonReducer.template.hpp"

namespace PolygonReducer {
    void S_Polygon::MoveAllPoints()
    {
        for (UINT i = 0; i < m_segments.GetSize(); i++ )
        {
            S::Coord _s (*m_segments[i].GetStart());
            m_segments[i].SetStart(S::Coord(_s.GetX() + 1.00, _s.GetY() + 1.00) );
            S::Coord _e (*m_segments[i].GetEnd());
            m_segments[i].SetEnd(S::Coord(_e.GetX() + 1.00, _e.GetY() + 1.00) );
        }
    }

    S_Polygon::S_Polygon(const API_ElementMemo* p_memo)
        :   m_polylines (*new GS::Array <S_Polyline*>())
        ,   m_segments  (*new GS::Array <S::Segment>())
    {
        S::Segment _segment;
        
        S::Array<Int32> _pends(p_memo->pends);
        S::Array<API_Coord> _coords(p_memo->coords);
        S::Array<API_PolyArc> _parcs(p_memo->parcs);
        S::Array<UInt32> _vertexIDs(p_memo->vertexIDs);

        int _idx = 0;

        if (    _coords[0].x - EPS < 0.00
            &&  _coords[0].x + EPS > 0.00
            &&  _coords[0].y - EPS < 0.00
            &&  _coords[0].y + EPS > 0.00)
            m_isPolygon = true;
        else
            m_isPolygon = false;

        for (UInt32 i = 2; i < _coords.GetSize(); i++)
        {
            API_Coord c1 (_coords[i - 1]) ;
            API_Coord c2 (_coords[i]);
            S::Segment _segment (_idx++, _vertexIDs[i - 1], _vertexIDs[i], c1, c2);

            m_segments.Push(_segment);
        }

        //for (UInt32 i = 0; i < _parcs.GetSize(); i++)
        //{
        //    _segments[_parcs[i].begIndex].SetArc(_parcs[i].arcAngle);
        //}

        //for (UInt32 i = 0; i < _pends.GetSize() - 1; i++)
        //{
        //    GS::Array<API_Coord> *_coordsFiltered = ArraySlice<API_Coord>(_coords, _pends[i] + 1, _pends[i + 1]);
        //    GS::Array<UInt32>    *_vertexIDsFiltered = ArraySlice<UInt32>(_vertexIDs, _pends[i] + 1, _pends[i + 1]);
        //    GS::Array<API_PolyArc> *_parcsFiltered = new GS::Array<API_PolyArc>();

        //    auto _it = _parcs.Enumerate();
        //    while (_it != NULL)
        //    {
        //        if (_it->begIndex >= _pends[i] && _it->endIndex <= _pends[i + 1])
        //        {
        //            _parcsFiltered->Push(*_it);
        //        }
        //        _it++;
        //    }

        //    m_polylines.Push(new S_Polyline(_coordsFiltered, _parcsFiltered, _vertexIDsFiltered));
        //}
    }


    S_Polygon::~S_Polygon() {
    }


    void S_Polygon::updateMemo(API_ElementMemo* o_memo)
    {
        S::Array<API_Coord> _coords;
        S::Array<API_PolyArc> _parcs;
        S::Array<Int32> _pends;
        S::Array<UInt32> _vertIDs;

        API_Coord _ac = m_segments[0].GetStart()->ToAPICoord();

        _coords.Push(_ac);

        for (UInt32 i = 1; i < m_polylines.GetSize(); i++)
        {
            S_Polyline* _pl = m_polylines[i];

            for (UInt32 i = 1; i < _pl->m_segments->GetSize(); i++)
            {
                S::Segment _seg = m_segments[i];
                _ac = _seg.GetEnd()->ToAPICoord();
                _coords.Push(_ac);

                if (_seg.GetRad() > EPS)
                {
                    API_PolyArc _parc = { 0, 0, _seg.GetAng() };  //TODO indexes
                    _parcs.Push(_parc);
                }
            }
            _pends.Push(0);     //TODO indexes
        }

        BNZeroMemory(o_memo, sizeof(API_ElementMemo));

        o_memo->coords = _coords.ToNeigs();
        o_memo->parcs = _parcs.ToNeigs();
        o_memo->pends = _pends.ToNeigs();
        o_memo->vertexIDs = _vertIDs.ToNeigs();
    }


    std::string S_Polygon::getGDLcode() {
        using namespace std;

        string result = "";

        for (UInt16 i = 0; i < m_segments.GetSize(); i++)
        {
            S::Segment _s = m_segments[i];
            result += _s.ToString();
        }

        return result;
    }

    API_ElementMemo S_Polygon::getMemo()
    {
        S::Array<API_Coord> _coords;
        S::Array<API_PolyArc> _parcs;
        S::Array<Int32> _pends;
        S::Array<UInt32> _vertIDs;

        API_Coord _ac = *new API_Coord (S::Coord(m_isPolygon ? 0.00 : -1.00, 0.00).ToAPICoord());
        _coords.Push(_ac);

        _ac = *new API_Coord(m_segments[0].GetStart()->ToAPICoord());

        _coords.Push(_ac);
        _pends.Push((Int32)0);
        _vertIDs.Push((UInt32)0);

        //for (UInt32 i = 1; i < m_polylines.GetSize(); i++)
        //{
        //    S_Polyline* _pl = m_polylines[i];

        //    for (UInt32 i = 1; i < _pl->m_segments->GetSize(); i++)
        //    {
        //        S::Segment* _seg = m_segments[i];
        //        _ac = _seg->GetEnd()->ToAPICoord();
        //        _coords.Push(&_ac);

        //        if (_seg->GetRad() > EPS)
        //        {
        //            API_PolyArc _parc = { 0, 0, _seg->GetAng() };  //TODO indexes
        //            _parcs.Push(&_parc);
        //        }
        //    }
        //    _pends.Push(0);     //TODO indexes
        //}

        UInt32 maxId = 0;

        for (UInt32 i = 0; i < m_segments.GetSize(); i++)
        {
            S::Coord _c = *m_segments[i].GetEnd();
            //_c.SetX(_c.GetX() + 1.00);
            //_c.SetY(_c.GetY() + 1.00);
            _ac = *new API_Coord(_c.ToAPICoord());
            _coords.Push(_ac);
            _vertIDs.Push((UInt32)i+1);
            maxId = i+1;
        }

        _vertIDs.Push((UInt32)1);                               //TODO
        _pends.Push((Int32)m_segments.GetSize() + 1);
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


    void S_Polygon::setupArcs(void)
        //going through segments and unify them as arcs if they are so
    {
    }
}

