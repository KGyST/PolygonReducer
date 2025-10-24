#pragma once

#ifndef S_SEGMENT_HPP
#define S_SEGMENT_HPP

#define ACExtension
#include "APIEnvir.h"
#include "ACAPinc.h"					// also includes APIdefs.h
#include "Point2DData.h"
#include "Sector2DData.h"
#include <string>
#include "Coord.hpp"
#include <optional>


enum class LogFormat {
    Default,
    Index,
    Short,
    Detailed
};


namespace S {
    class Segment
    {
    private:
        unsigned int    m_idx;
        Coord           m_start;
        Coord           m_end;

        unsigned int    m_status1;
        unsigned int    m_status2;
        //unsigned int prvIdx;
        //unsigned int nxtIdx;
        unsigned int    m_startIdx;         // vertexID
        unsigned int    m_endIdx;           // vertexID
        Segment*        m_previous;
        Segment*        m_next;
        bool            m_startPoint;       // Is it a start of a subcontour
        //For arcs:
        std::optional<Coord>           m_center;
        //double          m_angle;
        //float           m_radius;

        //DELETE
        void init(int idx, int startIdx, int endIdx, const Coord& p_start, const Coord& p_end);


    public:
        inline Segment() {};
        inline Segment(const Coord& start, const Coord& end) { init(0, 0, 0, start, end); };
        inline Segment(int idx, const Coord& start, const Coord& end) { init(idx, 0, 0, start, end); };
        inline Segment(int idx, const API_Coord& start, const API_Coord& end) { init(idx, 0, 0, Coord(start), Coord(end)); };
        inline Segment(int idx, int startIdx, int endIdx, const Coord& start, const Coord& end) { init(idx, startIdx, endIdx, start, end); };
        //Segment(int idx=0, int startIdx=0, int endIdx=0, const Coord& start=NULL, const Coord& end = NULL, Segment* prev = NULL, Segment* next =NULL);
        //Segment(Sector sect);

        //Segment& operator=(const Segment&) noexcept;

        unsigned int GetIdx() const { return m_idx; }
        const Coord* GetStart() const { return &m_start; }
        const Coord* GetEnd() const { return &m_end; }
        Segment* GetPrev() const { return m_previous; }
        Segment* GetNext() const { return m_next; }
        unsigned int GetStartIdx() const { return m_startIdx; }
        unsigned int GetEndIdx() const { return m_endIdx; }
        std::optional<double>   GetRad() const;
        std::optional<double>  GetAng() const;
        std::optional<Coord>  GetCenter() const { return m_center; }
        void SetCenter(const Coord i_center) { m_center = i_center; }

        const Coord MidPoint() const;
        const Segment MidPerp() const;

        const double GetLength() const;
        const Sector toSector() const;

        void SetArc(double angle);
        //void SetArc(double angle, const Coord center);

        inline void SetStart(const Coord& start) { m_start = start; };
        inline void SetEnd(const Coord& end) { m_end = end; };
        inline void SetStartIdx(const unsigned int startIdx) { m_startIdx = startIdx; };
        inline void SetEndIdx(const unsigned int endIdx) { m_endIdx = endIdx; };

        inline void SetPrev(Segment* prev) { m_previous = prev; };
        inline void SetNext(Segment* next) { m_next = next; };

        void Segment::intersect(Segment* io_other);
        
        inline std::string ToString() const { return ToString(LogFormat::Default); };
        std::string ToString(LogFormat) const;

        //bool S::Segment::operator()(S::Segment *s1, S::Segment *s2);
    };
}

#endif // !S_SEGMENT_HPP
