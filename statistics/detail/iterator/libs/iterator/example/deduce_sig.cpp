//////////////////////////////////////////////////////////////////////////////
// iterator::transform_iterator::deduce_sig.h                               //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_ITERATOR_TRANSFORM_ITERATOR_DEDUCE_SIG_FP_ER_2010_HPP
#define LIBS_ITERATOR_TRANSFORM_ITERATOR_DEDUCE_SIG_FP_ER_2010_HPP
#include <ostream>
#include <vector>
#include <boost/range.hpp>
#include <boost/iterator/transform_iterator_deduce_sig_fp.hpp>
#include <boost/iterator/transform_iterator_deduce_sig_uf.hpp>


struct f{
	struct result_impl{ typedef bool type; };

	template<typename S>
    struct result{};
    
    template<typename F,typename T>
    struct result<F(const T&)>:result_impl{};

    template<typename F,typename T>
    struct result<F(T&)>:result_impl{};
    
    template<typename T>
    bool operator()(const T& x)const{ return (x==0); }

};

struct g{
	typedef bool result_type;

    template<typename T>
    bool operator()(const T& x)const{ return (x==0); }

};

template<typename T>
bool is_zero(const T& x){ return (x==0); }

void example_deduce_sig(std::ostream& os){
	using namespace boost;

	typedef int val_;
	typedef std::vector<val_> vec_;
    vec_ vec(10,0); 
	vec[3] = 1;
	vec[6] = 2;
	vec[9] = 3;

// Cannot compile	
/*
    std::copy(
		make_transform_iterator(
    		boost::begin(vec),		
			g()
    	),
		make_transform_iterator(
    		boost::end(vec),		
			g()
    	),
		std::ostream_iterator<val_>(os," ")
    ); 
*/
    std::copy(
		make_transform_iterator2(
    		boost::begin(vec),		
			f()
    	),
		make_transform_iterator2(
    		boost::end(vec),		
			f()
    	),
		std::ostream_iterator<val_>(os," ")
    ); 
    os << std::endl;
    std::copy(
		make_transform_iterator2(
    		boost::begin(vec),		
			g()
    	),
		make_transform_iterator2(
    		boost::end(vec),		
			g()
    	),
		std::ostream_iterator<val_>(os," ")
    );
	os << std::endl;

	typedef boost::range_iterator<const vec_>::type c_it_;

    make_transform_iterator2<bool>(
        boost::const_begin(vec),	// will not compile with begin(vec);	
        is_zero
    );

}

#endif