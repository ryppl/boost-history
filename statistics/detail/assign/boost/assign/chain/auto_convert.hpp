//////////////////////////////////////////////////////////////////////////////
// assign::detail::chain_auto_convert.hpp                                        //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_CHAIN_AUTO_CONVERT_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_CHAIN_AUTO_CONVERT_ER_2010_HPP
#include <boost/typeof/typeof.hpp>
#include <boost/range.hpp>
#include <boost/range/chain.hpp>
#include <boost/assign/chain/pair_traits.hpp>
#include <boost/assign/chain/converter.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace result_of{
namespace chain_auto_convert{
    
    template<typename Conv,typename R1,typename R2>
    struct generic{
        typedef typename boost::range_reference<R1>::type r1_;
        typedef typename boost::range_reference<R2>::type r2_;
        typedef typename Conv::template apply<r1_,r2_>::type to_;
        typedef result_of::convert_range<to_,R1> caller1_;
        typedef result_of::convert_range<to_,R2> caller2_;
        typedef typename caller1_::type conv_r1_;
        typedef typename caller2_::type conv_r2_;
		static conv_r1_& conv_r1;
		static conv_r2_& conv_r2;
        typedef BOOST_TYPEOF_TPL(
        	boost::chain(
        		conv_r1,
            	conv_r2
        	)
        ) type;

        // MSVC:
        //typedef BOOST_TYPEOF_NESTED_TYPEDEF_TPL(
        //    nested,
        //    boost::chain(conv1,conv22)
        //);
        //typedef typename nested::type type;

        typedef detail::functional::converter<to_> converter_;
        static type call(R1& r1, R2& r2){
            conv_r1_ conv_r1 = caller1_::call(r1);
            conv_r2_ conv_r2 = caller2_::call(r2);
            return boost::chain(conv_r1,conv_r2);
            // boost::chain(caller1_::call(r1),caller2_::call(r2)); // Not!
        }
    };

    template<typename R1,typename R2>
    struct ignore_wrapper  
        : chain_auto_convert::generic<pair_traits::meta::ignore_wrapper,R1,R2>
        {};

    template<typename R1,typename R2>
    struct filter_wrapper  
        : chain_auto_convert::generic<pair_traits::meta::filter_wrapper,R1,R2>
        {};

}// chain_auto_convert
}// result_of

    // generic
    template<typename Conv,typename R1,typename R2> 
    typename result_of::chain_auto_convert::generic<Conv,R1,R2>::type
    chain_auto_convert(R1& r1, R2& r2){
        typedef result_of::chain_auto_convert::generic<Conv,R1,R2> g_;
        return g_::call(r1,r2);
    }

    // ignore_wrapper
    template<typename R1,typename R2> 
    typename result_of::chain_auto_convert::ignore_wrapper<R1,R2>::type
    chain_auto_convert_ignore_wrapper(R1& r1,R2& r2){
        typedef result_of::chain_auto_convert::ignore_wrapper<R1,R2> g_;
        return g_::call(r1,r2);
    }

    // filter_wrapper
    template<typename R1,typename R2> 
    typename result_of::chain_auto_convert::filter_wrapper<R1,R2>::type
    chain_auto_convert_filter_wrapper(R1& r1,R2& r2){
        typedef result_of::chain_auto_convert::filter_wrapper<R1,R2> g_;
        return g_::call(r1,r2);
    }

    // default
    template<typename R1,typename R2> 
    typename result_of::chain_auto_convert::filter_wrapper<R1,R2>::type
    chain_auto_convert(R1& r1,R2& r2){
        typedef result_of::chain_auto_convert::filter_wrapper<R1,R2> g_;
        return g_::call(r1,r2);
    }

}// detail
}// assign
}// boost

#endif
