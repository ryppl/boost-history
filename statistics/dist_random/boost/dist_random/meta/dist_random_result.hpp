//////////////////////////////////////////////////////////////////////////////
// dist_random::meta::dist_random_result.hpp                                //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_DIST_RANDOM_META_DIST_RANDOM_RESULT_HPP_ER_2009
#define BOOST_DIST_RANDOM_META_DIST_RANDOM_RESULT_HPP_ER_2009
#include <boost/dist_random/meta/dist_random.hpp>

namespace boost{

    // Specialized on D
    template<typename D>
    struct dist_random_result{
        typedef typename dist_random<D>::type r_;
        typedef typename r_::result_type type;
    };

}// boost

#endif