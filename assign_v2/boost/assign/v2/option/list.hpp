//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_OPTION_LIST_ER_2010_HPP
#define BOOST_ASSIGN_V2_OPTION_LIST_ER_2010_HPP
#include <boost/assign/v2/detail/keyword.hpp>
#include <boost/assign/v2/interpreter/crtp.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

    template<typename Lhs, typename Option>
    struct modulo_result
    {
        static Lhs lhs;
        static Option option;

        #ifdef BOOST_MSVC
        BOOST_TYPEOF_NESTED_TYPEDEF_TPL(nested, lhs % option )
        typedef typename nested::type type;
		#else        
        typedef BOOST_TYPEOF_TPL( lhs % option ) type;
		#endif
    };

	template<typename Head>
    struct option_list_exit 
    	: boost::is_same<Head, keyword_aux::nil>
    {};

namespace result_of{

    template<
    	typename Head, 
        typename Tail, 
        typename Lhs, 
        bool exit = option_list_exit<Head>::value
    >
    struct apply_list_option
    {
    	typedef typename apply_list_option<
        	typename Tail::head_type, typename Tail::tail_type, Lhs
        >::type lhs_;
        
        typedef typename modulo_result<lhs_, Head>::type type;
    };

    template<
    	typename Head, 
        typename Tail, 
        typename Lhs
    >
    struct apply_list_option<Head, Tail, Lhs, true>
    {
		typedef Lhs type;    
    };
    
}// result_of
    
	template<
    	typename Head = keyword_aux::nil, 
    	typename Tail = boost::mpl::empty_base, 
        bool exit = option_list_exit<Head>::value
    >
    struct list_option : public Tail
    {

		typedef Head head_type;
        typedef Tail tail_type;

		list_option(){ /*exit = true*/ }
		list_option(Tail tail, Head h)
        	: Tail( tail ), head_( h )
    	{}
    
    	template<typename Option>
        struct result
        {
        	typedef list_option<Option, list_option> type;
        };
    
    	template<typename Option>
        typename result<Option>::type
    	operator%(Option option)const
        {
        	typedef typename result<Option>::type result_;
            return result_( *this, option );
        }
                        
        template<typename Lhs>
        typename boost::lazy_disable_if_c<
        	exit, result_of::apply_list_option<Head, Tail, Lhs> 
        >::type
        apply(Lhs const& lhs)const
        {
        	return static_cast<
            	Tail const&
            >( *this ).apply( lhs ) % this->head_;
        }

        template<typename Lhs>
        typename boost::lazy_enable_if_c<
        	exit, result_of::apply_list_option<Head, Tail, Lhs> 
        >::type
        apply(Lhs const& lhs)const{ return lhs; }
        
        private:
        Head head_;
    
    };

	typedef list_option<> empty_list_option;

    template<
    	typename C, typename F, typename ModifierTag, 
    	typename DataTag, typename D,
        typename H, typename T
    >
    typename result_of::apply_list_option<H, T, D>::type
    operator%(
        interpreter_crtp<C, F, ModifierTag, DataTag, D> const& lhs,
        list_option<H, T> const& list
    )
    {
		return list.apply( lhs );
	}
    
}// interpreter_aux
namespace{
	interpreter_aux::empty_list_option _list_option
    	= interpreter_aux::empty_list_option();
}
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_OPTION_LIST_ER_2010_HPP
