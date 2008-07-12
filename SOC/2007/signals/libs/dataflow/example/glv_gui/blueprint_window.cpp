// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <glv.h>
#include "blueprint_window.hpp"
#include "blueprint_component.hpp"

namespace boost { namespace dataflow { namespace glv_gui {

blueprint_window::blueprint_window()
    : glv::View(glv::Rect(800, 600))
    , m_layout(*this, glv::Direction::E, glv::Place::TL, 5, 5, 5)
    , m_dragged(NULL)
{
    this->colors().back.set(0);
    m_next_created_x = 100;
    m_next_created_y = 50;
}

void blueprint_window::add_component(std::auto_ptr<blueprint::component> c, const std::string &name)
{
    std::cout << "Adding component" << name << std::endl;
    blueprint_component *bc = new blueprint_component(name, *c, m_network.add_component(c));
    add_component_helper(*bc);
}

void blueprint_window::add_component(boost::function<blueprint::component *()> f, const std::string &name)
{
    std::cout << "Adding component" << name << std::endl;

    std::auto_ptr<blueprint::component> c(f());
    blueprint_component *bc = new blueprint_component(name, *c, m_network.add_component(c));
    add_component_helper(*bc);

/*    gui::wnd<blueprint_component> bc = gui::create<blueprint_component>
        ( gui::_pos = std::make_pair(m_next_created_x, m_next_created_y)
        , gui::_size = std::make_pair(100, 100)
        , gui::_text = name
        , gui::_parent = this_);*/

//    std::auto_ptr<blueprint::component> c(f());
//    blueprint::network::component_type id = m_network.add_component(c);
//    bc->set_component(m_network[id], id);


//    add_component_helper(*bc);
}

void blueprint_window::add_component_helper(blueprint_component &bc)
{
    m_layout << bc;
}


void blueprint_window::register_port_click(blueprint_component_port *port)
{
    if(m_dragged)
    {
        std::cout << "Attempting connect..." << std::endl;
        if (m_network.are_connectable(m_dragged->component().id(), m_dragged->id(), port->component().id(), port->id()))
        {
            m_network.add_connection(m_dragged->component().id(), m_dragged->id(), port->component().id(), port->id(), true);
            m_connections.push_back(connection(m_dragged, port));
        }
        else
            std::cout << "Ports not connectable" << std::endl;
        m_dragged = NULL;
    }
    else
    {
        std::cout << "Initiating connect - click on destination port to make connection" << std::endl;
        m_dragged = port;
    }
}

void blueprint_window::onDraw()
{
    glv::draw::begin(glv::draw::Lines);
/*    if(m_dragged)
    {
		glv::draw::color(1, 0, 0);			// set color to red
		glv::draw::vertex(m_dragged->center_x_root(),m_dragged->center_y_root());
        glv::draw::vertex(m_dragged->drag_x() - left(), m_dragged->drag_y() - top());
    }*/
    for (std::vector<connection>::iterator it=m_connections.begin(); it!=m_connections.end(); it++)
    {
        glv::draw::color(0, 1, 0);
        glv::draw::vertex(it->first->center_x_root(), it->first->center_y_root());
        glv::draw::vertex(it->second->center_x_root(), it->second->center_y_root());
    }
    glv::draw::end();
}

namespace detail
{
    double distance(int x0, int y0, int x1, int y1)
    {
        int dx = x1 - x0;
        int dy = y1 - y0;
        return sqrt(dx*dx+dy*dy);    
    }
    
    double distance(int x0, int y0, int x1, int y1, int xp, int yp)
    {
        int dx = x1 - x0;
        int dy = y1 - y0;
        if (dx==0 && dy==0)
            return distance(x0, y0, xp, yp);
            
        double r = double((xp - x0) * dx + (yp - y0) * dy) / (dx*dx + dy*dy);
        if (r<0)
            return distance(x0, y0, xp, yp);
        else if (r>1)
            return distance(x1, y1, xp, yp);
        else
            return distance((1-r)*x0 + r*x1, (1-r)*y0 + r*y1, xp, yp);
    }
}

/*
int blueprint_window::handle(int event)
{
    if (fltk::Window::handle(event))
        return 1;
    bool found=false;
    if(event == fltk::PUSH)
        for (std::vector<Connection>::iterator it=m_connections.begin(); it!=m_connections.end(); it++)
            if (detail::distance(
                it->first->center_x_root(),
                it->first->center_y_root(),
                it->second->center_x_root(),
                it->second->center_y_root(),
                fltk::event_x(),
                fltk::event_y()) < 3)
            {
                m_selected = it;
                found = true;
            }
    if (!found)
        m_selected = std::vector<Connection>::iterator();
    redraw();
    return 0;
}*/


} } }