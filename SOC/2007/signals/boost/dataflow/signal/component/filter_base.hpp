// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_FILTER_BASE_HPP
#define SIGNAL_NETWORK_FILTER_BASE_HPP

#include <boost/dataflow/signal/support.hpp>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/signal.hpp>

#ifdef _THREAD_SAFE_SIGNAL_HPP
#define SIGNAL_NETWORK_THREAD_SAFE
#else
#define SIGNAL_NETWORK_TRACKABLE
#endif

namespace boost { namespace signals {

template<typename Signal>
class filter_base
#ifdef SIGNAL_NETWORK_TRACKABLE
: public boost::signals::trackable
#endif
{
public:
    struct get_proxied_object
    {
        template<typename T>
        Signal &operator()(T &t)
        {
            return t.get_proxied_producer();
        }
    };
    typedef boost::dataflow::signals::keyed_consumer port_traits;
    typedef boost::dataflow::mutable_proxy_port<
        boost::dataflow::signals::mechanism,
        boost::dataflow::ports::producer,
        Signal,
        get_proxied_object
        > proxy_port_traits;
};

} }

#endif // SIGNAL_NETWORK_FILTER_BASE_HPP
