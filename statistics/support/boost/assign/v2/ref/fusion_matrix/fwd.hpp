//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_FUSION_MATRIX_FWD_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_FUSION_MATRIX_FWD_ER_2010_HPP
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
// do nothing
#else
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#endif

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace fusion_matrix_aux{

    template<
    	std::size_t N, typename L,typename Tag1, typename Tag2,
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
        typename...Args
#else
        BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
            BOOST_ASSIGN_V2_LIMIT_ARITY,
            typename T,
            boost::mpl::na
        )
#endif
    >
    class container;

}// fusion_matrix_aux
}// ref
}// v2
}// assign
}// boost

#endif
