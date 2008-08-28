/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__COMPOUND_HPP
#define BOOST__GUIGL__VIEW__IMPL__COMPOUND_HPP


#include <boost/guigl/access.hpp>
#include <boost/guigl/event.hpp>
#include <boost/guigl/view/compound.hpp>

#include <iostream>

namespace boost { namespace guigl { namespace view {

template<typename BaseView>
inline void compound<BaseView>::draw_prologue()
{
    BaseView::draw_prologue();
    for(ptr_vector<positioned<> >::iterator it=m_children.begin(); it!=m_children.end(); it++)
        access::draw(*it);
}

template<typename BaseView>
inline void compound<BaseView>::draw_epilogue()
{
    BaseView::draw_epilogue();
}

namespace detail {

template<typename BaseView>
struct compound_event_visitor
    : public boost::static_visitor<>
{
public:
    typedef bool result_type;

    compound_event_visitor(compound<BaseView> &view, const event_type &event_info)
        : view(view), event_info(event_info)
    {}
    
    void set_mouse_focus(positioned<> &child) const
    {
        if(!view.m_mouse_focus_child)
        {
            // we entered the mouse focus child
            view.m_mouse_focus_child = &child;
            access::on_event(child, entry_exit_event(region::entry));
        }
        else if(view.m_mouse_focus_child != &child)
        {
            // we changed the mouse focus child
            access::on_event(*view.m_mouse_focus_child, entry_exit_event(region::exit));
            view.m_mouse_focus_child = &child;
            access::on_event(*view.m_mouse_focus_child, entry_exit_event(region::entry));
        }
    }
    
    bool operator()(const button_event &event_info) const
    {
        for(ptr_vector<positioned<> >::iterator it=view.children().begin(); it!=view.children().end(); it++)
        {
            if(!inside(event_info.position, *it))
                continue;
            // if we are sending an event to a child view, it must be active
            set_mouse_focus(*it);
            if(access::on_event(*it, this->event_info))
                return true;
        }
        return false;
    }
    
    bool operator()(const movement_event &event_info) const
    {
        for(ptr_vector<positioned<> >::iterator it=view.children().begin(); it!=view.children().end(); it++)
        {
            if(!inside(event_info.position, *it))
            {
                if(&*it == view.m_mouse_focus_child)
                {
                    // we left the mouse focus child
                    view.m_mouse_focus_child = 0;
                    access::on_event(*it, entry_exit_event(region::exit));
                }
                continue;
            }
            else
            {
                set_mouse_focus(*it);
                // only the mouse focus child gets movement events
                // forward the movement event
                movement_event translated_event_info(event_info);
                translated_event_info.position -= it->position();
                access::on_event(*it, translated_event_info);
                return true;
            }
        }
        return true;        
    }

    bool operator()(const entry_exit_event &event_info) const
    {
        if(event_info.region == region::exit && view.m_mouse_focus_child)
            access::on_event(*view.m_mouse_focus_child, event_info);
        view.m_mouse_focus_child = 0;
        return true;
    }
    
    
    static bool inside(const position_type &position, const positioned<> &view)
    {
        return inside(position.x, view.position().x,  view.position().x + view.size().x)
                && inside(position.y, view.position().y,  view.position().y + view.size().y);
    }
    static bool inside(double pos, double min, double max)
    {
        return (pos >= min && pos <= max);
    }

    compound<BaseView> &view;
    const event_type &event_info;
};

}

template<typename BaseView>
inline bool compound<BaseView>::on_event(const event_type &event_info)
{
    if(base_type::on_event(event_info))
        return true;
    else
        return boost::apply_visitor( detail::compound_event_visitor<BaseView>(*this, event_info), event_info);
}

}}}

#endif // BOOST__GUIGL__VIEW__IMPL__COMPOUND_HPP