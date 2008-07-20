//[ glvgui_graph

// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <glv.h>
#include <glv_binding_glut.h>

#include "blueprint_bank.hpp"
#include "blueprint_window.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/dataflow/support/fusion_component.hpp>
#include <boost/dataflow/blueprint/component_bank.hpp>
#include <boost/dataflow/support/runtime.hpp>

namespace df=boost::dataflow;
namespace blueprint=boost::dataflow::blueprint;

// provide Dataflow support for a BGL graph type
namespace boost { namespace dataflow { namespace graph {

struct tag : public default_tag
{};

typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS> graph_type;

struct vertex;
struct in_vertex_port;
struct out_vertex_port;

struct out_vertex_port_traits : public dataflow::complemented_port_traits<ports::producer, in_vertex_port, tag>
{};

struct out_vertex_port : public dataflow::port<out_vertex_port_traits>
{
    out_vertex_port(vertex &c)
        : m_vertex(c) {}
    vertex &m_vertex;
};

struct in_vertex_port_traits : public dataflow::complemented_port_traits<ports::consumer, out_vertex_port, tag>
{};

struct in_vertex_port : public dataflow::port<in_vertex_port_traits>
{
    in_vertex_port(vertex &c)
        : m_vertex(c) {}
    vertex &m_vertex;
};

struct component_traits
    : public dataflow::fusion_component_traits<
        fusion::vector<
            in_vertex_port,
            out_vertex_port            
        >,
        mpl::map< >,
        tag>
{
    static fusion_ports get_ports(vertex &v)
    {
        return fusion_ports(
            in_vertex_port(v),
            out_vertex_port(v));
    }
};

struct vertex : public dataflow::component<component_traits>
{
    vertex(graph_type &graph)
        : m_graph(graph)
    {
        m_vertex_descriptor = add_vertex(graph);
    }
    graph_type &m_graph;
    graph_type::vertex_descriptor m_vertex_descriptor;
};

// an empty component that can be invoked... nothing to do with BGL
struct invocable_component_traits : public dataflow::fusion_component_traits<fusion::vector<>, mpl::map<>, df::graph::tag>
{
    template<typename Component>
    static fusion_ports get_ports(Component &component)
    {
        return fusion_ports();
    }
};

class invocable_component : public dataflow::component<invocable_component_traits>
{
public:
    invocable_component(graph_type &graph)
        : m_graph(graph)
    {}
protected:
    graph_type &m_graph;
};

}

namespace extension {

    // how to connect two vertices
    template<>
    struct binary_operation_impl<graph::out_vertex_port_traits, graph::in_vertex_port_traits, operations::connect>
    {
        typedef void result_type;
        
        template<typename Producer, typename Consumer>
        result_type operator()(Producer &producer, Consumer &consumer)
        {
            BOOST_ASSERT(&producer.m_vertex.m_graph == &consumer.m_vertex.m_graph);
            add_edge(producer.m_vertex.m_vertex_descriptor, consumer.m_vertex.m_vertex_descriptor, producer.m_vertex.m_graph);
        }
    };

    // how to invoke a component
    template<>
    struct component_operation_impl<graph::invocable_component_traits, operations::invoke>
    {
        typedef void result_type;
        
        template<typename Component>
        result_type operator()(Component &component)
        {
            component.invoke();
        }
    };

}

} } 

DATAFLOW_RUNTIME_PROPERTY(graph::tag, const char *, "Dataflow.Graph")
// 30 us supposed to be a UUID :-)
DATAFLOW_RUNTIME_PROPERTY(graph::tag, int, 30)

// A component that computes the number of strongly connected components in the
// graph
class strong_components_computer
    : public df::graph::invocable_component
    , public glv::Label
{
public:
    typedef glv::View runtime_base_class_type;

    strong_components_computer(df::graph::graph_type &graph)
        : df::graph::invocable_component(graph)
        , glv::Label("")
    {
        anchor(glv::Place::CL);
        size(80);
    }
    void invoke()
    {
        std::vector<int> component(num_vertices(m_graph)), discover_time(num_vertices(m_graph));
        std::vector<boost::default_color_type> color(num_vertices(m_graph));
        std::vector<unsigned> root(num_vertices(m_graph));
        int num = boost::strong_components(m_graph, &component[0], 
                              boost::root_map(&root[0]).
                              color_map(&color[0]).
                              discover_time_map(&discover_time[0]));

        label(boost::lexical_cast<std::string>(num));
    }
};

// A component_bank with some components
class example_bank : public blueprint::tag_component_bank<df::graph::tag>
{
public:
    example_bank()
    {
        add_component<df::graph::vertex>("v", boost::ref(m_graph));
        add_component<strong_components_computer>("sc", boost::ref(m_graph));
    }
private:
    // this is the BGL graph
    df::graph::graph_type m_graph;
};

int main()
{
    glv::GLV top;
	glv::Window win(640, 640, "GLV Blueprint GUI", &top);
    glv::Placer placer(top, glv::Direction::E, glv::Place::TL, 0, 0, 0);
    
    using namespace boost::dataflow::glv_gui;

    // create the blueprint and component windows
    blueprint_bank bank;
    blueprint_window window;

    // initialize the bank
    bank.set_bank(example_bank());
    bank.set_blueprint(window);
    
    placer << bank.view() << window;
    glv::Application::run();
}

//]

