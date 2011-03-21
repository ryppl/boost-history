//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_AUX_LIST_TUPLE_LIST_TUPLE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_AUX_LIST_TUPLE_LIST_TUPLE_ER_2010_HPP
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/int.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_tuple_aux{

    typedef int list_size_type;
    typedef int tuple_size_type;

    struct root{};

// (*) MSVC workaround : tail has to be variable, not a base class
// http://social.msdn.microsoft.com/Forums/en/vclanguage/thread/68d0361c-1813-4689-aaa3-b78f47eaf8ad
//    template<typename T>
//    struct tail_holder
//    {
//
//        tail_holder(T const& t) : tail( t ){}
//
//        T const& tail;
//    };

    template<list_size_type N, typename H, typename T, typename D>
    class fetch_tuple
    {
        typedef D const& result_of_derived_;
        result_of_derived_ derived()const
        {
            return static_cast<
                result_of_derived_
            >( *this );
        }

        public:

        template<list_size_type I> 
        struct is_head : ::boost::mpl::bool_< I + 1 == N >{};
        template<list_size_type I>
        struct tail_result : H::template get_result<I>{};
		struct head_result{ typedef T const& type; };

        template<list_size_type I>
        struct get_result : ::boost::mpl::eval_if<
        	is_head<I>,
            head_result,
            tail_result<I>
        >{};

        template<list_size_type I>
        typename boost::lazy_enable_if<is_head<I>, get_result<I> >::type
        get( ::boost::mpl::int_<I> index)const
        {
            return this->derived().head();
        }

        template<list_size_type I>
        typename boost::lazy_disable_if<is_head<I>, get_result<I> >::type
        get( ::boost::mpl::int_<I> index )const
        {
            return this->derived().tail().get( index );
        }

    };

}// list_tuple_aux
}// ref
}// v2
}// assign
}// boost

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/ref/aux_/list_tuple/cpp0x/container.hpp>
#else
#include <boost/assign/v2/ref/aux_/list_tuple/cpp03/container.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

#include <boost/mpl/apply.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/unpack_args.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace nth_result_of{ 

    class list_tuple
    {
        typedef list_tuple_aux::root root_;
        typedef list_tuple_aux::container<0, root_> state_;

        struct forward_op{

            template<typename NewState, typename ArgList>
            struct apply{
                typedef list_tuple_aux::result<NewState> meta_;
                typedef ::boost::mpl::unpack_args< meta_ > g_;
                typedef typename ::boost::mpl::apply1<g_, ArgList>::type type;
            };

        };

        public:

        template<typename ArgsList>
        struct apply : ::boost::mpl::fold<ArgsList, state_, forward_op>{};

    };

}// nth_result_of
namespace result_of{

    typedef list_tuple_aux::container<0, list_tuple_aux::root> list_tuple;

}// result_of
namespace
{

    result_of::list_tuple const
        list_tuple = result_of::list_tuple();

}
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_AUX_LIST_TUPLE_LIST_TUPLE_ER_2010_HPP
