//////////////////////////////////////////////////////////////////////////////
// assign::chain_auto_convert.hpp                                           //
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
#include <boost/assign/auto_size/chain/pair_range_traits.hpp>
#include <boost/assign/auto_size/chain/converter.hpp>

// Usage:
//    chain_auto_convert(r1,r2)
// returns a range that is formed by joining r1 and r2 whose reference is
// convertible from either of those of r1 and r2. 
//
// Requirements : if either of r1 or r1 contains reference wrappers (W) pointing 
// to some type T, 
//     is_ref_wrapper<W>::value                        true
//     ref_wrapper_traits::meta::value_of<W>::type     T

namespace boost{
namespace assign{
namespace result_of{
namespace chain_auto_convert{
    
    template<typename Conv,typename R1,typename R2>
    struct generic : detail::pair_range_traits::generic<Conv,R1,R2>{
        typedef detail::pair_range_traits::generic<Conv,R1,R2> super_;
        typedef typename super_::new_range1_ new_range1_;
        typedef typename super_::new_range2_ new_range2_;
        static new_range1_& new_range1;
        static new_range2_& new_range2;
        typedef BOOST_TYPEOF_TPL(
            boost::chain(new_range1,new_range2)
        ) type;

        // MSVC:
        //typedef BOOST_TYPEOF_NESTED_TYPEDEF_TPL(
        //    nested,
        //    boost::chain(conv_r1,conv_r2)
        //);
        //typedef typename nested::type type;

        static type call(R1& r1, R2& r2){
            super_::internal_check();
            typedef typename super_::caller1_ caller1_;
            typedef typename super_::caller2_ caller2_;
            new_range1_ nr1 = caller1_::call(r1);
            new_range2_ nr2 = caller2_::call(r2);
            return boost::chain(nr1,nr2);
            //boost::chain(caller1_::call(r1),caller2_::call(r2)); // Not!
        }
    };

    template<typename R1,typename R2>
    struct ignore_wrapper  
        : chain_auto_convert::generic<detail::pair_traits::meta::ignore_wrapper,R1,R2>
        {};

    template<typename R1,typename R2>
    struct convert_wrapper  
        : chain_auto_convert::generic<detail::pair_traits::meta::convert_wrapper,R1,R2>
        {};

}// chain_auto_convert
}// result_of

    // generic
    template<typename Conv,typename R1,typename R2> 
    typename result_of::chain_auto_convert::generic<Conv,R1,R2>::type
    chain_auto_convert(R1& r1, R2& r2){
        typedef result_of::chain_auto_convert::generic<Conv,R1,R2> caller_;
        return caller_::call(r1,r2);
    }

    template<typename Conv,typename R1,typename R2> 
    typename result_of::chain_auto_convert::generic<Conv,
        const R1,const R2>::type
    chain_auto_convert(const R1& r1, const R2& r2){
        return chain_auto_convert<Conv,const R1,const R2>(r1,r2);
    }

    // default uses convert_wrapper
    template<typename R1,typename R2> 
    typename result_of::chain_auto_convert::convert_wrapper<R1,R2>::type
    chain_auto_convert(R1& r1,R2& r2){
        typedef result_of::chain_auto_convert::convert_wrapper<R1,R2> caller_;
        return caller_::call(r1,r2);
    }

    template<typename R1,typename R2> 
    typename result_of::chain_auto_convert::convert_wrapper<
        const R1,const R2>::type
    chain_auto_convert(const R1& r1,const R2& r2){
        typedef result_of::chain_auto_convert::convert_wrapper<
            const R1,const R2> caller_;
        return caller_::call(r1,r2);
    }

}// assign
}// boost

#endif
