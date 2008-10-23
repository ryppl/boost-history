/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__ACTIVE_COLORED_HPP
#define BOOST__GUIGL__VIEW__ACTIVE_COLORED_HPP

#include <boost/guigl/view/base.hpp>

namespace boost { namespace guigl { namespace view {

/// Behavior adding a modifiable active color which can be used for drawing.
template<typename BaseView=base>
class active_colored : public BaseView
{
    typedef BaseView base_type;
public:
    /** Sets the active color to the _active_color named parameter. */
    template<typename ArgumentPack>
    active_colored(const ArgumentPack &args)
        : base_type(args)
        , m_active_color(args[_active_color])
    {}
    active_colored(const active_colored &rhs)
        : base_type(static_cast<const base_type &>(rhs))
        , m_active_color(rhs.m_active_color)
    {}
    
    /** Returns the active color. */
    const color_type &active_color() const
    {   return m_active_color; }
    /** Sets the active color. */
    const void set_active_color(const color_type &color)
    {   m_active_color = color; }

protected:
    /** Sets the OpenGL drawing color to the active color. */
    void use_active_color();

private:
    color_type m_active_color;
};

}}}

#endif // BOOST__GUIGL__VIEW__ACTIVE_COLORED_HPP
