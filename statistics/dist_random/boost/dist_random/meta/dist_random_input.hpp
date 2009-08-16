//////////////////////////////////////////////////////////////////////////////
// dist_random::meta::dist_random_input.hpp                                 //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_DIST_RANDOM_META_DIST_RANDOM_INPUT_HPP_ER_2009
#define BOOST_DIST_RANDOM_META_DIST_RANDOM_INPUT_HPP_ER_2009

namespace boost{

    // Specialized on D
    template<typename D>
    struct dist_random_input{
        typedef typename dist_random<D>::type r_;
        typedef typename r_::input_type type;
    };

}// boost

#endif
