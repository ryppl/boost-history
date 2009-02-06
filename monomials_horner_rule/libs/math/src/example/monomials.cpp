///////////////////////////////////////////////////////////////////////////////
// example/monomials.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <algorithm>
#include <boost/math/special_functions/binomial.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <iostream>
#include <boost/math/monomials.hpp>
#include <libs/math/src/example/monomials.h>

void example_monomials(){
    std::cout << "-> example_monomials" << std::endl;

    typedef std::vector<double> vars_type;
    namespace math = boost::math;
    typedef math::ifgt::monomials<vars_type> monoms_type;

    unsigned max_d = 5;
    unsigned max_p = 5;

    vars_type vars;
    {using namespace boost::assign; vars+=0.0,1.0,2.0,3.0,4.0;}


    for(unsigned d = 1; d<max_d; d++){
        std::cout << " -> d=" << d << std::endl;
        vars_type::iterator e = boost::begin(vars);
        std::advance(e,d);
        vars_type sub;
        copy(boost::begin(vars),e,back_inserter(sub));
        BOOST_ASSERT(boost::size(sub)-d==0);

        for(unsigned int p = 1; p<max_p; p++){
            std::cout << "  ->p=" << p << std::endl;
            //std::cout << boost::math::binomial_coefficient<double>(p+d,d)
            //<< std::endl;

            monoms_type monoms;

            monoms(sub,p);
            copy(
                boost::begin(monoms()),
                boost::end(monoms()),
                std::ostream_iterator<double>(std::cout," ")
            );
            std::cout << "  <-" << std::endl;
        }
            std::cout << " <-" << std::endl;
    }
    std::cout << "<-" << std::endl;
}
