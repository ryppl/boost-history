//////////////////////////////////////////////////////////////////////////////
// dist_random::example::location_scale_students_t.cpp                      //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/static_assert.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <boost/standard_distribution/distributions/students_t.hpp>
#include <boost/standard_distribution/transformation/location_scale.hpp>
#include <boost/random/students_t.hpp>
#include <boost/random/location_scale.hpp>
#include <boost/dist_random/functional/check_convergence.hpp>
#include <boost/dist_random/distributions/students_t.hpp>
#include <boost/dist_random/distributions/location_scale.hpp>

#include <libs/dist_random/example/location_scale_students_t.h>

void example_location_scale_students_t(std::ostream& out){

    out << "example_location_scale_students_t -> " << std::endl;
    using namespace boost;
    
    // Types
    typedef std::string                                     str_;
    typedef double                                          val_;
    typedef std::vector<val_>                               vals_;
    typedef mt19937                                         urng_;

    typedef math::students_t_distribution<val_>             mstud_;
    typedef math::location_scale_distribution<mstud_>       mdist_;

    typedef boost::random::check_convergence<val_>          check_;

    // Constants
    const val_ df           = 10;
    const val_ mu           = 1.0;
    const val_ sigma        = 2.0;
    const unsigned n_draw   = 1e3;
    const unsigned n_ks     = 1e1;

    BOOST_STATIC_ASSERT(n_draw % n_ks == 0);
    mdist_ mdist( mstud_( df ), mu , sigma);
    urng_ urng;

/*
    check_ check;
    check(
        mdist,
        make_random(mdist),
        n_draw,
        n_ks,
        urng
    );
    
    out << check;
*/
    out << "<-" << std::endl;

}