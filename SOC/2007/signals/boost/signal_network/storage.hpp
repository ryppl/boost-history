// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_GENERATOR_HPP
#define SIGNAL_NETWORK_GENERATOR_HPP

#include <boost/signal_network/detail/storable.hpp>
#include <boost/signal_network/filter.hpp>
#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>

#include <boost/function_types/parameter_types.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/adapted/mpl.hpp>
#include <boost/fusion/functional/adapter/fused.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/same_traits.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/fusion/support/is_sequence.hpp>

#include <boost/signal_network/detail/unfused_typed_class.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

/** \brief Stores and transmits arguments received from a signal.
    \param Signature Signature of the signal sent.
*/
template<typename Signature,
    typename OutSignal=default_out_signal,
    typename Combiner = boost::last_value<typename boost::function_traits<Signature>::result_type>,
    typename Group = int,
    typename GroupCompare = std::less<Group> >
    class storage : public filter<Signature, typename OutSignal::default_normal_type, Combiner, Group, GroupCompare>
{
#ifndef DOXYGEN_DOCS_ONLY
protected:
    typedef filter<Signature, typename OutSignal::default_normal_type, Combiner, Group, GroupCompare> base_type;
    typedef storage<Signature, OutSignal, Combiner, Group, GroupCompare> this_type;
public:

    typedef typename detail::mpl_storable<typename base_type::parameter_types>::type storable_types;
    typedef typename boost::fusion::result_of::as_vector<storable_types >::type storable_vector;

    typedef
        boost::fusion::unfused_typed_class<storage<
        Signature, typename OutSignal::default_unfused_type, Combiner, Group, GroupCompare>,
        typename base_type::parameter_types> unfused;
#endif

    /**	Initializes the stored parameter values using the provided sequence.
        \param[in] seq Sequence from which the stored parameter sequence is initialized from.
        */
    template<typename Seq>
    storage(const Seq &seq) : stored(seq) {}
    /**	Initializes the stored parameter values using its default constructor.
        */    
    storage() {}

    /**	Sends a signal containing the stored parameter values.
        \return Return value of the sent signal.
    */
    typename base_type::signal_type::result_type operator()()
    {
        return base_type::fused_out(stored);
    }
    /**	Sends a signal containing the stored parameter values.
        \return Return value of the sent signal.
    */
    typename base_type::signal_type::result_type operator()(const boost::fusion::vector<> &)
    {
        return base_type::fused_out(stored);
    }

#ifndef DOXYGEN_DOCS_ONLY
    template<class Seq>
    struct result : public boost::enable_if<typename boost::mpl::and_<
        boost::fusion::traits::is_sequence<Seq>,
        boost::mpl::greater<boost::fusion::result_of::size<Seq>, boost::mpl::int_<0> > >::type
        > {};
#endif
    
    /**	Sets the stored parameter values using the provided sequence.
        \param[in] seq Sequence to whose value the stored parameter sequence is assigned to.
        
        <b>Note:</b> Enabled only for fusion sequences of size >= 1.
        */
    template <class Seq>
#ifndef DOXYGEN_DOCS_ONLY
        typename boost::enable_if<typename boost::mpl::and_<
        boost::fusion::traits::is_sequence<Seq>,
        boost::mpl::greater<boost::fusion::result_of::size<Seq>, boost::mpl::int_<0> > >::type
        >::type
#else
    void
#endif
    operator()(const Seq &seq)
    {
            stored = seq;
    }

    /** \return A reference to the fusion vector of stored parameter values.
    */
    const storable_vector &stored_vector()
    {
        return stored;
    }
    /**	\return The stored value of parameter N using an optimizing cast.
    */
    template<int N>
    typename boost::fusion::result_of::at_c<storable_vector, N>::type at()
    {
        return boost::fusion::at_c<N>(stored);
    }
    /** \return The stored value of parameter N typed exactly as it appears in the Signature.
    */
    template<int N>
    typename boost::mpl::at_c<typename base_type::parameter_types, N>::type value_at()
    {
        return boost::fusion::at_c<N>(stored);
    }

    /** \return The slot selector for the related at function.
    */
    template<int N>
        slot_selector_t
#ifndef DOXYGEN_DOCS_ONLY
        <this_type,
        typename boost::fusion::result_of::at_c<storable_vector, N>::type ()>
#endif
    at_slot()
    {
        return boost::signal_network::slot_selector<
            typename boost::fusion::result_of::at_c<storable_vector, N>::type ()>
            (*this, &this_type::template at<N>);
    }
    /** \return The slot selector for the value_at function.
    */
    template<int N>
   	slot_selector_t
#ifndef DOXYGEN_DOCS_ONLY
        <this_type,
        typename boost::mpl::at_c<typename base_type::parameter_types, N>::type ()>
#endif
    value_at_slot()
	{
        return boost::signal_network::slot_selector<
            typename boost::mpl::at_c<typename base_type::parameter_types, N>::type ()>
            (*this, &this_type::template value_at<N>);
    }
protected:
    storable_vector stored;
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE


#endif // SIGNAL_NETWORK_GENERATOR_HPP