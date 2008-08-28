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

template<typename BaseView=base>
class active_colored : public BaseView
{
    typedef BaseView base_type;
public:
    template<typename ArgumentPack>
    active_colored(const ArgumentPack &args)
        : base_type(args)
        , m_active_color(args[_active_color])
    {}
    
    const color_type &active_color() const
    {   return m_active_color; }
    const void active_color(const color_type &color)
    {   m_active_color = color; }
protected:
    void use_active_color();
private:
    color_type m_active_color;
};

}}}

#endif // BOOST__GUIGL__VIEW__ACTIVE_COLORED_HPP