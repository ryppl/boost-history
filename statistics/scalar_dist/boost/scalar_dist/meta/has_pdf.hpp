///////////////////////////////////////////////////////////////////////////////
// scalar_dist::meta::is_scalar_dist.hpp                                    //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SCALAR_DIST_META_IS_HAS_PDF_ER_2009
#define BOOST_SCALAR_DIST_META_IS_HAS_PDF_ER_2009
#include <boost/scalar_dist/meta/is_scalar_dist.hpp>

namespace boost{
namespace math{

    template<typename D> struct has_pdf 
        : is_scalar_dist<D> {};

}
}

#endif

