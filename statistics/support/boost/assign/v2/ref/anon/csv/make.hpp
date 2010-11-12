//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_ANON_CSV_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_ANON_CSV_MAKE_ER_2010_HPP
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repeat_from_to.hpp>
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/assign/v2/ref/anon/csv/nth_result_of.hpp>
#include <boost/assign/v2/ref/anon/csv/pp.hpp>
 
// Usage
// using namespace boost::assign::v2::ref::csv;
//	csv_anon<assign_tag::name>( a, b, c );
 
namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace result_of{

	template<typename Tag1,typename T>
    struct empty_csv_anon 
    	: nth_result_of::csv_anon<0, Tag1, T>{};
    
}// result_of

	template<typename Tag1, typename T>
    typename result_of::empty_csv_anon<Tag1, T>::type
	csv_anon( keyword_aux::nil )
    {
    	typedef typename result_of::empty_csv_anon<Tag1, T>::type result_;
        return result_();
    }

#define MACRO( z, n, data )\
 BOOST_ASSIGN_V2_REF_CSV_ARRAY_tpl(T      , 	n,	1,	Tag1 ) \
 BOOST_ASSIGN_V2_REF_CSV_ARRAY_tpl(T const, 	n,	1,	Tag1 ) \
/**/

BOOST_PP_REPEAT_FROM_TO(
	1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    MACRO,
    ~
)

#undef MACRO

}// ref
}// v2
}// assign
}// boost

#endif
