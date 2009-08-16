//////////////////////////////////////////////////////////////////////////////
// dist_random::example::sample.cpp                                         //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <ostream>
#include <boost/random/mersenne_twister.hpp>
#include <boost/standard_distribution/distributions/include.hpp>
#include <boost/dist_random/distributions/include.hpp>
#include <boost/dist_random/random/sample.hpp>

void example_sample(std::ostream& out){
    out << "-> example_sample";

    using namespace boost;
    typedef double val_;
    typedef mt19937 urng_;
    urng_ urng;

    {
        typedef math::chi_squared_distribution<val_>    dist_;
        sample(dist_(4),urng);
    }
    {
        typedef math::exponential_distribution<val_>    dist_;
        sample(dist_(),urng);
    }
    {
        typedef math::gamma_distribution<val_>          dist_;
        sample(dist_(4),urng);
    }
    {
        typedef math::normal_distribution<val_>         dist_;
        sample(dist_(),urng);
    }
    {
        typedef math::students_t_distribution<val_>     dist_;
        sample(dist_(4),urng);
    }
    {
        typedef math::students_t_distribution<val_>     dist_;
        typedef math::location_scale_distribution<dist_> lsdist_;
        sample(lsdist_(dist_(4),0,1),urng);
    }
    out << "<-";
}
