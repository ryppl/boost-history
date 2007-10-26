// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_VTK_SUPPORT_HPP
#define BOOST_DATAFLOW_VTK_SUPPORT_HPP

#include "vtkAlgorithmOutput.h"
#include "vtkActor.h"
#include "vtkAlgorithm.h"
#include "vtkRenderWindow.h"

#include <boost/dataflow/support.hpp>
#include <boost/dataflow/connection/port_map.hpp>

#include <boost/assert.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/fusion/sequence/container/map.hpp>
#include <boost/type_traits/is_base_of.hpp>

//[ vtk_mechanism

namespace boost { namespace dataflow { namespace vtk {

// The vtk mechanism tag
struct mechanism;

} } } // namespace boost::dataflow::vtk

//]

//[  vtk_algorithm_output_producer
namespace boost { namespace dataflow { namespace vtk {

// PortTraits for vtkAlgorithmOutput objects, which produce data.
// We specify the Mechanism (vtk::mechanism), PortCategory (ports::producer),
// and the most refined concept which the port satisfies (ports::port).
struct vtk_algorithm_output_producer
    : public port_traits<mechanism, ports::producer, concepts::port> {};
    
} } } // namespace boost::dataflow::vtk

// This macro specializes the port_traits_of template to the specified
// PortTraits, and also verifies that the PortTraits requirements are satisfied.
// The port_traits_of template is used by the Dataflow library to associate
// a Port with its PortTraits.
DATAFLOW_PORT_TRAITS(vtkAlgorithmOutput, vtk::vtk_algorithm_output_producer)
//]


//[ vtk_algorithm_consumer
namespace boost { namespace dataflow { namespace vtk {

struct vtk_algorithm_consumer
    : public port_traits<mechanism, ports::consumer, concepts::port> {};
    
} } } // namespace boost::dataflow::vtk

// Since vtkAlgorithm is typically inherited, we will specialize the
// port_traits_of template for all its descendants.
DATAFLOW_PORT_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<vtkAlgorithm BOOST_PP_COMMA() T>,
    vtk::vtk_algorithm_consumer)
//]

//[ vtk_connect_impl_algorithm
namespace boost { namespace dataflow { namespace extension {

template<>
struct binary_operation_impl<operations::connect, vtk::vtk_algorithm_output_producer, vtk::vtk_algorithm_consumer>
{
    template<typename Producer, typename Consumer>
    struct apply
    {
        static void call(Producer &producer, Consumer &consumer)
        {
            consumer.AddInputConnection(&producer);
        }
    };
};

template<>
struct binary_operation_impl<operations::connect_only, vtk::vtk_algorithm_output_producer, vtk::vtk_algorithm_consumer>
{
    template<typename Producer, typename Consumer>
    struct apply
    {
        static void call(Producer &producer, Consumer &consumer)
        {
            consumer.SetInputConnection(&producer);
        }
    };
};
    
} } } // namespace boost::dataflow::vtk

//]


//[ vtk_algorithm_proxy_producer

namespace boost { namespace dataflow { namespace vtk {

struct vtk_algorithm_proxy_producer
    : public proxy_port_traits<mechanism, ports::producer> {};

} } } // namespace boost::dataflow::vtk

DATAFLOW_PROXY_PORT_TRAITS_ENABLE_IF(
    T,
    mpl::and_<
        boost::is_base_of<vtkAlgorithm BOOST_PP_COMMA() T> BOOST_PP_COMMA()
        mpl::not_<boost::is_base_of<vtkMapper BOOST_PP_COMMA() T> >
    >,
    vtk::vtk_algorithm_proxy_producer)

namespace boost { namespace dataflow { namespace extension {

    template<>
    struct get_port_impl<vtk::vtk_algorithm_proxy_producer>
    {
        template<typename ProxyProducer>
        struct apply
        {
            typedef vtkAlgorithmOutput &type;
            
            static type call(ProxyProducer &t)
            {
                return *t.GetOutputPort();
            }
        };
    };

} } } // namespace boost::dataflow::extension

//]

//[ vtk_actor_filter

namespace boost { namespace dataflow { namespace vtk {

struct vtk_actor_producer
    : public port_traits<mechanism, ports::producer, concepts::port>
{};

struct vtk_actor_consumer
    : public port_traits<mechanism, ports::consumer, concepts::port>
{};

} } } // namespace boost::dataflow::vtk

DATAFLOW_PORT_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<vtkActor BOOST_PP_COMMA() T>,
    vtk::vtk_actor_producer)

DATAFLOW_PORT_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<vtkActor BOOST_PP_COMMA() T>,
    vtk::vtk_actor_consumer)

//]

//[ vtk_mapper_producer

namespace boost { namespace dataflow { namespace vtk {

struct vtk_mapper_producer
    : public port_traits<mechanism, ports::producer, concepts::port> {};

struct vtk_mapper_proxy : public port<vtk_mapper_producer>
{
    vtkMapper *ptr;
    vtk_mapper_proxy(vtkMapper *mapper) : ptr(mapper) {}
    operator vtkMapper * () const {return ptr;}
};

typedef boost::fusion::map<
            boost::fusion::pair<vtk::vtk_algorithm_consumer, vtkAlgorithmOutput &>,
            boost::fusion::pair<vtk::vtk_actor_consumer, vtk_mapper_proxy>
        > vtk_mapper_map;

struct vtk_mapper_proxy_producer
    : public proxy_port_traits<
        vtk::mechanism,
        ports::producer
//        port_map<mechanism, ports::producer, vtk_mapper_map>
    > {};
    
} } } // namespace boost::dataflow::vtk

