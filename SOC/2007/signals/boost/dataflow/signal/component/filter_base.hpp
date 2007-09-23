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
    template<typename Mechanism, typename Enable=void>
    struct dataflow;
    
    template<typename Mechanism>
    struct dataflow<
        Mechanism,
        typename enable_if<
            is_same<Mechanism, boost::dataflow::signals_mechanism>
        >::type>
    {
        typedef boost::dataflow::signal_consumer consumer_category;
        typedef boost::dataflow::mutable_proxy_producer proxy_producer_category;
        typedef Signal proxy_producer_for;
        
        template<typename T>
        static typename boost::dataflow::get_proxied_producer_type<boost::dataflow::signals_mechanism, Signal>
         ::type &get_proxied_producer(const T &t)
        {   return t.get_proxied_producer();  }

    };
};

} }

#endif // SIGNAL_NETWORK_FILTER_BASE_HPP
