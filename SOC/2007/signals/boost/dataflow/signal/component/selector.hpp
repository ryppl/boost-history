// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_MULTIPLEXER_HPP
#define SIGNAL_NETWORK_MULTIPLEXER_HPP

#include <boost/dataflow/signal/component/conditional.hpp>

namespace boost { namespace signals {

namespace detail
{
    template<typename T>
    struct multiplex
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
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename Combiner = boost::last_value<typename boost::function_types::result_type<Signature>::type>,
    typename Group = int,
    typename GroupCompare = std::less<Group>
>
class multiplexer : public conditional<int, detail::<bool>, Signature, OutSignal, Combiner, Group, GroupCompare>
{
protected:
    typedef conditional<volatile bool, detail::identity<bool>, Signature, OutSignal, Combiner, Group, GroupCompare> base_type;
public:
    
    /** Initializes the junction to be enabled.
    */
    junction(bool enabled=true)
    {
        enable();
    }
    /** Enables the junction (signals will be forwarded).
    */
    void enable() {base_type::member = true;}
    /**	Disables the junction (signals will not be forwarded).
    */
    void disable() {base_type::member = false;}
};

} } // namespace boost::signals

#endif