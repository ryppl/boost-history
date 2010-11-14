//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/assign/v2/ref/anon/csv/nth_result_of.hpp>
#include <boost/assign/v2/ref/anon/csv/make.hpp>
#include <boost/assign/v2/ref/anon/csv/pp.hpp>


// Usage:
// #define xxx_Tag1 name
// BOOST_ASSIGN_V2_REF_CSV_ARRAY_ASSIGN_XXX( ns )
// #undef xxx_Tag1

#ifndef BOOST_ASSIGN_V2_REF_CSV_ARRAY_iter
#define BOOST_ASSIGN_V2_REF_CSV_ARRAY_iter( z, n, data ) \
    BOOST_ASSIGN_V2_REF_CSV_ARRAY_tpl(	T      ,	n, 	0, 	xxx_Tag1	) \
    BOOST_ASSIGN_V2_REF_CSV_ARRAY_tpl(	T const, 	n, 	0, 	xxx_Tag1	) \
/**/
#endif
#ifndef BOOST_ASSIGN_V2_REF_CSV_ARRAY_ASSIGN_XXX
#define BOOST_ASSIGN_V2_REF_CSV_ARRAY_ASSIGN_XXX( NS ) \
namespace boost{ \
namespace assign{ \
namespace v2{ \
namespace ref{ \
namespace NS{ \
namespace nth_result_of{ \
    template<std::size_t N,typename T> \
    struct csv_anon : ref::nth_result_of::csv_anon<N, xxx_Tag1, T>{};\
} \
	template<typename T>\
    typename ref::result_of::csv_anon_nil<xxx_Tag1, T>::type\
	csv_anon( keyword_aux::nil )\
    {\
    	typedef typename ref::result_of::csv_anon_nil<\
        	xxx_Tag1,\
            T\
        >::type result_;\
        return result_();\
    }\
BOOST_PP_REPEAT_FROM_TO( \
	1, \
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY), \
    BOOST_ASSIGN_V2_REF_CSV_ARRAY_iter, \
    ~ \
) \
} \
} \
} \
} \
} \
/**/

#endif
