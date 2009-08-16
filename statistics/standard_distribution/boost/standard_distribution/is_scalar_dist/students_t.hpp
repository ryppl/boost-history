///////////////////////////////////////////////////////////////////////////////
// standard_distribution::is_scalar_dist::students_t.hpp                    //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_IS_SCALAR_DIST_STUDENTS_T_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_IS_SCALAR_DIST_STUDENTS_T_ER_2009
#include <boost/mpl/bool.hpp>
#include <boost/scalar_dist/meta/is_scalar_dist.hpp>
#include <boost/math/distributions/students_t.hpp>

namespace boost{
namespace math{

    template<typename T,typename P>
    struct is_scalar_dist< math::students_t_distribution<T,P> > 
        : mpl::bool_<true>{};

}
}

#endif