// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_JUNCTION_HPP
#define SIGNAL_NETWORK_JUNCTION_HPP

#include <boost/signal_network/conditional.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

namespace detail
{
    template<typename T>
    struct identity
    {
        T operator ()(T t) const
        {
            return t;
        }
    };
}

/** \brief Forwards a single signal to multiple slots, and can
also be disabled to stop the flow of signals.
    junction is a conditional with Condition identity and Member volatile bool
*/
template<typename Signature,
    typename OutSignal=default_out_signal,
    typename Combiner = boost::last_value<typename boost::function_types::result_type<Signature>::type>,
    typename Group = int,
    typename GroupCompare = std::less<Group>,
    typename Base = conditional<detail::identity<bool>, volatile bool, Signature, OutSignal, Combiner, Group, GroupCompare>
>
class junction : public Base
{
public:
    typedef junction<Signature, OutSignal, Combiner, Group, GroupCompare, typename Base::unfused > unfused;
    
    /** Initializes the junction to be enabled.
    */
    junction(bool enabled=true)
    {
        enable();
    }
    /** Enables the junction (signals will be forwarded).
    */
    void enable() {Base::member = true;}
    /**	Disables the junction (signals will not be forwarded).
    */
    void disable() {Base::member = false;}
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif