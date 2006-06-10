#ifndef BOOST_ACT_TYPE_TRAITS_HAS_PARALLEL_SAFE_ASSIGN_HPP
#define BOOST_ACT_TYPE_TRAITS_HAS_PARALLEL_SAFE_ASSIGN_HPP

#include <boost/type_traits/has_trivial_assign.hpp>

namespace boost
{
namespace act
{

template< typename TargetType >
struct has_parallel_safe_assign
  : has_trivial_assign< TargetType >
{
};

}
}

#endif
