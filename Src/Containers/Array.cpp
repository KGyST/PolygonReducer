#include "Array.hpp"


template <class Type>
void S::Array<Type>::sort(GS::UIndex start, GS::UIndex end, bool (*funcSort)())
{
  auto _start = ConstIterator(*this, start);
  auto _end = ConstIterator(*this, end);
  GS::Sort(_start, _end, funcSort);
}

