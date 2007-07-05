// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_COMPONENT_TRAITS_HPP
#define SIGNAL_NETWORK_COMPONENT_TRAITS_HPP

#include <boost/type_traits/integral_constant.hpp>

namespace boost { namespace signals {

template<class T, typename Enable=void>
struct is_component : public boost::false_type {};

template<class T, typename Enable=void>
struct get_signal;

template<typename T, typename Signature, typename Enable=void>
struct get_slot;

template<typename T, typename Enable=void>
struct get_signature;

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_COMPONENT_TRAITS_HPP
