/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__CUSTOM_EVENT_PROCESSING_HPP
#define BOOST__GUIGL__VIEW__CUSTOM_EVENT_PROCESSING_HPP

#include <boost/guigl/view/base.hpp>

namespace boost { namespace guigl { namespace view {

template<typename Callable, typename BaseView=base>
class custom_event_processing : public BaseView
{
public:
    typedef BaseView base_type;

    template<typename ArgumentPack>
    custom_event_processing(const ArgumentPack &args)
        : base_type(args)
        , m_on_event(args[_draw_prologue|Callable()])
    {}
    custom_event_processing(const custom_event_processing &rhs)
        : base_type(static_cast<const base_type &>(rhs))
        , m_on_event(rhs.m_on_event)
    {}
    Callable &event_handler()
    {
        return m_on_event;
    }
    const Callable &event_handler() const
    {
        return m_on_event;
    }
protected:
    bool on_event(const event_type &event)
    {
        if(!base_type::on_event(event))
            if(m_on_event)
                return m_on_event(event);
            else return false;
        return true;
    };
private:
    Callable m_on_event;
};

}}}

#endif // BOOST__GUIGL__VIEW__CUSTOM_EVENT_PROCESSING_HPP
