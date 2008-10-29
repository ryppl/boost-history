/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__CLICKABLE_HPP
#define BOOST__GUIGL__VIEW__IMPL__CLICKABLE_HPP


#include <boost/guigl/view/clickable.hpp>


namespace boost { namespace guigl { namespace view {

template<typename Derived, typename Button, typename BaseView>
inline void clickable<Derived, Button, BaseView>::button_down(bool state)
{
    m_button_down = state;
    static_cast<Derived *>(this)->clickable_button_down(state);
}

namespace detail {

    template<typename Derived, typename Button, typename BaseView>
    struct clickable_static_visitor
        : public boost::static_visitor<>
    {
    public:
        clickable_static_visitor(clickable<Derived, Button, BaseView> &c)
            : m_clickable(c)
        {}
        
        typedef bool result_type;
        
        template<typename EventInfo>
        bool operator()(const EventInfo &) const
        {
            return false;
        }
        
        bool operator()(const button_event &event_info) const
        {
            if(event_info.button != m_clickable.clickable_button())
                return false;
            if(event_info.direction == direction::down)
            {
                m_clickable.button_down(true);
                return true;
            }
            if(event_info.direction == direction::up && m_clickable.m_button_down)
            {
                m_clickable.button_down(false);
                m_clickable.clickable_on_click();
                return true;
            }
            return false;
        }
        
        bool operator()(const entry_exit_event &event_info) const
        {
            if(event_info.region == region::exit)
                m_clickable.button_down(false);
            return true;
        }

        clickable<Derived, Button, BaseView> &m_clickable;
    };

}

template<typename Derived, typename Button, typename BaseView>
inline bool clickable<Derived, Button, BaseView>::on_event(const event_type &event_info)
{
    if(base_type::on_event(event_info))
        return true;
    else
        return boost::apply_visitor(detail::clickable_static_visitor<Derived,Button,BaseView>(*this), event_info);
}


}}}

#endif // BOOST__GUIGL__VIEW__IMPL__CLICKABLE_HPP
