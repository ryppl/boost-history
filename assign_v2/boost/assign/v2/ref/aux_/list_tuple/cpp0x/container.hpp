//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_AUX_LIST_TUPLE_CPP0X_CONTAINER_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_AUX_LIST_TUPLE_CPP0X_CONTAINER_ER_2010_HPP
#include <utility>
#include <boost/assign/v2/ref/aux_/tuple/cpp0x/data.hpp>
#include <boost/config.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_tuple_aux{

    template<typename NewState>
    struct result
    {
        template<typename...Args>
        struct apply : NewState::template result<Args...>{};
    };

    template<list_size_type N, typename Tail, typename...Args>
    class container 
    	: public fetch_tuple<
        	N, L,
        	tuple_aux::data<Args...>,
        	list_tuple_aux::container<N, Tail, Args...>
    	>
    {
    	typename ::boost::mpl::eval_if_c<
        	N == 0,
        	::boost::mpl::identity< Tail >,
        	::boost::mpl::identity< Tail const& >
    	>::type tail_value_;
        typedef tuple_aux::data<Args...> head_value_;

        tail_value_ t;
        head_value_ h;

        public:
        tail_value_ const& tail()const{ return this->t; }
        head_value_ const& head()const{ return this->h; }

        typedef list_tuple_aux::list_size_type list_size_type;
        typedef list_tuple_aux::tuple_size_type tuple_size_type;

        BOOST_STATIC_CONSTANT( list_size_type, static_get_size = N );
        BOOST_STATIC_CONSTANT(
            tuple_size_type,
            static_tuple_size = sizeof...(Args)
        );

        container()
        {
            BOOST_STATIC_ASSERT( N == 0 );
        }

        explicit container(
            const Tail& t_,
            Args&&... args
        ) : t( t_ ), h( std::forward<Args>( args )... )
        {
            BOOST_STATIC_ASSERT( N > 0 );
        }

        template<typename... Args1>
        struct result
        {
            typedef container<N + 1, container, Args1...> type;
        };

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
