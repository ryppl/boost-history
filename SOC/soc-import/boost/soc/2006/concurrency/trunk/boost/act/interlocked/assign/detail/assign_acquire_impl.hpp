#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_ACQUIRE_IMPL_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_ACQUIRE_IMPL_HPP

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

#include <boost/act/interlocked/assign/detail/assign_acquire_windows.hpp>

#else // Else: No support for assign acquire (default to assign)

#include <boost/act/interlocked/assign/detail/assign_impl.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

namespace assign_acquire {

template< typename TargetType, typename SourceType >
struct are_valid_params
  : detail::assign::are_valid_params< TargetType, SourceType > {};

template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  are_valid_params< TargetType, SourceType >
, remove_volatile< TargetType >
>
::type
assign_acquire( TargetType& destination, SourceType const& new_value )
{
  return detail::assign::assign( destination, new_value );
}

}

} } } }

#endif

#endif
