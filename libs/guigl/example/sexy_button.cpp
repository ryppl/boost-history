/*=================================---------------------------------------------
Copyright 2009 Andrey Torba, Stjepan Rajko

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/guigl/application.hpp>
#include <boost/guigl/window.hpp>
//#include <boost/guigl/widget/button.hpp>

#include <boost/guigl/view/impl/clickable.hpp>
#include <boost/guigl/view/impl/positioned.hpp>
#include <boost/guigl/view/impl/active_colored.hpp>
#include <boost/guigl/view/impl/mouse_tracking.hpp>

#include <boost/guigl/platform/opengl.hpp>

#include <boost/guigl/gl.hpp>

#include <boost/fusion/container/generation/make_map.hpp>
#include <boost/fusion/include/make_map.hpp>
#include <boost/fusion/include/at_key.hpp>

using namespace boost::guigl;

struct circle2d_generator {
    position_type center;
    double radius;

    circle2d_generator(
        position_type center_,
        double radius_):
    center(center_),
        radius(radius_)
    {}

    typedef position_type result_type;
    result_type operator()(double angle) const
    {
        return position_type(
            center.x + std::cos(angle) * radius,
            center.y + std::sin(angle) * radius);
    }
};

#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>

#include <boost/fusion/include/convert.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/map.hpp>

template<class ColorVector, class BaseType>
class colored : public BaseType {
private:
    template<class ColorVector>
    struct create_color_map {
        typedef typename boost::fusion::result_of::as_map<
            typename boost::mpl::transform<
            ColorVector,
            boost::fusion::pair<boost::mpl::placeholders::_, boost::guigl::color_type>
            >::type
        >::type type;
    };

    typename create_color_map<ColorVector>::type m_color_map;

public:
    typedef BaseType base_type;

    template<typename ArgumentPack>
    colored(const ArgumentPack &args)
        : base_type(args)
    {}

    template<class ColorKey>
    color_type const& color()
    { 
        return boost::fusion::at_key<ColorKey>(m_color_map);
    }

    template<class ColorKey>
    void set_color(color_type const& clr)
    { 
        boost::fusion::at_key<ColorKey>(m_color_map) = clr;
    }

    template<class ColorKey>
    void use_color()
    {
        color_type const& clr = color<ColorKey>();
        gl::color<float>(clr[0], clr[1], clr[2], clr[3]);
    }

};

class sexy_button;

typedef
colored<boost::mpl::vector<struct active_color, struct bg_color, struct highlight_color>,
view::mouse_tracking<
view::clickable<sexy_button,button::left_type,
view::positioned<>
> > > button_base_type;

class sexy_button : public button_base_type {
public:
    typedef button_base_type base_type;

    template<typename ArgumentPack>
    sexy_button(const ArgumentPack &args)
        : base_type(args)
    {}

    void on_mouse_enter()
    {
        window::redraw(*this);
    }

    void on_mouse_leave()
    {
        window::redraw(*this);
    }

    //void on_mouse_move()
    //  {
    //  window::redraw(*this);
    //  }

    void draw_circle_segment(circle2d_generator const& circle, int phase, int angle)
    {
        position_type pos;
        for(int i = phase; i <= phase+angle; i += 10)
        {
            pos = circle(double(i)/180*3.1425926);
            gl::vertex(pos.x, pos.y);
        }
    }

    void draw_rounded_rect()
    {
        double radius = (std::min)(10., (std::min)(size().x, size().y)/2);

        draw_circle_segment(
            circle2d_generator(position_type(radius, radius), radius),
            180,
            90);

        draw_circle_segment(
            circle2d_generator(position_type(size().x - radius, radius), radius),
            270,
            90);

        draw_circle_segment(
            circle2d_generator(position_type(size().x - radius, size().y - radius), radius),
            0,
            90);

        draw_circle_segment(
            circle2d_generator(position_type(radius, size().y - radius), radius),
            90,
            90);
    }

    void draw()
    {
        base_type::draw_prologue();

        glEnable (GL_LINE_SMOOTH);
        //glEnable (GL_POLYGON_SMOOTH);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);    //glEnable(GL_POLYGON_SMOOTH);
        //glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);    //glEnable(GL_POLYGON_SMOOTH);

        {
            gl::scoped_matrix m;
            double x_offset = button_down() ? (size().x*.1)/2 : 0.;
            double y_offset = button_down() ? (size().y*.1)/2 : 0.;
            gl::translate(x_offset, y_offset);
            double scale_value = button_down() ? 0.9 : 1.;
            gl::scale(scale_value, scale_value, scale_value);

            { // shadow
                gl::scoped_matrix m;
                gl::translate(2., 2.);
                gl::color(black(0.1f));
                glBegin(GL_POLYGON);
                draw_rounded_rect();
                glEnd();
            }

            if(mouse_state().inside)
                use_color<highlight_color>();
            else
                use_color<bg_color>();
            glBegin(GL_POLYGON);
            draw_rounded_rect();
            glEnd();

            glBegin(GL_LINE_LOOP);
            draw_rounded_rect();
            glEnd();


            gl::line_width(0.5);
            gl::color(black(mouse_state().inside ? 1.0f : 0.7f));
            glBegin(GL_LINE_LOOP);
            draw_rounded_rect();
            glEnd();

            gl::color(black(0.1f));
            double radius = (std::min)(10., (std::min)(size().x, size().y)/2);
            glBegin(GL_POLYGON);
            draw_circle_segment(
                circle2d_generator(position_type(radius, radius), radius),
                180,
                90);

            draw_circle_segment(
                circle2d_generator(position_type(size().x - radius, radius), radius),
                270,
                90);
            glEnd();

        }

        base_type::draw_epilogue();
    }

    void clickable_on_click()
    {
        window::redraw(*this);
    }

    void clickable_button_down(bool state)
    {
        window::redraw(*this);
    }

    bool on_event(const event_type &event_info)
    {
        return base_type::on_event(event_info);
    }
};


int main()
{
    double border = 10;
    position_type window_size(300, 300);
    position_type btn_size((window_size.x - border*5)/2, (window_size.y - border*5)/2);

    window test_window1((
        _label = "custom example",
        _size = window_size,
        _background = black()
        ));

    sexy_button* btn1 = new sexy_button((
        _size = position_type(window_size.x - border*2, window_size.y - border*2),
        _position = position_type(border, border) ));
    btn1->set_color<bg_color>(make_color(132, 177, 232));
    btn1->set_color<highlight_color>(make_color(160, 197, 241));

    sexy_button* btn2 = new sexy_button((
        _size = btn_size,
        _position = position_type(border*2, border*2) ));
    btn2->set_color<bg_color>(make_color(12, 114, 163));
    btn2->set_color<highlight_color>(make_color(3, 95, 138));

    sexy_button* btn3 = new sexy_button((
        _size = btn_size,
        _position = position_type(border*3 + btn_size.x, border*2) ));
    btn3->set_color<bg_color>(make_color(8, 208, 60));
    btn3->set_color<highlight_color>(make_color(9, 171, 51));

    sexy_button* btn4 = new sexy_button((
        _size = btn_size,
        _position = position_type(border*2, border*3 + btn_size.y) ));
    btn4->set_color<bg_color>(make_color(255, 35, 87));
    btn4->set_color<highlight_color>(make_color(219, 4, 55));

    sexy_button* btn5 = new sexy_button((
        _size = btn_size,
        _position = position_type(border*3 + btn_size.x, border*3 + btn_size.y) ));
    btn5->set_color<bg_color>(make_color(249, 230, 73));
    btn5->set_color<highlight_color>(make_color(255, 255, 25));

    test_window1
        << btn1 << btn2 << btn3 << btn4 << btn5
        ;

    application::run();
}
