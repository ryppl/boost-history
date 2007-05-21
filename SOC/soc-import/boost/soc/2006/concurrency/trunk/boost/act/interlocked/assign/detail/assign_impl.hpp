#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_IMPL_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_IMPL_HPP

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

#include <boost/act/interlocked/assign/detail/assign_windows.hpp>

#else

#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

namespace assign {

template< typename TargetType, typename SourceType >
struct are_valid_params
{
  BOOST_MPL_ASSERT_MSG
  (
    ( mpl::apply< mpl::always< mpl::false_ >, TargetType >::type::value )
  , INTERLOCKED_ASSIGN_NOT_DEFINED_FOR_THIS_SYSTEM
  , ()
  );
};

}

} } } } 

#endif

#endif
