/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__THREE_DIMENSIONAL_HPP
#define BOOST__GUIGL__VIEW__THREE_DIMENSIONAL_HPP

#include <boost/guigl/view/base.hpp>

namespace boost { namespace guigl { namespace view {

/// Behavior adding three-dimensionality to the view.
/** three_dimensional adds support for three-dimensional graphics in a view.
*/
template<typename BaseView=base>
class three_dimensional : public BaseView
{
    typedef BaseView base_type;
public:
    template<typename ArgumentPack>
    three_dimensional(const ArgumentPack &args)
        : base_type(args)
    {}

protected:
    void draw_prologue();
    void draw_epilogue();
};

}}}

#endif // BOOST__GUIGL__VIEW__THREE_DIMENSIONAL_HPP