//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_FUSION_LIST_FWD_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_FUSION_LIST_FWD_ER_2010_HPP

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace fusion_list_aux{

    template<std::size_t N, typename L, typename Tag,typename T> 
    class container;

    template<typename A, std::size_t N, typename L, typename Tag, typename T>
    void assign_array(A& a, fusion_list_aux::container<N, L, Tag, T> const & f);

}// fusion_list_aux
}// ref
}// v2
}// assign
}// boost

#endif
