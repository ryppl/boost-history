//////////////////////////////////////////////////////////////////////////////
// assign::detail::pair_range_traits.hpp                                    //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_PAIR_RANGE_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_PAIR_RANGE_TRAITS_ER_2010_HPP
#include <boost/range.hpp>
#include <boost/assign/auto_size/chain/pair_traits.hpp>
#include <boost/assign/auto_size/chain/converter.hpp>
#include <boost/assign/auto_size/chain/is_reference_wrapper.hpp>

// Maps (R1,R2) to a reference that is convertible to by each elem of either. 

namespace boost{
namespace assign{
namespace detail{
namespace pair_range_traits{

    template<typename Conv,typename R1,typename R2>
    struct generic{
        typedef typename boost::range_reference<R1>::type ref1_;
        typedef typename boost::range_reference<R2>::type ref2_;
        typedef typename 
            Conv::template apply<ref1_,ref2_>::type convertible_to_;

        typedef detail::result_of::convert_range<convertible_to_,R1> caller1_;
        typedef detail::result_of::convert_range<convertible_to_,R2> caller2_;
        
        typedef typename caller1_::type new_range1_;
        typedef typename caller2_::type new_range2_;
        
        static void internal_check(){
            caller1_::internal_check();
            caller1_::internal_check();
        }

        struct convertible_to_traits{
            typedef typename boost::remove_reference<convertible_to_>::type c_;
            typedef typename boost::remove_const<c_>::type val_;
            typedef detail::is_ref_wrapper<val_> is_wrapper;
            
            // Expected : (!is_wrapper::value), if Conv = convert_wrapper,
            // usually,  but not necessarilly, for ex if a ref-wrapper point to 
            // another ref-wrapper.
        };
        
    };

    template<typename R1,typename R2>
    struct ignore_wrapper 
     : pair_range_traits::generic<pair_traits::meta::ignore_wrapper,R1,R2>
     {};

    template<typename R1,typename R2>
    struct convert_wrapper 
     : pair_range_traits::generic<pair_traits::meta::convert_wrapper,R1,R2>
     {};

}// pair_range_traits
}// detail
}// assign
}// boost

#endif
