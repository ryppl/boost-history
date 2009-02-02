/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__MOUSE_TRACKING_HPP
#define BOOST__GUIGL__VIEW__MOUSE_TRACKING_HPP

#include <boost/guigl/view/base.hpp>

namespace boost { namespace guigl {

struct mouse_state
{
    mouse_state()
        : inside(false)
        , position(0,0)
        , button_down(false)
    {}
    bool inside;
    position_type position;
    bool button_down;    
};

namespace view {

namespace detail {

    template<typename BaseView>
    struct mouse_tracking_static_visitor;

}

template<typename BaseView=base>
class mouse_tracking : public BaseView
{
    typedef BaseView base_type;
public:
    template<typename ArgumentPack>
    mouse_tracking(const ArgumentPack &args)
        : base_type(args)
    {}
    mouse_tracking(const mouse_tracking &rhs)
        : base_type(static_cast<const base_type &>(rhs))
    {}

    const guigl::mouse_state &mouse_state() const
    {   return m_mouse_state; }

protected:
    bool BOOST_EXPORT_SYMBOLS on_event(const event_type &event_info);

private:
    guigl::mouse_state m_mouse_state;

    friend struct detail::mouse_tracking_static_visitor<BaseView>;
};

}}}

#endif // BOOST__GUIGL__VIEW__CLICKABLE_HPP
