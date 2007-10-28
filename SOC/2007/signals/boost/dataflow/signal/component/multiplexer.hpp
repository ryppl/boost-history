// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_MULTIPLEXER_HPP
#define SIGNAL_NETWORK_MULTIPLEXER_HPP

#include <boost/dataflow/connection/port_map.hpp>
#include <boost/dataflow/signal/component/conditional_templated.hpp>
#include <boost/dataflow/signal/connection/slot_selector.hpp>

#include <boost/fusion/container/map.hpp>

#include <functional>


namespace boost { namespace signals {

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
class multiplexer : public conditional_templated<int, std::equal_to<int>, Signature, OutSignal, Combiner, Group, GroupCompare>
{
protected:
    typedef conditional_templated<int, std::equal_to<int>, Signature, OutSignal, Combiner, Group, GroupCompare> base_type;
public:
    
    /** Initializes the multiplexer to forward the specified input.
    */
    multiplexer(int selector=0)
    {   select(selector); }
    /** Enables the junction (signals will be forwarded).
    */
    void select(int selector)
    {   base_type::member = selector; }
    
        typedef boost::fusion::map<
        boost::fusion::pair<boost::dataflow::signals::producer<Signature>, slot_selector<Signature, multiplexer> >,
        boost::fusion::pair<
            boost::dataflow::signals::producer<typename base_type::fused_signature_type>,
            slot_selector<typename base_type::fused_signature_type, multiplexer>
        >
    > slot_map;
    
    template<int N>
    boost::dataflow::port_map<boost::dataflow::signals::mechanism, boost::dataflow::ports::consumer, slot_map>
    //slot_selector<Signature, multiplexer>
    slot()
    {
        //return make_slot_selector<Signature> (&multiplexer::template operator()<N>, *this);
        return slot_map
        (make_slot_selector<Signature> (&multiplexer::template operator()<N>, *this),
         make_slot_selector<typename base_type::fused_signature_type> (&multiplexer::template operator()<N>, *this));

    }
};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_MULTIPLEXER_HPP