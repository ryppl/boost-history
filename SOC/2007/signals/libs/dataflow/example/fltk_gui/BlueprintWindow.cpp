// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "BlueprintWindow.hpp"
#include "BlueprintComponent.hpp"
//#include <fltk/MenuBar.hpp>

namespace boost { namespace dataflow { namespace fltk_gui {

BlueprintWindow::BlueprintWindow(int width, int height, const char *label)
    : fltk::Window(width, height)
{
    copy_label(label);
    begin();
/*    fltk::MenuBar menubar(0,0,width,25);
    menubar.callback(callback);
    menubar.begin();
    fltk::ItemGroup file("&Blueprint");
    file.begin();
    Item* o = new Item("Instantiate connections");
    file.end();
    end();*/
}

void BlueprintWindow::add_component(std::auto_ptr<blueprint::component> c, const std::string &name)
{
    BlueprintComponent *bc = new BlueprintComponent(100, 100, 100, 100, name.c_str(), *c, m_network.add_component(c));
    add(bc);
    redraw();
}

bool BlueprintWindow::register_drop(BlueprintComponentPort *port)
{
    return m_network.are_connectable(m_dragged->component().id(), m_dragged->id(), port->component().id(), port->id());
}

void BlueprintWindow::connect_dragged_with(BlueprintComponentPort *port)
{
    m_network.add_connection(m_dragged->component().id(), m_dragged->id(), port->component().id(), port->id(), true);
}

} } }