/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/guigl/application.hpp>
#include <boost/guigl/layout/grid.hpp>
#include <boost/guigl/window.hpp>
#include <boost/guigl/widget/button.hpp>
#include <boost/guigl/widget/label.hpp>
#include <boost/guigl/widget/labeled_button.hpp>
#include <boost/guigl/widget/labeled_slider.hpp>

#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>

#include "two_spheres.hpp"
#include "two_buttons.hpp"

#include <iostream>


using namespace boost::guigl;

color_type make_grey(float value)
{
    return color_type(value, value, value);
}

void idle()
{
    std::cout << "Idle called once." << std::endl;
    application::on_idle().disconnect_all_slots();
}

void timer()
{
    std::cout << "5 seconds have elapsed." << std::endl;
}

int main()
{
    window test_window1(( _label = "window example 1", _size=size_type(300,300) ));
    window test_window2(( _size=size_type(300,100), _label = "window example 2" ));
    window test_window3(( _label = "window example 3" ));
    
    test_window1 << new widget::label((
        _label = "Label",
        _size=size_type(100,30),
        _background=color_type(1,1,1),
        _color=color_type(0,0,0) ));

    widget::labeled_button *b1 = new widget::labeled_button((
        _size=size_type(100,30),
        _position=position_type(50, 50),
        _background=color_type(1,1,1),
        _active_color=color_type(1,0,0),
        _color=color_type(0,0,0),
        _label="Button"));
    test_window1 << b1;
    
    widget::labeled_slider *s = new widget::labeled_slider((
        _label="Slider",
        _size=size_type(100,30),
        _position=position_type(50,80),
        _background=color_type(0.5,0.5,0.5),
        _active_color=color_type(0,1,0),
        _min=0.1,_max=0.9,
        _step=0.1 ));
    test_window1 << s;
    

    // clicking the button changes the slider value to 0.5
    b1->on_click.connect(boost::bind(&widget::slider::set_value, s, 0.5));
    // moving the slider changes the label color of the button
    s->on_value_change.connect(boost::bind(&widget::labeled_button::set_color, b1, boost::bind(make_grey, _1)));
    
    test_window2
        << new two_buttons((
            _size=size_type(100,50),
            _position=position_type(50, 5) ));
    
    layout::grid grid_layout(( _grid_size=test_window3.size(), _horizontal=3, _vertical=3 ));
    for(int i=1; i<=9; i++)
        test_window3 << grid_layout.create<widget::button>(( _background=color_type(1.0f/i,1.0f/i,1.0f/i) ));
    
    window test_window_3d(( _depth = true, _label="3D", _color=make_grey(1) ));
    test_window_3d << new two_spheres(_period = 0.01);
    
    application::on_idle().connect(&idle);
    application::timeout(&timer, 5000);
    application::run();
}