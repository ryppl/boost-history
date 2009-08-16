//////////////////////////////////////////////////////////////////////////////
// dist_random::example::generate_n.cpp                                     //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <ostream>
#include <vector>
#include <iterator>
#include <boost/range.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/standard_distribution/distributions/normal.hpp>
#include <boost/dist_random/distributions/normal.hpp>
#include <boost/dist_random/random/generate_n.hpp>
#include <libs/dist_random/example/generate_n.h>

void example_generate_n(std::ostream& out){
    out << "-> example_generate_n";
    using namespace boost;
    typedef double val_;
    typedef std::vector<val_> vals_;
    typedef mt19937 urng_;
    const long n = 1e2;
    urng_ urng;
    vals_ draws(n);
    vals_ lpdfs;
    {
        typedef math::normal_distribution<val_> dist_;
        dist_ dist;
        lpdfs.clear();
        generate_n(
            boost::begin(draws),
            std::back_inserter(lpdfs),
            n,
            dist,
            urng
        );

        std::copy(
            boost::begin(lpdfs),
            boost::end(lpdfs),
            std::ostream_iterator<val_>(out," ")
        );
        
    }

    out << "<-" << std::endl;
}