//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_UTILITY_CSV_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_UTILITY_CSV_MAKE_ER_2010_HPP
#include <boost/mpl/vector.hpp>
#include <boost/assign/v2/utility/csv/result.hpp>

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#endif

namespace boost{
namespace assign{
namespace v2{

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

	template<typename F, typename T>
    typename result_of::csv<F const&, 
    	::boost::mpl::vector<T>
    >::type
    csv( F const& functor, T&& t )
    {
    	return functor( t );
    }

	template<typename F, typename T, typename Args>
    typename result_of::csv<F const&, 
    	::boost::mpl::vector<T, Args...>
    >::type
    csv( F const& functor, T&& t, Args&&... args )
    {
    	return csv( functor( t ), std::forward<Args>( args )... );
	}

#else
#define BOOST_ASSIGN_V2_MACRO1(z, i, data) BOOST_PP_CAT(T, i) data
#define BOOST_ASSIGN_V2_MACRO2(z, i, data) ( BOOST_PP_CAT(_, i) )
#define BOOST_ASSIGN_V2_MACRO3(z, N, data)\
\
	template<typename F BOOST_PP_ENUM_TRAILING_PARAMS(N, typename T)>\
    typename result_of::csv<F const&, \
    	::boost::mpl::vector<BOOST_PP_ENUM(N, BOOST_ASSIGN_V2_MACRO1, &)>\
    >::type\
    csv( \
    	F const& functor\
    	BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N, T, & _)\
    )\
    {\
    	return functor BOOST_PP_REPEAT(N, BOOST_ASSIGN_V2_MACRO2, ~ );\
	}\
\
	template<typename F BOOST_PP_ENUM_TRAILING_PARAMS(N, typename T)>\
    typename result_of::csv<F const&,\
    	::boost::mpl::vector<BOOST_PP_ENUM(N, BOOST_ASSIGN_V2_MACRO1, const &)>\
    >::type\
    csv( \
    	F const& functor\
    	BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N, T, const & _)\
    )\
    {\
    	return functor BOOST_PP_REPEAT(N, BOOST_ASSIGN_V2_MACRO2, ~ );\
	}\
\
/**/
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO3,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2
#undef BOOST_ASSIGN_V2_MACRO3
#endif BOOST_ASSIGN_V2_ENABLE_CPP0X

}// v2
}// assign
}// boost

#endif
