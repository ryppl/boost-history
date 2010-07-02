/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__NAVIGABLE_HPP
#define BOOST__GUIGL__VIEW__NAVIGABLE_HPP

#include <boost/guigl/view/base.hpp>
#include <boost/guigl/view/draggable.hpp>
#include <boost/guigl/window.hpp>

namespace boost { namespace guigl { namespace view {

template<typename BaseView=base>
class navigable : public draggable<navigable<BaseView>, guigl::button::left_type, BaseView>
{
public:
    typedef draggable<navigable<BaseView>, guigl::button::left_type, BaseView> base_type;

    template<typename ArgumentPack>
    navigable(const ArgumentPack &args)
        : base_type(args)
        , m_angle(0,0)
        , m_distance(500)
    {}
    navigable(const navigable &rhs)
        : base_type(static_cast<const base_type &>(rhs))
        , m_angle(rhs.m_angle)
        , m_distance(rhs.m_distance)
    {}

    double distance() const
    {   return m_distance; }
    
    void set_distance(double distance)
    {
        m_distance = distance;
        guigl::window::redraw(*this);
    }
protected:
    void draw_prologue();
    void draw_epilogue();
    
    void draggable_on_drag(const position_type &position);
    void draggable_on_end_drag(const position_type &position) {};

    friend class draggable<navigable<BaseView>, guigl::button::left_type, BaseView>;

private:
    position_type m_angle;
    double m_distance;
};

}}}

#endif // BOOST__GUIGL__VIEW__NAVIGABLE_HPP