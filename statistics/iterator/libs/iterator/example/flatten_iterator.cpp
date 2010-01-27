///////////////////////////////////////////////////////////////////////////////
// example::iterator::flatten_iterator.cpp                                   //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <iostream>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/range.hpp>
#include <vector>
#include <boost/iterator/flatten_iterator.hpp>
#include <boost/range/flatten_range.hpp>
#include <libs/iterator/example/flatten_iterator.h>

void example_flatten_iterator(std::ostream& out){
    out << "->example_flatten_iterator : ";

    using namespace boost;

    typedef int                                    		val_;
    typedef std::vector<val_>                           vals_;
    typedef range_size<vals_>::type                     size_;
	typedef std::vector<vals_>							mat_;

	vals_ vals1, vals2, vals3;
	mat_ mat; 
    {
    	using namespace boost::assign;
        mat.push_back(boost::assign::list_of(1)(2)(3)(4)(5));
        mat.push_back(boost::assign::list_of(6)(7)(8)(9)(10));
		mat.push_back(boost::assign::list_of(11)(12)(13)(14)(15));
	}

	{
    	typedef boost::range_iterator<mat_>::type			it_mat_;
		typedef flatten_iterator<it_mat_>					flat_it_;
	
		flat_it_ b = make_flatten_iterator(boost::begin(mat),boost::end(mat));
		flat_it_ e = make_flatten_iterator(boost::end(mat),boost::end(mat));

		int j = 0;
    	int n = std::distance(b,e);
    	BOOST_ASSERT(
    		n == boost::size(vals1) + boost::size(vals2) + boost::size(vals3)
    	);
		for(flat_it_ i = b; i!=e; i++, j++){
        	val_ val = *i;
   			BOOST_ASSERT(val = j);
   			BOOST_ASSERT(std::distance(b,e)==n-j);
    	}
    }
	{
    	typedef boost::range_iterator<const mat_>::type		it_mat_;
		typedef flatten_iterator<it_mat_>					flat_it_;
        
		const mat_& cmat = mat;
	
		flat_it_ b = make_flatten_iterator(boost::begin(cmat),boost::end(cmat));
		flat_it_ e = make_flatten_iterator(boost::end(cmat),boost::end(cmat));

		int j = 0;
    	int n = std::distance(b,e);
    	BOOST_ASSERT(
    		n == boost::size(vals1) + boost::size(vals2) + boost::size(vals3)
    	);
		for(flat_it_ i = b; i!=e; i++, j++){
        	val_ val = *i;
   			BOOST_ASSERT(val = j);
   			BOOST_ASSERT(std::distance(b,e)==n-j);
    	}
    }
    
    out << "<-" << std::endl;
}