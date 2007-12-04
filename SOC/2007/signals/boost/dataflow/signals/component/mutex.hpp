// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_MUTEX_HPP
#define SIGNAL_NETWORK_MUTEX_HPP

#include <boost/dataflow/signals/component/instantiator.hpp>
#include <boost/thread/mutex.hpp>

namespace boost { namespace signals {

/** \brief Ensures a component is processing only one signal at a time when using multiple threads.
*/
template<typename Signature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename SignalArgs=typename default_signal_args<Signature>::type
>
class mutex : public
    instantiator<
        mutex<Signature, OutSignal, SignalArgs>,
        boost::mutex, boost::mutex::scoped_lock, Signature, OutSignal, SignalArgs>
{
};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_MUTEX_HPP
