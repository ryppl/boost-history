/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__WIDGET__LABEL_HPP
#define BOOST__GUIGL__WIDGET__LABEL_HPP

#include <boost/guigl/view/colored.hpp>
#include <boost/guigl/view/labeled.hpp>
#include <boost/guigl/view/solid_background.hpp>
#include <boost/guigl/view/positioned.hpp>

namespace boost { namespace guigl { namespace widget {

typedef view::labeled<
            view::colored<
                view::solid_background<
                    view::positioned<>                
        >   >   >   label_base_type;

class label : public label_base_type
{
    typedef label_base_type base_type;
public:
    template<typename ArgumentPack>
    label(const ArgumentPack &args)
        : base_type(args)
    {}

protected:
    void draw();
    bool on_event(const event_type &event_info);

    friend class guigl::access;
};

}}}

#endif // BOOST__GUIGL__WIDGET__LABELED_BUTTON_HPP