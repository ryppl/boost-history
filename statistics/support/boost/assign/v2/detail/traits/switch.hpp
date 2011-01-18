//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_TRAITS_SWITCH_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_TRAITS_SWITCH_ER_2010_HPP
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/apply.hpp>
//#include <boost/mpl/always.hpp>
#include <boost/mpl/placeholders.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace switch_tag{

	// Each tag in this namespace identifies a switch statement.

}//switch_tag
namespace switch_aux{
    
    // Fully specialize by inheriting from wrap<Tag,F>
    // To terminate the switch, inherit from wrap<Tag>
    template<typename SwitchTag, int k> struct case_{};
    
    template<typename T>
    struct default_f : ::boost::mpl::true_{};
    
    template<typename Tag, template<typename> class F = switch_aux::default_f>
    struct helper
    {
        typedef Tag tag;
        template<typename T> struct apply : F<T>
        {
        	// Is Tag compatible with T? Answer : F<T>::value.
        };
    };

    template<typename H> // helper
    struct extract_tag
    {
        typedef typename H::tag type;
    };

    template<typename SwitchTag, typename T, int k = 0>
    struct result : ::boost::mpl::eval_if<
        ::boost::mpl::apply1< switch_aux::case_<SwitchTag, k>, T >,
        switch_aux::extract_tag< switch_aux::case_<SwitchTag, k> >,
        switch_aux::result< SwitchTag, T, k + 1>
    >{};

}// switch_aux
}// v2
}// assign
}// boost

#endif
