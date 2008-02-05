// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "BlueprintComponent.hpp"
#include <fltk/events.h>

namespace boost { namespace dataflow { namespace fltk_gui {

class BlueprintComponentCore : public fltk::Button
{
public:
    BlueprintComponentCore(int x, int y, int w, int h, bool invocable)
        : fltk::Button(x, y, w, h, invocable ? "invoke" : 0)
        , m_dragged(false)
        , m_invocable(invocable)
    {
        callback((fltk::Callback *)on_push, this);
    }
    int handle(int event);
private:
    static void on_push(fltk::Widget *widget, BlueprintComponentCore *core)
    {
        if (!core->m_dragged && core->m_invocable)
            core->component().invoke();
    }
    BlueprintComponent &component()
    {   return *static_cast<BlueprintComponent *>(parent()); }
    int push_x, push_y;
    bool m_dragged;
    bool m_invocable;
};

int BlueprintComponentCore::handle(int event)
{
    switch (event)
    {
    case fltk::DRAG:
        parent()->position(parent()->x() + fltk::event_x_root() - push_x, parent()->y() + fltk::event_y_root() - push_y);
    case fltk::PUSH:
        push_x = fltk::event_x_root();
        push_y = fltk::event_y_root();
        m_dragged = event==fltk::DRAG;
        return fltk::Button::handle(event);
    }
    return fltk::Button::handle(event);
}

int BlueprintComponentPort::handle(int event)
{
    switch(event)
    {
        case fltk::DND_RELEASE:
            return blueprint().register_drop(this);;
        case fltk::DND_ENTER:
        case fltk::DND_LEAVE:
            return 1;
        case fltk::PASTE:
            blueprint().connect_dragged_with(this);
            return 1;
        case fltk::DRAG:
            fltk::copy("port", 4);
            blueprint().register_drag(this);
            fltk::dnd();
    }
    return fltk::Button::handle(event);
}

BlueprintComponent::BlueprintComponent(int x, int y, int w, int h, const char *label, blueprint::component &c, id_type id)
    : fltk::Group(x, y, w, h)
{
    BlueprintComponent_(label);
    set_component(c, id);
}

BlueprintComponent::BlueprintComponent(int x, int y, int w, int h, const char *label)
    : fltk::Group(x, y, w, h)
{
    BlueprintComponent_(label);
}

void BlueprintComponent::BlueprintComponent_(const char *label)
{
    copy_label(label);
    add(m_gui_group = new fltk::Group(0,80,100,60));
    resize(100, 160);
}

void BlueprintComponent::gui_begin()
{
    m_gui_group->begin();
}

void BlueprintComponent::gui_end()
{
    m_gui_group->end();
}

void BlueprintComponent::set_component(blueprint::component &c, id_type id)
{
    m_component=&c;
    m_id = id;
    add(new BlueprintComponentCore(20, 20, 60, 60, c.is_invocable()));
    
    int y_left = 20;
    int y_right = 20;
    
    for (size_t i=0; i<c.num_ports(); i++)
        if (c.get_port(i).traits().category().is<ports::producer>())
        {
            add(new BlueprintComponentPort(80, y_right, 20, 20, "@->", i));
            y_right+=20;
        }
        else
        {
            add(new BlueprintComponentPort(0, y_left, 20, 20, "@>", i));
            y_left+=20;
        }
}


} } }
