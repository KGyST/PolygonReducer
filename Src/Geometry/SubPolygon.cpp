#include "SubPolygon.hpp"
#include "../PolygonReducer.template.hpp"
#include "Algorithms.hpp"
#include "Coord.hpp"
#include <boost/format.hpp>

using namespace Geometry;
using namespace std;

namespace S {
  SubPolygon::SubPolygon(Array<API_Coord>* coords, Array<API_PolyArc>* pars, Array<UInt32>* vertexIDs, bool i_isHole)
    : m_isHole(i_isHole)
  {
    Coord* centerPrev = NULL;
    Coord pointPrev = (*coords)[0], pointThis;

    Coord centerThis;
    UINT firstIdx = 0;
    double eps = 0, radEps = 0;

    const Segment s((*coords)[0], (*coords)[1]);
    Sector midPerpPrev = s.MidPerp().ToSector();

    for (UInt16 i = 1; i < coords->GetSize(); ++i)
    {
      pointThis = (*coords)[i];
      Segment s(pointPrev, pointThis);

      const Sector midPerpThis = s.MidPerp().ToSector();

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

  SubPolygon::SubPolygon(const SubPolygon& other) {
    _deepCopy(other);
  }

  //  SubPolygon::SubPolygon(SubPolygon&& i_other) noexcept
  //  {
  //      m_segments.SetCapacity(i_other.m_segments.GetSize());
  //      for (Segment* s : i_other.m_segments)
  //      {
  //          m_segments.Push(s);
  //      }
    //i_other.m_segments.Clear();
  //  }

  Segment* SubPolygon::GetShortestSegment() const
  {
    Segment* _shortestSeg = nullptr;

    for (auto s : m_segments)
    {
      if (_shortestSeg == nullptr || s->GetLength() < _shortestSeg->GetLength())
        _shortestSeg = s;
    }

    return _shortestSeg;
  }

  SubPolygon& SubPolygon::operator=(const SubPolygon& other) {
    if (this != &other) {
      for (Segment* s : m_segments)
        delete s;
      m_segments.Clear();

      _deepCopy(other);
    }
    return *this;
  }

  void SubPolygon::_deepCopy(const SubPolygon& other)
  {
    GS::HashTable<Segment*, Segment*> usedSegments;
    m_segments.SetCapacity(other.m_segments.GetSize());

    for (Segment* oldSeg : other.m_segments) {
      Segment* newSeg = new Segment(*oldSeg);
      m_segments.Push(newSeg);

      usedSegments.Add(oldSeg, newSeg);
    }

    for (Segment* oldSeg : other.m_segments) {
      Segment* pNewSeg = usedSegments.Get(oldSeg);

      Segment* prevOfOld = oldSeg->GetPrev();
      Segment* prevSeg = usedSegments.Get(prevOfOld);
      pNewSeg->SetPrev(prevSeg);

      Segment* nextOfOld = oldSeg->GetNext();
      Segment* nextSeg = usedSegments.Get(nextOfOld);
      pNewSeg->SetNext(nextSeg);
    }
  }

  void SubPolygon::RemoveShortestSegment()
  {
    Segment* shortestSegment = GetShortestSegment();
    Segment* _prevSeg = shortestSegment->GetPrev();
    Segment* _nextSeg = shortestSegment->GetNext();

    logger.Log(GS::UniString("Removing segment: ") + shortestSegment->ToString(LogFormat::Short));

    _prevSeg->Intersect(_nextSeg);

    RemoveSegment(shortestSegment);
  }

  double SubPolygon::GetShortestEdgeLength() const
  {
    double _minLength;

    if (m_segments.GetSize() == 0)
      return 0.0;
    else
      _minLength = DBL_MAX;

    for (auto s : m_segments)
    {
      double l = s->GetLength();

      if (s->GetLength() < _minLength)
        _minLength = l;
    }

    return _minLength;
  }

  std::string SubPolygon::ToString() const
  {
    std::string result = "";

    for (auto s : m_segments)
    {
      result += s->ToString(LogFormat::Default) + std::string(" ");
    }

    // Remove last space
    if (!result.empty())
      result.pop_back();

    return result;
  }

  // --

  void SubPolygon::Preprocess()
  {
    RemoveCollinear();
    PolyToArc();
    logger.Log(GS::UniString("After Preprocess: " + ToString()));
  }

  void SubPolygon::Postprocess()
  {
    //ArcToPoly();
  }

  // ----

  //void _createArc(Array<Segment*> &io_arc, const Coord* i_prevMidPerpIntSectPt, Array <Segment*> &io_delArc)
  //{
  //    UINT minSize = 3;

  //    if (io_arc.GetSize() > minSize)
  //    {
  //        io_arc[0]->SetCenter(*i_prevMidPerpIntSectPt);
  //        io_arc[0]->SetEnd(*io_arc.GetLast()->GetEnd());

  //        for (Segment* sToDelete : io_arc.Slice(1, io_arc.GetSize()))
  //        {
  //          io_delArc.Push(sToDelete);
  //        }
  //    }

  //    io_arc.Clear();
  //}

  void SubPolygon::PolyToArc()
  {
    Segment* prevSegment = m_segments[-1];
    unique_ptr<Coord> prevMidPerpIntSectPt = make_unique<Coord>(), midPerpIntSectPt = make_unique<Coord>() ;
    Array<Segment*> arc, _delarc;
    UINT minSize = 3;

    for (Segment* s : m_segments)
    {
      std::optional<Coord> _int = s->IntersectMidPerp(prevSegment);

      if (_int)
        *midPerpIntSectPt = *_int;
      else
        midPerpIntSectPt.reset();

      if (prevMidPerpIntSectPt && midPerpIntSectPt)
        if (*prevMidPerpIntSectPt == *midPerpIntSectPt)
        {
          if (arc.GetSize() == 0)
          {
            std::optional<int> _index = m_segments.IndexOf(s);
            if (_index)
            {
              arc.Push(m_segments[*_index - 2]);
              arc.Push(m_segments[*_index - 1]);
            }
          }

          arc.Push(s);
        }
        else
        {
          //_createArc(arc, prevMidPerpIntSectPt, _delarc);

          UINT minSize = 3;

          if (arc.GetSize() > minSize)
          {
            //arc[0]->SetCenter(*prevMidPerpIntSectPt);
            arc[0]->SetEnd(*arc.GetLast()->GetEnd());
            Segment* _nextSegment = arc.GetLast()->GetNext();
            arc[0]->SetNext(_nextSegment);
            _nextSegment->SetPrev(arc[0]);
            std::optional<double> a1 = AngleBetween(arc[0]->GetStart()->ToCoord(), prevMidPerpIntSectPt->ToCoord(), arc[0]->GetEnd()->ToCoord());
            if (a1)
            {
              arc[0]->SetAng(-*a1);

              for (Segment* sToDelete : arc.Slice(1, arc.GetSize()))
              {
                _delarc.Push(sToDelete);
              }
            }
            else
              logger.Log("Error calculating arc angle in PolyToArc", 0, LogLev_ERROR);
          }

          arc.Clear();
        }

      if (midPerpIntSectPt)
        *prevMidPerpIntSectPt = *midPerpIntSectPt;

      prevSegment = s;
    }

    // FIXME if arc goes beyond the end and continues at the beginning

    //delete prevSegment;

    for (Segment* sToDelete : _delarc)
    {
      m_segments.DeleteAll(sToDelete);
    }

    logger.Log(GS::UniString("After PolyToArc: " + ToString()));
  }

  void SubPolygon::RemoveCollinear()
  {
    Segment* prevSegment = m_segments[-1];

    for (Segment* s : m_segments)
    {
      if (s->IsCollinear(prevSegment))
      {
        prevSegment->SetEnd(*s->GetEnd());
        m_segments.DeleteAll(s);
      }

      prevSegment = s;
    }
  }

  void SubPolygon::ArcToPoly() {}

  double SubPolygon::_SignedArea() const {
    double area = 0.0;
    for (Segment* s : m_segments) {
      const Coord *p1 = s->GetStart() ;
      const Coord *p2 = s->GetEnd() ;
      area += (p1->GetX()  * p2->GetY() - p2->GetX() * p1->GetY());
    }
    return area / 2.0;
  }

  bool SubPolygon::IsClockWise() const {
    return _SignedArea() < 0.0;
  }
}

