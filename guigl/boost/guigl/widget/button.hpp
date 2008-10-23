/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__WIDGET__BUTTON_HPP
#define BOOST__GUIGL__WIDGET__BUTTON_HPP

#include <boost/guigl/view/active_colored.hpp>
#include <boost/guigl/view/clickable.hpp>
#include <boost/guigl/view/positioned.hpp>
#include <boost/guigl/view/solid_background.hpp>
#include <boost/signal.hpp>

namespace boost { namespace guigl { namespace widget {

class button;

typedef
    view::active_colored<
        view::solid_background<
            view::clickable<button,
                view::positioned<>
    >   >   > button_base_type;

/// A button which sends a signal when clicked.
class button : public button_base_type
{
    typedef button_base_type base_type;
public:
    template<typename ArgumentPack>
    button(const ArgumentPack &args)
        : button_base_type(args)
    {}
    button(const button &rhs)
        : base_type(static_cast<const base_type &>(rhs))
    {}
    
    button &operator=(const button &rhs)
    {
        base_type::operator=(*static_cast<const base_type *>(&rhs));
        return *this;
    }
    boost::signal<void()> on_click;
protected:
    BOOST_GUIGL_WIDGET_DRAW
    bool on_event(const event_type &event_info);

private:
    void clickable_button_down(bool state);
    void clickable_on_click()
    {   on_click(); }
    
    friend class view::clickable<button, view::positioned<> >;
};

}}}

#endif // BOOST__GUIGL__WIDGET__BUTTON_HPP
