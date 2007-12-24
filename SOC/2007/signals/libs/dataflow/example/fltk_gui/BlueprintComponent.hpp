// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BLUEPRINT_COMPONENT_HPP
#define BLUEPRINT_COMPONENT_HPP

#include <fltk/Button.h>
#include "BlueprintWindow.hpp"

namespace boost { namespace dataflow { namespace fltk_gui {

class BlueprintComponent : public fltk::Group
{
    typedef blueprint::network::component_type id_type;
public:
    BlueprintComponent(int x, int y, int w, int h, const char *label, blueprint::component &c, id_type id);
    void invoke() {m_component.invoke();}
    id_type id() {return m_id;}
private:
    blueprint::component &m_component;
    std::string m_name;
    id_type m_id;
};

class BlueprintComponentPort : public fltk::Button
{
public:
    BlueprintComponentPort(int x, int y, int w, int h, const char *label, size_t id)
        : fltk::Button(x, y, w, h, label), m_id(id)
    {}
    int handle(int event);
    BlueprintComponent &component()
    {   return *static_cast<BlueprintComponent *>(parent()); }
    size_t id() {return m_id;}
private:
    BlueprintWindow &blueprint()
    {   return *static_cast<BlueprintWindow *>(parent()->parent()); }
    size_t m_id;
};

} } }

#endif