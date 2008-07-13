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
#include <boost/dataflow/support/port/port_adapter.hpp>
#include <boost/dataflow/support/fusion_component.hpp>
#include <boost/dataflow/support/fusion_keyed_port.hpp>

#include <boost/assert.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/type_traits/is_base_of.hpp>

//[ vtk_tag

namespace boost { namespace dataflow { namespace vtk {

// The vtk framework tag
struct tag : public default_tag {};

} } } // namespace boost::dataflow::vtk

//]

//[  vtk_algorithm_output_producer
namespace boost { namespace dataflow { namespace vtk {

// PortTraits for vtkAlgorithmOutput objects, which produce data.
// We specify the PortCategory (ports::producer) and Tag (vtk::tag).

struct vtk_algorithm_output_producer
    : public port_traits<ports::producer, tag> {};
    
} } } // namespace boost::dataflow::vtk

// This macro specializes the traits_of template to the specified
// PortTraits, and also verifies that the PortTraits requirements are satisfied.
// The traits_of template is used by the Dataflow library to associate
// a Port with its PortTraits.
DATAFLOW_TRAITS(vtkAlgorithmOutput, vtk::vtk_algorithm_output_producer)
//]


//[ vtk_algorithm_consumer
namespace boost { namespace dataflow { namespace vtk {

struct vtk_algorithm_consumer
    : public port_traits<ports::consumer, vtk::tag> {};
    
typedef port_adapter<vtkAlgorithm, vtk::vtk_algorithm_consumer, vtk::tag>
    vtk_algorithm_consumer_adapter;
    
} } } // namespace boost::dataflow::vtk

//]

//[ vtk_connect_impl_algorithm
namespace boost { namespace dataflow { namespace extension {

// To implement Connectable, we specialize the binary_operation_impl
// functor template.  We specify three things:
//   producer PortTraits (vtk::vtk_algorithm_output_producer)
//   consumer PortTraits (vtk::vtk_algorithm_consumer)
//   operation (operations::connect)
template<>
struct binary_operation_impl<vtk::vtk_algorithm_output_producer, vtk::vtk_algorithm_consumer, operations::connect>
{
    typedef void result_type;
    
    template<typename Producer, typename Consumer>
    void operator()(Producer &producer, Consumer &consumer)
    {
        // To interface with port_adaptor objects, we use get_object
        get_object(consumer).AddInputConnection(&get_object(producer));
    }
};

// To implement OnlyConnectable, we do the same thing except now the operation
// is operations::connect_only
template<>
struct binary_operation_impl<vtk::vtk_algorithm_output_producer, vtk::vtk_algorithm_consumer, operations::connect_only>
{
    typedef void result_type;

    template<typename Producer, typename Consumer>
    void operator()(Producer &producer, Consumer &consumer)
    {
        get_object(consumer).SetInputConnection(&get_object(producer));
    }
};
    
} } } // namespace boost::dataflow::vtk

//]


//[ vtk_algorithm_component

namespace boost { namespace dataflow { namespace vtk {

namespace detail {
    
    typedef mpl::map<
                mpl::pair<dataflow::default_port_selector
                    <dataflow::args::left>,
                    mpl::int_<0> >,
                mpl::pair<dataflow::default_port_selector
                    <dataflow::args::right>,
                    mpl::int_<1> >
            >::type default_map;

}

// First we need a ComponentTraits type
template<typename T>
struct vtk_algorithm_component_traits
    : public dataflow::fusion_component_traits<
        fusion::vector<
            vtkAlgorithmOutput &,
            vtk_algorithm_consumer_adapter>,
        detail::default_map,
        tag>
{
    template<typename Component>
    static typename vtk_algorithm_component_traits::fusion_ports get_ports(Component &c)
    {
        return typename vtk_algorithm_component_traits::fusion_ports(*c.GetOutputPort(), c);
    }
};

} } } // namespace boost::dataflow::vtk

// Then we associate all descendants of vtkAlgorithm with the ComponentTraits.
// vtkMapper is a descendant of vtkAlgorithm, but we want to exclude it's
// descendants from this registration because they will be treated differently.
DATAFLOW_TRAITS_ENABLE_IF(
    T,
    mpl::and_<
        boost::is_base_of<vtkAlgorithm BOOST_PP_COMMA() T> BOOST_PP_COMMA()
        mpl::not_<boost::is_base_of<vtkMapper BOOST_PP_COMMA() T> >
    >,
    vtk::vtk_algorithm_component_traits<T>)

//]

//[ vtk_actor_filter

namespace boost { namespace dataflow { namespace vtk {

struct vtk_actor_producer
    : public port_traits<ports::producer, tag>
{};

struct vtk_actor_consumer
    : public port_traits<ports::consumer, tag>
{};

template<typename T>
struct vtk_actor_component_traits
    : public dataflow::fusion_component_traits<
        fusion::vector<
            dataflow::port_adapter<T, vtk_actor_producer, tag>,
            dataflow::port_adapter<T, vtk_actor_consumer, tag> >,
        detail::default_map,
        tag>
{
    template<typename Component>
    static typename vtk_actor_component_traits::fusion_ports get_ports(Component &c)
    {
        return typename vtk_actor_component_traits::fusion_ports(c, c);
    }
};

} } } // namespace boost::dataflow::vtk

DATAFLOW_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<vtkActor BOOST_PP_COMMA() T>,
    vtk::vtk_actor_component_traits<T>)

//]

//[ vtk_mapper_producer

