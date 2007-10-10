// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_VTK_SUPPORT_HPP
#define BOOST_DATAFLOW_VTK_SUPPORT_HPP

#include "vtkActor.h"
#include "vtkAlgorithm.h"
#include "vtkRenderWindow.h"

#include <boost/dataflow/support.hpp>
#include <boost/dataflow/connection/producer_map.hpp>

#include <boost/fusion/sequence/container/map.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace dataflow {

struct vtk;

struct vtk_algorithm_output_producer;
struct vtk_algorithm_consumer;
struct vtk_algorithm_proxy_producer;
struct vtk_algorithm_mapper_producer;
struct vtk_prop_producer;
struct vtk_mapper_consumer;
struct vtk_renderer_filter;
struct vtk_rendererwindow_consumer;

struct vtk_algorithm_data;
struct vtk_mapper_data;

template<>
struct producer_category_of<vtk, vtkAlgorithmOutput *>
{
    typedef vtk_algorithm_output_producer type;
};

template<typename T>
struct proxy_producer_category_of<
    vtk, T,
    typename boost::enable_if<boost::is_base_of<vtkAlgorithm,T> >::type
>
{
    typedef vtk_algorithm_proxy_producer type;
};

template<>
struct producer_category_of<vtk, vtkMapper *>
{
    typedef vtk_algorithm_mapper_producer type;
};

template<typename T>
struct proxied_producer_of<
    vtk, T,
    typename boost::enable_if<boost::is_base_of<vtkAlgorithm,T> >::type
>
{
    typedef boost::dataflow::producer_map<
        boost::fusion::map<
            boost::fusion::pair<vtk_algorithm_data, vtkAlgorithmOutput *>,
            boost::fusion::pair<vtk_mapper_data, vtkMapper *>
        > > type;
};

namespace extension
{
    template<>
    struct get_proxied_producer_impl<vtk, vtk_algorithm_proxy_producer>
    {
        template<typename ProxyProducer>
        struct result
        {
            typedef boost::dataflow::producer_map<
                boost::fusion::map<
                    boost::fusion::pair<vtk_algorithm_data, vtkAlgorithmOutput *>,
                    boost::fusion::pair<vtk_mapper_data, vtkMapper *>
                > > type;
        };
        
        template<typename ProxyProducer>
        struct apply
        {
            static typename result<ProxyProducer>::type
            call(ProxyProducer &t)
            {
                return boost::fusion::map<
                    boost::fusion::pair<vtk_algorithm_data,vtkAlgorithmOutput *>,
                    boost::fusion::pair<vtk_mapper_data, vtkMapper *>
                > (t.GetNumberOfOutputPorts() ? t.GetOutputPort() : (vtkAlgorithmOutput *)NULL, (vtkMapper *)&t);
            }
        };
    };

}

template<typename T>
struct consumer_category_of<
    vtk, T,
    typename boost::enable_if<boost::is_base_of<vtkAlgorithm,T> >::type
>
{
    typedef vtk_algorithm_consumer type;
};

template<typename T>
struct consumed_type_of<
    vtk, T,
    typename boost::enable_if<boost::is_base_of<vtkAlgorithm,T> >::type
>
{
    typedef vtk_algorithm_data type;
};



namespace extension
{
    template<>
    struct connect_impl<vtk, vtk_algorithm_output_producer, vtk_algorithm_consumer>
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(const Producer &producer, Consumer &consumer)
            {
                consumer.AddInputConnection(producer);
            }
        };
    };
    
    template<>
    struct connect_impl<vtk, vtk_algorithm_mapper_producer, vtk_mapper_consumer>
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(const Producer &producer, Consumer &consumer)
            {
                consumer.SetMapper(producer);
            }
        };
    };
    
    template<>
    struct connect_impl<vtk, vtk_prop_producer, vtk_renderer_filter>
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
    struct connect_impl<vtk, vtk_renderer_filter, vtk_rendererwindow_consumer>
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

template<typename T>
struct producer_category_of<
    vtk, T,
    typename boost::enable_if<boost::is_base_of<vtkActor,T> >::type
>
{
    typedef vtk_prop_producer type;
};

template<typename T>
struct consumer_category_of<
    vtk, T,
    typename boost::enable_if<boost::is_base_of<vtkActor,T> >::type
>
{
    typedef vtk_mapper_consumer type;
};

template<typename T>
struct consumed_type_of<
    vtk, T,
    typename boost::enable_if<boost::is_base_of<vtkActor,T> >::type
>
{
    typedef vtk_mapper_data type;
};

template<typename T>
struct filter_category_of<
    vtk, T,
    typename boost::enable_if<boost::is_base_of<vtkRenderer,T> >::type
>
{
    typedef vtk_renderer_filter type;
};

template<typename T>
struct consumer_category_of<
    vtk, T,
    typename boost::enable_if<boost::is_base_of<vtkRenderWindow,T> >::type
>
{
    typedef vtk_rendererwindow_consumer type;
};

} }

template<typename Producer, typename Consumer>
inline void connect(Producer &producer, Consumer &consumer)
{
    boost::dataflow::connect<boost::dataflow::vtk>(producer, consumer);
}

template<typename Producer, typename Consumer>
inline void connect(const Producer &producer, Consumer &consumer)
{
    boost::dataflow::connect<boost::dataflow::vtk>(producer, consumer);
}

template<typename Producer, typename Consumer>
inline void connect(Producer &producer, const Consumer &consumer)
{
    boost::dataflow::connect<boost::dataflow::vtk>(producer, consumer);
}

template<typename Producer, typename Consumer>
inline void connect(const Producer &producer, const Consumer &consumer)
{
    boost::dataflow::connect<boost::dataflow::vtk>(producer, consumer);
}

#endif // BOOST_DATAFLOW_VTK_SUPPORT_HPP