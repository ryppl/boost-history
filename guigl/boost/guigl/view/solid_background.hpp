/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__SOLID_BACKGROUND_HPP
#define BOOST__GUIGL__VIEW__SOLID_BACKGROUND_HPP

#include <boost/guigl/parameters.hpp>
#include <boost/guigl/view/base.hpp>


namespace boost { namespace guigl { namespace view {

template<typename BaseView=base>
class solid_background : public BaseView
{
public:
    typedef BaseView base_type;

    template<typename ArgumentPack>
    solid_background(const ArgumentPack &args)
        : base_type(args)
        , m_background_color(args[_background])
    {}
    
    const color_type &background_color() const
    {   return m_background_color; }

protected:
    BOOST_GUIGL_DRAW
    
private:
    color_type m_background_color;
};

}}}

#endif // BOOST__GUIGL__VIEW__SOLID_BACKGROUND_HPP