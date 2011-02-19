//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_DEQUE_CSV_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_DEQUE_CSV_ER_2010_HPP
#include <boost/type_traits/decay.hpp>
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/put/deque/cont.hpp>
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/put/deque/functor.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#endif

namespace boost{
namespace assign{
namespace v2{
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
namespace put_csv_deque_aux{

	template<typename T, typename R>
    void impl(R& r){}

	template<typename T, typename R, typename...Args>
    void impl(
        R& r,
        T const& t,
        Args&&...args
    )
    {
        r( t );
        put_csv_deque_aux::impl<T>(r, std::forward<Args>( args )... );
    }

}//put_csv_deque_aux

	template<typename T, typename... Args>
    typename result_of::deque<
    	typename boost::decay<T>::type
    >::type
    csv_deque(const T& t, Args const& ...  args)
    {
    	typedef typename boost::decay<T>::type decay_;
        typedef typename result_of::deque<decay_>::type result_;
        result_ result = deque<decay_>( v2::_nil );
        put_csv_deque_aux::impl<T>(result, t, args...);
        return result;
    }

#else
#define BOOST_ASSIGN_V2_MACRO1(z, i, data) ( BOOST_PP_CAT(_, i) )
#define BOOST_ASSIGN_V2_MACRO2(z, N, data)\
	template<typename T>\
    typename result_of::deque<\
    	typename boost::decay<T>::type\
    >::type\
    csv_deque( BOOST_PP_ENUM_PARAMS(N, T const & _) )\
    {\
    	typedef typename boost::decay<T>::type decay_;\
        return deque<decay_>( v2::_nil) BOOST_PP_REPEAT(N, BOOST_ASSIGN_V2_MACRO1, ~ );\
    }\
/**/
BOOST_PP_REPEAT_FROM_TO(
	1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO2,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2
#endif

}// v2
}// assign
}// boost

#endif
