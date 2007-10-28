// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_COUNTER_HPP
#define SIGNAL_NETWORK_COUNTER_HPP

#include <boost/dataflow/signal/component/applicator.hpp>
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
template<typename Signature,
typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
typename T=int,
typename Combiner = boost::last_value<typename boost::function_types::result_type<Signature>::type>,
typename Group = int,
typename GroupCompare = std::less<Group> >
class counter : public applicator<T, detail::postincrement<T>, Signature, OutSignal, Combiner, Group, GroupCompare>
{
protected:
    typedef applicator<T, detail::postincrement<T>, Signature, OutSignal, Combiner, Group, GroupCompare> base_type;
public:

    /** Initializes the internal counter to 0.
    */
    counter()
    {   reset(); }
    
    /** Sets the internal counter to 0.
    */
    void reset()
    {   base_type::member = 0; }

    /** \return The internal signal counter.
    */
    T count() const
    {   return base_type::member; }
};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_COUNT_HPP