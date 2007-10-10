// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SIGNALS_COMPONENT_STORAGE_HPP
#define BOOST_DATAFLOW_SIGNALS_COMPONENT_STORAGE_HPP

#include <boost/dataflow/signal/component/conditional_modifier.hpp>
#include <boost/dataflow/signal/component/detail/storable.hpp>
#include <boost/dataflow/signal/connection/slot_selector.hpp>
#include <boost/dataflow/connection/consumer_map.hpp>

#include <boost/fusion/sequence/container/vector.hpp>
#include <boost/fusion/sequence/view/transform_view.hpp>
#include <boost/fusion/sequence/container/map.hpp>
#include <boost/dataflow/detail/make_ref.hpp>

namespace boost { namespace signals {

    template<typename Signature,
    typename OutSignal,
    typename Combiner,
    typename Group,
    typename GroupCompare
>
class storage;

namespace tag
{
    struct storage_invocable;
}

namespace detail
{
}
// storage_modifier should be in the detail namespace, but MSVC complains when
// making boost::signals::storage a friend.

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
        const storable_vector &stored_vector() {return stored;}
        void open() {opened = true;}
        void close() {opened = false;}
    protected:
        storable_vector stored;
        volatile bool opened;

        template<typename Sig, typename OutSignal, typename Combiner, typename Group, typename GroupCompare>
        friend class storage;
    };


/** \brief Stores and transmits arguments received from a signal.
    \param Signature Signature of the signal sent.
*/
template<typename Signature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename Combiner = boost::last_value<typename boost::function_traits<Signature>::result_type>,
    typename Group = int,
    typename GroupCompare = std::less<Group>
>
class storage : public conditional_modifier<storage_modifier<Signature>, Signature, OutSignal, Combiner, Group, GroupCompare>
{
protected:
    typedef conditional_modifier<storage_modifier<Signature>, Signature, OutSignal, Combiner, Group, GroupCompare> base_type;
public:
    typedef typename storage_modifier<Signature>::parameter_types parameter_types;

    typedef typename storage_modifier<Signature>::storable_types storable_types;
    typedef typename storage_modifier<Signature>::storable_vector storable_vector;
    
    typedef tag::storage_invocable invocable_category;

    /**	Initializes the stored parameter values using the provided sequence.
        \param[in] seq Sequence from which the stored parameter sequence is initialized from.
        */
    template<typename Seq>
        storage(const Seq &seq) : base_type(seq) {}
    /**	Initializes the stored parameter values using its default constructor.
        */    
    storage() {}
    
    void open() {base_type::member.open();}
    void close() {base_type::member.close();}

    /**	Sends a signal containing the stored parameter values.
        \return Return value of the sent signal.
    */
    typename base_type::signal_type::result_type send()
    {
        boost::fusion::transform_view<storable_vector, boost::dataflow::detail::make_ref>
            view(base_type::modification.stored, boost::dataflow::detail::make_ref());
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

    typedef boost::fusion::map<
        boost::fusion::pair<boost::dataflow::signal_producer<void()>, slot_selector<void (), storage> >,
        boost::fusion::pair<
            boost::dataflow::signal_producer<void(const boost::fusion::vector<> &)>,
            slot_selector<void (const boost::fusion::vector<> &), storage>
        >
    > send_map;

    boost::dataflow::consumer_map<send_map>
    send_slot()
    {
        return send_map
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


namespace boost { namespace dataflow {

namespace extension {
    
    template<>
    struct invoke_impl<boost::signals::tag::storage_invocable>
    {
        template<typename Invocable>
        struct apply
        {
            static void call(Invocable &invocable)
            {
                invocable.send();
            }
            static void call(const Invocable &invocable)
            {
                invocable.send();
            }
        };
    };
    
}

} } // namespace boost::dataflow
    

#endif // BOOST_DATAFLOW_SIGNALS_COMPONENT_STORAGE_HPP