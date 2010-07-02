// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <glv.h>
#include "blueprint_window.hpp"
#include "blueprint_component.hpp"
#include <boost/foreach.hpp>

namespace boost { namespace dataflow { namespace glv_gui {

blueprint_window::blueprint_window()
    : glv::View(glv::Rect(800, 600))
    , m_dragged(NULL)
    , m_layout(100, 200, glv::Rect(600, 800), 2, 5)
    , m_status("Status bar...", 0, 580)
    , m_rotate_component_layout(glv::Rect(50, 50), false)
{
    *this << m_status;
    m_rotate_component_layout << *new glv::Label("Change\nStyle");
    m_rotate_component_layout.pos(glv::Place::TL, 520, 550);
    *this << m_rotate_component_layout;
    m_rotate_component_layout.attachHandler(style_change_handler, this);
    m_layout.parent = this;
    this->colors().back.set(0);
}

void blueprint_window::style_change_handler(glv::Notifier *sender, void *this__)
{
    blueprint_window *this_(reinterpret_cast<blueprint_window *>(this__));
    
    BOOST_FOREACH(blueprint_component *c, this_->m_components)
    {
        c->layout_style(!c->layout_style());
        c->arrange_layout();
        this_->m_status.label("Switched style to " + boost::lexical_cast<std::string>(c->layout_style()));
    }
}


void blueprint_window::add_component(std::auto_ptr<blueprint::component> c, const std::string &name)
{
    m_status.label("Adding component");

    blueprint::component &cr = *c;
    blueprint_component *bc = new blueprint_component(name, cr, m_network.add_component(c));
    m_components.push_back(bc);
    m_layout << *bc;
    bc->arrange_layout();
}

void blueprint_window::register_port_click(blueprint_component_port *port)
{
    if(m_dragged)
    {
        if (m_network.are_connectable(m_dragged->component().id(), m_dragged->id(), port->component().id(), port->id()))
        {
            m_network.add_connection(m_dragged->component().id(), m_dragged->id(), port->component().id(), port->id(), true);
            m_connections.push_back(connection(m_dragged, port));
            m_status.label("Ports connected...");
        }
        else
            m_status.label("Ports not connectable");
        m_dragged = NULL;
    }
    else
    {
        m_status.label("Initiating connect - click on destination port to make connection");
        m_dragged = port;
    }
}

void blueprint_window::onDraw()
{
    glv::draw::lineWidth(5);
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
        return sqrt(double(dx*dx+dy*dy));    
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