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

    template<typename Derived, typename BaseView>
    struct clickable_static_visitor;

}

template<typename Derived, typename BaseView=base>
class clickable : public BaseView
{
    typedef BaseView base_type;
public:
    template<typename ArgumentPack>
    clickable(const ArgumentPack &args)
        : base_type(args)
        , m_button_down(false)
    {}

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

    friend struct detail::clickable_static_visitor<Derived,BaseView>;
};

}}}

#endif // BOOST__GUIGL__VIEW__CLICKABLE_HPP