DATAFLOW_PROXY_PORT_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<vtkMapper BOOST_PP_COMMA() T>,
    vtk::vtk_mapper_proxy_producer)

namespace boost { namespace dataflow { namespace extension {

template<>
struct get_port_impl<vtk::vtk_mapper_proxy_producer>
{
    template<typename ProxyProducer>
    struct apply
    {
        typedef const port_map<vtk::mechanism, ports::producer, vtk::vtk_mapper_map> type;
        
        static type call(ProxyProducer &t)
        {
            return vtk::vtk_mapper_map(t.GetNumberOfOutputPorts() ?
                *t.GetOutputPort() : *(vtkAlgorithmOutput *)NULL,
                vtk::vtk_mapper_proxy(&t));
        }
    };
};

template<>
struct binary_operation_impl<operations::connect_only, vtk::vtk_mapper_producer, vtk::vtk_actor_consumer>
{
    template<typename Producer, typename Consumer>
    struct apply
    {
        static void call(Producer &producer, Consumer &consumer)
        {
            consumer.SetMapper(producer);
        }
    };
};

template<>
struct binary_operation_impl<operations::connect, vtk::vtk_mapper_producer, vtk::vtk_actor_consumer>
{
    template<typename Producer, typename Consumer>
    struct apply
    {
        static void call(Producer &producer, Consumer &consumer)
        {
            BOOST_ASSERT(!consumer.GetMapper());
            consumer.SetMapper(producer);
        }
    };
};

} } } // namespace boost::dataflow::extension


//]

//[ vtk_setup_rest

namespace boost { namespace dataflow { namespace vtk {

struct vtk_renderer_producer
    : public port_traits<mechanism, ports::producer, concepts::port>
{};

struct vtk_renderer_consumer
    : public port_traits<mechanism, ports::consumer, concepts::port>
{};


struct vtk_rendererwindow_consumer
    : public port_traits<mechanism, ports::consumer, concepts::port>
{};

} } } // namespace boost::dataflow::vtk

DATAFLOW_PORT_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<vtkRenderer BOOST_PP_COMMA() T>,
    vtk::vtk_renderer_producer)

DATAFLOW_PORT_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<vtkRenderer BOOST_PP_COMMA() T>,
    vtk::vtk_renderer_consumer)

DATAFLOW_PORT_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<vtkRenderWindow BOOST_PP_COMMA() T>,
    vtk::vtk_rendererwindow_consumer)

namespace boost { namespace dataflow { namespace extension {

template<>
struct binary_operation_impl<operations::connect, vtk::vtk_actor_producer, vtk::vtk_renderer_consumer>
{
    template<typename Producer, typename Consumer>
    struct apply
    {
        static void call(Producer &producer, Consumer &consumer)
        {
            consumer.AddActor(&producer);
        }
    };
};

template<>
struct binary_operation_impl<operations::connect, vtk::vtk_renderer_producer, vtk::vtk_rendererwindow_consumer>
{
    template<typename Producer, typename Consumer>
    struct apply
    {
        static void call(Producer &producer, Consumer &consumer)
        {
            consumer.AddRenderer(&producer);
        }
    };
};

} } } // namespace boost::dataflow::extension

//]

//[ vtk_support_pointer

namespace boost { namespace dataflow {

namespace vtk {

    template<typename T>
    struct pointer_proxy_producer : public proxy_port_traits<vtk::mechanism, ports::producer> {};

    template<typename T>
    struct pointer_proxy_consumer : public proxy_port_traits<vtk::mechanism, ports::consumer> {};

} // namespace vtk

template<typename T>
struct proxy_port_traits_of<vtk::mechanism, ports::producer, T *>
{
    typedef vtk::pointer_proxy_producer<T> type;
};

template<typename T>
struct proxy_port_traits_of<vtk::mechanism, ports::consumer, T *>
{
    typedef vtk::pointer_proxy_consumer<T> type;
};

namespace extension {

    template<typename T>
    struct get_port_impl<vtk::pointer_proxy_producer<T> >
    {
        template<typename ProxyProducer>
        struct apply
        {
            typedef T & type;
            
            static type call(ProxyProducer &t)
            {
                return *t;
            }
        };
    };

    template<typename T>
    struct get_port_impl<vtk::pointer_proxy_consumer<T> >
    {
        template<typename ProxyConsumer>
        struct apply
        {
            typedef T & type;
            
            static type call(ProxyConsumer &t)
            {
                return *t;
            }
        };
    };
    
} // namespace extension

} } // namespace boost::dataflow

//]

//[ vtk_specialize_connect
#define DATAFLOW_TEMPLATE_MECHANISM boost::dataflow::vtk::mechanism

#define DATAFLOW_TEMPLATE_BINARY_OPERATION connect
#include <boost/dataflow/templates/binary_operation.hpp>
#undef DATAFLOW_TEMPLATE_BINARY_OPERATION

#define DATAFLOW_TEMPLATE_BINARY_OPERATION connect_only
#include <boost/dataflow/templates/binary_operation.hpp>
#undef DATAFLOW_TEMPLATE_BINARY_OPERATION

#undef DATAFLOW_TEMPLATE_MECHANISM
// We now have connect and connect_only functions that each take a
// vtk ProducerPort and vtk ConsumerPort as arguments, and try to connect them.
//]

#endif // BOOST_DATAFLOW_VTK_SUPPORT_HPP