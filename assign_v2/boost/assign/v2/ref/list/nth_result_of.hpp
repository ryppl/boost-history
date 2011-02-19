//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_LIST_NTH_RESULT_OF_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_LIST_NTH_RESULT_OF_ER_2010_HPP
#include <boost/mpl/fold.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/vector.hpp> //user
#include <boost/assign/v2/ref/list/fwd.hpp>
#include <boost/assign/v2/ref/list/make.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_aux{

    template<typename Tag>
    struct nth_result
    {

        typedef typename ref::empty_list<Tag>::type state_;

        template<typename State, typename T>
        struct result : State::template result<T>{};

        template<typename Vec>
        struct apply : ::boost::mpl::fold<
            Vec,
            state_,
            result< ::boost::mpl::_1, ::boost::mpl::_2>
        >{};

    };

}// fusion_aux
namespace nth_result_of{

    template<typename Tag>
    struct list
    {
        template<typename Vec>
        struct apply : ::boost::mpl::apply1<
            list_aux::nth_result<Tag>,
            Vec
        >{};
    };

}// nth_result_of
}// ref
}// v2
}// assign
}// boost

#endif
