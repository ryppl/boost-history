#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_ACQUIRE_DEFAULT_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_ACQUIRE_DEFAULT_HPP

#include <boost/act/interlocked/assign/detail/assign_impl.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

namespace assign {

template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  are_valid_params< TargetType, SourceType >
, assign_result< TargetType, SourceType >
>
::type
assign_acquire( TargetType& destination, SourceType const& new_value )
{
  return assign( destination, new_value );
}

}

} } } }

#endif
