//////////////////////////////////////////////////////////////////////////////
// statistics::estimator_concept::meta::estimate_input.hpp                  //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_ESTIMATOR_CONCEPT_META_ESTIMATE_INPUT_HPP_ER_2009
#define  BOOST_STATISTICS_ESTIMATOR_CONCEPT_META_ESTIMATE_INPUT_HPP_ER_2009
#include <boost/tuple/tuple.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/binary_op/meta/is_tuple.hpp>

namespace boost{
namespace statistics{
namespace estimator_concept{

template <typename T>
struct estimate_input : mpl::eval_if<
    binary_op::is_tuple<T>,         // T = (X,Y) 
    boost::tuples::element<0,T>,    // X
    mpl::identity<T>                
>{};

}// estimator_concept
}// statistics
}// boost

#endif