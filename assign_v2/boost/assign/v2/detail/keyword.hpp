//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_KEYWORD_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_KEYWORD_ER_2010_HPP
#include <boost/preprocessor/cat.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_detail_keyword
namespace keyword_aux{

    struct ignore{ 
        ignore(){} 
        template<typename Anything> ignore(Anything){}
    };
    struct key/*<-*/{}/*->*/;
    struct nil/*<-*/{}/*->*/;
    struct use_default/*<-*/{}/*->*/;
    struct value/*<-*/{}/*->*/;

}// keyword_aux
/*<-*/namespace{/*->*/
    const keyword_aux::ignore _ignore/*<-*/ = keyword_aux::ignore()/*->*/;
    const keyword_aux::key _key/*<-*/ = keyword_aux::key()/*->*/;
    const keyword_aux::nil _nil/*<-*/ = keyword_aux::nil()/*->*/;
    const keyword_aux::use_default _use_default/*<-*/ = keyword_aux::use_default()/*->*/;
    const keyword_aux::value _value/*<-*/ = keyword_aux::value()/*->*/;
/*<-*/}/*->*/
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_DETAIL_KEYWORD_ER_2010_HPP
