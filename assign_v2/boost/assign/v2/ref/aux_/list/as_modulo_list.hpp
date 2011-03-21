//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_LIST_REF_AS_MODULO_LIST_ER_2010_HPP
#define BOOST_ASSIGN_V2_LIST_REF_AS_MODULO_LIST_ER_2010_HPP
#include <boost/assign/v2/ref/aux_/list/at.hpp>
#include <boost/assign/v2/ref/aux_/list/fwd.hpp>
#include <boost/config.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/typeof/typeof.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_aux{

    template<size_type N, typename Lhs, typename ParList, 
        bool exit = (N == ::boost::mpl::size<ParList>::value)>
    struct as_modulo_list_impl
    {
        typedef typename ::boost::mpl::at_c<ParList, N>::type at_;
        
        template<typename P>
        struct result
        {
            static Lhs lhs;
            static P p;

#ifdef BOOST_MSVC
			BOOST_TYPEOF_NESTED_TYPEDEF_TPL(nested, lhs % p )
    		typedef typename nested::type type;
#else        
        	typedef BOOST_TYPEOF_TPL( lhs % p ) type;
#endif
        };
        
        typedef typename result<at_>::type result_;

        typedef as_modulo_list_impl<N + 1, result_, ParList> next_;

        typedef typename next_::type type;

        template<typename RhsList>
        static type call(Lhs const& lhs, RhsList const& rhs_list)
        {
            return next_::call( lhs % ref::at<N>( rhs_list ) , rhs_list );
        }

    };

    template<size_type N, typename Lhs, typename ParList>
    struct as_modulo_list_impl<N, Lhs, ParList, true>
    {

        typedef Lhs type;

        template<typename RhsList>
        static type call(Lhs const& lhs, RhsList const& rhs_list)
        {
            return lhs;
        }

    };

namespace result_of{

    // ParList could, in principle, be specified in terms (Tag, H, T), but we 
    // chose not to (that would be less practical in client code).
    template<typename Lhs, typename ParList>
    struct as_modulo_list : as_modulo_list_impl<0, Lhs, ParList>{};
    
}// result_of

    // Warning Parlist is first argument, here, whereas second in result_of::
    template<typename ParList, typename Lhs, typename Tag, typename H, typename T>
    typename result_of::as_modulo_list<Lhs, ParList>::type
    as_modulo_list( Lhs const& lhs, container<Tag, H, T> const& rhs_list )
    {
        typedef result_of::as_modulo_list<Lhs, ParList> caller_;
        return caller_::call( lhs, rhs_list );
    }

}// list_aux
using namespace list_aux::result_of;
using list_aux::as_modulo_list;
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_LIST_REF_AS_MODULO_LIST_ER_2010_HPP
