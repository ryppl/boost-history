/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__LABELED_HPP
#define BOOST__GUIGL__VIEW__LABELED_HPP

#include <boost/guigl/view/base.hpp>
#include <boost/guigl/window.hpp>

namespace boost { namespace guigl { namespace view {

template<typename BaseView=base>
class labeled : public BaseView
{
public:
    typedef BaseView base_type;

    template<typename ArgumentPack>
    labeled(const ArgumentPack &args)
        : base_type(args)
        , m_label(args[_label])
    {}
    labeled(const labeled &rhs)
        : base_type(static_cast<const base_type &>(rhs))
        , m_label(rhs.m_label)
    {}
    
    const std::string &label() const
    {   return m_label; }

    void set_label(const std::string &label)
    {
        m_label = label;
        guigl::window::redraw(*this);
    }
protected:
    void draw_prologue();

private:
    std::string m_label;
};

}}}

#endif // BOOST__GUIGL__VIEW__POSITIONED_HPP
