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

#ifndef BOOST_ACT_DETAIL_AS_FUNCTION_IMPL_HPP
#define BOOST_ACT_DETAIL_AS_FUNCTION_IMPL_HPP

#include "../parallel_safe.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename Type >
struct as_function_impl_type
{
  explicit as_function_impl_type( Type const& target_init )
    : target( target_init )
  {
  }

  Type target;
};
/*
// ToDo: Account for explicit result type
//       Make less copies
template< typename Type >
struct as_safe_function_impl_type
{
  explicit as_safe_function_impl_type( Type const& target_init )
    : target( target_init )
  {
  }

  parallel_safe_fun< Type > target;
};
*/
struct as_function_type
{
  as_function_type()
  {
  }

  as_function_type operator ()() const
  {
    return as_function_type();
  }

  // ToDo: Possibly make const version (check semantics)
  template< typename Type >
  as_function_impl_type< Type >
  operator ()( Type const& target_init ) const
  {
    return as_function_impl_type< Type >( target_init );
  }
};

struct as_safe_function_type
{
  as_safe_function_type()
  {
  }

  as_safe_function_type operator ()() const
  {
    return as_safe_function_type();
  }

  // ToDo: Possibly make const version (check semantics)
  template< typename Type >
  as_function_impl_type< parallel_safe_fun< Type > >
  operator ()( Type const& target_init ) const
  {
    return as_function_impl_type< parallel_safe_fun< Type > >( target_init );
  }
};

}
}
}

#endif
