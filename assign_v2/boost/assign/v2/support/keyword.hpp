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

    typedef keyword_aux::ignore         kwd_ignore_;
    typedef keyword_aux::element        kwd_element_;
    typedef keyword_aux::key            kwd_key_;
    typedef keyword_aux::map            kwd_map_;
    typedef keyword_aux::nil            kwd_nil_;
    typedef keyword_aux::use_default    kwd_use_default_;
    typedef keyword_aux::value          kwd_value_;
/*<-*/namespace{/*->*/
    const kwd_ignore_ _ignore/*<-*/ = kwd_ignore_()/*->*/;
    const kwd_element_ _element/*<-*/ = {}/*->*/;
    const kwd_key_ _key/*<-*/ = {}/*->*/;
    const kwd_map_ _map/*<-*/ = {}/*->*/;
    const kwd_nil_ _nil/*<-*/ = {}/*->*/;
    const kwd_use_default_ _use_default/*<-*/ = {}/*->*/;
    const kwd_value_ _value/*<-*/ = {}/*->*/;

/*<-*/}/*->*/
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_SUPPORT_KEYWORD_ER_2011_HPP
