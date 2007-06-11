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

#ifndef BOOST_ACT_ALGORITHM_FILL_HPP
#define BOOST_ACT_ALGORITHM_FILL_HPP

#include "detail/make_algo.hpp"

#include "fill/fill_fwd.hpp"

#include "../parallel_safe.hpp"

#include "generate.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename SourceType >
class safe_filler
  : public parallel_safe
{
public:
  safe_filler( SourceType const source_init )
    : source_m( source_init )
  {
  }
public:
  SourceType const& operator ()() const
  {
    return source_m;
  }
private:
  SourceType const source_m;
};

template< typename SourceType >
safe_filler< SourceType >
make_safe_filler( SourceType const& source )
{
  return safe_filler< SourceType >( source );
}

}

BOOST_ACT_DETAIL_IMPLEMENT_ALGO( ((typename),IteratorType)
                                 ((typename),ValueType)
                               , (void)
                               , fill
                               , ((IteratorType),begin)
                                 ((IteratorType),end)
                                 ((ValueType const&),value)
                               )
{
  generate[ AlgoModel() ]( begin, end
                         , detail::make_safe_filler( value )
                         );
}

}
}

#endif
