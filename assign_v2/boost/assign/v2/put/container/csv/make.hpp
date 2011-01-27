//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CONTAINER_CSV_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_CONTAINER_CSV_MAKE_ER_2010_HPP
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/detail/traits/container/value.hpp>
#include <boost/assign/v2/put/container/functor/make.hpp>
#include <boost/assign/v2/put/container/csv/adapter.hpp>
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#endif

namespace boost{
namespace assign{
namespace v2{
namespace result_of{

	template<typename C>
    struct csv_put
    {
		typedef csv_put_aux::adapter<C> type;
	};

}// result_of

	template<typename C>
	typename result_of::csv_put<C>::type
	csv_put( C& cont )
    {
		typedef typename result_of::csv_put<C>::type result_;
    	return result_( cont );
    }

}// v2
}// assign
}// boost

#endif
