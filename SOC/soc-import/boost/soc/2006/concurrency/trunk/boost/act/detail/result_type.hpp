//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_DETAIL_RESULT_TYPE_HPP
#define BOOST_ACT_DETAIL_RESULT_TYPE_HPP

#include <boost/type_traits/function_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/empty_base.hpp>

#include "has_result_type.hpp"

namespace boost
{
namespace act
{
namespace detail
{

struct no_default_type;

template< typename Type
        , typename DefaultType
        , typename Enabler = void
        >
struct result_type_impl
{
  typedef DefaultType type;
};

template< typename Type
        , typename DefaultType
        >
struct result_type_impl
       <
         Type
       , DefaultType
       , typename ::boost::enable_if
         <
           ::boost::is_function< Type >
         >
         ::type
       >
{
  typedef typename ::boost::function_traits< Type >::result_type type;
};

template< typename Type
        , typename DefaultType
        >
struct result_type_impl
       <
         Type
       , DefaultType
       , typename::boost::mpl::apply
         <
           ::boost::mpl::always< void >
         , typename Type::result_type
         >
         ::type
       >
{
  typedef typename Type::result_type type;
};

template< typename Type
        , typename DefaultType = no_default_type
        >
struct result_type
  : ::boost::mpl::if_
    <
      ::boost::mpl::or_
      <
        has_result_type< Type >
      , ::boost::mpl::not_
        <
          ::boost::is_same< DefaultType, no_default_type >
        >
      >
    , result_type_impl< Type, DefaultType >
    , ::boost::mpl::empty_base
    >
    ::type
{
};

}

}
}

#endif
