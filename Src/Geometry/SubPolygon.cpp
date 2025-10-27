#include "SubPolygon.hpp"
#include "../PolygonReducer.template.hpp"
#include "Algorithms.hpp"

using namespace Geometry;

namespace S {
    SubPolygon::SubPolygon(Array<API_Coord>* coords, Array<API_PolyArc>* pars, Array<UInt32>* vertexIDs, bool i_isHole)
		: m_isHole(i_isHole)
    {
        Coord *centerPrev = NULL;
        Coord pointPrev = (*coords)[0], pointThis;

        Coord centerThis;
        UINT firstIdx = 0;
        double eps = 0, radEps = 0;

        const Segment s((*coords)[0], (*coords)[1]);
        Sector midPerpPrev = s.MidPerp().ToSector();

        for(UInt16 i = 1; i < coords->GetSize(); ++i)
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

    SubPolygon::SubPolygon(const SubPolygon& i_other)
    {
		Segment* _prevSeg = nullptr;

        m_segments.SetCapacity(i_other.m_segments.GetSize());
        for (Segment* s : i_other.m_segments)
        {
			Segment* newSeg = new Segment(*s);
            m_segments.Push(newSeg);
            if (_prevSeg != nullptr)
            {
                newSeg->SetPrev(_prevSeg);
                _prevSeg->SetNext(newSeg);
			}
            _prevSeg = newSeg;
        }

		m_segments[0]->SetPrev(m_segments.GetLast());
		m_segments.GetLast()->SetNext(m_segments[0]);
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

    Segment* SubPolygon::getShortestSegment() const
    { 
        Segment* _shortestSeg = nullptr;

        for (auto s : m_segments)
        {
            if (_shortestSeg == nullptr || s->GetLength() < _shortestSeg->GetLength())
                _shortestSeg = s;
		}

        return _shortestSeg;
    }

    //SubPolygon& SubPolygon::operator=(const SubPolygon& other) {
    //    if (this != &other) {
    //        SubPolygon temp(other);
    //        using std::swap;
    //        swap((*this).m_segments, temp.m_segments);
    //    }
    //    return *this;
    //}

    void SubPolygon::removeShortestSegment()
    {
        Segment* shortestSegment = getShortestSegment();
        Segment* _prevSeg = shortestSegment->GetPrev();
        Segment* _nextSeg = shortestSegment->GetNext();

        logger.Log(GS::UniString("Removing segment: ") + shortestSegment->ToString(LogFormat::Short));

        _prevSeg->Intersect(_nextSeg);

        removeSegment(shortestSegment);
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
    }

    void SubPolygon::Postprocess()
    {
        //ArcToPoly();
    }

    // ----

    void _createArc(Array<Segment*> &io_arc, const Coord* i_prevMidPerpIntSectPt, CircularArray <Segment*> &io_segments)
    {
        UINT minSize = 3;

        if (io_arc.GetSize() > minSize)
        {
            io_arc[0]->SetCenter(*i_prevMidPerpIntSectPt);
            io_arc[0]->SetEnd(*io_arc.GetLast()->GetEnd());

            for (Segment* sToDelete : io_arc.Slice(1, io_arc.GetSize()))
            {
                io_segments.DeleteAll(sToDelete);
            }
        }

        io_arc.Clear();
    }

    void SubPolygon::PolyToArc()
    {

        Segment* prevSegment = m_segments[-1];
        Coord* prevMidPerpIntSectPt = new Coord, * midPerpIntSectPt = new Coord;
        Array<Segment*> arc;
        UINT minSize = 3;

        for (Segment* s : m_segments)
        {
            std::optional<Coord> _int = s->IntersectMidPerp(prevSegment);

            if (_int)
                midPerpIntSectPt = &(*_int);
            else
				midPerpIntSectPt = nullptr;

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
                    _createArc(arc, prevMidPerpIntSectPt, m_segments);
                }

            if (midPerpIntSectPt)
                *prevMidPerpIntSectPt = *midPerpIntSectPt;

            prevSegment = s;
        }

        if (arc.GetSize() )
			// FIXME if arc goes beyond the end and continues at the beginning
            _createArc(arc, prevMidPerpIntSectPt, m_segments);

        delete prevSegment, prevMidPerpIntSectPt, midPerpIntSectPt;
    }

    void SubPolygon::RemoveCollinear()
    {
        Segment* prevSegment = m_segments[-1];

        for (Segment *s: m_segments)
        {
            if (s->IsCollinear(prevSegment) )
            {
                prevSegment->SetEnd(*s->GetEnd());
                m_segments.DeleteAll(s);
            }

            prevSegment = s;
        }
    }

    void SubPolygon::ArcToPoly(){}
}

