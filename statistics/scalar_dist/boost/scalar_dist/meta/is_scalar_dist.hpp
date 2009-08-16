///////////////////////////////////////////////////////////////////////////////
// scalar_dist::is_distribution.hpp                                         //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SCALAR_DIST_META_IS_SCALAR_DIST_ER_2009
#define BOOST_SCALAR_DIST_META_IS_SCALAR_DIST_ER_2009
#include <boost/mpl/bool.hpp>

namespace boost{
namespace math{

    template<typename D>
    struct is_scalar_dist : mpl::bool_<false> {};
    // What for? enable_if<is_scalar_dist<D>,T> ..

}// math
}// boost

#endif