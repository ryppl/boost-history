//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_SUPPORT_KEYWORD_ER_2011_HPP
#define BOOST_ASSIGN_V2_SUPPORT_KEYWORD_ER_2011_HPP
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>

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
    const element_ _element/*<-*/ = {}/*->*/;
    const key_ _key/*<-*/ = {}/*->*/;
    const map_ _map/*<-*/ = {}/*->*/;
    const nil_ _nil/*<-*/ = {}/*->*/;
    const use_default_ _use_default/*<-*/ = {}/*->*/;
    const value_ _value/*<-*/ = {}/*->*/;

/*<-*/}/*->*/
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_SUPPORT_KEYWORD_ER_2011_HPP
