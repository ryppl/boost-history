/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__ACCESS_HPP
#define BOOST__GUIGL__ACCESS_HPP


namespace boost { namespace guigl {

namespace view {

template<typename BaseView>
class compound;

namespace detail {

    template<typename BaseView>
    class compound_event_visitor;

}

}

struct access
{
private:
    template<typename T>
    static void draw(T &t)
    {
        t.draw();
    }

    template<typename T, typename E>
    static bool on_event(T &t, const E& e)
    {
        return t.on_event(e);
    }
    
    template<typename Child, typename Parent>
    static void parent(Child &child, Parent &parent)
    {
        child.parent(parent);
    }

    friend class window;
    template<typename BaseType>
    friend class view::compound;
    template<typename BaseType>
    friend class view::detail::compound_event_visitor;
};

}}

#endif // BOOST__GUIGL__ACCESS_HPP
