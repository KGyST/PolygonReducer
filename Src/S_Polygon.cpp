#include "S_Polygon.hpp"
#include "PolygonReducer.hpp"

// ---------------------------------- Prototypes -------------------------------

S_Polygon::S_Polygon(const API_ElementMemo* p_memo) {
    polylines = new GS::Array <S_Polyline>();
    segments = new GS::Array <S_Segment>();

    GS::Array<Int32>        _pends = *new GS::Array<Int32>();
    GS::Array<API_Coord>   _coords = *new GS::Array<API_Coord>();
    GS::Array<API_PolyArc>  _parcs = *new GS::Array<API_PolyArc>();
    GS::Array<UInt32>   _vertexIDs = *new GS::Array<UInt32>();
    GS::Array<S_Segment> _segments = *new GS::Array<S_Segment>();

    UInt32 err;

    err = ConvertToGSArray<Int32, Int32>(p_memo->pends, &_pends);
    err = ConvertToGSArray<API_Coord, API_Coord>(p_memo->coords, &_coords);
    err = ConvertToGSArray<API_PolyArc, API_PolyArc>(p_memo->parcs, &_parcs);
    err = ConvertToGSArray<UInt32, UInt32>(p_memo->vertexIDs, &_vertexIDs);

    S_Segment _segment;
    int _idx = 0;

    for (UInt32 i = 1; i < _coords.GetSize(); i++)
    {
        _segment = *new S_Segment(_idx++, _vertexIDs[i - 1], _vertexIDs[i], _coords[i - 1], _coords[i]);

        for (UInt32 j = 0; j < _parcs.GetSize(); j++)
        {
            if (_parcs[j].begIndex >= _pends[i] && _parcs[j].endIndex < _pends[i + 1])
            {
                _segment.SetArc(_parcs[j].arcAngle);
            }
        }

        _segments.Push(_segment);
    }

    for (UInt32 i = 0; i < _pends.GetSize() - 1; i++)
    {
        GS::Array<API_Coord> _c = *ArraySlice<API_Coord>(_coords, _pends[i] + 1, _pends[i + 1]);
        GS::Array<UInt32>    _v = *ArraySlice<UInt32>(_vertexIDs, _pends[i] + 1, _pends[i + 1]);
        GS::Array<API_PolyArc> _parcsFiltered = *new GS::Array<API_PolyArc>();
        _parcsFiltered.EnsureSize(_pends[i + 1] - _pends[i] + 1);

        //polylines->Push(*new S_Polyline(_c, _parcsFiltered, _v));
    }
}

S_Polygon::~S_Polygon() {}

