// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_MULTIPLEXER_HPP
#define SIGNAL_NETWORK_MULTIPLEXER_HPP

#include <boost/dataflow/support/fusion_keyed_port.hpp>
#include <boost/dataflow/signals/component/conditional_templated.hpp>

#include <boost/config.hpp>
#include <boost/fusion/container/map.hpp>

#include <functional>

#ifndef BOOST_MSVC
#define BOOST_TEMPLATE_OPERATOR template operator
#else
#define BOOST_TEMPLATE_OPERATOR operator
#endif

namespace boost { namespace signals {

/** \brief Forwards a single signal to multiple slots, and can
also be disabled to stop the flow of signals.
    junction is a conditional with Condition identity and Member volatile bool
*/
template<
    typename Signature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename SignalArgs=typename default_signal_args<Signature>::type
>
class multiplexer
    : public conditional_templated<
        multiplexer<Signature, OutSignal, SignalArgs>,
        int, std::equal_to<int>, Signature, OutSignal, SignalArgs>
{
public:
    /** Initializes the multiplexer to forward the specified input.
    */
    multiplexer(int selector=0)
    {   select(selector); }
    /** Enables the junction (signals will be forwarded).
    */
    void select(int selector)
    {   multiplexer::member = selector; }
    
    boost::function<void(int)>
    select_slot()
    {
        return bind_mem_fn_overload<void(int)> (&multiplexer::select, *this);
    }
    
    typedef boost::fusion::map<
        boost::fusion::pair<boost::dataflow::signals::producer<Signature>, boost::function<Signature> >,
        boost::fusion::pair<
            boost::dataflow::signals::producer<typename multiplexer::fused_signature_type>,
            boost::function<typename multiplexer::fused_signature_type>
        >
    > slot_map;
    
    template<int N>
    boost::dataflow::fusion_keyed_port<boost::dataflow::ports::consumer, slot_map, boost::dataflow::signals::tag>
    slot()
    {
        return slot_map
        (bind_mem_fn_overload<Signature> (&multiplexer::BOOST_TEMPLATE_OPERATOR()<N>, *this),
         bind_mem_fn_overload<typename multiplexer::fused_signature_type>
            (&multiplexer::BOOST_TEMPLATE_OPERATOR()<N>, *this));

    }
};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_MULTIPLEXER_HPP
