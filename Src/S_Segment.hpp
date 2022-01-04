#pragma once

#ifndef S_SEGMENT_HPP
#define S_SEGMENT_HPP

#define ACExtension
#include "ACAPinc.h"					// also includes APIdefs.h
#include "Point2DData.h"
#include "Sector2DData.h"
#include <string>
namespace PolygonReducer {

    class S_Segment
    {
    private:
        unsigned int    m_idx;
        API_Coord       m_start;
        API_Coord       m_end;
        double          m_length;

        unsigned int    m_status1;
        unsigned int    m_status2;
        //unsigned int prvIdx;
        //unsigned int nxtIdx;
        unsigned int    m_startIdx;  // vertexID
        unsigned int    m_endIdx;    // vertexID
        S_Segment*      m_previous;
        S_Segment*      m_next;
        bool            m_startPoint;        // Is it a start of a subcontour

        API_Coord*      m_center;
        double          m_angle;
        float           m_radius;

        //DELETE
        void init(int idx, int startIdx, int endIdx, API_Coord& p_start, API_Coord& p_end);


    public:
        S_Segment() {};
        inline S_Segment(API_Coord& start, API_Coord& end) { init(0, 0, 0, start, end); };
        S_Segment(int idx, API_Coord& start, API_Coord& end) { init(idx, 0, 0, start, end); };
        S_Segment(int idx, int startIdx, int endIdx, API_Coord& start, API_Coord& end) { init(idx, startIdx, endIdx, start, end); };
        ~S_Segment();

        const API_Coord *getStart() { return &m_start; };
        const API_Coord *getEnd() { return &m_end; };

        void SetArc(double angle);
        void SetArc(double angle, Coord center);
        std::string toString();
    };
}
#endif // !S_SEGMENT_HPP
