// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_CONDITION_HPP
#define SIGNAL_NETWORK_CONDITION_HPP

#include <boost/dataflow/signals/component/applicator.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

namespace boost { namespace signals {

namespace detail
{
    struct cond_and_mutex
    {
        boost::condition &c;
        boost::mutex &m;
        cond_and_mutex(boost::condition &c, boost::mutex &m)
            : c(c), m(m)
        {}
    };
    
    struct notify_all
    {
        void operator()(cond_and_mutex &cnm)
        {
            boost::mutex::scoped_lock lock(cnm.m);
            cnm.c.notify_all();
        }
    };
} // namespace detail

/** \brief Signals a thread condition when a signal is received.
*/
template<typename Signature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename SignalArgs=typename default_signal_args<Signature>::type
>
class condition : public
    applicator<
        condition<Signature, OutSignal, SignalArgs>,
        detail::cond_and_mutex, detail::notify_all, Signature, OutSignal, SignalArgs>
{
    typedef applicator<
        condition<Signature, OutSignal, SignalArgs>,
        detail::cond_and_mutex, detail::notify_all, Signature, OutSignal, SignalArgs> base_type;
public:
    condition(boost::condition &cond, boost::mutex &m)
        : base_type(detail::cond_and_mutex(cond, m))
    {}
};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_CONDITION_HPP
