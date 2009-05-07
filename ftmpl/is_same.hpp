#ifndef BOOST_FTMPL_IS_SAME_HPP
#define BOOST_FTMPL_IS_SAME_HPP

#include "value.hpp"
#include "type.hpp"

namespace boost { namespace ftmpl {

struct is_same_t
{
  template< typename L, typename R >
  type_t< false_t >& operator ()( L&, R& );

  template< typename L >
  type_t< true_t >& operator()( L&, L& );
};

extern type_t< is_same_t >& is_same;

} }

#endif // BOOST_FTMPL_IS_SAME_HPP
