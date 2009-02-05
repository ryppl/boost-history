/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__WIDGET__CUSTOM_HPP
#define BOOST__GUIGL__WIDGET__CUSTOM_HPP

#include <boost/guigl/export_symbols.hpp>
#include <boost/guigl/view/custom_drawable.hpp>
#include <boost/guigl/view/positioned.hpp>
#include <boost/function.hpp>

namespace boost { namespace guigl { namespace widget {

class custom;

typedef
    view::custom_drawable<boost::function<void()>,
        view::positioned<>
    > custom_base_type;

class custom : public custom_base_type
{
public:
    typedef custom_base_type base_type;

    template<typename ArgumentPack>
    custom(const ArgumentPack &args)
        : base_type(args)
    {}
    custom(const custom &rhs)
        : base_type(static_cast<const base_type &>(rhs))
    {}
    
protected:
    friend class ::boost::guigl::access;
    void BOOST_EXPORT_SYMBOLS draw();
    bool BOOST_EXPORT_SYMBOLS on_event(const event_type &event_info);
};

}}}

#endif // BOOST__GUIGL__WIDGET__BUTTON_HPP
