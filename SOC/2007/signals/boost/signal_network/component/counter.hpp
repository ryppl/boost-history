// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_COUNTER_HPP
#define SIGNAL_NETWORK_COUNTER_HPP

#include <boost/signal_network/applicator.hpp>
#include <boost/type_traits/remove_volatile.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

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
}
/** \brief Counts the number of signals passing through the component.
    counter is an applicator with a postincrement application and default member of type volatile int.
    \param T Type of the internal counter variable.
*/
template<typename Signature,
typename T=volatile int,
typename OutSignal=default_out_signal,
typename Combiner = boost::last_value<typename boost::function_types::result_type<Signature>::type>,
typename Group = int,
typename GroupCompare = std::less<Group>,
typename Base = applicator<detail::postincrement<T>, T, Signature, OutSignal, Combiner, Group, GroupCompare> >
class counter : public Base
{
public:
    typedef counter<Signature, T, OutSignal, Combiner, Group, GroupCompare,
    typename applicator<detail::postincrement<T>, T, Signature, OutSignal, Combiner, Group, GroupCompare>::unfused >
        unfused;

    /** Initializes the internal counter to 0.
    */
    counter()
    {   reset(); }
    
    /** Sets the internal counter to 0.
    */
    void reset()
    {   Base::member = 0; }

    /** \return The internal signal counter.
    */
    typename boost::remove_volatile<T>::type count() const
    {   return Base::member; }
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif // SIGNAL_NETWORK_COUNT_HPP