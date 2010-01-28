///////////////////////////////////////////////////////////////////////////////
// example::iterator::flatten_iterator.cpp                                   //
//                                                                           //
/////////////////////////////////////////////////////////////////////////////// 
#include <iostream>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/range.hpp>
#include <vector>
#include <boost/iterator/flatten_iterator.hpp>
#include <boost/range/flatten_range.hpp>
#include <libs/iterator/example/flatten_iterator.h>

void example_flatten_iterator(std::ostream& os){
    os << "->example_flatten_iterator : ";

    using namespace boost;

    typedef int                                    		val_;
    typedef std::vector<val_>                           vals_;
    typedef range_size<vals_>::type                     size_;
	typedef std::vector<vals_>							mat_;

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
        typedef flat_it_::difference_type 					diff_;
	
		flat_it_ b(boost::begin(mat),boost::end(mat));
		flat_it_ e(boost::end(mat),boost::end(mat));

		int j = 0;
    	int n = std::distance(b,e);
        os << "n=" << n << std::endl;
    	BOOST_ASSERT(
    		n == boost::size(mat[0]) + boost::size(mat[1]) + boost::size(mat[2])
    	);
        flat_it_ i = b;
        BOOST_ASSERT(i==b);
		++i;
        BOOST_ASSERT(i!=b);
        // temporary --->
		//n = b.distance_to(i); 	
        //BOOST_ASSERT(n==1);
		//n = i.distance_to(b);
        //BOOST_ASSERT(n==-1);	
        // <---
		n = std::distance(b,i); // fails : should be 1
        os << "n=" << n << std::endl;


/*
		for(; i!=e; i++, j++){
        	val_ val = *i;
   			BOOST_ASSERT(val == j+1);
            diff_ d = std::distance(i,e);
            std::cout << "d = " << d << std::endl;
   			BOOST_ASSERT(d==n-j);
    	}
*/
    }
/*
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
*/    
    os << "<-" << std::endl;
}