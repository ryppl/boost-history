// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BLUEPRINT_COMPONENT_HPP
#define BLUEPRINT_COMPONENT_HPP

#include "glv.h"
#include "blueprint_window.hpp"

namespace boost { namespace dataflow { namespace glv_gui {

class blueprint_invoke_button : public glv::Button
{
public:
    blueprint_invoke_button()
        : glv::Button(glv::Rect(60,60), false)
    {}
private:
    bool onEvent(glv::Event::t e, glv::GLV& glv);
};

class blueprint_component : public glv::View
{
    typedef blueprint::network::component_type id_type;
public:
    blueprint_component(const std::string &name, blueprint::component &c, id_type id);
    void set_component(blueprint::component &c, id_type id);
    void gui_begin();
    void gui_end();
    void invoke() {m_component->invoke();}
    id_type id() {return m_id;}
private:
    static blueprint_component *m_currently_constructed;
    blueprint::component *m_component;
    std::string m_name;
    id_type m_id;
    glv::View m_left_buttons, m_right_buttons;
    glv::Placer m_layout, m_left_layout, m_right_layout;
    blueprint_invoke_button m_invoke_button;  
};

class blueprint_component_port
    : public glv::Button
{
public:
    blueprint_component_port(size_t id)
        : glv::Button(glv::Rect(20,20), false), m_id(id)
    {
        *this << new glv::Label(">");
    }
    bool onEvent(glv::Event::t e, glv::GLV& glv);
    blueprint_component &component()
    {   return *static_cast<blueprint_component *>(parent->parent); }
    int center_x_root()
    {   return width()/2 + left() + parent->left() + parent->parent->left();}
    int center_y_root()
    {   return height()/2 + top() + parent->top() + parent->parent->top();}
    size_t id() {return m_id;}
    glv::space_t drag_x()
    {   return m_mouse_x; }
    glv::space_t drag_y()
    {   return m_mouse_y; }
private:
    blueprint_window &blueprint()
    {   return *static_cast<blueprint_window *>(parent->parent->parent); }
    size_t m_id;
    glv::space_t m_mouse_x, m_mouse_y;
};

} } }

#endif
