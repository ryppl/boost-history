/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__MOUSE_TRACKING_HPP
#define BOOST__GUIGL__VIEW__IMPL__MOUSE_TRACKING_HPP

#include <boost/guigl/view/mouse_tracking.hpp>
#include <boost/guigl/platform/opengl.hpp>
#include <boost/guigl/widget/window.hpp>

#include <boost/utility/enable_if.hpp>

#include <iostream>

namespace boost { namespace guigl { namespace view {

namespace detail {

    template<typename BaseView, typename Enable=void>
    struct get_button_state
    {
        static bool is_down(const mouse_tracking<BaseView> &)
        {   return false; }
    };
    
    template<typename BaseView>
    struct get_button_state<BaseView, typename enable_if<is_positioned<BaseView> >::type>
    {
        static bool is_down(mouse_tracking<BaseView> &view)
        {
            const widget::window *window = dynamic_cast<const widget::window *> (view.root());
            return window->mouse_state().button_down;
        }
    };

    template<typename BaseView>
    struct mouse_tracking_static_visitor
        : public boost::static_visitor<>
    {
    public:
        mouse_tracking_static_visitor(mouse_tracking<BaseView> &m)
            : m_mouse_tracking(m)
        {}
        
        typedef bool result_type;
        
        template<typename EventInfo>
        bool operator()(const EventInfo &) const
        {
            return false;
        }

        bool operator()(const movement_event &event_info) const
        {
            m_mouse_tracking.m_mouse_state.position = event_info.position;
            return false;
        }

        bool operator()(const button_event &event_info) const
        {
            m_mouse_tracking.m_mouse_state.button_down = (event_info.direction == direction::down);
            return false;
        }
        
        bool operator()(const entry_exit_event &event_info) const
        {
            if(event_info.region == region::exit)
                m_mouse_tracking.m_mouse_state.inside = false;
            else
            {
                m_mouse_tracking.m_mouse_state.inside = true;
                m_mouse_tracking.m_mouse_state.button_down = get_button_state<BaseView>::is_down(m_mouse_tracking);
            }
            return false;
        }

        mouse_tracking<BaseView> &m_mouse_tracking;
    };

}

template<typename BaseView>
inline bool mouse_tracking<BaseView>::on_event(const event_type &event_info)
{
    bool response = base_type::on_event(event_info);
    boost::apply_visitor(detail::mouse_tracking_static_visitor<BaseView>(*this), event_info);
    return response;
}


}}}

#endif // BOOST__GUIGL__VIEW__IMPL__MOUSE_TRACKING_HPP