#ifndef BOOST_ACTIVE_DETAIL_ACTIVE_INTEGRAL_SPEC_HPP
#define BOOST_ACTIVE_DETAIL_ACTIVE_INTEGRAL_SPEC_HPP

#include "../active_interface.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace boost
{
namespace act
{

template< typename Type, typename ActiveDetails >
class active_interface< Type, ActiveDetails
                      , typename enable_if< is_integral< Type > >::type
                      >
{
private:
  typedef typename ActiveDetails::child_type child_type;
public:
  friend child_type& operator ++( child_type& active_target )
  {
    ++active_target->inactive_value();
    return static_cast< child_type& >( active_target );
  }
};

}
}

#endif
