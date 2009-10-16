///////////////////////////////////////////////////////////////////////////////
// statistics::importance_weights::example::scale_to_finite_sum.cpp          //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <vector>
#include <ostream>
#include <iterator>
#include <boost/range.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/importance_weights/algorithm/scale_to_finite_sum.hpp>
#include <libs/importance_weights/example/scale_to_finite_sum.h>
    
void example_scale_to_finite_sum(std::ostream& out){
    out << "-> example_algorithm_scale_sum_to_finite : ";
    using namespace boost;
        
    // Types
    typedef double val_;
    typedef std::vector<val_> vec_;
    typedef numeric::bounds<val_> bounds_;
        
    // Constants
    const range_size<vec_>::type n = 10;
    
    // Initialization
    val_ val =  bounds_::highest();
    out << "val = " << val << std::endl;
    BOOST_ASSERT( !math::isinf( val ) );
    BOOST_ASSERT( math::isinf( val + val ) );
        
    vec_ vec_in(n,val);
    {
        using namespace assign;
        vec_in += val, val, val;
    }

    // Computations
    val_ c = statistics::importance_weights::scale_to_finite_sum(
        boost::begin(vec_in),
        boost::end(vec_in)
    );
    BOOST_ASSERT( !math::isinf( c ) );

    out << "c = " << c << std::endl;

    val_ cum_sum = std::accumulate(
        boost::begin(vec_in),
        boost::end(vec_in),
        static_cast<val_>(0)
    );
    
    out << "cum_sum = " << cum_sum << std::endl;
    
    BOOST_ASSERT( !math::isinf( cum_sum ) );
        
    out << "<-" << std::endl;
        
}
    
