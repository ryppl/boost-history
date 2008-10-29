/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__CLICKABLE_HPP
#define BOOST__GUIGL__VIEW__CLICKABLE_HPP

#include <boost/guigl/view/base.hpp>

namespace boost { namespace guigl { namespace view {

namespace detail {

    template<typename Derived, typename Button, typename BaseView>
    struct clickable_static_visitor;

}

/// Behavior capturing clicking.
/** clickable adds clicking behavior to the view.
    The Derived class must have the method clickable_on_click(), which
    will be called when the user completes a click in the view.
*/
template<typename Derived, typename Button, typename BaseView=base>
class clickable : public BaseView
{
public:
    typedef BaseView base_type;

    template<typename ArgumentPack>
    clickable(const ArgumentPack &args)
        : base_type(args)
        , m_button_down(false)
    {}
    clickable(const clickable &rhs)
        : base_type(static_cast<const base_type &>(rhs))
        , m_button_down(false)
    {}
    button::enum_type clickable_button()
    {   return Button::value; }
protected:
    bool on_event(const event_type &event_info);

    bool button_down()
    {   return m_button_down; }
private:
    bool m_button_down;
    
    void button_down(bool state);
    void clickable_on_click()
    {
        static_cast<Derived *>(this)->clickable_on_click();
    }

    friend struct detail::clickable_static_visitor<Derived,Button,BaseView>;
};

}}}

#endif // BOOST__GUIGL__VIEW__CLICKABLE_HPP
