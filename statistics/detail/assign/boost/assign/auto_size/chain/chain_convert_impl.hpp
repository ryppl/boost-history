//////////////////////////////////////////////////////////////////////////////
// assign::detail::chain_convert_impl.hpp                                   //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_CHAIN_CONVERT_IMPL_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_CHAIN_CONVERT_IMPL_ER_2010_HPP
#include <boost/range/chain.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/auto_size/chain/pair_range_traits.hpp>

namespace boost{
namespace assign{

namespace result_of{

    template<typename Conv,typename R1,typename R2>
    struct chain_convert_impl{
        typedef detail::pair_range_traits::generic<Conv,R1,R2> traits_;
        typedef typename traits_::new_range1_ new_range1_;
        typedef typename traits_::new_range2_ new_range2_;
        static const new_range1_& new_range1;
        static const new_range2_& new_range2;

        typedef BOOST_TYPEOF_TPL( boost::chain(new_range1,new_range2) ) type;
        
        static type call(R1& r1,R2& r2)
        {
            traits_::internal_check();
            typedef typename traits_::caller1_ caller1_;
            typedef typename traits_::caller2_ caller2_;
            return boost::chain(caller1_::call(r1),caller2_::call(r2));
        }
    };
    
}// result_of

template<typename Conv,typename R1,typename R2>
typename result_of::chain_convert_impl<Conv,R1,R2>::type
chain_convert_impl(R1& r1, R2& r2){
    typedef result_of::chain_convert_impl<Conv,R1,R2> caller_;
    return caller_::call(r1,r2);
}
template<typename Conv,typename R1,typename R2>
typename result_of::chain_convert_impl<Conv,const R1,const R2>::type
chain_convert_impl(const R1& r1, const R2& r2){
    typedef result_of::chain_convert_impl<Conv,const R1,const R2> caller_;
    return caller_::call(r1,r2);
}

}// assign
}// boost

#endif
 
