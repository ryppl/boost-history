//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_CSV_ARRAY_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_CSV_ARRAY_ER_2010_HPP
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/ref/array/alloc/instant.hpp>
#include <boost/assign/v2/ref/array/size_type.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace array_aux{
namespace nth_result_of{

    template<array_size_type N, typename U>
    struct csv_array 
    {
        typedef array_aux::instant_alloc<N, U> type;
    };

}// result_of

    template<typename U>
    typename nth_result_of::csv_array<0, U>::type
    csv_array( keyword_aux::nil )
    {
		typedef typename nth_result_of::csv_array<0, U>::type result_;
        return result_();
    }

}// array_aux
using array_aux::csv_array;
//[nth_result_of
namespace nth_result_of{ 

    template<array_size_type N, typename /*<<U& has to be a reference>>*/U>
    struct /*<<Meta-function>>*/csv_array 
    //<-
    	: array_aux::nth_result_of::csv_array<N, U>
    //->
    {};

}// nth_result_of
//]
}// ref
}// v2
}// assign
}// boost

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/ref/array/cpp0x/csv_array.hpp>
#else 
#include <boost/assign/v2/ref/array/cpp03/csv_array.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

#endif // BOOST_ASSIGN_V2_REF_ARRAY_CSV_ARRAY_ER_2010_HPP
