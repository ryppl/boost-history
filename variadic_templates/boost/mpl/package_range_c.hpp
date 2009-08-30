#ifndef BOOST_MPL_PACKAGE_RANGE_C_HPP_INCLUDED
#define BOOST_MPL_PACKAGE_RANGE_C_HPP_INCLUDED
//  (C) Copyright Larry Evans 2009.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include <boost/mpl/package_c.hpp>
#include <boost/mpl/eval_if.hpp>
namespace boost
{
namespace mpl
{
      template
      < typename PackageC
      , typename IntervalC
      >
      struct
    package_range_forward
    ;
      template
      < typename Value
      , Value... Vals
      , Value Start
      , Value Finish
      >
      struct
    package_range_forward
      < package_c<Value, Vals...>
      , package_c<Value,Start,Finish>
      >
    : package_range_forward
      < package_c<Value, Finish, Vals...>
      , package_c<Value, Start, Value(Finish-1)>
      >
    {
    };
      template
      < typename Value
      , Value... Vals
      , Value Start
      >
      struct
    package_range_forward
      < package_c<Value, Vals...>
      , package_c<Value,Start,Start>
      >
    {
            typedef
          package_c<Value, Start,Vals...>
        type
        ;
    };
      template
      < typename PackageC
      , typename IntervalC
      >
      struct
    package_range_backward
    ;
      template
      < typename Value
      , Value... Vals
      , Value Start
      , Value Finish
      >
      struct
    package_range_backward
      < package_c<Value, Vals...>
      , package_c<Value,Start,Finish>
      >
    : package_range_backward
      < package_c<Value, Vals..., Finish>
      , package_c<Value, Start, Value(Finish-1)>
      >
    {
    };
      template
      < typename Value
      , Value... Vals
      , Value Start
      >
      struct
    package_range_backward
      < package_c<Value, Vals...>
      , package_c<Value,Start,Start>
      >
    {
            typedef
          package_c<Value, Vals..., Start>
        type
        ;
    };
      template
      < typename Value
      , Value Start
      , Value Finish
      >
      struct
    package_range_c
    /**@brief
     *  Produce (depending on compare of Start and Finish) either:
     *    default:
     *      package_c<Value,Start,Start+1,Start+2,...,Finish-1>
     *    else:
     *      package_c<Value,Finish-1,...,Start+2,Start+1,Start>
     */
    : eval_if_c
      < Start == Finish
      , package_c<Value>
      , eval_if_c
        < Start < Finish
        , package_range_forward
          < package_c<Value>
          , package_c<Value,Start,Value(Finish-1)>
          >
        , package_range_backward
          < package_c<Value>
          , package_c<Value,Finish,Value(Start-1)>
          >
        >
      >
    {
    };
      
}}//exit boost::mpl
#endif

