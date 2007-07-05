// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_FILTER_BASE_HPP
#define SIGNAL_NETWORK_FILTER_BASE_HPP

#include <boost/signal_network/component/traits.hpp>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/signal.hpp>

#ifdef _THREAD_SAFE_SIGNAL_HPP
#define SIGNAL_NETWORK_THREAD_SAFE
#else
#define SIGNAL_NETWORK_TRACKABLE
#endif

namespace boost { namespace signals {

class filter_base
#ifdef SIGNAL_NETWORK_TRACKABLE
: public boost::signals::trackable
#endif
{};

template<class T, typename Enable=void>
struct is_filter : public boost::false_type { };

template<class T>
struct is_filter<T, typename boost::enable_if<boost::is_base_of<filter_base, T> >::type >
    : public boost::true_type { };

template<class T>
struct is_component<T, typename boost::enable_if<is_filter<T> >::type >
    : public boost::true_type { };

template<class T>
struct get_signal<T, typename boost::enable_if<is_filter<T> >::type >
{
    typename T::signal_type &operator()(const T &t) {return t.default_signal();}
};

template<class T>
struct get_signature<T, typename boost::enable_if<is_filter<T> >::type>
{
    typedef typename T::signature_type type;
};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_FILTER_BASE_HPP
