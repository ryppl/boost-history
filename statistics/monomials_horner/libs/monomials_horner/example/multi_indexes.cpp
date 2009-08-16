///////////////////////////////////////////////////////////////////////////////
// monomials_horner::example::multi_indexes.cpp                              //
//                                                                           //
//  (C) Copyright 2009 Erwann Rogard                                         //
//  Use, modification and distribution are subject to the                    //
//  Boost Software License, Version 1.0. (See accompanying file              //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <iterator>
#include <boost/range.hpp>
#include <boost/monomials_horner/multi_indexes.hpp>

void example_multi_indexes(std::ostream& out){
    out << "-> example_multi_indexes" << std::endl;

    using namespace boost;
    using namespace monomials_horner;

    typedef multi_indexes<3>      mi_type;
    typedef mi_type::iter_type          mi_iter_type;
    typedef mi_type::iter_range_type    mi_iter_range_type;

    unsigned int degree1 = 2;

    mi_iter_range_type ir = mi_type::get(degree1);

    for(mi_iter_type i = boost::begin(ir); i<boost::end(ir); i++){
        copy(
            boost::begin(*i),
            boost::end(*i),
            std::ostream_iterator<double>(out," ")
        ); out << std::endl;
    }out << std::endl;

    ir = mi_type::get(degree1+1);//TODO bug if say get(degree+1)

    for(mi_iter_type i = boost::begin(ir); i<boost::end(ir); i++){
        copy(
            boost::begin(*i),
            boost::end(*i),
            std::ostream_iterator<double>(out," ")
        ); out << std::endl;
    }

    out << "<-" << std::endl;
};
