/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__EXAMPLE__TWO_BUTTONS_HPP
#define BOOST__GUIGL__EXAMPLE__TWO_BUTTONS_HPP

#include <boost/guigl/view/static_compound.hpp>
#include <boost/guigl/widget/labeled_button.hpp>
#include <boost/guigl/layout/grid.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/make_vector.hpp>

typedef
    boost::guigl::view::static_compound<boost::fusion::vector<
                                            boost::guigl::widget::labeled_button,
                                            boost::guigl::widget::labeled_button
                                        >,
        boost::guigl::view::positioned<>
    >   two_buttons_base_type;

class two_buttons : public two_buttons_base_type
{
    typedef two_buttons_base_type base_type;
public:
    template<typename Args>
    two_buttons(const Args &args)
        : base_type((args, boost::guigl::_children=boost::fusion::make_vector(
            boost::guigl::widget::labeled_button((boost::guigl::_label = "top button", boost::guigl::_background = boost::guigl::color_type(0.5,0.5,0.5))),
            boost::guigl::widget::labeled_button(boost::guigl::_label = "bottom button")
        ) ))
    {
        using namespace boost::guigl::keywords;
        boost::guigl::layout::grid l((_grid_size=args[_size], _vertical=2));
        l.apply_layout(children());
    }
protected:
    void draw();
    bool on_event(const boost::guigl::event_type &event_info);
};

#endif // BOOST__GUIGL__EXAMPLE__TWO_BUTTONS_HPP
