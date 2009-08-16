///////////////////////////////////////////////////////////////////////////////
// binary_op::example::find_accumulate.cpp                                   //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <vector>
#include <list>
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/range.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/functional/mean_var_accumulator.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/functional/find.hpp>
#include <boost/binary_op/functional/match_accumulator.hpp>
#include <libs/binary_op/example/find_accumulate.h>

void example_find_accumulate(std::ostream& out){
    out << "-> example_find_accumulate : ";
    using namespace boost;
    using namespace binary_op;
    namespace fun_ = functional;
    
    typedef unsigned val_;
    typedef std::vector<val_> val_vec_;
    typedef accumulators::stats<
        accumulators::tag::mean
    >  stat_;
    typedef accumulators::accumulator_set<val_,stat_> a_;
    typedef tuple<val_,a_> xa_;
    // tuple elements are accessed by reference so a tuple of values
    // rather than references is enough, as long as the tuple itself is 
    // passed by reference to the functor below.
    typedef std::vector<xa_> vec_xa_; 
    
    const unsigned n = 5;
    const unsigned n_repeat = 2;
    
    val_vec_ vec_x(n);
    val_vec_ vec_y(n);
    vec_xa_ vec_xa;
    {
        for(unsigned i = 0; i<n; i++){
            vec_x[i] = i;
            vec_y[i] = i;
        } 
    }
    
    typedef functional::find find_;
    typedef match_accumulator<vec_xa_&,find_> functor_;
    
    // Look here for difference with match_accumulator2
    std::for_each(
        make_zip_iterator( make_tuple( begin(vec_x), begin(vec_y) ) ),
        make_zip_iterator( make_tuple( end(vec_x), end(vec_y) ) ),
        functor_(vec_xa)
    );
    
    for(val_ l = 0; l<n_repeat; l++){
        for(val_ i = 0; i<n; i++){
            val_ j = vec_y[i];
            const a_& a = get<1>(vec_xa.at(i));
            val_ k = accumulators::mean(a);
            BOOST_ASSERT(j == k);
        }
    }
    
    out << "<-" << std::endl;
    
}
