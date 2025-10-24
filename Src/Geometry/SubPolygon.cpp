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
        Sector midPerpPrev = s.MidPerp().toSector();

        for(UInt16 i = 1; i < coords->GetSize(); ++i)
        {
            pointThis = (*coords)[i];
            Segment s(pointPrev, pointThis);

            const Sector midPerpThis = s.MidPerp().toSector();

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

    std::string SubPolygon::ToString() const
    {
		std::string result = "";

        for(auto s : m_segments)
        {
            result += s->ToString(LogFormat::Index) + std::string(" ");
		}

		// Remove last space
		if (!result.empty())
		    result.pop_back();

		return result;
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

        _prevSeg->intersect(_nextSeg);

        _prevSeg->SetEndIdx(shortestSegment->GetEndIdx());

        removeSegment(shortestSegment);
    }

    double SubPolygon::getShortestEdgeLength() const
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
}

