// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_MUTEX_HPP
#define SIGNAL_NETWORK_MUTEX_HPP

#include <boost/signal_network/instantiator.hpp>
#include <boost/thread/mutex.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

/** \brief Ensures a component is processing only one signal at a time when using multiple threads.
*/
template<typename Signature,
typename OutSignal=default_out_signal,
typename Combiner = boost::last_value<typename boost::function_types::result_type<Signature>::type>,
typename Group = int,
typename GroupCompare = std::less<Group>,
typename Base = instantiator<boost::mutex::scoped_lock, boost::mutex, Signature, OutSignal, Combiner, Group, GroupCompare> >
class mutex : public Base
{
public:
    typedef mutex<Signature, OutSignal, Combiner, Group, GroupCompare, typename Base::unfused> unfused;
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif // SIGNAL_NETWORK_MUTEX_HPP
