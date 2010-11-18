//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_ER_2010_HPP

#include <boost/assign/v2/ref/anon/csv/nth_result_of.hpp>
#include <boost/assign/v2/ref/anon/csv/make.hpp>

// Associate a nested namespace with an assign tag.
// Usage:
// BOOST_ASSIGN_V2_REF_CSV_ARRAY_ASSIGN_XXX_1( NS, TAG )
// BOOST_ASSIGN_V2_REF_CSV_ARRAY_ASSIGN_XXX_2( NS, TAG )

#ifndef BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_1
#define BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_1( NS, TAG ) \
namespace boost{ \
namespace assign{ \
namespace v2{ \
namespace ref{ \
namespace NS{ \
namespace nth_result_of{ \
    template<std::size_t N, typename T> \
    struct csv_anon : ref::nth_result_of::csv_anon<N, TAG, T>{};\
} \
	template<typename T>\
    typename ref::result_of::empty_csv_anon<TAG, T>::type\
	csv_anon( keyword_aux::nil )\
    {\
    	typedef typename ref::result_of::empty_csv_anon<\
        	TAG,\
            T\
        >::type result_;\
        return result_();\
    }\
} \
} \
} \
} \
} \
/**/

#endif // BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_1

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>

#ifndef BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_M
#define BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_M(NS, TAG, U, Brgs)\
    template<typename T,typename...Args>\
    typename csv_anon_aux::result<TAG, U, Brgs...>::type\
    csv_anon( U& u, Brgs&...args )\
    {\
        typedef csv_anon_aux::result<TAG, U, Brgs...> caller_;\
        return caller_::call( u, args... );\
    }\
/**/

#endif // BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_M

#ifndef BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_2(NS, TAG)
#define BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_2(NS, TAG)\
namespace boost{ \
namespace assign{ \
namespace v2{ \
namespace ref{ \
namespace NS{ \
BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_M(NS, TAG, T, Args)\
BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_M(NS, TAG, T const, Args const)\
} \
} \
} \
} \
} \
/**/
#endif // BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_2

#else
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
//#include <boost/assign/v2/ref/anon/csv/pp.hpp>

#ifndef BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_M1
#define BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_M1(n, U, TAG)\
    template<typename T>\
    typename nth_result_of::csv_anon<\
        n,\
        U\
    >::type\
    csv_anon( BOOST_PP_ENUM_PARAMS(n, U& _) )\
    {\
        return ref::csv_anon<TAG>( BOOST_PP_ENUM_PARAMS(n, _) );\
    }\
/**/
#endif

#ifndef BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_M2
#define BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_M2(z, n, TAG)\
BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_M1(n, T, TAG)\
BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_M1(n, T const, TAG)\
/**/
#endif

#ifndef BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_2
#define BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_2( NS, TAG ) \
namespace boost{ \
namespace assign{ \
namespace v2{ \
namespace ref{ \
namespace NS{ \
BOOST_PP_REPEAT_FROM_TO( \
	1, \
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY), \
    BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_M2, \
    TAG \
) \
} \
} \
} \
} \
} \
/**/

#endif // BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_2

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

#endif // BOOST_ASSIGN_V2_REF_ANON_CSV_ASSIGN_XXX_ER_2010_HPP
