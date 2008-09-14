/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__STATIC_COMPOUND_HPP
#define BOOST__GUIGL__VIEW__IMPL__STATIC_COMPOUND_HPP


#include <boost/guigl/access.hpp>
#include <boost/guigl/event.hpp>
#include <boost/guigl/view/static_compound.hpp>

#include <boost/fusion/include/accumulate.hpp>
#include <boost/fusion/include/for_each.hpp>


namespace boost { namespace guigl { namespace view {

namespace detail {
    
    template<typename StaticCompound>
    struct draw
    {
        template<typename View>
        void operator()(View &view) const
        {
            StaticCompound::draw(view);
        }
    };
    
}

template<typename ChildrenSequence, typename BaseView>
inline void static_compound<ChildrenSequence, BaseView>::draw_prologue()
{
    BaseView::draw_prologue();
    fusion::for_each(m_children, detail::draw<static_compound>());
}


namespace detail {

inline bool inside(double pos, double min, double max)
{
    return (pos >= min && pos <= max);
}

inline bool inside(const position_type &position, const positioned<> &view)
{
    return inside(position.x, view.position().x,  view.position().x + view.size().x)
            && inside(position.y, view.position().y,  view.position().y + view.size().y);
}


template<typename Visitor>
struct propagate_button_event
{
    typedef bool result_type;
    
    propagate_button_event(const Visitor &visitor, const button_event &event_info)
        : visitor(visitor)
        , event_info(event_info)
    {}
    template<typename ChildView>
    bool operator()(ChildView &child_view, bool handled)
    {
        if(handled)
            return true;
        if(!inside(event_info.position, child_view))
            return false;
        // if we are sending an event to a child view, it must be active
        visitor.set_mouse_focus(child_view);
        button_event translated_event_info(event_info);
        translated_event_info.position -= child_view.position();
        if(visitor.on_event(child_view, translated_event_info))
            return true;
        return false;
    }
    
    const Visitor &visitor;
    const button_event &event_info;
};

template<typename Visitor>
struct propagate_movement_event
{
    typedef bool result_type;
    
    propagate_movement_event(const Visitor &visitor, const movement_event &event_info)
        : visitor(visitor)
        , event_info(event_info)
    {}
    template<typename ChildView>
    bool operator()(ChildView &child_view, bool handled)
    {
        if(handled)
            return true;
        if(!inside(event_info.position, child_view))
        {
            if(&child_view == visitor.mouse_focus_child())
            {
                // we left the mouse focus child
                visitor.no_mouse_focus();
                visitor.on_event(child_view, entry_exit_event(region::exit));
            }
            return false;
        }
        else
        {
            visitor.set_mouse_focus(child_view);
            // only the mouse focus child gets movement events
            // forward the movement event
            movement_event translated_event_info(event_info);
            translated_event_info.position -= child_view.position();
            visitor.on_event(child_view, translated_event_info);
            return true;
        }
    }
    
    const Visitor &visitor;
    const movement_event &event_info;
};

template<typename ChildrenSequence, typename BaseView>
struct static_compound_event_visitor
    : public boost::static_visitor<>
{
    typedef static_compound<ChildrenSequence, BaseView> view_type;
public:
    typedef bool result_type;

    static_compound_event_visitor(view_type &view, const event_type &event_info)
        : view(view), event_info(event_info)
    {}
    
    positioned<> *mouse_focus_child() const
    {
        return view.m_mouse_focus_child;
    }
    
    void no_mouse_focus() const
    {
        view.m_mouse_focus_child = 0;
    }
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
        return boost::fusion::accumulate(view.children(), false, propagate_button_event<static_compound_event_visitor>(*this, event_info));
    }
    
    bool operator()(const movement_event &event_info) const
    {
        boost::fusion::accumulate(view.children(), false, propagate_movement_event<static_compound_event_visitor>(*this, event_info));
        return true;
    }

    bool operator()(const entry_exit_event &event_info) const
    {
        if(event_info.region == region::exit && view.m_mouse_focus_child)
            access::on_event(*view.m_mouse_focus_child, event_info);
        view.m_mouse_focus_child = 0;
        return true;
    }
    
    template<typename T>
    static bool on_event(T &t, const event_type& e)
    {
        return access::on_event(t, e);
    }

    view_type &view;
    const event_type &event_info;
};

}

template<typename ChildrenSequence, typename BaseView>
inline bool static_compound<ChildrenSequence, BaseView>::on_event(const event_type &event_info)
{
    if(base_type::on_event(event_info))
        return true;
    else
        return boost::apply_visitor( detail::static_compound_event_visitor<ChildrenSequence, BaseView>(*this, event_info), event_info);
}

}}}

#endif // BOOST__GUIGL__VIEW__IMPL__STATIC_COMPOUND_HPP