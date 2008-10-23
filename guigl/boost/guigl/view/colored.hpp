/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__COLORED_HPP
#define BOOST__GUIGL__VIEW__COLORED_HPP

#include <boost/guigl/view/base.hpp>

namespace boost { namespace guigl { namespace view {

template<typename BaseView=base>
class colored : public BaseView
{
    typedef BaseView base_type;
public:
    template<typename ArgumentPack>
    colored(const ArgumentPack &args)
        : base_type(args)
        , m_color(args[_color])
    {}
    colored(const colored &rhs)
        : base_type(static_cast<const base_type &>(rhs))
        , m_color(rhs.m_color)
    {}
    
    const color_type &color() const
    {   return m_color; }
    const void set_color(const color_type &color)
    {   m_color = color; }
    
protected:
    void draw_prologue();

private:
    color_type m_color;
};

}}}

#endif // BOOST__GUIGL__VIEW__COLORED_HPP
