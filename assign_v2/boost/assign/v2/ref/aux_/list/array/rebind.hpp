//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_AUX_LIST_ARRAY_REBIND_ER_2011_HPP
#define BOOST_ASSIGN_V2_REF_AUX_LIST_ARRAY_REBIND_ER_2011_HPP
#include <boost/assign/v2/ref/aux_/list/fwd.hpp>
#include <boost/assign/v2/ref/aux_/list/at.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/size_t.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_aux{

    template<
        size_type K, typename A,
        typename Tag, typename H, typename T
    >
    void assign_array(
        ::boost::mpl::true_ /*exit*/,
        A& a,
        const list_aux::container<Tag, H, T>& l
    )
    {
        /*exit*/
    }

    template<
        size_type K, typename A,
        typename Tag, typename H, typename T
    >
    void assign_array(
        ::boost::mpl::false_ /*exit*/,
        A& a,
        const list_aux::container<Tag, H, T>& l
    )
    {
        typedef ::boost::mpl::int_<K-1> index_;
        // GCC4.4 wants fully qual at
        a[ K - 1 ].rebind( ref::at<K-1>( l ) ) ;
        typedef index_ next_size_;
        typedef ::boost::mpl::int_<0> zero_;
        typedef typename ::boost::mpl::equal_to<
                next_size_, zero_
        >::type exit_;
        assign_array<K-1>( exit_(), a, l );
    }

    // A must be a static array of reference wrappers
    template<typename A, typename Tag, typename H, typename T>
    void assign_array(
        A& a,
        list_aux::container<Tag, H, T> const & l
    )
    {
        typedef list_aux::container<Tag, H, T> list_;
        BOOST_STATIC_ASSERT( A::static_size <= list_::tail_static_size::value );
        typedef ::boost::mpl::int_<0> zero_;
        typedef ::boost::mpl::int_<A::static_size> size_;
        typedef typename ::boost::mpl::equal_to<size_, zero_>::type exit_;
        list_aux::assign_array<size_::value>( exit_(), a, l );
    }

}// list_aux

using list_aux::assign_array;

}// ref
}// v2
}// assign
}// boost

#endif
