//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_ANON_CSV_ASSIGN_XXX_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_ANON_CSV_ASSIGN_XXX_ER_2010_HPP
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/assign/v2/detail/config/arity_bound.hpp>
#include <boost/assign/v2/ref/anon/csv/nth_result_of.hpp>
#include <boost/assign/v2/ref/anon/csv/pp.hpp>

// Usage:
// #define xxx_Tag1 name
// BOOST_ASSIGN_V2_REF_CSV_ARRAY_ASSIGN_XXX( ns )
// #undef xxx_Tag1

#define BOOST_ASSIGN_V2_REF_CSV_ARRAY_iter( z, n, data ) \
    BOOST_ASSIGN_V2_REF_CSV_ARRAY_tpl(	T      ,	n, 	0, 	xxx_Tag1	) \
    BOOST_ASSIGN_V2_REF_CSV_ARRAY_tpl(	T const, 	n, 	0, 	xxx_Tag1	) \
/**/
#define BOOST_ASSIGN_V2_REF_CSV_ARRAY_ASSIGN_XXX( NS ) \
namespace boost{ \
namespace assign{ \
namespace v2{ \
namespace ref{ \
namespace NS{ \
namespace nth_result_of{ \
    template<std::size_t N,typename T> \
    struct csv_anon : ref::nth_result_of::csv_anon<N,xxx_Tag1,T>{};\
} \
BOOST_PP_REPEAT_FROM_TO( \
	1, \
    BOOST_ASSIGN_V2_CSV_ARITY_BOUND, \
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
