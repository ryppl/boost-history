//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_STATIC_ARRAY_DETAIL_LAZY_ALLOC_FWD_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_STATIC_ARRAY_DETAIL_LAZY_ALLOC_FWD_ER_2010_HPP
 
namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace alloc_tag{
	struct lazy_alloc;
}// alloc_tag
namespace anon_aux{ 
   template<std::size_t N, typename Tag, typename T, typename D>
   class lazy_alloc;
}// anon_aux   
}// ref 
}// v2    
}// assign
}// boost

#endif
