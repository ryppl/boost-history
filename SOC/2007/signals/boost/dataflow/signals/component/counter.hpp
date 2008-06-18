// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_COUNTER_HPP
#define SIGNAL_NETWORK_COUNTER_HPP

#include <boost/dataflow/signals/component/applicator.hpp>
#include <boost/type_traits/remove_volatile.hpp>

namespace boost { namespace signals {

namespace detail
{
    template<typename T>
    struct postincrement
    {
        void operator()(T &t)
        {
            t++;
        }
    };
} // namespace detail

/** \brief Counts the number of signals passing through the component.
    counter is an applicator with a postincrement application and default member of type volatile int.
    \param T Type of the internal counter variable.
*/
template<
    typename Signature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename T=int,
    typename SignalArgs=typename default_signal_args<Signature>::type
 >
class counter : public applicator<
    counter<Signature, OutSignal, T, SignalArgs>,
    T, detail::postincrement<T>, Signature, OutSignal, SignalArgs>
{
public:
    /** Initializes the internal counter to 0.
    */
    counter()
    {   reset(); }
    
    /** Sets the internal counter to 0.
    */
    void reset()
    {   counter::member = 0; }

    /** \return The internal signal counter.
    */
    T count() const
    {   return counter::member; }
};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_COUNT_HPP
