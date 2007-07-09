// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_GENERATOR_HPP
#define SIGNAL_NETWORK_GENERATOR_HPP

#include <boost/signal_network/component/conditional_modifier.hpp>
#include <boost/signal_network/component/detail/storable.hpp>
#include <boost/signal_network/connection/slot_selector_map.hpp>

#include <boost/fusion/sequence/container/vector.hpp>
#include <boost/fusion/sequence/view/transform_view.hpp>
#include <boost/fusion/sequence/container/map.hpp>
#include <boost/type_traits/add_reference.hpp>

namespace boost { namespace signals {

namespace detail
{
    struct make_ref
    {
        template<typename Sig>
        struct result;
        
        template<typename T>
        struct result<make_ref(T&)>
            : boost::add_reference<T>
        {};
        
        template<typename T>
        typename boost::add_reference<T>::type operator()(T& t) const
        {
            return t;
        }
    };
    
    template <typename Signature>
    class storage_modifier
    {
    public:
        typedef typename boost::function_types::parameter_types<Signature>::type parameter_types;
        typedef typename detail::mpl_storable<parameter_types>::type storable_types;
        typedef typename boost::fusion::result_of::as_vector<storable_types >::type storable_vector;

        storage_modifier(bool opened=true) : opened(opened) {}
        template<typename Seq>
            storage_modifier(const Seq &seq, bool opened=true) : stored(seq), opened(opened) {}        

        template<typename Sig>
        struct result;

        template<typename S, typename T1>
        struct result<storage_modifier<S>(const T1 &)>
        {
            typedef boost::optional<const T1 &> type;
        };
        template<typename T1>
        typename result<storage_modifier(const T1 &)>::type operator()(const T1 &t1)
        {
            stored = t1;
            if (opened)
                return t1;
            else
                return typename result<storage_modifier(const T1 &)>::type ();
        }
    protected:
        storable_vector stored;
        volatile bool opened;

        template<typename Sig, typename OutSignal, typename Combiner, typename Group, typename GroupCompare>
        friend class storage;
    };
}

/** \brief Stores and transmits arguments received from a signal.
    \param Signature Signature of the signal sent.
*/
template<typename Signature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename Combiner = boost::last_value<typename boost::function_traits<Signature>::result_type>,
    typename Group = int,
    typename GroupCompare = std::less<Group>
>
class storage : public conditional_modifier<detail::storage_modifier<Signature>, Signature, OutSignal, Combiner, Group, GroupCompare>
{
protected:
    typedef conditional_modifier<detail::storage_modifier<Signature>, Signature, OutSignal, Combiner, Group, GroupCompare> base_type;
public:
    typedef typename detail::storage_modifier<Signature>::parameter_types parameter_types;

    typedef typename detail::storage_modifier<Signature>::storable_types storable_types;
    typedef typename detail::storage_modifier<Signature>::storable_vector storable_vector;

    /**	Initializes the stored parameter values using the provided sequence.
        \param[in] seq Sequence from which the stored parameter sequence is initialized from.
        */
    template<typename Seq>
        storage(const Seq &seq) : base_type(seq) {}
    /**	Initializes the stored parameter values using its default constructor.
        */    
    storage() {}
    
    void open() {base_type::member.opened = true;}
    void close() {base_type::member.opened = false;}

    /**	Sends a signal containing the stored parameter values.
        \return Return value of the sent signal.
    */
    typename base_type::signal_type::result_type send()
    {
        boost::fusion::transform_view<storable_vector, detail::make_ref>
            view(base_type::modification.stored, detail::make_ref());
        return base_type::fused_out(view);
    }
    /**	Sends a signal containing the stored parameter values.
        \return Return value of the sent signal.
    */
    typename base_type::signal_type::result_type send(const boost::fusion::vector<> &)
    {
        return send();
    }
    /** \return A reference to the fusion vector of stored parameter values.
    */
    const storable_vector &stored_vector()
    {
        return base_type::modification.stored;
    }
    /**	\return The stored value of parameter N using an optimizing cast.
    */
    template<int N>
    typename boost::fusion::result_of::at_c<storable_vector, N>::type at()
    {
        return boost::fusion::at_c<N>(base_type::modification.stored);
    }
    /** \return The stored value of parameter N typed exactly as it appears in the Signature.
    */
    template<int N>
    typename boost::mpl::at_c<parameter_types, N>::type value_at()
    {
        return boost::fusion::at_c<N>(base_type::modification.stored);
    }

    boost::fusion::map<
        boost::fusion::pair<void(), slot_selector<void (), storage> >,
        boost::fusion::pair<void(const boost::fusion::vector<> &),
            slot_selector<void (const boost::fusion::vector<> &), storage> >
    >
    send_slot()
    {
        return     boost::fusion::map<
        boost::fusion::pair<void(), slot_selector<void (), storage> >,
        boost::fusion::pair<void(const boost::fusion::vector<> &),
            slot_selector<void (const boost::fusion::vector<> &), storage> >
        >        
        (make_slot_selector<void ()> (&storage::send, *this),
         make_slot_selector<void (const boost::fusion::vector<> &)> (&storage::send, *this));
    }
    /** \return The slot selector for the related at function.
    */
    template<int N>
        slot_selector
#ifndef DOXYGEN_DOCS_ONLY
        <typename boost::fusion::result_of::at_c<storable_vector, N>::type (), storage>
#endif
    at_slot()
    {
        return make_slot_selector<
            typename boost::fusion::result_of::at_c<storable_vector, N>::type ()>
            (&storage::template at<N>, *this);
    }
    /** \return The slot selector for the value_at function.
    */
    template<int N>
   	slot_selector
#ifndef DOXYGEN_DOCS_ONLY
        <typename boost::mpl::at_c<parameter_types, N>::type (), storage>
#endif
    value_at_slot()
	{
        return make_slot_selector<
            typename boost::mpl::at_c<parameter_types, N>::type ()>
            (&storage::template value_at<N>, *this);
    }
protected:
    storable_vector &stored() {return base_type::modification.stored;}

};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_GENERATOR_HPP