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

    struct kwd_ignore_
    {
        kwd_ignore_(){}
        template<typename Anything> kwd_ignore_(Anything){}
    };
    struct kwd_element_/*<-*/{}/*->*/;
    struct kwd_key_/*<-*/{}/*->*/;
    struct kwd_map_/*<-*/{}/*->*/;
    struct kwd_nil_/*<-*/{}/*->*/;
    struct kwd_use_default_/*<-*/{}/*->*/;
    struct kwd_value_/*<-*/{}/*->*/;

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
