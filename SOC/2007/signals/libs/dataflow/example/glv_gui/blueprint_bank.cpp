// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "blueprint_bank.hpp"
#include "blueprint_window.hpp"

namespace boost { namespace dataflow { namespace glv_gui {

class component_button : public glv::Button
{
public:
    component_button(blueprint_bank &bank, const std::string &label)
        : glv::Button(glv::Rect(60, 60), false), m_label(label), m_bank(bank)
    {
        glv::Label *l = new glv::Label(label);
        l->pos(glv::Place::TL,0,5);
        l->size(1);
        l->size(60/l->width());
        *this << *l;
    }
    void spawn()
    {
        m_bank.m_blueprint->add_component(std::auto_ptr<blueprint::component>(m_bank.m_bank[m_label]()), m_label);
    }
private:
    std::string m_label;
    blueprint_bank &m_bank;
};

bool spawn_component(glv::View * v, glv::GLV & glv)
{
	component_button * b = static_cast<component_button *>(v);

	b->spawn();
    return false;
}

void blueprint_bank::rearrange()
{
    for(blueprint::component_bank::iterator it=m_bank.begin(); it!=m_bank.end(); it++)
    {
        m_component_buttons.push_back(new component_button(*this, it->c_str()));
        m_component_buttons.back()(glv::Event::MouseDown, spawn_component);
        m_layout << m_component_buttons.back();
    }
}

} } }
