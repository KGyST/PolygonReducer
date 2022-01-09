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
        unsigned int    m_startIdx;  // vertexID
        unsigned int    m_endIdx;    // vertexID
        Segment*        m_previous;
        Segment*        m_next;
        bool            m_startPoint;        // Is it a start of a subcontour

        Coord           m_center;
        double          m_angle;
        float           m_radius;

        //DELETE
        void init(int idx, int startIdx, int endIdx, const Coord& p_start, const Coord& p_end);


    public:
        Segment() {};
        inline Segment(const Coord& start, const Coord& end) { init(0, 0, 0, start, end); };
        Segment(int idx, const Coord& start, const Coord& end) { init(idx, 0, 0, start, end); };
        Segment(int idx, const API_Coord& start, const API_Coord& end) { init(idx, 0, 0, Coord(start), Coord(end)); };
        Segment(int idx, int startIdx, int endIdx, const Coord& start, const Coord& end) { init(idx, startIdx, endIdx, start, end); };
        //Segment(Sector sect);
        ~Segment();

        Coord *getStart() { return &m_start; };
        Coord *getEnd() { return &m_end; };

        const Coord midPoint() const;
        const Segment midPerp() const;

        const double getLength() { return sqrt(pow((m_end.getX() - m_start.getX()), 2) + pow((m_end.getY() - m_start.getY()), 2));  };
        //const Sector toSector();

        void SetArc(double angle);
        void SetArc(double angle, const Coord center);

        std::string toString();
    };
}

#endif // !S_SEGMENT_HPP
