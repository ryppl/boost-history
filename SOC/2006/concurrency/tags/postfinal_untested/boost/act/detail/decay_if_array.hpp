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

#ifndef BOOST_ACT_DETAIL_DECAY_IF_ARRAY_HPP
#define BOOST_ACT_DETAIL_DECAY_IF_ARRAY_HPP

#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/remove_extent.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Check const correctness

template< typename Type >
struct decay_impl
{
  typedef typename ::boost::remove_extent< Type >::type* const type;
};

template< typename Type >
struct decay_if_array_meta
  : ::boost::mpl::eval_if< ::boost::is_array< Type >
                         , decay_impl< Type >
                         , ::boost::mpl::identity< Type >
                         >
{
};

template< typename Type >
typename ::boost::lazy_enable_if
<
  ::boost::is_array< Type >
, ::boost::remove_extent< Type >
>
::type*
decay_if_array( Type const& source )
{
  return source;
}

template< typename Type >
typename ::boost::disable_if
<
  ::boost::is_array< Type >
, Type const&
>
::type
decay_if_array( Type const& source )
{
  return source;
}

}
}
}

#endif
