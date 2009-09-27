//////////////////////////////////////////////////////////////////////////////
// dist_random::example::chi_squared.cpp                                    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <iterator>
#include <boost/static_assert.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <boost/standard_distribution/distributions/chi_squared.hpp>
#include <boost/dist_random/distributions/chi_squared.hpp>
#include <boost/dist_random/functional/check_convergence.hpp>
#include <boost/random/ref_distribution.hpp>
#include <boost/dist_random/functional/make_random.hpp>

#include <libs/dist_random/example/chi_squared.h>

void example_chi_squared(std::ostream& out){

    out << "example_chi_squared -> " << std::endl;
    using namespace boost;
    
    // Types
    typedef double                                          val_;
    typedef std::vector<val_>                               vals_;
    typedef mt19937                                         urng_;

    typedef math::chi_squared_distribution<val_>            mdist_;
    typedef boost::random::check_convergence<val_>          check_;

    // Constants
    const val_ df           = 10;
    const unsigned n_draw   = 1e3;
    const unsigned n_ks     = 1e1;

    BOOST_STATIC_ASSERT(n_draw % n_ks == 0);
    mdist_ mdist( df );
    urng_ urng;

    check_ check;
    /*
    check(
        mdist,
        make_random(mdist),
        n_draw,
        n_ks,
        urng
    );
    */

    
    out << check;
    out << "<-" << std::endl;

}
