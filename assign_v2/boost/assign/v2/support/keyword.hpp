//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_SUPPORT_KEYWORD_ER_2010_HPP
#define BOOST_ASSIGN_V2_SUPPORT_KEYWORD_ER_2010_HPP
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
    struct element/*<-*/{}/*->*/;
    struct key/*<-*/{}/*->*/;
    struct map/*<-*/{}/*->*/;
    struct nil/*<-*/{}/*->*/;
    struct use_default/*<-*/{}/*->*/;
    struct value/*<-*/{}/*->*/;

}//keyword_aux
    typedef keyword_aux::ignore         ignore_;
    typedef keyword_aux::element        element_;
    typedef keyword_aux::key            key_;
    typedef keyword_aux::map            map_;
    typedef keyword_aux::nil            nil_;
    typedef keyword_aux::use_default    use_default_;
    typedef keyword_aux::value          value_;    
/*<-*/namespace{/*->*/
    const ignore_ _ignore/*<-*/ = ignore_()/*->*/;
    const element_ _element/*<-*/ = element_()/*->*/;
    const key_ _key/*<-*/ = key_()/*->*/;
    const map_ _map/*<-*/ = map_()/*->*/;
    const nil_ _nil/*<-*/ = nil_()/*->*/;
    const use_default_ _use_default/*<-*/ = use_default_()/*->*/;
    const value_ _value/*<-*/ = value_()/*->*/;
/*<-*/}/*->*/
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_SUPPORT_KEYWORD_ER_2010_HPP
