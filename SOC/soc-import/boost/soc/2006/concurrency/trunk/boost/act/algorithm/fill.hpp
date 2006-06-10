//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)


#ifndef BOOST_ACT_ALGORITHM_FILL_HPP
#define BOOST_ACT_ALGORITHM_FILL_HPP

#include "detail/make_algo.hpp"

#include "../act_fwd.hpp"

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

BOOST_ACT_DETAIL_MAKE_ALGO_IMPL( ((typename),IteratorType)
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
