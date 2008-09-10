/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__NAVIGABLE_HPP
#define BOOST__GUIGL__VIEW__NAVIGABLE_HPP

#include <boost/guigl/view/base.hpp>
#include <boost/guigl/view/draggable.hpp>

namespace boost { namespace guigl { namespace view {

template<typename BaseView=base>
class navigable : public draggable<navigable<BaseView>, BaseView>
{
public:
    typedef draggable<navigable<BaseView>, BaseView> base_type;

    template<typename ArgumentPack>
    navigable(const ArgumentPack &args)
        : base_type(args)
        , m_angle(0,0)
    {}

protected:
    void draw_prologue();
    void draw_epilogue();
    
    void draggable_on_drag(const position_type &position);

    friend class draggable<navigable<BaseView>, BaseView>;

private:
    position_type m_angle;
};

}}}

#endif // BOOST__GUIGL__VIEW__NAVIGABLE_HPP