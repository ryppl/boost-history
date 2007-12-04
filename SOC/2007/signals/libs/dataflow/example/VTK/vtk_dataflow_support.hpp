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
#include <boost/fusion/container/map.hpp>
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

// To implement Connectable, we specialize the binary_operation_impl
// functor template.  We specify three things:
//   operation (operations::connect)
//   producer PortTraits (vtk::vtk_algorithm_output_producer)
//   consumer PortTraits (vtk::vtk_algorithm_consumer)
template<>
struct binary_operation_impl<operations::connect, vtk::vtk_algorithm_output_producer, vtk::vtk_algorithm_consumer>
{
    template<typename Producer, typename Consumer>
    void operator()(Producer &producer, Consumer &consumer)
    {
        consumer.AddInputConnection(&producer);
    }
};

// To implement OnlyConnectable, we do the same thing except now the operation
// is operations::connect_only
template<>
struct binary_operation_impl<operations::connect_only, vtk::vtk_algorithm_output_producer, vtk::vtk_algorithm_consumer>
{
    template<typename Producer, typename Consumer>
    void operator()(Producer &producer, Consumer &consumer)
    {
        consumer.SetInputConnection(&producer);
    }
};
    
} } } // namespace boost::dataflow::vtk

//]


//[ vtk_algorithm_proxy_producer

namespace boost { namespace dataflow { namespace vtk {

// First we need a ProxyPortTraits type
struct vtk_algorithm_proxy_producer
    : public proxy_port_traits<mechanism, ports::producer> {};

} } } // namespace boost::dataflow::vtk

// Then we associate all descendants of vtkAlgorithm with the ProxyPortTraits.
// vtkMapper is a descendant of vtkAlgorithm, but we want to exclude it's
// descendants from this registration because they will be treated differently.
DATAFLOW_PROXY_PORT_TRAITS_ENABLE_IF(
    T,
    mpl::and_<
        boost::is_base_of<vtkAlgorithm BOOST_PP_COMMA() T> BOOST_PP_COMMA()
        mpl::not_<boost::is_base_of<vtkMapper BOOST_PP_COMMA() T> >
    >,
    vtk::vtk_algorithm_proxy_producer)

namespace boost { namespace dataflow { namespace extension {

// Finally, we specialize the get_port_impl functor template.
    template<>
    struct get_port_impl<vtk::vtk_algorithm_proxy_producer>
    {
        typedef vtkAlgorithmOutput & result_type;

        template<typename ProxyProducer>
        result_type operator()(ProxyProducer &t)
        {
            return *t.GetOutputPort();
        }
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

// First we need a PortTraits type that we will use for vtkMapper connections.
struct vtk_mapper_producer
    : public port_traits<mechanism, ports::producer, concepts::port> {};

// Since vtkMapper itself will have multiple "faces" (depending on what we
// are connecting to it), we will define a proxy type for vtkMapper that we
// will use to specify that we want to use the mapper output for a vtkActor.
// (rather than wanting to use the output for a vtkAlgorithm, in for which
// we can use vtkAlgorithmOutput as the proxy object).
struct vtk_mapper_proxy : public port<vtk_mapper_producer>
{
    vtkMapper *ptr;
    vtk_mapper_proxy(vtkMapper *mapper) : ptr(mapper) {}
    operator vtkMapper * () const {return ptr;}
};

// Next, we define a fusion map type to hold the mapping between consumers
// and Port types.
typedef boost::fusion::map<
            boost::fusion::pair<vtk::vtk_algorithm_consumer, vtkAlgorithmOutput &>,
            boost::fusion::pair<vtk::vtk_actor_consumer, vtk_mapper_proxy>
        > vtk_mapper_map;

// ...And a ProxyPortTraits type...
struct vtk_mapper_proxy_producer
    : public proxy_port_traits<
        vtk::mechanism,
        ports::producer
    > {};
    
} } } // namespace boost::dataflow::vtk

// ... that we associate with vtkMapper descendants.
DATAFLOW_PROXY_PORT_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<vtkMapper BOOST_PP_COMMA() T>,
    vtk::vtk_mapper_proxy_producer)

