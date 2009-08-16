///////////////////////////////////////////////////////////////////////////////
// monomials_horner::example::multi_factorial.cpp                            //
//                                                                           //
//  (C) Copyright 2009 Erwann Rogard                                         //
//  Use, modification and distribution are subject to the                    //
//  Boost Software License, Version 1.0. (See accompanying file              //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <iterator>
#include <boost/range.hpp>
#include <boost/monomials_horner/multi_indexes_derived.hpp>
#include <libs/monomials_horner/example/multi_factorial.h>

void example_multi_factorial(std::ostream& out){
    out << "-> example_multi_factorial" << std::endl;

    using namespace boost;
    using namespace monomials_horner;

    const unsigned d = 3;
    typedef multi_indexes_derived<d,multi_factorial>        mf_type;
    typedef multi_indexes_derived<d,multi_degree>           md_type;
    typedef multi_indexes_derived<d,multi_power2divfact>    mp_type;
    typedef mf_type::iter_range_type            mf_iter_range_type;
    typedef md_type::iter_range_type            md_iter_range_type;
    typedef mp_type::iter_range_type            mp_iter_range_type;

    unsigned int degree1 = 5;

    mf_iter_range_type mf_ir = mf_type::get(degree1);

    out << " -> multi_factorial<" << d << ">";
    out << "::degree(" << degree1 << ")" << std::endl;
    copy(
        boost::begin(mf_ir),
        boost::end(mf_ir),
        std::ostream_iterator<double>(out," ")
    ); out << std::endl;

    md_iter_range_type md_ir = md_type::get(degree1);

    out << " -> multi_degree<" << d << ">";
    out << "::degree(" << degree1 << ")" << std::endl;
    copy(
        boost::begin(md_ir),
        boost::end(md_ir),
        std::ostream_iterator<double>(out," ")
    ); out << std::endl;
    mp_iter_range_type mp_ir = mp_type::get(degree1);

    out << " -> multi_p2df<" << d << ">";
    out << "::degree(" << degree1 << ")" << std::endl;
    copy(
        boost::begin(mp_ir),
        boost::end(mp_ir),
        std::ostream_iterator<double>(out," ")
    ); out << std::endl;

    out << "<-" << std::endl;

};
