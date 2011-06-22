#ifndef BOOST_ARRAY_STEPPER_MK_IOTA_HPP_INCLUDED
#define BOOST_ARRAY_STEPPER_MK_IOTA_HPP_INCLUDED
#include <numeric>
#include <vector>
namespace boost
{
namespace array_stepper
{
  template
  < typename Value
  >
  std::vector<Value>
mk_iota
  ( Value first
  , Value last
  )
  {
      int size=last-first;
      std::vector<Value> result(abs(size));
      if(size>0)
      {
          std::iota( result.begin(), result.end(), first);
      }
      else
      {
          std::iota( result.rbegin(), result.rend(), last);
      }
      return result;
  }
}//exit array_stepper namespace
}//exit boost namespace
#endif
