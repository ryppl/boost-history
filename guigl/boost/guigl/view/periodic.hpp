/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__PERIODIC_HPP
#define BOOST__GUIGL__VIEW__PERIODIC_HPP

#include <boost/guigl/view/base.hpp>
#include <boost/guigl/application.hpp>
#include <boost/bind.hpp>

namespace boost { namespace guigl { namespace view {

template<typename Derived, typename BaseView=base>
class periodic : public BaseView
{
public:
    typedef BaseView base_type;

    template<typename ArgumentPack>
    periodic(const ArgumentPack &args)
        : base_type(args)
        , m_enabled(false)
    {
        if(args[_period]>0)
            enable(args[_period]);
    }
    periodic(const periodic &rhs)
        : base_type(static_cast<const base_type &>(rhs))
        , m_enabled(false)
    {}
    
    void enable(double seconds)
    {
        m_enabled = true;
        m_milliseconds = int(seconds * 1000);
        schedule_callback();
    }
    
    void disable()
    {
        m_enabled = false;
    }
private:
    void schedule_callback()
    {
        guigl::application::timeout(boost::bind(&periodic::timer_callback, this), m_milliseconds);
    }
    void timer_callback()
    {
        if(m_enabled)
        {
            static_cast<Derived *>(this)->periodic_callback();
            schedule_callback();
        }
    }   
    bool m_enabled;
    unsigned m_milliseconds;
};

}}}

#endif // BOOST__GUIGL__VIEW__COLORED_HPP
