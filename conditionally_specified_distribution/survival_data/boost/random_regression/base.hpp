///////////////////////////////////////////////////////////////////////////////
//  random_regression_base.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANDOM_REGRESSION_BASE_HPP_ER_2009
#define BOOST_RANDOM_REGRESSION_BASE_HPP_ER_2009

namespace boost{

// Concept RandomRegression
// Valid expression
// rr(U& u,const X& x)
// Associated types:
// R::input_type
// R::result_type

struct random_regression_base{
    // Derive from this base to indicate that models RandomRegression
};


//    // Not in use at present
//    template<typename T>
//    struct crtp : base{};

}//boost

#endif // RANDOM_REGRESSION_BASE_HPP_ER_2009
