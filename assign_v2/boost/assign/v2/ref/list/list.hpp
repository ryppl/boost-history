//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_LIST_LIST_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_LIST_LIST_ER_2010_HPP
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/ref/list/container.hpp>
#include <boost/assign/v2/ref/list/fwd.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector/vector0.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_aux{

    template<typename Tag>
    struct empty_list{ typedef list_aux::container<Tag> type; };

namespace nth_result_of{

    template<typename Tag>
    struct list
    {

        typedef typename empty_list<Tag>::type empty_list_;

        template<typename State, typename T>
        struct result : State::template result<T>{};

        template<typename ArgList>
        struct apply : ::boost::mpl::fold<
            ArgList,
            empty_list_,
            result< ::boost::mpl::_1, ::boost::mpl::_2>
        >{};

    };

}// result_of

	template<typename Tag>
	typename ::boost::mpl::apply1<
    	nth_result_of::list<Tag>, 
        ::boost::mpl::vector0<>
    >::type
    list( keyword_aux::nil )
    {
		return typename ::boost::mpl::apply1<
    		nth_result_of::list<Tag>, 
        	::boost::mpl::vector0<>
    	>::type();
    }

}// list_aux

using list_aux::list;

template<typename Tag> struct empty_list : list_aux::empty_list<Tag>{};

namespace nth_result_of{

    template<typename Tag>
    struct list 
    	: list_aux::nth_result_of::list<Tag>
	{};

}// nth_result_of
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_LIST_LIST_ER_2010_HPP
