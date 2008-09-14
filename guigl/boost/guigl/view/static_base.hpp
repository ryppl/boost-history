/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__STATIC_BASE_HPP
#define BOOST__GUIGL__VIEW__STATIC_BASE_HPP

#include <boost/guigl/access.hpp>
#include <boost/guigl/event.hpp>
#include <boost/guigl/parameters.hpp>

namespace boost { namespace guigl {

class window;

namespace view {

class static_base
{

public:
    template<typename ArgumentPack>
    static_base(const ArgumentPack &args)
        : m_size(args[_size])
    {}
    
    const size_type &size() const
    {   return m_size; }
    
protected:
    void draw_prologue() {}
    void draw_epilogue() {}
    
    bool on_event(const event_type &e)
    {
        return false;
    };

    size_type m_size;
    
    friend class boost::guigl::access;
};

}}}


#endif // BOOST__GUIGL__VIEW__STATIC_BASE_HPP