#pragma once
#ifndef UTILS2_HPP
#define UTILS2_HPP
    class nDigits {
    public:
        int m_head;
        int m_tail;
        nDigits(const int p_head, const int p_tail) :
            m_head(p_head),
            m_tail(p_tail) {}
        bool operator==(const nDigits p_nDigits);
    };

    nDigits getDigits(const double p_number);
#endif // !UTILS2_HPP

