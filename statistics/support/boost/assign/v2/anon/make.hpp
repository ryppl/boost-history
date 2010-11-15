//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_ANON_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_ANON_MAKE_ER_2010_HPP
#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/anon/cont.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace result_of{

    template<typename T>
    struct anon
    {
    	typedef typename boost::remove_cv<T>::type t_;
    	typedef typename anon_aux::impl<t_>::type cont_;
        typedef result_of::put<cont_> traits_;
        typedef typename traits_::f_ f_;
        typedef typename traits_::modifier_tag_ modifier_tag_;
        typedef anon_aux::cont<t_,f_,modifier_tag_> type;
    };

}// result_of

    template<typename T>
    typename result_of::anon<T>::type
	anon( keyword_aux::nil )
    {
    	typedef typename result_of::anon<T>::type result_;
        return result_();
    }

}// v2
}// assign
}// boost

#endif
