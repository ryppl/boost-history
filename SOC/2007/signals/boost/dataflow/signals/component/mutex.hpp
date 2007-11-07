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
    typename Combiner = boost::last_value<typename boost::function_types::result_type<Signature>::type>,
    typename Group = int,
    typename GroupCompare = std::less<Group>
>
class mutex : public
    instantiator<boost::mutex, boost::mutex::scoped_lock, Signature, OutSignal, Combiner, Group, GroupCompare>
{
};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_MUTEX_HPP
