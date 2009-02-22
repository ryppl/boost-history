/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__WIDGET__WINDOW_HPP
#define BOOST__GUIGL__WIDGET__WINDOW_HPP

#include <boost/guigl/export_symbols.hpp>
#include <boost/guigl/view/custom_event_processing.hpp>
#include <boost/guigl/view/compound.hpp>
#include <boost/guigl/view/mouse_tracking.hpp>
#include <boost/guigl/view/window.hpp>
#include <boost/function.hpp>

namespace boost { namespace guigl { namespace widget {

typedef view::custom_event_processing<boost::function<bool(const event_type &)>,
            view::compound<
                view::mouse_tracking<
                    view::window<>
        >   >   > window_base_type;

class window
    : public window_base_type
{
    typedef window_base_type base_type;
public:
    template<typename ArgumentPack>
    window(const ArgumentPack &args)
        : base_type(args)
    {}
    
protected:
    BOOST_EXPORT_SYMBOLS void draw();
    BOOST_EXPORT_SYMBOLS bool on_event(const event_type &event_info);
};

}}}

#endif // BOOST__GUIGL__WIDGET__WINDOW_HPP
