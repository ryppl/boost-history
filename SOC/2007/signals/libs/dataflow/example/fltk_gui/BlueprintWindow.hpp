// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BLUEPRINT_WINDOW_HPP
#define BLUEPRINT_WINDOW_HPP

#include <fltk/Window.h>

#include <boost/dataflow/blueprint/network.hpp>
#include <boost/dataflow/signals/runtime_support.hpp>

namespace boost { namespace dataflow { namespace fltk_gui {

class BlueprintComponent;
class BlueprintComponentPort;

class BlueprintWindow : public fltk::Window
{
public:
    BlueprintWindow(int width, int height, const char *label = 0);
    void add_component(std::auto_ptr<blueprint::component> c, const std::string &name);
    void register_drag(BlueprintComponentPort *port)
    {
        m_dragged = port;
    }
    bool register_drop(BlueprintComponentPort *port);
    void connect_dragged_with(BlueprintComponentPort *port);
private:
    blueprint::network m_network;
    std::vector<BlueprintComponent *> m_components;
    BlueprintComponentPort *m_dragged;
};

} } }
#endif