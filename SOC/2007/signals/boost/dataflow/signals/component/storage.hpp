// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SIGNALS_COMPONENT_STORAGE_HPP
#define BOOST_DATAFLOW_SIGNALS_COMPONENT_STORAGE_HPP

#include <boost/dataflow/support/fusion_keyed_port.hpp>
#include <boost/dataflow/detail/make_ref.hpp>
#include <boost/dataflow/signals/component/conditional_modifier.hpp>
#include <boost/dataflow/signals/component/detail/storable.hpp>

#include <boost/mpl/map.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/view/transform_view.hpp>
#include <boost/fusion/container/map.hpp>


namespace boost { namespace signals {

template<typename Signature, typename OutSignal, typename SignalArgs>
class storage;

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
		storage_modifier(const storage_modifier &rhs) : stored(rhs.stored), opened(rhs.opened) {}
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

        template<typename Sig, typename OutSignal, typename SignalArgs>
        friend class storage;
    };


//template<typename Storage>
//struct storage_component_traits
//    : public Storage::base_type::dataflow_traits
//{};

template<typename Storage>
struct storage_component_traits
    : public dataflow::fusion_component_traits<
        fusion::vector<
            typename Storage::signal_type &,
            dataflow::port_adapter<
                Storage,
                dataflow::signals::call_consumer<typename Storage::in_signatures_type>,
                dataflow::signals::tag>,
            dataflow::port_adapter<
                Storage,
                dataflow::signals::extract_producer<typename Storage::signature_type>,
                dataflow::signals::tag>
        >,
        mpl::map<
            mpl::pair<dataflow::default_port_selector
                <dataflow::args::left, dataflow::signals::connect_mechanism>,
                mpl::int_<0> >,
            mpl::pair<dataflow::default_port_selector
                <dataflow::args::right, dataflow::signals::connect_mechanism>,
                mpl::int_<1> >
        >,
        dataflow::signals::tag>
{
    template<typename Component>
    static typename storage_component_traits::fusion_ports get_ports(Component &component)
    {
        return typename storage_component_traits::fusion_ports(
            component.default_signal(),
            component,
            component);
    };
};

/** \brief Stores and transmits arguments received from a signal.
    \param Signature Signature of the signal sent.
*/
template<
    typename Signature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename SignalArgs=typename default_signal_args<Signature>::type
>
class storage : public conditional_modifier<
    storage<Signature,OutSignal,SignalArgs>,
    storage_modifier<Signature>,
    Signature,
    OutSignal,
    SignalArgs>
{
public:
    typedef conditional_modifier<storage, storage_modifier<Signature>, Signature, OutSignal, SignalArgs> base_type;
protected:
	using base_type::modification;
public:
    typedef typename storage_modifier<Signature>::parameter_types parameter_types;
    typedef typename storage_modifier<Signature>::storable_types storable_types;
    typedef typename storage_modifier<Signature>::storable_vector storable_vector;
    
    typedef storage_component_traits<storage> dataflow_traits;

	storage(const storage &rhs) : base_type(rhs.modification) {}

    /**	Initializes the stored parameter values using the provided sequence.
        \param[in] seq Sequence from which the stored parameter sequence is initialized from.
        */
    template<typename Seq>
    storage(const Seq &seq, bool opened=true) : base_type(seq, opened) {}
    /**	Initializes the stored parameter values using its default constructor.
        */    
    storage() {}
    
    void open() {base_type::modification.open();}
    void close() {base_type::modification.close();}

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
	typename base_type::signal_type::result_type call(const boost::function<typename base_type::signature_type> &f)
    {
        boost::fusion::fused<boost::function<typename base_type::signature_type> const &> fused_out(f);
        return fused_out(base_type::modification.stored);
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
        boost::fusion::pair<boost::dataflow::signals::producer<void()>, boost::function<void ()> >,
        boost::fusion::pair<
            boost::dataflow::signals::producer<void(const boost::fusion::vector<> &)>,
            boost::function<void (const boost::fusion::vector<> &)>
        >
    > send_map;

    boost::dataflow::fusion_keyed_port<boost::dataflow::ports::consumer, send_map, dataflow::signals::tag>
    send_slot()
    {
        return send_map
        (bind_mem_fn_overload<void ()> (&storage::send, *this),
         bind_mem_fn_overload<void (const boost::fusion::vector<> &)> (&storage::send, *this));
    }
    /** \return The slot selector for the related at function.
    */
    template<int N>
        boost::function
#ifndef DOXYGEN_DOCS_BUILD
        <typename boost::fusion::result_of::at_c<storable_vector, N>::type ()>
#endif
    at_slot()
    {
        return bind_mem_fn_overload<
            typename boost::fusion::result_of::at_c<storable_vector, N>::type ()>
            (&storage::template at<N>, *this);
    }
    /** \return The slot selector for the value_at function.
    */
    template<int N>
   	boost::function
#ifndef DOXYGEN_DOCS_ONLY
        <typename boost::mpl::at_c<parameter_types, N>::type ()>
#endif
    value_at_slot()
	{
        return bind_mem_fn_overload<
            typename boost::mpl::at_c<parameter_types, N>::type ()>
            (&storage::template value_at<N>, *this);
    }
protected:
    storable_vector &stored() {return base_type::modification.stored;}
};

} } // namespace boost::signals


namespace boost { namespace dataflow {

namespace extension {
    
    template<typename T>
    struct component_operation_impl<boost::signals::storage_component_traits<T>, operations::invoke >
    {
        typedef void result_type;
        
        template<typename Invocable>
        void operator()(Invocable &invocable)
        {
            invocable.send();
        }
    };
    
/*    template<typename T>
    struct get_port_impl<boost::signals::storage_component_traits<T> >
    {
        template<typename Component>
        struct port_types
        {
            typedef fusion::vector<signal<T> &, function<T>, Component &, function<T> > type;
        };
        
        template<typename Component>
        typename port_types<Component>::type ports(Component &component)
        {
            return typename port_types<Component>::type(
                component.get_proxied_producer(),
                get_keyed_port<signals::producer<T> >(component),
                component,
                get_keyed_port<signals::extract_producer<T> >(component)
                );
        }

        template<typename FArgs> struct result;

        template<typename F, typename Component, typename N>
        struct result<F(Component &, N)>
        {
            typedef typename fusion::result_of::value_at<
                typename port_types<Component>::type,
                N>::type type;
        };
        
        template<typename Component, typename N>
        typename result<get_port_impl(Component &, N)>::type
        operator()(Component &component, N)
        {
            return fusion::at<N>(ports(component));
        }
    };
    
    template<typename T>
    struct binary_operation_impl<operations::extract, signals::extract_producer<T>, signals::extract_consumer<T> >
    {
        template<typename Producer, typename Consumer>
        void operator()(Producer &producer, Consumer &consumer)
        {
            producer.call(consumer);
        }
    };*/
}

} } // namespace boost::dataflow
    

#endif // BOOST_DATAFLOW_SIGNALS_COMPONENT_STORAGE_HPP
