/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__ACCESS_HPP
#define BOOST__GUIGL__ACCESS_HPP


#include <boost/guigl/event.hpp>


namespace boost { namespace guigl {

namespace view {

template<typename BaseView>
class compound;

template<typename ChildrenSequence, typename BaseView>
class static_compound;

namespace detail {

    template<typename BaseView>
    struct compound_event_visitor;

    template<typename ChildrenSequence, typename BaseView>
    struct static_compound_event_visitor;

}

}

class access
{
private:
    template<typename T>
    static void draw(T &t)
    {
        t.draw();
    }

    template<typename T>
    static bool on_event(T &t, const event_type& e)
    {
        return t.on_event(e);
    }
    
    template<typename Child, typename Parent>
    static void set_parent(Child &child, Parent &parent)
    {
        child.set_parent(parent);
    }

    friend class window;
    template<typename BaseType>
    friend class view::compound;
    template<typename BaseType>
    friend struct view::detail::compound_event_visitor;
    
    template<typename ChildrenSequence, typename BaseView>
    friend class view::static_compound;
    template<typename ChildrenSequence, typename BaseType>
    friend struct view::detail::static_compound_event_visitor;
};

}}

#endif // BOOST__GUIGL__ACCESS_HPP
