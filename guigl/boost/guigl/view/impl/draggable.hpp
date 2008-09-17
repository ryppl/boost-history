/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__DRAGGABLE_HPP
#define BOOST__GUIGL__VIEW__IMPL__DRAGGABLE_HPP

#include <boost/guigl/view/draggable.hpp>
#include <boost/guigl/view/impl/mouse_tracking.hpp>
#include <boost/guigl/platform/opengl.hpp>


namespace boost { namespace guigl { namespace view {

namespace detail {

    template<typename Derived, typename BaseView>
    struct draggable_static_visitor
        : public boost::static_visitor<>
    {
    public:
        draggable_static_visitor(draggable<Derived, BaseView> &c)
            : m_draggable(c)
        {}
        
        typedef bool result_type;
        
        template<typename EventInfo>
        bool operator()(const EventInfo &) const
        {
            return false;
        }

        bool operator()(const button_event &event_info) const
        {
            if(event_info.direction == direction::down)
            {
                m_draggable.m_drag_origin = m_draggable.mouse_state().get().position;
                m_draggable.call_draggable_on_drag(m_draggable.mouse_state().get().position);
            }
            else
                m_draggable.call_draggable_on_end_drag(m_draggable.mouse_state().get().position);
            return true;
        }
        
        bool operator()(const movement_event &event_info) const
        {
            if(m_draggable.mouse_state().get().button_down)
                m_draggable.call_draggable_on_drag(event_info.position);
            return true;
        }
        
        draggable<Derived, BaseView> &m_draggable;
    };

}

template<typename Derived, typename BaseView>
inline bool draggable<Derived, BaseView>::on_event(const event_type &event_info)
{
    if(base_type::on_event(event_info))
        return true;
    else
        return boost::apply_visitor(detail::draggable_static_visitor<Derived,BaseView>(*this), event_info);
}


}}}

#endif // BOOST__GUIGL__VIEW__IMPL__DRAGGABLE_HPP