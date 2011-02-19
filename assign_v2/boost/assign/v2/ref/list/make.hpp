//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_LIST_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_LIST_MAKE_ER_2010_HPP
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/ref/list/container.hpp>

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace ref{

    template<typename Tag>
    struct empty_list{ typedef list_aux::container<Tag> type; };
    
    template<typename Tag>
    typename empty_list<Tag>::type
    list( keyword_aux::nil )
    {
        typedef typename empty_list<Tag>::type result_;
        return result_();
    }

}// ref
}// v2
}// assign
}// boost

#endif
