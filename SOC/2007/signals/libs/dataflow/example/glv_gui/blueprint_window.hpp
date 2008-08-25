// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BLUEPRINT_WINDOW_HPP
#define BLUEPRINT_WINDOW_HPP

#include "glv.h"
#include "glv_pimpl_binding.h"

#include <boost/dataflow/blueprint/network.hpp>
#include <boost/dataflow/signals/runtime_support.hpp>

namespace boost { namespace dataflow { namespace glv_gui {

class blueprint_component;
class blueprint_component_port;

class connection
    :  public std::pair<blueprint_component_port *, blueprint_component_port *>
{
public:
    connection(blueprint_component_port *p1, blueprint_component_port *p2)
        : std::pair<blueprint_component_port *, blueprint_component_port *>(p1, p2)
    {}
};

class blueprint_window
    : public glv::View
{
public:
    blueprint_window();
    void add_component(std::auto_ptr<blueprint::component> c, const std::string &name);
    void register_port_click(blueprint_component_port *port);
private:
    static void style_change_handler(glv::Notifier *sender, void *this_);
    void onDraw();
    blueprint::network m_network;
    std::vector<blueprint_component *> m_components;
    blueprint_component_port *m_dragged;
    std::vector<connection> m_connections;
    std::vector<connection>::iterator m_selected;
    glv::LayoutGrid m_layout;
    glv::Label m_status;
    glv::Button m_rotate_component_layout;
};

} } }
#endif
