/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__CUSTOM_DRAWABLE_HPP
#define BOOST__GUIGL__VIEW__CUSTOM_DRAWABLE_HPP

#include <boost/guigl/view/base.hpp>

namespace boost { namespace guigl { namespace view {

template<typename Callable, typename BaseView=base>
class custom_drawable : public BaseView
{
public:
    typedef BaseView base_type;

    template<typename ArgumentPack>
    custom_drawable(const ArgumentPack &args)
        : base_type(args)
        , m_draw_prologue(args[_draw_prologue|Callable()])
        , m_draw_epilogue(args[_draw_epilogue|Callable()])
    {}
    custom_drawable(const custom_drawable &rhs)
        : base_type(static_cast<const base_type &>(rhs))
        , m_draw_prologue(rhs.m_draw_prologue)
        , m_draw_epilogue(rhs.m_draw_epilogue)
    {}
    template<typename T>
    void set_draw_prologue(const T &t)
    {   m_draw_prologue = t; }
    template<typename T>
    void set_draw_epilogue(const T &t)
    {   m_draw_epilogue = t; }

protected:
    void draw_prologue()
    {
        base_type::draw_prologue();
        if(m_draw_prologue)
            m_draw_prologue();
    };
    void draw_epilogue()
    {
        if(m_draw_epilogue)
            m_draw_epilogue();
        base_type::draw_epilogue();
    };
private:
    Callable m_draw_prologue, m_draw_epilogue;
};

}}}

#endif // BOOST__GUIGL__VIEW__CUSTOM_DRAWABLE_HPP
