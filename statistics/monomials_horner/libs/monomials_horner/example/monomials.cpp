///////////////////////////////////////////////////////////////////////////////
// monomials_horner::example::monomials.cpp                                  //
//                                                                           //
//  (C) Copyright 2009 Erwann Rogard                                         //
//  Use, modification and distribution are subject to the                    //
//  Boost Software License, Version 1.0. (See accompanying file              //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <boost/math/special_functions/binomial.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <boost/monomials_horner/monomials.hpp>
#include <libs/monomials_horner/example/monomials.h>

void example_monomials(std::ostream& out){
    out << "-> example_monomials" << std::endl;

    using namespace boost;
    using namespace monomials_horner;

    typedef std::vector<double> vars_type;
    typedef monomials<vars_type> monoms_type;

    unsigned max_d = 5;
    unsigned max_p = 5;    vars_type vars;
    {
        using namespace boost::assign; 
        vars += 0.0,1.0,2.0,3.0,4.0;
    }

    for(unsigned d = 1; d<max_d; d++){
        out << " -> d=" << d << std::endl;
        vars_type::iterator e = boost::begin(vars);
        std::advance(e,d);
        vars_type sub;
        copy(boost::begin(vars),e,back_inserter(sub));
        BOOST_ASSERT(boost::size(sub)-d==0);

        for(unsigned int p = 1; p<max_p; p++){
            out << "  ->p=" << p << std::endl;
            //out << boost::math::binomial_coefficient<double>(p+d,d)
            //<< std::endl;

            monoms_type monoms;

            monoms(sub,p);
            copy(
                boost::begin(monoms()),
                boost::end(monoms()),
                std::ostream_iterator<double>(out," ")
            );
            out << "  <-" << std::endl;
        }
            out << " <-" << std::endl;
    }
    out << "<-" << std::endl;
}
