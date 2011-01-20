//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_FUSION_FWD_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_FUSION_FWD_ER_2010_HPP
#include <boost/assign/v2/ref/fusion/size_type.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace fusion_aux{

    template<fusion_aux::size_type N, typename L, 
    	typename Tag1,typename Tag2, typename T>
    class container;
    
    template<typename A, fusion_aux::size_type N, typename L, 
    	typename Tag1, typename Tag2, typename T>
    void assign_array(
    	A& a, 
        fusion_aux::container<N, L, Tag1, Tag2, T> const & f
    );

}// fusion_aux
}// ref
}// v2
}// assign
}// boost

#endif
