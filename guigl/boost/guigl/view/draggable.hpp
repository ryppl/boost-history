/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__DRAGGABLE_HPP
#define BOOST__GUIGL__VIEW__DRAGGABLE_HPP

#include <boost/guigl/view/base.hpp>
#include <boost/guigl/view/mouse_tracking.hpp>

namespace boost { namespace guigl { namespace view {

namespace detail {

    template<typename Derived, typename BaseView>
    struct draggable_static_visitor;

}

template<typename Derived, typename BaseView=base>
class draggable : public mouse_tracking<BaseView>
{
    typedef mouse_tracking<BaseView> base_type;
public:
    template<typename ArgumentPack>
    draggable(const ArgumentPack &args)
        : base_type(args)
    {}

protected:
    BOOST_GUIGL_DRAW

    bool on_event(const event_type &event_info);

private:
    void draggable_on_drag(const position_type &position)
    {
        static_cast<Derived *>(this)->draggable_on_drag(position);
    }

    friend struct detail::draggable_static_visitor<Derived,BaseView>;
};

}}}

#endif // BOOST__GUIGL__VIEW__CLICKABLE_HPP