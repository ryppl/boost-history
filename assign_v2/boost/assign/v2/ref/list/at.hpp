//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_LIST_AT_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_LIST_AT_ER_2010_HPP
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/call_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/assign/v2/ref/list/fwd.hpp>
#include <boost/assign/v2/ref/list/size_type.hpp>
#include <boost/assign/v2/ref/list/holder.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_aux{

    template<size_type I, typename T>
    struct is_head : 
        ::boost::mpl::bool_< I + 1 == T::size::value>{};

namespace result_of{

		template<size_type I, typename T>
        struct at : ::boost::mpl::eval_if<
        	list_aux::is_head<I, T>
            ,head_reference<T>
            ,result_of::at<I, typename tail_of<T>::type>
        >{};
                
}// result_of

		template<size_type I, typename T>
        typename boost::lazy_enable_if<
        	list_aux::is_head<I, T>,
        	list_aux::result_of::at<I, T>
        >::type
        at_helper(T const& t)
        {
        	return t.head();
        }
        
		template<size_type I, typename T>
        typename boost::lazy_disable_if<
        	is_head<I, T>,
        	list_aux::result_of::at<I, T>
        >::type
        at_helper(T const& t)
        {
        	return at_helper<I>( t.tail() );
        }

		template<size_type I, typename Tag, typename H, typename T>
        typename list_aux::result_of::at<I, container<Tag, H, T> >::type
        at(container<Tag, H, T> const& t)
        {
        	return at_helper<I>( t );
        }

}// list_aux
using list_aux::at;
namespace result_of{

    template<list_aux::size_type I, typename T>
    struct at : list_aux::result_of::at<I, T>{};

}// result_of
}// ref
}// v2
}// assign
}// boost

#endif
