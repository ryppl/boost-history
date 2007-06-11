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

#ifndef BOOST_ACT_DETAIL_ACTIVE_COPIER_HPP
#define BOOST_ACT_DETAIL_ACTIVE_COPIER_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_void.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename ResultType, typename Enabler = void >
struct active_copier
{
  template< typename SourceType >
  ResultType operator ()( SourceType const* source ) const
  {
    return *source;
  }

  template< typename SourceType >
  ResultType operator ()( SourceType* source ) const
  {
    return *source;
  }
};

template< typename ResultType >
struct active_copier< ResultType
                    , typename ::boost::enable_if
                      <
                        ::boost::is_void< ResultType >
                      >
                      ::type
                    >
{
  template< typename SourceType >
  ResultType operator ()( SourceType const* source ) const
  {
  }

  template< typename SourceType >
  ResultType operator ()( SourceType* source ) const
  {
  }

  ResultType operator ()() const
  {
  }
};

}
}
}

#endif
