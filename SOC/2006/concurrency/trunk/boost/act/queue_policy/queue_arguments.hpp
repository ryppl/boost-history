#ifndef BOOST_ACT_QUEUE_POLICY_QUEUE_ARGUMENTS_HPP
#define BOOST_ACT_QUEUE_POLICY_QUEUE_ARGUMENTS_HPP

#include <boost/act/queue_policy/queue_arguments_fwd.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace act {

struct one_pusher
  : mpl::true_ {}; // Implies 1 pusher

struct many_pushers
  : mpl::false_ {}; // Implies many pushers

struct one_popper
  : mpl::true_ {}; // Implies 1 popper

struct many_poppers
  : mpl::false_ {}; // Implies many poppers


namespace detail {

template< typename Type >
struct is_queue_push_info
  : mpl::or_< is_same< Type, one_pusher >
            , is_same< Type, many_pushers >
            > {};

template< typename Type >
struct is_queue_pop_info
  : mpl::or_< is_same< Type, one_popper >
            , is_same< Type, many_poppers >
            > {};

}

} }

#endif