namespace boost { namespace dataflow { namespace vtk {

// First we need a PortTraits type that we will use for vtkMapper connections.
struct vtk_mapper_producer
    : public port_traits<ports::producer, tag> {};

// Next, we define a fusion map type to hold the mapping between consumers
// and Port types.
typedef boost::fusion::map<
            boost::fusion::pair<vtk::vtk_algorithm_consumer, vtkAlgorithmOutput &>,
            boost::fusion::pair<vtk::vtk_actor_consumer, port_adapter<vtkMapper, vtk_mapper_producer, tag> >
        > vtk_mapper_map;

// ...And a ComponentTraits type...
template<typename T>
struct vtk_mapper_component_traits
    : public dataflow::fusion_component_traits<
        fusion::vector<
            fusion_keyed_port<ports::producer, vtk::vtk_mapper_map, tag>,
            vtk_algorithm_consumer_adapter >,
        detail::default_map,
        tag>
{
    template<typename Component>
    static typename vtk_mapper_component_traits::fusion_ports get_ports(Component &c)
    {
        return typename vtk_mapper_component_traits::fusion_ports(
            fusion_keyed_port<dataflow::ports::producer, vtk::vtk_mapper_map, tag>(
                vtk::vtk_mapper_map(c.GetNumberOfOutputPorts() ? *c.GetOutputPort() : *(vtkAlgorithmOutput *)NULL,
                port_adapter<vtkMapper, vtk_mapper_producer, tag>(c))),
            c);
    }
};
    
} } } // namespace boost::dataflow::vtk

// ... that we associate with vtkMapper descendants.
DATAFLOW_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<vtkMapper BOOST_PP_COMMA() T>,
    vtk::vtk_mapper_component_traits<T>)

namespace boost { namespace dataflow { namespace extension {

// Finally, we provide implementations for connect and connect_only
// between vtk_mapper_producer and vtk_actor_consumer
template<>
struct binary_operation_impl<vtk::vtk_mapper_producer, vtk::vtk_actor_consumer, operations::connect_only>
{
    typedef void result_type;

    template<typename Producer, typename Consumer>
    void operator()(Producer &producer, Consumer &consumer)
    {
        consumer.SetMapper(producer);
    }
};

template<>
struct binary_operation_impl<vtk::vtk_mapper_producer, vtk::vtk_actor_consumer, operations::connect>
{
    typedef void result_type;

    template<typename Producer, typename Consumer>
    void operator()(Producer &producer, Consumer &consumer)
    {
        BOOST_ASSERT(!get_object(consumer).GetMapper());
        get_object(consumer).SetMapper(&get_object(producer));
    }
};

} } } // namespace boost::dataflow::extension


//]

//[ vtk_setup_rest

namespace boost { namespace dataflow { namespace vtk {

struct vtk_renderer_producer
    : public port_traits<ports::producer, tag>
{};

struct vtk_renderer_consumer
    : public port_traits<ports::consumer, tag>
{};

template<typename T>
struct vtk_renderer_component_traits
    : public dataflow::fusion_component_traits<
        fusion::vector<
            dataflow::port_adapter<T, vtk_renderer_producer, tag>,
            dataflow::port_adapter<T, vtk_renderer_consumer, tag> >, 
        detail::default_map,
        tag>
{
    template<typename Component>
    static typename vtk_renderer_component_traits::fusion_ports get_ports(Component &c)
    {
        return typename vtk_renderer_component_traits::fusion_ports(c, c);
    }
};

struct vtk_rendererwindow_consumer
    : public port_traits<ports::consumer, tag>
{};

} } } // namespace boost::dataflow::vtk

DATAFLOW_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<vtkRenderer BOOST_PP_COMMA() T>,
    vtk::vtk_renderer_component_traits<T>)

DATAFLOW_TRAITS_ENABLE_IF(
    T,
    boost::is_base_of<vtkRenderWindow BOOST_PP_COMMA() T>,
    vtk::vtk_rendererwindow_consumer)

namespace boost { namespace dataflow { namespace extension {

template<>
struct binary_operation_impl<vtk::vtk_actor_producer, vtk::vtk_renderer_consumer, operations::connect>
{
    typedef void result_type;

    template<typename Producer, typename Consumer>
    void operator()(Producer &producer, Consumer &consumer)
    {
        get_object(consumer).AddActor(&get_object(producer));
    }
};

template<>
struct binary_operation_impl<vtk::vtk_renderer_producer, vtk::vtk_rendererwindow_consumer, operations::connect>
{
    typedef void result_type;

    template<typename Producer, typename Consumer>
    void operator()(Producer &producer, Consumer &consumer)
    {
        get_object(consumer).AddRenderer(&get_object(producer));
    }
};

} } } // namespace boost::dataflow::extension

//]

//[ vtk_support_pointer

//]

//[ vtk_specialize_connect
// the include templates expect DATAFLOW_TEMPLATE_TAG to have
// the mechanism type
#define DATAFLOW_TEMPLATE_TAG boost::dataflow::vtk::tag
#define DATAFLOW_TEMPLATE_MECHANISM boost::dataflow::default_mechanism

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

#undef DATAFLOW_TEMPLATE_MECHANISM
#undef DATAFLOW_TEMPLATE_TAG

// We now have connect and connect_only functions that each take a
// vtk ProducerPort and vtk ConsumerPort as arguments, and try to connect them.

// We also have operators >>= for the connect operation,
// and ^= for the connect_only operation.

//]

#endif // BOOST_DATAFLOW_VTK_SUPPORT_HPP
