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

#ifndef BOOST_ACT_DETAIL_AS_FUTURE_IMPL_HPP
#define BOOST_ACT_DETAIL_AS_FUTURE_IMPL_HPP

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Come up with a way to get a raw reference to an action's contained type
//       in order to prevent an extra copy construction when used as a future.
//       Also, eliminate all copies for immediate-style models.
template< typename Type >
class future_type
{
public:
  explicit future_type( Type& ref_init )
    : ref_m( ref_init )
  {
  }
public:
  operator Type&() const
  {
    return ref_m;
  }
private:
  Type& ref_m;
};

template< typename Type, typename ActModel >
struct remove_future_type
{
  typedef Type type;
};

// ToDo: Change to add scoped action
template< typename Type, typename ActModel >
struct remove_future_type< future_type< Type > >
{
  typedef typename add_action< Type, ActModel >::type type;
};

struct as_future_type
{
  as_future_type()
  {
  }

  as_future_type operator ()() const
  {
    return *this;
  }

  template< typename Type >
  future_type< Type > operator ()( Type& ref_init ) const
  {
    return future_type< Type >( ref_init );
  }
};

}
}
}

#endif
