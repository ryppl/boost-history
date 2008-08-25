// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <glv.h>
#include "blueprint_component.hpp"
#include <iostream>
namespace boost { namespace dataflow { namespace glv_gui {

bool blueprint_invoke_button::onEvent(glv::Event::t e, glv::GLV& glv)
{
    bool response = glv::Button::onEvent(e, glv);
    if(e == glv::Event::MouseUp)
        static_cast<blueprint_component *>(parent)->invoke();
    return response;
}

bool blueprint_component_port::onEvent(glv::Event::t e, glv::GLV& glv)
{
    bool result = glv::Button::onEvent(e, glv);
    switch(e)
    {
        case glv::Event::MouseUp:
            blueprint().register_port_click(this);
        default:
            break;
    }
    return result;
}

blueprint_component::blueprint_component(const std::string &name, blueprint::component &c, id_type id)
    : glv::View(glv::Rect(100, 100))
    , m_label(name)
    , m_layout_style(0)
{
    (*this)	(glv::Event::MouseDrag, glv::Behavior::mouseMove);

    set_component(c, id);
    arrange_layout();
}

void blueprint_component::set_component(blueprint::component &c, id_type id)
{
    m_component=&c;
    m_id = id;

    if(c.is_invocable())
        m_invoke_button.enable(glv::Visible);
    else
        m_invoke_button.disable(glv::Visible);

    for (size_t i=0; i<c.num_ports(); i++)
        m_ports.push_back(new blueprint_component_port(i));
}

void blueprint_component::arrange_layout()
{
    glv::Placer m_layout, m_left_layout, m_right_layout;
    
    switch(m_layout_style)
    {
    case 1:
        m_layout = glv::Placer(*this, glv::Direction::E, glv::Place::CL, 0, 50, 0);
        m_left_buttons.extent(20, 100);
        m_right_buttons.extent(20, 100);
        m_invoke_button.extent(60, 60);
        m_left_layout = glv::Placer(m_left_buttons,glv::Direction::S, glv::Place::TL, 0, 0, 0);
        m_right_layout = glv::Placer(m_right_buttons,glv::Direction::S, glv::Place::TL, 0, 80, 0);
        break;
    default:
        m_layout = glv::Placer(*this, glv::Direction::S, glv::Place::TC, 50, 0, 0);
        m_left_buttons.extent(100, 20);
        m_right_buttons.extent(100, 20);
        m_invoke_button.extent(95,60);
        m_left_layout = glv::Placer(m_left_buttons,glv::Direction::E, glv::Place::TL, 0, 0, 0);
        m_right_layout = glv::Placer(m_right_buttons,glv::Direction::E, glv::Place::TL, 80, 0, 0);
    }

    m_label.size(1);
    m_label.size(100/m_label.width());
    m_label.pos(glv::Place::TL,0,20);
    *this << m_label;

    m_layout << m_left_buttons << m_invoke_button << m_right_buttons;

    for (size_t i=0; i<m_component->num_ports(); i++)
        if(m_component->get_port(i).traits().category().is<ports::producer>())
            m_right_layout << *m_ports[i];
        else
            m_left_layout << *m_ports[i];

    blueprint::based_component<glv::View> *view_component = dynamic_cast<blueprint::based_component<glv::View> *>(m_component);
    if(view_component)
    {
        glv::View *view = view_component->get_pointer();
        *this << *view;
        extent(100,200);
    }
    else
        extent(100, 100);
}
    
} } }
