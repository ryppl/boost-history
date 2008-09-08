/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__WINDOW_HPP
#define BOOST__GUIGL__VIEW__WINDOW_HPP

#include <boost/guigl/view/base.hpp>

namespace boost { namespace guigl { namespace view {

template<typename BaseView=base>
class window : public BaseView
{
public:
    typedef BaseView base_type;

    template<typename Args>
    window(const Args &args)
        : base_type(args)
        , m_background_color(args[_background])
    {}
    
    const color_type &background_color() const
    {   return m_background_color; }
    
protected:
    void draw_prologue();

private:
    color_type m_background_color;
};

}}}

#endif // BOOST__GUIGL__WINDOW_VIEW_HPP