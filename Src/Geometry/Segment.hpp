#pragma once

#ifndef S_SEGMENT_HPP
#define S_SEGMENT_HPP

#define ACExtension
#include "ACAPinc.h"					// also includes APIdefs.h
#include "Point2DData.h"
#include "Sector2DData.h"
#include <string>
#include "Coord.hpp"

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
        Coord           m_center;
        double          m_angle;
        float           m_radius;

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
        ~Segment();

        inline unsigned int GetIdx() const { return m_idx; };
        inline const Coord* GetStart() const { return &m_start; };
        inline const Coord* GetEnd() const { return &m_end; };
        inline Segment* GetPrev() const { return m_previous; };
        inline Segment* GetNext() const { return m_next; };
        inline unsigned int GetStartIdx() const { return m_startIdx; };
        inline unsigned int GetEndIdx() const { return m_endIdx; };
        inline float   GetRad() const { return m_radius; };
        inline double  GetAng() const { return m_angle; };

        const Coord MidPoint() const;
        const Segment MidPerp() const;

        const double GetLength() const;
        //const Sector toSector();

        void SetArc(double angle);
        void SetArc(double angle, const Coord center);

        inline void SetStart(const Coord& start) { m_start = start; };
        inline void SetEnd(const Coord& end) { m_end = end; };
        inline void SetStartIdx(const unsigned int startIdx) { m_startIdx = startIdx; };
        inline void SetEndIdx(const unsigned int endIdx) { m_endIdx = endIdx; };

        inline void SetPrev(Segment* prev) { m_previous = prev; };
        inline void SetNext(Segment* next) { m_next = next; };

        std::string ToString() const;

        //bool S::Segment::operator()(S::Segment *s1, S::Segment *s2);
    };
}

#endif // !S_SEGMENT_HPP
