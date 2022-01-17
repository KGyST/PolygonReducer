#include "S_Polygon.hpp"
#include "PolygonReducer.template.hpp"

namespace PolygonReducer {
    S_Polygon::S_Polygon(const API_ElementMemo* p_memo)
        :   m_polylines (new GS::Array <S_Polyline*>())
        ,   m_segments  (new GS::Array <S::Segment*>())
    {
        S::Array<S::Segment> _segments ;
        S::Array<Int32> _pends(p_memo->pends);
        S::Array<API_Coord> _coords(p_memo->coords);
        S::Array<API_PolyArc> _parcs(p_memo->parcs);
        S::Array<UInt32> _vertexIDs(p_memo->vertexIDs);

        S::Segment _segment;
        int _idx = 0;

        for (UInt32 i = 1; i < _coords.GetSize(); i++)
        {
            API_Coord c1 (_coords[i - 1]) ;
            API_Coord c2 (_coords[i]);
            S::Segment _segment (_idx++, _vertexIDs[i - 1], _vertexIDs[i], c1, c2);

            _segments.Push(_segment);
            m_segments->Push(&_segment);
        }

        for (UInt32 i = 0; i < _parcs.GetSize(); i++)
        {
            _segments[_parcs[i].begIndex].SetArc(_parcs[i].arcAngle);
        }

        for (UInt32 i = 0; i < _pends.GetSize() - 1; i++)
        {
            GS::Array<API_Coord> *_coordsFiltered = ArraySlice<API_Coord>(_coords, _pends[i] + 1, _pends[i + 1]);
            GS::Array<UInt32>    *_vertexIDsFiltered = ArraySlice<UInt32>(_vertexIDs, _pends[i] + 1, _pends[i + 1]);
            GS::Array<API_PolyArc> *_parcsFiltered = new GS::Array<API_PolyArc>();

            auto _it = _parcs.Enumerate();
            while (_it != NULL)
            {
                if (_it->begIndex >= _pends[i] && _it->endIndex <= _pends[i + 1])
                {
                    _parcsFiltered->Push(*_it);
                }
                _it++;
            }

            m_polylines->Push(new S_Polyline(_coordsFiltered, _parcsFiltered, _vertexIDsFiltered));
        }
    }


    S_Polygon::~S_Polygon() {
    }


    //API_ElementMemo* S_Polygon::getResultMemo()
    //{
    //    API_ElementMemo* _memo{};               //TODO get from selection
    //    S::Array<API_Coord*> _coords;
    //    S::Array<API_PolyArc*> _parcs;
    //    S::Array<Int32*> _pends;
    //    S::Array<UInt32*> _vertIDs;

    //    _coords.Push(&(*m_segments)[0]->GetStart()->ToAPICoord());

    //    for (UInt32 i = 1; i < m_polylines->GetSize(); i++)
    //    {
    //        S_Polyline* _pl = (*m_polylines)[i];

    //        for (UInt32 i = 1; i < _pl->m_segments->GetSize(); i++)
    //        {
    //            S::Segment* _seg = (*m_segments)[i];
    //            _coords.Push(&_seg->GetEnd()->ToAPICoord());
    //            if (_seg->GetRad() > EPS)
    //            {
    //                API_PolyArc _parc{ 0, 0, _seg->GetAng() };  //TODO indexes
    //                _parcs.Push(&_parc);
    //            }
    //        }
    //        _pends.Push(0);     //TODO indexes
    //    }

    //    _memo->coords = _coords.ToNeigs();
    //    _memo->parcs = _parcs.ToNeigs();
    //    _memo->pends = _pends.ToNeigs();
    //    _memo->vertexIDs = _vertIDs.ToNeigs();

    //    return _memo;
    //}


    std::string S_Polygon::getGDLcode() {
        using namespace std;

        string result = "";

        for (UInt16 i = 0; i < m_segments->GetSize(); i++)
        {
            S::Segment* _s = (*m_segments)[i];
            result += _s->ToString();
        }

        return result;
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

