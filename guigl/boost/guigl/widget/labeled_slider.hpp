/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__WIDGET__LABELED_SLIDER_HPP
#define BOOST__GUIGL__WIDGET__LABELED_SLIDER_HPP

#include <boost/guigl/export_symbols.hpp>
#include <boost/guigl/view/colored.hpp>
#include <boost/guigl/view/labeled.hpp>
#include <boost/guigl/widget/slider.hpp>

namespace boost { namespace guigl { namespace widget {

typedef view::labeled<
            view::colored<
                widget::slider
        >   > labeled_slider_base_type;

class labeled_slider : public labeled_slider_base_type
{
    typedef labeled_slider_base_type base_type;
public:
    template<typename ArgumentPack>
    labeled_slider(const ArgumentPack &args)
        : labeled_slider_base_type(args)
    {}

protected:
    BOOST_EXPORT_SYMBOLS void draw();
    BOOST_EXPORT_SYMBOLS bool on_event(const event_type &event_info);
    friend class guigl::access;
};

}}}

#endif // BOOST__GUIGL__WIDGET__LABELED_SLIDER_HPP
