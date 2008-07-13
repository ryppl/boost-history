// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BLUEPRINT_COMPONENT_BANK_HPP
#define BLUEPRINT_COMPONENT_BANK_HPP

#include <boost/dataflow/blueprint/component_bank.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <glv.h>
#include <glv_binding_glut.h>

namespace boost { namespace dataflow { namespace glv_gui {

class blueprint_window;

class blueprint_bank
{
public:
    blueprint_bank()
        : m_view(glv::Rect(70, 600))
        , m_layout(m_view, glv::Direction::S, glv::Place::TL, 5, 5, 5)
    {
        m_view.colors().back.set(0);
    };

    void set_bank(const blueprint::component_bank &bank)
    {
        m_bank = bank;
        rearrange();
    }
    void set_blueprint(blueprint_window &blueprint)
    {
        m_blueprint = &blueprint;
    }
    
    glv::View &view()
    {   return m_view; }

    typedef boost::ptr_vector<glv::Button> component_buttons_type;
private:
    void rearrange();
    blueprint_window * m_blueprint;
    blueprint::component_bank m_bank;
    glv::View m_view;
    glv::Placer m_layout;
	
    component_buttons_type m_component_buttons;
    friend class component_button;
};

} } }

#endif
