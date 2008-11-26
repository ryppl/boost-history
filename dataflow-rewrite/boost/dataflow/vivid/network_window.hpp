/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__VIVID__NETWORK_WINDOW_HPP
#define BOOST__DATAFLOW__VIVID__NETWORK_WINDOW_HPP


#include <boost/dataflow/blueprint/port_binary_operation.hpp>
#include <boost/dataflow/blueprint/operation_adapter.hpp>
#include <boost/dataflow/vivid/entity_widget.hpp>
#include <boost/guigl/draw.hpp>
#include <boost/guigl/geometry.hpp>
#include <boost/guigl/window.hpp>
#include <boost/guigl/widget/compound.hpp>
#include <boost/guigl/widget/label.hpp>
#include <boost/guigl/layout/grid.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <memory>


namespace boost { namespace dataflow { namespace vivid {


template<typename BlueprintFramework>
class network_widget : public guigl::widget::compound
{
    typedef entity_widget<BlueprintFramework> entity_widget_type;

public:
    typedef guigl::widget::compound base_type;
    
    template<typename Args>
    network_widget(const Args &args)
        : base_type(args)
    {}
    void draw_prologue()
    {
        base_type::draw_prologue();
        glLineWidth(5);
        glBegin(GL_LINES);
        glColor3f(0,1,0);
        for(typename std::vector<connection_type>::iterator it=m_connections.begin(); it!= m_connections.end(); it++)
        {
            guigl::draw::vertex(guigl::geometry::midpoint(*it->first));
            guigl::draw::vertex(guigl::geometry::midpoint(*it->second));
        }
        glEnd();
    }
    void draw()
    {
        draw_prologue();
        draw_epilogue();
    }
    void add_connection(entity_widget_type *port1, entity_widget_type *port2)
    {
        m_connections.push_back(connection_type(port1, port2));
    }
private:
    typedef std::pair<entity_widget_type *,entity_widget_type *> connection_type;
    std::vector<connection_type> m_connections;
};

template<typename BlueprintFramework>
class network_window
{
    typedef blueprint::framework_entity<BlueprintFramework> framework_entity_type;
    typedef blueprint::framework_context<BlueprintFramework> framework_context_type;
    typedef entity_widget<BlueprintFramework> entity_widget_type;
    typedef blueprint::port<BlueprintFramework> port_type;
public:
    network_window()
        : m_window(( guigl::_label="Network", guigl::_size=guigl::size_type(800, 600) ))
        , m_layout(( guigl::_grid_size=guigl::size_type(800, 500), guigl::_horizontal=8, guigl::_vertical=5 ))
        , m_dragged(0)
    {
        m_window << (m_status = new guigl::widget::label((
            guigl::_label="Status bar...",
            guigl::_size(800, 50),
            guigl::_position(0, 550) )) );
        m_window << (m_network_widget = new network_widget<BlueprintFramework>(( guigl::_background(1,1,1), guigl::_size(800, 500) )) );
        
        add_operation(typename mpl::at_c<typename BlueprintFramework::framework_type::operations, 0>::type());
        m_selected_operation = &m_operations[0];
    }
    
    void add_entity(std::auto_ptr<framework_entity_type> entity)
    {
        entity_widget_type *widget = m_layout.create<entity_widget_type>((
            _entity=entity.release(),
            guigl::_background(.5,.5,.5),
            guigl::_label="entity" ));
            
        (*m_network_widget) << widget;
        guigl::window::redraw(*m_network_widget);
        
        widget->on_click.connect(boost::bind(&network_window::clicked, this, widget));
    }
    
    framework_context_type &framework_context()
    {   return m_framework_context; }
    
    template<typename Operation>
    void add_operation(const Operation &operation)
    {
        m_operations.push_back(new blueprint::operation_adapter<Operation>(operation));
    }
private:
    void clicked(entity_widget_type *widget)
    {
        if(m_dragged)
        {
            port_type
                & port1 = static_cast<blueprint::port<BlueprintFramework> &>(m_dragged->entity())
              , & port2 = static_cast<blueprint::port<BlueprintFramework> &>(widget->entity());
            
            if (port_binary_operation_will_succeed(port1, port2, *m_selected_operation))
            {
                port_binary_operation(port1, port2, *m_selected_operation);
                m_network_widget->add_connection(m_dragged, widget);
                m_status->set_label("Ports connected...");
            }
            else
                m_status->set_label("Ports not connectable");
            m_dragged = NULL;
        }
        else
        {
            m_status->set_label("Initiating connect - click on destination port to make connection");
            m_dragged = widget;
        }
    }

    framework_context_type m_framework_context;
    guigl::window m_window;
    guigl::widget::label *m_status;
    network_widget<BlueprintFramework> *m_network_widget;
    guigl::layout::grid m_layout;
    entity_widget_type *m_dragged;
    
    blueprint::operation *m_selected_operation;
    boost::ptr_vector<blueprint::operation> m_operations;
};

} } }
#endif // BOOST__DATAFLOW__VIVID__NETWORK_WINDOW_HPP
