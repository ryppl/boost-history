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

namespace boost { namespace dataflow {

namespace vtk {

// The vtk mechanism tag
struct mechanism;

} // namespace boost::dataflow::vtk

//]

//[  vtk_algorithm_output_producer
// Tag for vtkAlgorithmOutput objects, which serve as proxies for
// vtkAlgorithm ouput ports.
namespace vtk {

struct vtk_algorithm_output_producer
    : public port_traits<mechanism, ports::producer, concepts::producer> {};
    
} // namespace boost::dataflow::vtk

DATAFLOW_PORT_CATEGORY(vtkAlgorithmOutput, vtk::vtk_algorithm_output_producer)

//]


//[ vtk_algorithm_consumer
namespace vtk {

struct vtk_algorithm_consumer
    : public port_traits<mechanism, ports::consumer, concepts::consumer> {};
    
} // namespace boost::dataflow::vtk

DATAFLOW_PORT_CATEGORY_ENABLE_IF(
    T,
    boost::is_base_of<vtkAlgorithm BOOST_PP_COMMA() T>,
    vtk::vtk_algorithm_consumer)

//]

//[ vtk_connect_impl_algorithm
namespace extension {

    template<>
    struct binary_impl<vtk::mechanism, operations::connect, vtk::vtk_algorithm_output_producer, vtk::vtk_algorithm_consumer>
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
    struct binary_impl<vtk::mechanism, operations::connect_only, vtk::vtk_algorithm_output_producer, vtk::vtk_algorithm_consumer>
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
    
}

//]


//[ vtk_algorithm_proxy_producer

namespace vtk {

struct vtk_algorithm_proxy_producer
    : public proxy_port_traits<vtkAlgorithmOutput> {};

} // namespace boost::dataflow::vtk

DATAFLOW_PROXY_PORT_CATEGORY_ENABLE_IF(vtk::mechanism, ports::producer,
    T,
    mpl::and_<
        typename boost::is_base_of<vtkAlgorithm BOOST_PP_COMMA() T> BOOST_PP_COMMA()
        mpl::not_<boost::is_base_of<vtkMapper BOOST_PP_COMMA() T> >
    >,
    vtk::vtk_algorithm_proxy_producer)

namespace extension {

    template<>
    struct get_proxied_port_impl<vtk::mechanism, ports::producer, vtk::vtk_algorithm_proxy_producer>
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
}

//]

//[ vtk_actor_filter

namespace vtk {

struct vtk_actor_producer
    : public port_traits<mechanism, ports::producer, concepts::producer>
{};

struct vtk_actor_consumer
    : public port_traits<mechanism, ports::consumer, concepts::consumer>
{};

}

DATAFLOW_PORT_CATEGORY_ENABLE_IF(
    T,
    boost::is_base_of<vtkActor BOOST_PP_COMMA() T>,
    vtk::vtk_actor_producer)

DATAFLOW_PORT_CATEGORY_ENABLE_IF(
    T,
    boost::is_base_of<vtkActor BOOST_PP_COMMA() T>,
    vtk::vtk_actor_consumer)
    
//]

//[ vtk_mapper_producer

namespace vtk {

struct vtk_mapper_producer
    : public port_traits<mechanism, ports::producer, concepts::producer> {};

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
        //ports::producer,
        port_map<mechanism, ports::producer, vtk_mapper_map>
    > {};
    
}

DATAFLOW_PROXY_PORT_CATEGORY_ENABLE_IF(vtk::mechanism, ports::producer,
    T,
    boost::is_base_of<vtkMapper BOOST_PP_COMMA() T>,
    vtk::vtk_mapper_proxy_producer)

namespace extension {

    template<>
    struct get_proxied_port_impl<vtk::mechanism, ports::producer, vtk::vtk_mapper_proxy_producer>
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
    struct binary_impl<vtk::mechanism, operations::connect_only, vtk::vtk_mapper_producer, vtk::vtk_actor_consumer>
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
    struct binary_impl<vtk::mechanism, operations::connect, vtk::vtk_mapper_producer, vtk::vtk_actor_consumer>
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

}

//]

//[ vtk_setup_rest

namespace vtk {

struct vtk_renderer_producer
    : public port_traits<mechanism, ports::producer, concepts::producer>
{};

struct vtk_renderer_consumer
    : public port_traits<mechanism, ports::consumer, concepts::consumer>
{};


struct vtk_rendererwindow_consumer
    : public port_traits<mechanism, ports::consumer, concepts::consumer>
{};

}

DATAFLOW_PORT_CATEGORY_ENABLE_IF(
    T,
    boost::is_base_of<vtkRenderer BOOST_PP_COMMA() T>,
    vtk::vtk_renderer_producer)

DATAFLOW_PORT_CATEGORY_ENABLE_IF(
    T,
    boost::is_base_of<vtkRenderer BOOST_PP_COMMA() T>,
    vtk::vtk_renderer_consumer)

DATAFLOW_PORT_CATEGORY_ENABLE_IF(
    T,
    boost::is_base_of<vtkRenderWindow BOOST_PP_COMMA() T>,
    vtk::vtk_rendererwindow_consumer)



namespace extension {

    template<>
    struct binary_impl<vtk::mechanism, operations::connect, vtk::vtk_actor_producer, vtk::vtk_renderer_consumer>
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
    struct binary_impl<vtk::mechanism, operations::connect, vtk::vtk_renderer_producer, vtk::vtk_rendererwindow_consumer>
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
}

//]

//[ vtk_support_pointer

namespace vtk {

template<typename T>
struct pointer_proxy_port : public proxy_port_traits<T> {};

//template<typename T>
//struct pointer_proxy_port : public proxy_port_traits<T> {};

}

template<typename T>
struct proxy_port_traits_of<vtk::mechanism, ports::producer, T *>
{
    typedef vtk::pointer_proxy_port<T> type;
};

template<typename T>
struct proxy_port_traits_of<vtk::mechanism, ports::consumer, T *>
{
    typedef vtk::pointer_proxy_port<T> type;
};

namespace extension {

    template<typename T>
    struct get_proxied_port_impl<vtk::mechanism, ports::producer, vtk::pointer_proxy_port<T> >
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
    struct get_proxied_port_impl<vtk::mechanism, ports::consumer, vtk::pointer_proxy_port<T> >
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
    
}

//]

} }

//[ vtk_specialize_connect
#define DATAFLOW_TEMPLATE_MECHANISM boost::dataflow::vtk::mechanism
#include <boost/dataflow/templates/connect.hpp>
#undef DATAFLOW_TEMPLATE_MECHANISM
//]
#endif // BOOST_DATAFLOW_VTK_SUPPORT_HPP