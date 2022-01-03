#include "Utils2.hpp"
#include <math.h>
#include <stdlib.h>

nDigits getDigits(const double p_number) {
    nDigits result(int(log10(abs(p_number))), 0);
    const int NMAX = 10;

    int _r = 0;
    double _test = 0;
    do
    {
        _test = p_number * (pow(double(10), _r++));
    } while ((_test == double(int(_test))) && _r < NMAX);

    result.m_tail = _r - 1;

    return result;
}

bool nDigits::operator==(const nDigits p_nDigits) {
    return p_nDigits.m_head == m_head && p_nDigits.m_tail == m_tail;
}