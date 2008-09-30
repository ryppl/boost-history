/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__DRAGGABLE_HPP
#define BOOST__GUIGL__VIEW__DRAGGABLE_HPP

#include <boost/guigl/view/base.hpp>
#include <boost/guigl/view/mouse_tracking.hpp>

namespace boost { namespace guigl { namespace view {

namespace detail {

    template<typename Derived, typename BaseView>
    struct draggable_static_visitor;

}

/// Behavior capturing dragging.
/** Behavior capturing dragging (moving the mouse over the view with the button
    held down).
*/
template<typename Derived, typename BaseView=base>
class draggable : public mouse_tracking<BaseView>
{
public:
    typedef mouse_tracking<BaseView> base_type;

    template<typename ArgumentPack>
    draggable(const ArgumentPack &args)
        : base_type(args)
        , m_dragging(false)
    {}
    draggable(const draggable &rhs)
        : base_type(static_cast<const base_type &>(rhs))
        , m_dragging(false)
    {}

protected:
    bool on_event(const event_type &event_info);
    const position_type &drag_origin() const
    {   return m_drag_origin; }
    void set_drag_origin(const position_type &origin)
    {   m_drag_origin = origin; }

private:
    void call_draggable_on_drag(const position_type &position)
    {
        static_cast<Derived *>(this)->draggable_on_drag(position);
    }
    void call_draggable_on_end_drag(const position_type &position)
    {
        static_cast<Derived *>(this)->draggable_on_end_drag(position);
    }

    friend struct detail::draggable_static_visitor<Derived,BaseView>;
    
    position_type m_drag_origin;
    bool m_dragging;
};

}}}

#endif // BOOST__GUIGL__VIEW__CLICKABLE_HPP