//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_ANON_FUNCTOR_NTH_RESULT_OF_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_ANON_FUNCTOR_NTH_RESULT_OF_ER_2010_HPP
#include <boost/assign/v2/ref/anon/generic/nth_result_of.hpp>
#include <boost/assign/v2/ref/anon/detail/lazy_alloc_fwd.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace anon_aux{
	typedef ref::alloc_tag::lazy_alloc ext_tag2_;
}// anon_aux
namespace nth_result_of{

    template<std::size_t N,typename Tag1,typename T>
    struct anon : ref::nth_result_of::generic_anon<
        N, Tag1, anon_aux::ext_tag2_, T
    >
    {};

}// nth_result_of
}// ref
}// v2
}// assign
}// boost

#endif