namespace boost { namespace dataflow { namespace extension {

// Now we specialize the get_port_imple functor template that will return
// a port_map for a port with vtk_mapper_proxy_producer ProxyPortTraits.
// The port_map object is a KeyedPort and takes care of providing the
// appropriate Port depending on the consumer.
template<>
struct get_port_impl<vtk::vtk_mapper_proxy_producer>
{
    typedef const port_map<vtk::mechanism, ports::producer, vtk::vtk_mapper_map> result_type;

    template<typename ProxyProducer>
    result_type operator()(ProxyProducer &t)
    {
        return vtk::vtk_mapper_map(t.GetNumberOfOutputPorts() ?
            *t.GetOutputPort() : *(vtkAlgorithmOutput *)NULL,
            vtk::vtk_mapper_proxy(&t));
    }
};

// Finally, we provide implementations for connect and connect_only
// between vtk_mapper_producer and vtk_actor_consumer
template<>
struct binary_operation_impl<operations::connect_only, vtk::vtk_mapper_producer, vtk::vtk_actor_consumer>
{
    template<typename Producer, typename Consumer>
    void operator()(Producer &producer, Consumer &consumer)
    {
        consumer.SetMapper(producer);
    }
};

template<>
struct binary_operation_impl<operations::connect, vtk::vtk_mapper_producer, vtk::vtk_actor_consumer>
{
    template<typename Producer, typename Consumer>
    void operator()(Producer &producer, Consumer &consumer)
    {
        BOOST_ASSERT(!consumer.GetMapper());
        consumer.SetMapper(producer);
    }
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
    void operator()(Producer &producer, Consumer &consumer)
    {
        consumer.AddActor(&producer);
    }
};

template<>
struct binary_operation_impl<operations::connect, vtk::vtk_renderer_producer, vtk::vtk_rendererwindow_consumer>
{
    template<typename Producer, typename Consumer>
    void operator()(Producer &producer, Consumer &consumer)
    {
        consumer.AddRenderer(&producer);
    }
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
        typedef T & result_type;

        template<typename ProxyProducer>
        result_type operator()(ProxyProducer &t)
        {
            return *t;
        }
    };

    template<typename T>
    struct get_port_impl<vtk::pointer_proxy_consumer<T> >
    {
        typedef T & result_type;
        
        template<typename ProxyConsumer>
        result_type operator()(ProxyConsumer &t)
        {
            return *t;
        }
    };
    
} // namespace extension

} } // namespace boost::dataflow

//]

//[ vtk_specialize_connect
// the include templates expect DATAFLOW_TEMPLATE_TAG to have
// the mechanism type
#define DATAFLOW_TEMPLATE_TAG boost::dataflow::vtk::mechanism

// the binary_operation.hpp template expects DATAFLOW_TEMPLATE_BINARY_OPERATION
#   define DATAFLOW_TEMPLATE_BINARY_OPERATION connect
#       include <boost/dataflow/templates/binary_operation.hpp>

// the operator.hpp template expects DATAFLOW_TEMPLATE_OPERATOR
#       define DATAFLOW_TEMPLATE_OPERATOR >>=
#           include <boost/dataflow/templates/operator.hpp>
#       undef DATAFLOW_TEMPLATE_OPERATOR

#   undef DATAFLOW_TEMPLATE_BINARY_OPERATION

#   define DATAFLOW_TEMPLATE_BINARY_OPERATION connect_only
#       include <boost/dataflow/templates/binary_operation.hpp>

#       define DATAFLOW_TEMPLATE_OPERATOR ^=
#           include <boost/dataflow/templates/operator.hpp>
#       undef DATAFLOW_TEMPLATE_OPERATOR

#   undef DATAFLOW_TEMPLATE_BINARY_OPERATION

#undef DATAFLOW_TEMPLATE_TAG

// We now have connect and connect_only functions that each take a
// vtk ProducerPort and vtk ConsumerPort as arguments, and try to connect them.

// We also have operators >>= for the connect operation,
// and ^= for the connect_only operation.

//]

#endif // BOOST_DATAFLOW_VTK_SUPPORT_HPP