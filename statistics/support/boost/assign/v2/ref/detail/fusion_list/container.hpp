//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_FUSION_LIST_CONTAINER_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_FUSION_LIST_CONTAINER_ER_2010_HPP
#include <boost/mpl/int.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/config.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/assign/v2/ref/wrapper/framework.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

namespace boost{
	struct use_default;
namespace assign{ 
namespace v2{
namespace ref{
namespace fusion_list_aux{

    template<std::size_t N, typename L, typename Tag,typename T>
    class container
    {
    
    	typedef boost::mpl::int_<0> int0_;
        typedef boost::mpl::int_<1> int1_;
        typedef boost::mpl::int_<N> size_;
        typedef typename boost::mpl::minus<size_,int1_>::type index_;
        typedef Tag assign_tag_;
        typedef typename ref::wrapper<assign_tag_,T> wrapper_;

        typedef assign_tag::copy link_tag_;

        public:       

		typedef std::size_t size_type;
        BOOST_STATIC_CONSTANT(bool, static_sisze = N);
        typedef typename boost::mpl::equal_to<size_,int0_>::type is_empty_;
        typedef typename boost::mpl::if_<
        	is_empty_, L, ref::wrapper<link_tag_, const L>
        >::type link_;
        typedef typename boost::mpl::if_<is_empty_, L, wrapper_>::type head_;

        explicit container(){} 
        explicit container(const L& l,T& h)
        	:link( l ), 
            head( h ) 
        {} 

        // ------ operator() ----- //
		//     adds an element	   //
        // ----------------------- //

        template<typename T1>
        struct result{
            typedef container this_;
            typedef container<N+1,this_,Tag,T1> type;
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

        // ------ operator[] ----- //
		//   retrieves an element  //
        // ----------------------- //

        template<int I>
        struct is_head : boost::mpl::bool_< I + 1 == N >{};

        template<int I>
        struct link_retrieve : L::template retrieve<I>{};

		template<int I>
		struct retrieve : boost::mpl::eval_if<
        	is_head<I>,
            boost::mpl::identity<T&>, 
            link_retrieve<I>
        >{};

		// A template is needed even as the argument is known, to prevent 
        // compiler errors for N = 0.
        template<int I>
        typename boost::lazy_enable_if<
        	is_head<I>,
        	retrieve<I>
        >::type
        static_lookup(boost::mpl::int_<I> same_index)const
        { 
        	return this->head; 
        }

        template<int I>
        typename boost::lazy_disable_if<
        	is_head<I>,
        	retrieve<I>
        >::type
        static_lookup(boost::mpl::int_<I> smaller_index)const
        { 
        	return this->link.unwrap().static_lookup( smaller_index ); 
        }
        
        link_ pop()const
        { 
        	// Warning : probably inefficient
        	return link; 
        }
        T& back()const{ return this->head; }
        
        protected:
        mutable link_ link; // This is a wrappe
        mutable head_ head;

    };

}// fusion_list_aux
}// ref
}// v2
}// assign
}// boost

#endif
