//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_LIST_TUPLE_CPP0X_CONTAINER_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_LIST_TUPLE_CPP0X_CONTAINER_ER_2010_HPP
#include <utility>

#include <boost/mpl/identity.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/static_assert.hpp>
#include <boost/config.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/assign/v2/ref/tuple/cpp0x.hpp>
#include <boost/assign/v2/ref/list_tuple/extraction.hpp>
#include <boost/assign/v2/ref/list_tuple/common.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_tuple_aux{

    template<int N, typename L, typename...Args> class container;

    template<int N, typename L, typename...Args>
    struct meta_result
    {

        typedef container<N, L, Args...> this_;

		template<typename... Args1>
        struct apply
        {
            typedef container<
                N + 1,
                this_,
                Args1...
            > type;
        };

	};

    template<typename NewState>
    struct result
    {
        template<typename...Args>
        struct apply : NewState::template result<Args...>{};
    };

    template<int N, typename L, typename...Args>
    class container :
    boost::mpl::eval_if_c<
        N == 0,
        boost::mpl::identity< boost::mpl::empty_base >,
        boost::mpl::identity< link<L> >
    >::type,
    public list_tuple_aux::extraction<
        N, L,
        tuple_aux::container<Args...>,
        list_tuple_aux::container<N, L, Args...>
    >
    {
        typedef link<L> link_;
        typedef meta_result<N, L, Args...> meta_result_;

        typedef tuple_aux::container<Args...> tuple_;
        tuple_ tuple;

        public:
        tuple_ const& get_tuple()const{ return this->tuple; }
        link_ const& get_link()const{ return *this; }

        typedef int size_type;
        BOOST_STATIC_CONSTANT( size_type, static_get_size = N );
        BOOST_STATIC_CONSTANT(
            size_type,
            static_tuple_size = sizeof...(Args)
        );

        container()
        {
            BOOST_STATIC_ASSERT( N == 0 );
        }

        explicit container(
            const L& l,
            Args&&... args
        )
        : link_( l ),
        tuple(
            std::forward<Args>( args )...
        )

        {
            BOOST_STATIC_ASSERT( N > 0 );
        }

        template<typename... Args1>
        struct result : meta_result_::template apply<Args1...>{};

        template<typename... Args1>
        typename result<Args1...>::type
        operator()( Args1&&...args1 )const
        {
            typedef typename result<Args1...>::type result_type;
            return result_type(
                *this,
                std::forward<Args1>( args1 )...
            );
        }

    };

}// list_tuple_aux
}// ref
}// v2
}// assign
}// boost

#endif
