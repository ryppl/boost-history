///////////////////////////////////////////////////////////////////////////////
// example/multi_factorial.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <algorithm>
#include <iostream>
#include <iterator>
#include <boost/range.hpp>
#include <boost/math/multi_factorial.hpp>
#include <boost/math/multi_indexes_derived.hpp>
#include <libs/math/src/example/multi_factorial.h>
void example_multi_factorial(){
    std::cout << "-> example_multi_factorial" << std::endl;

    namespace math = boost::math;

    const unsigned d = 3;
    //typedef math::multi_factorial<d>          mf_type;
    typedef math::multi_indexes_derived<d,math::multi_factorial_op>   mf_type;
    typedef math::multi_indexes_derived<d,math::multi_degree_op>      md_type;
    typedef math::multi_indexes_derived<d,math::multi_power2divfact_op> mp_type;
    typedef mf_type::iter_range_type            mf_iter_range_type;
    typedef md_type::iter_range_type            md_iter_range_type;
    typedef mp_type::iter_range_type            mp_iter_range_type;

    unsigned int degree1 = 5;

    mf_iter_range_type mf_ir = mf_type::get(degree1);

    std::cout << " -> multi_factorial<" << d << ">";
    std::cout << "::degree(" << degree1 << ")" << std::endl;
    copy(
        boost::begin(mf_ir),
        boost::end(mf_ir),
        std::ostream_iterator<double>(std::cout," ")
    ); std::cout << std::endl;

    md_iter_range_type md_ir = md_type::get(degree1);

    std::cout << " -> multi_degree<" << d << ">";
    std::cout << "::degree(" << degree1 << ")" << std::endl;
    copy(
        boost::begin(md_ir),
        boost::end(md_ir),
        std::ostream_iterator<double>(std::cout," ")
    ); std::cout << std::endl;
    mp_iter_range_type mp_ir = mp_type::get(degree1);

    std::cout << " -> multi_p2df<" << d << ">";
    std::cout << "::degree(" << degree1 << ")" << std::endl;
    copy(
        boost::begin(mp_ir),
        boost::end(mp_ir),
        std::ostream_iterator<double>(std::cout," ")
    ); std::cout << std::endl;

    std::cout << "<-" << std::endl;

};
