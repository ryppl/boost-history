#ifndef BOOST_ARRAY_STEPPER_ARRAY_OWN_PRINT_HPP_INCLUDED
#define BOOST_ARRAY_STEPPER_ARRAY_OWN_PRINT_HPP_INCLUDED
#include <boost/array_stepper/array_own.hpp>
namespace boost
{
namespace array_stepper
{

  template
  < typename Value
  , typename Index
  , array_type ArrType
  >
  std::ostream&
operator<<
  ( std::ostream& sout
  , array< Value, Index, ArrType>const& a_arr
  )
  {
      unsigned const r=a_arr.rank();
      if(0<r)
      {
          unsigned const n=a_arr.size();
          sout<<"{ ";
          for( unsigned i=0; i<n; ++i)
          {
              if(0<i) sout<<", ";
              sout<<indent_buf_in;
              sout<<a_arr[i];
              sout<<indent_buf_out;
          }
          sout<<"}\n";
      }
      else
      {
          sout<<a_arr();
      }    
      return sout;
  }

}//exit array_stepper namespace
}//exit boost namespace
#endif
