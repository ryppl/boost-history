// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_SIGNAL_SUPPORT_HPP
#define SIGNAL_NETWORK_SIGNAL_SUPPORT_HPP

#include <boost/dataflow/support.hpp>
#include <boost/dataflow/support/port/port_adapter.hpp>
#include <boost/dataflow/utility/bind_mem_fn.hpp>
#include <boost/dataflow/utility/bind_mem_fn_overload.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/signal.hpp>

namespace boost { namespace dataflow {

namespace signals {

struct tag : public default_tag {};
struct connect_mechanism {};
struct extract_mechanism {};

template<typename T>
struct producer
    : public complemented_port_traits<ports::producer, boost::function<T>, tag>
{
    typedef T signature_type;
};

template<typename T>
struct consumer
    : public port_traits<ports::consumer, tag>
{
    typedef T signature_type;
};

template<typename T>
struct extract_producer
    : public complemented_port_traits<ports::producer, boost::function<T>, tag>
{
    typedef T signature_type;
};

namespace detail
{
    template<typename T>
    struct wrap_producer
    {
        typedef producer<T> type;
    };
    
    template<typename T>
    struct wrap_extract_producer
    {
        typedef extract_producer<T> type;
    };
    
    template<typename T>
    struct wrap_function
    {
        typedef boost::function<T> type;
    };
}

template<typename SignatureSequence=mpl::vector<> >
struct call_consumer
    : public keyed_port_traits<
        ports::consumer,
        mpl::joint_view<
            typename mpl::transform<
                SignatureSequence,
                detail::wrap_producer<mpl::_1>
            >::type,
            typename mpl::transform<
                SignatureSequence,
                detail::wrap_extract_producer<mpl::_1>
            >::type
        >,
        mpl::joint_view<
            typename mpl::transform<
                SignatureSequence,
                detail::wrap_function<mpl::_1>
            >::type,
            typename mpl::transform<
                SignatureSequence,
                detail::wrap_function<mpl::_1>
            >::type
        >,
        tag>
{};

} // namespace signals

template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
struct register_traits<boost::signal<Signature, Combiner, Group, GroupCompare>, signals::tag >
{
    typedef signals::producer<Signature> type;
};

template<typename Signature>
struct register_traits<boost::function<Signature>, signals::tag >
{
    typedef signals::consumer<Signature> type;
};

namespace extension
{
    template<typename SignatureSequence, typename Signature>
    struct get_keyed_port_impl<signals::call_consumer<SignatureSequence>, signals::producer<Signature> >
    {
        typedef const boost::function<Signature> result_type;
        
        template<typename ConsumerPort>
        result_type operator()(ConsumerPort &consumer)
        {
            typedef typename get_object_type<ConsumerPort>::type object_type;
            typedef typename boost::dataflow::utility::slot_type<Signature, object_type>::type mem_fn_type;
             
            return boost::dataflow::utility::bind_mem_fn<mem_fn_type, object_type>
            (static_cast<mem_fn_type>(&object_type::operator()), get_object(consumer));
        };
    };

    template<typename T>
    struct binary_operation_impl<signals::producer<T>, signals::consumer<T>, operations::connect>
    {
        typedef boost::signals::connection result_type;
        
        template<typename Producer, typename Consumer>
        result_type operator()(Producer &producer, Consumer &consumer)
        {
            return producer.connect(consumer);
        }
    };
    
    template<typename T>
    struct binary_operation_impl<signals::producer<T>, signals::consumer<T>, operations::disconnect>
    {
        typedef void result_type;
        
        template<typename Producer, typename Consumer>
        void operator()(Producer &producer, Consumer &consumer)
        {
            producer.disconnect(consumer);
        }
    };
    
    template<typename T>
    struct unary_operation_impl<signals::producer<T>, operations::disconnect_all>
    {
        typedef void result_type;
        
        template<typename Producer>
        void operator()(Producer &producer)
        {
            producer.disconnect_all_slots();
        }
    };
    
    template<typename SignatureSequence, typename Signature>
    struct get_keyed_port_impl<signals::call_consumer<SignatureSequence>, signals::extract_producer<Signature> >
    {
        typedef const boost::function<Signature> result_type;
        
        template<typename ConsumerPort>
        result_type operator()(ConsumerPort &consumer)
        {
            typedef typename get_object_type<ConsumerPort>::type object_type;
            typedef typename boost::dataflow::utility::slot_type<Signature, object_type>::type mem_fn_type;
            
            return boost::dataflow::utility::bind_mem_fn<mem_fn_type, object_type>
            (static_cast<mem_fn_type>(&object_type::operator()), get_object(consumer));
        };
    };

    template<typename T>
    struct binary_operation_impl<signals::extract_producer<T>, signals::consumer<T>, operations::extract>
    {
        typedef void result_type;
        
        template<typename Producer, typename Consumer>
        void operator()(Producer &producer, Consumer &consumer)
        {
            get_object(producer).call(consumer);
        }
    };
}

} } // namespace boost::dataflow

namespace boost { namespace signals {

#define DATAFLOW_TEMPLATE_TAG boost::dataflow::signals::tag

#define DATAFLOW_TEMPLATE_MECHANISM boost::dataflow::signals::connect_mechanism
#define DATAFLOW_TEMPLATE_BINARY_OPERATION connect
#include <boost/dataflow/templates/binary_operation.hpp>
#undef DATAFLOW_TEMPLATE_BINARY_OPERATION
#define DATAFLOW_TEMPLATE_BINARY_OPERATION disconnect
#include <boost/dataflow/templates/binary_operation.hpp>
#undef DATAFLOW_TEMPLATE_BINARY_OPERATION
#define DATAFLOW_TEMPLATE_UNARY_OPERATION disconnect_all
#include <boost/dataflow/templates/unary_operation.hpp>
#undef DATAFLOW_TEMPLATE_UNARY_OPERATION
#undef DATAFLOW_TEMPLATE_MECHANISM

#define DATAFLOW_TEMPLATE_MECHANISM boost::dataflow::signals::extract_mechanism
#define DATAFLOW_TEMPLATE_BINARY_OPERATION extract
#include <boost/dataflow/templates/binary_operation.hpp>
#undef DATAFLOW_TEMPLATE_BINARY_OPERATION
#undef DATAFLOW_TEMPLATE_MECHANISM
#undef DATAFLOW_TEMPLATE_TAG

template<typename Component>
inline typename enable_if<
    dataflow::is_component<Component, dataflow::signals::tag>,
    void
>::type invoke(Component &component)
{
    boost::dataflow::component_operation<boost::dataflow::operations::invoke, dataflow::signals::tag>(component);
}

using boost::dataflow::utility::bind_mem_fn;
using boost::dataflow::utility::bind_mem_fn_overload;

} } // namespace boost::phoenix

#endif // SIGNAL_NETWORK_SIGNAL_SUPPORT_HPP
