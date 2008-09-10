/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__WIDGET__COMPOUND_HPP
#define BOOST__GUIGL__WIDGET__COMPOUND_HPP

#include <boost/guigl/view/compound.hpp>
#include <boost/guigl/view/positioned.hpp>
#include <boost/guigl/view/solid_background.hpp>

namespace boost { namespace guigl { namespace widget {

typedef
    view::compound<
        view::solid_background<
            view::positioned<>
    >   >   compound_base_type;

/// A button which sends a signal when clicked.
class compound : public compound_base_type
{
public:
    typedef compound_base_type base_type;

    template<typename ArgumentPack>
    compound(const ArgumentPack &args)
        : base_type(args)
    {}

protected:
    void draw();
};

}}}

#endif // BOOST__GUIGL__WIDGET__COMPOUND_HPP