// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_JUNCTION_HPP
#define SIGNAL_NETWORK_JUNCTION_HPP

#include <boost/dataflow/signals/component/conditional.hpp>

namespace boost { namespace signals {

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
template<
    typename Signature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename SignalArgs=typename default_signal_args<Signature>::type
>
class junction
    : public conditional<
        junction<Signature, OutSignal, SignalArgs>,
        volatile bool, detail::identity<bool>, Signature, OutSignal, SignalArgs>
{
public:
    
    /** Initializes the junction to be enabled.
    */
    junction(bool opened=true)
    {
        junction::member=opened;
    }
    /** Enables the junction (signals will be forwarded).
    */
    void open() {junction::member = true;}
    /**	Disables the junction (signals will not be forwarded).
    */
    void close() {junction::member = false;}
};

} } // namespace boost::signals

#endif
