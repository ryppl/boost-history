/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__GEOMETRY_HPP
#define BOOST__GUIGL__GEOMETRY_HPP


#include <boost/guigl/types.hpp>


namespace boost { namespace guigl {

namespace geometry {

template<typename View>
position_type midpoint(const View &view)
{
    return view.position() + view.size() / 2;
}

}

}}

#endif // BOOST__GUIGL__GEOMETRY_HPP
