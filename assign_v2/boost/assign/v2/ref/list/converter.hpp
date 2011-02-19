//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_LIST_CONVERTER_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_LIST_CONVERTER_ER_2010_HPP
#include <boost/assign/v2/ref/list/fwd.hpp>
#include <boost/assign/v2/utility/convert/converter.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_aux{

#define SEQ (Tag)(H)(T)
#define U container<Tag, H, T>
BOOST_ASSIGN_V2_CONVERTER(U, SEQ)
#undef U
#undef SEQ

}// list_aux
}// ref
}// v2
}// assign
}// boost

#endif
