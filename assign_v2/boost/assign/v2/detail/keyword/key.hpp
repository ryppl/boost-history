//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_KEYWORD_KEY_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_KEYWORD_KEY_ER_2010_HPP

namespace boost{
namespace assign{
namespace v2{
//[syntax_detail_keyword_key
namespace keyword_aux{ struct key{}; }
//<-
namespace{
//->
    const keyword_aux::key _key/*<-*/ = keyword_aux::key()/*->*/;
//<-
}
//->
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_DETAIL_KEYWORD_KEY_ER_2010_HPP
