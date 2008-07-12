// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <glv.h>
#include "blueprint_component.hpp"

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
    , m_left_buttons(glv::Rect(20, 100))
    , m_right_buttons(glv::Rect(20, 100))
    , m_layout(*this, glv::Direction::E, glv::Place::CL, 0, 50, 0)
    , m_left_layout(m_left_buttons,glv::Direction::S, glv::Place::TL, 0, 0, 0)
    , m_right_layout(m_right_buttons,glv::Direction::S, glv::Place::TL, 0, 0, 0)
{
    glv::Label *l = new glv::Label(name);
    l->size(20);
    l->pos(glv::Place::TL,20,20);
    *this << *l;
    m_layout << m_left_buttons << m_invoke_button << m_right_buttons;
    (*this)	(glv::Event::MouseDrag, glv::Behavior::mouseMove);

    //    BlueprintComponent_(label);
    set_component(c, id);
}

void blueprint_component::set_component(blueprint::component &c, id_type id)
{
    m_component=&c;
    m_id = id;
        
    if(c.is_invocable())
        m_invoke_button.enable(glv::Visible);
    else
        m_invoke_button.disable(glv::Visible);
    
    blueprint::based_component<glv::View> *view_component = dynamic_cast<blueprint::based_component<glv::View> *>(&c);
    if(view_component)
    {
        std::cout << "View" << std::endl;
        glv::View *view = view_component->get_pointer();
        *this << *view;
        extent(100,200);
    }
    else
        std::cout << "Not View" << std::endl;
    
    for (size_t i=0; i<c.num_ports(); i++)
        if (c.get_port(i).traits().category().is<ports::producer>())
            m_right_layout << *(new blueprint_component_port(i));
        else
            m_left_layout << *(new blueprint_component_port(i));
}

blueprint_component *blueprint_component::m_currently_constructed;
    
} } }
