// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_COMPONENT_TRAITS_HPP
#define SIGNAL_NETWORK_COMPONENT_TRAITS_HPP

#include <boost/signal_network/detail/enable_if_defined.hpp>

namespace boost { namespace signals {

template<class T, typename Enable=void>
struct is_component : public boost::false_type {};

template<class T, typename Enable=void>
struct get_signal;

template<typename T, typename Enable=void>
struct get_signal_type;

template<typename T>
struct get_signal_type<T, typename detail::enable_if_defined<typename T::signal_type>::type >
{
    typedef typename T::signal_type type;
};

template<typename Signature, typename T, typename Enable=void>
struct get_slot;

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_COMPONENT_TRAITS_HPP
