//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_RESULT_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_RESULT_ER_2010_HPP
#include <boost/config.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/assign/v2/ref/detail/convert_traits/twin_values.hpp>
#include <boost/assign/v2/ref/detail/convert_traits/distinct_values.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace convert_traits{

	template<typename T1,typename T2>
	struct result
	{

    	template<typename T>
    	struct traits{
        
        	typedef typename boost::is_reference<T>::type is_ref_;
        	typedef typename boost::remove_reference<T>::type const_;
        	typedef typename boost::is_const<const_>::type is_const_;
        	typedef typename boost::remove_const<const_>::type val_;

    	};

    	typedef traits<T1> traits1_;
	    typedef traits<T2> traits2_;
      
    	typedef typename traits1_::val_ val1_;
	    typedef typename traits2_::val_ val2_;
      
	    typedef boost::is_same<val1_,val2_> same_val_;

	    typedef typename boost::mpl::and_<
    	    typename traits1_::is_ref_,
        	typename traits2_::is_ref_
	    >::type and_ref_;

	    typedef typename boost::mpl::or_<
    	    typename traits1_::is_const_,
    	    typename traits2_::is_const_
    	>::type or_const_;
        
	    typedef typename boost::mpl::eval_if<
    	    same_val_,
    	    twin_values::reference<
    	        val1_,
    	        or_const_::value,
    	        and_ref_::value
    	    >,
    	    distinct_values::reference<
    	        val1_,
    	        val2_,
    	        or_const_::value,
    	        and_ref_::value
    	    >
    	>::type type;
	
    	typedef boost::mpl::and_<
        	boost::is_convertible<T1,type>,
        	boost::is_convertible<T2,type>
	    > is_valid_type;

	    BOOST_STATIC_CONSTANT(bool, is_valid = is_valid_type::value);
        
	    static void test()
    	{
	        BOOST_STATIC_ASSERT( is_valid );
    	}
        
	};

}// convert_traits
}// ref
}// v2
}// assign
}// boost

#endif
