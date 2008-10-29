/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__WIDGET__SLIDER_HPP
#define BOOST__GUIGL__WIDGET__SLIDER_HPP

#include <boost/guigl/view/active_colored.hpp>
#include <boost/guigl/view/draggable.hpp>
#include <boost/guigl/view/positioned.hpp>
#include <boost/guigl/view/solid_background.hpp>
#include <boost/signal.hpp>

namespace boost { namespace guigl { namespace widget {

class slider;

typedef
    view::active_colored<
        view::draggable<slider,guigl::button::left_type,
            view::solid_background<
                view::positioned<>
    >   >   > slider_base_type;

/// A slider storing a floating point value, changable by dragging in the GUI.
class slider : public slider_base_type
{
    typedef slider_base_type base_type;
public:
    template<typename ArgumentPack>
    slider(const ArgumentPack &args)
        : slider_base_type(args)
        , m_value(args[_value|args[_min]])
        , m_min(args[_min]), m_max(args[_max]), m_step(args[_step])
    {}

    /// Signal emitting changed values.
    boost::signal<void(const double &)> on_value_change;

    /// Returns the value of the slider.
    double value()
    {   return m_value; }
    
    /// Sets the value of the slider.
    void set_value(double value);
protected:
    BOOST_GUIGL_WIDGET_DRAW
    bool on_event(const event_type &event_info);

private:
    double m_value;
    double m_min, m_max, m_step;
    
    void draggable_on_drag(const position_type &position);
    void draggable_on_end_drag(const position_type &position) {};
    
    friend class view::draggable<slider,guigl::button::left_type,
        view::solid_background<
            view::positioned<>
    >   >;
};

}}}

#endif // BOOST__GUIGL__WIDGET__SLIDER_HPP
