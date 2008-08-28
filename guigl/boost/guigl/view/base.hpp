/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__BASE_HPP
#define BOOST__GUIGL__VIEW__BASE_HPP

#include <boost/guigl/access.hpp>
#include <boost/guigl/event.hpp>
#include <boost/guigl/parameters.hpp>

namespace boost { namespace guigl {

class window;

namespace view {

class base
{

public:
    template<typename ArgumentPack>
    base(const ArgumentPack &args)
        : m_size(args[_size])
    {}
    virtual ~base()
    {}
    
    const size_type &size() const
    {   return m_size; }
    
protected:
    virtual void draw()=0;
    void draw_prologue() {}
    void draw_epilogue() {}
    
    virtual bool on_event(const event_type &e)
    {
        return false;
    };

    size_type m_size;
    
    friend class boost::guigl::access;
};

}}}

#define BOOST_GUIGL_DRAW                    \
    void draw_prologue();                   \
    void draw_epilogue();                   \

#define BOOST_GUIGL_WIDGET_DRAW             \
    friend class ::boost::guigl::access;    \
    void draw();                            \
    void draw_prologue();                   \
    void draw_epilogue();                   \

#define BOOST_GUIGL_DRAW_IMPL(view_type)    \
template<typename BaseView>                 \
void view_type<BaseView>::draw()            \
{                                           \
    draw_prologue();                        \
    draw_epilogue();                        \
}                                           \

#define BOOST_GUIGL_WIDGET_DRAW_IMPL(widget)\
void widget::draw()                         \
{                                           \
    draw_prologue();                        \
    draw_epilogue();                        \
}                                           \


#endif // BOOST__GUIGL__VIEW_HPP