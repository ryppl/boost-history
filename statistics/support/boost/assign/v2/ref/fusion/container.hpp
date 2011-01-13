//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_FUSION_CONTAINER_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_FUSION_CONTAINER_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/void.hpp>
#include <boost/config.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/assign/v2/ref/wrapper/framework.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/ref/fusion/policy.hpp>
#include <boost/assign/v2/ref/fusion/link_holder.hpp>
#include <boost/assign/v2/ref/fusion/head_holder.hpp>

namespace boost{
	struct use_default;
namespace assign{
namespace v2{
namespace ref{
namespace fusion_aux{

    template<std::size_t N, typename L,typename Tag1, typename Tag2, typename T>
	struct interface : boost::mpl::apply4<
        fusion_aux::policy<Tag2>,
        boost::mpl::int_<N>, L, Tag1, T
    >{};

    template<
    	std::size_t N, typename L, typename Tag1, typename Tag2, typename T>
    class container 
    	: public fusion_aux::link_holder<L, N == 0>,
        public fusion_aux::head_holder<Tag1, T, N == 0>,	
        public fusion_aux::interface<N, L, Tag1, Tag2, T>::type
    {

    	typedef boost::mpl::int_<0> int0_;
        typedef boost::mpl::int_<1> int1_;
        typedef boost::mpl::int_<N> size_;
        typedef typename boost::mpl::minus<size_,int1_>::type index_;
        typedef Tag1 assign_tag_;

		typedef typename fusion_aux::interface<
        	N, 
            L, 
            Tag1, 
            Tag2, 
            T
        >::type super_t;

		typedef fusion_aux::link_holder<L, size_::value == 0> link_;
		typedef fusion_aux::head_holder<Tag1, T, size_::value == 0> head_;

        public:

		// -- used by most_const -- //
		typedef T head_value_type;
        typedef L link_value_type;

		typedef std::size_t size_type;
        BOOST_STATIC_CONSTANT(std::size_t, static_size = N);
        typedef typename boost::mpl::equal_to<size_,int0_>::type is_empty_;

        explicit container(){}
        explicit container(const L& l, T& h)
            : link_( l ),
            head_( h )
        {}

        template<typename NewL, typename NewTag2, typename U>
        explicit container(const container<N, NewL, Tag1, NewTag2, U>& that)
            :link_( that ), 
            head_( that ),
            super_t( that ){}

        // ------ operator() ----- //
		//     adds an element	   //
        // ----------------------- //

        template<typename T1>
        struct result{
            typedef container this_;
            typedef container<N+1, this_, Tag1, Tag2, T1> type;
        };

        template<typename T1>
        typename result<T1>::type
        operator()(T1& t)const{
        	typedef typename result<T1>::type result_;
        	return result_( *this, t );
        }

        template<typename T1>
        typename result<T1 const>::type
        operator()(T1 const& t)const{
        	typedef typename result<T1 const>::type result_;
        	return result_( *this, t );
        }

        // ---- static_lookup ---- //

        template<int I>
        struct is_head : boost::mpl::bool_< I + 1 == N >{};

        template<int I>
        struct link_result_static_lookup : L::template 
        	result_static_lookup<I>{};

		template<int I>
		struct result_static_lookup : boost::mpl::eval_if<
        	is_head<I>,
            boost::mpl::identity<T&>,
            link_result_static_lookup<I>
        >{};

		// A template is needed even as the argument is known, to prevent
        // compiler errors for N = 0.
        template<int I>
        typename boost::lazy_enable_if<
        	is_head<I>,
        	result_static_lookup<I>
        >::type
        static_lookup(boost::mpl::int_<I> same_index)const
        {
        	return this->head.get(); 
        }

        template<int I>
        typename boost::lazy_disable_if<
        	is_head<I>,
        	result_static_lookup<I>
        >::type
        static_lookup(boost::mpl::int_<I> smaller_index)const
        {
        	return this->link().static_lookup( smaller_index );
        }

    };

}// fusion_aux
}// ref
}// v2
}// assign
}// boost

#endif
