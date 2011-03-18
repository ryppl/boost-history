//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_AUX_LIST_FWD_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_AUX_LIST_FWD_ER_2010_HPP

namespace boost{
    struct use_default; // TODO ?
namespace assign{
namespace v2{
namespace ref{
namespace list_aux{

    typedef int size_type;

    template<typename Tag> struct empty_list;

    struct void_;
    struct nil;

    template<typename Tag, typename H = void_, typename T = nil>
    class container;

}// list_aux
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_AUX_LIST_FWD_ER_2010_HPP

