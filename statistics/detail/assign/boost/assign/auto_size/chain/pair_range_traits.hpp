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
#include <boost/assign/auto_size/chain/convert_range.hpp>

// Maps (R1,R2) to a type convertible to by either of the dereference type of
// of the input ranges. 

namespace boost{
namespace assign{
namespace detail{
namespace pair_range_traits{

    template<typename Conv,typename R1,typename R2>
    class generic{
        typedef typename boost::range_reference<R1>::type ref1_;
        typedef typename boost::range_reference<R2>::type ref2_;
        
        template<typename X1,typename X2>
        struct helper : Conv::template apply<X1,X2>{};

        public:
        typedef typename helper<ref1_,ref2_>::type convertible_to_;
        typedef detail::result_of::convert_range<R1,convertible_to_> caller1_;
        typedef detail::result_of::convert_range<R2,convertible_to_> caller2_;
        
        typedef typename caller1_::type new_range1_;
        typedef typename caller2_::type new_range2_;
        
        static void internal_check(){
            caller1_::internal_check();
            caller1_::internal_check();
        }
        
    };

    template<typename R1,typename R2>
    struct ignore_wrapper 
     : pair_range_traits::generic<pair_traits::meta::ignore_wrapper,R1,R2>
     {};

    template<typename R1,typename R2>
    struct apply_conversion 
     : pair_range_traits::generic<pair_traits::meta::apply_conversion,R1,R2>
     {};

}// pair_range_traits
}// detail
}// assign
}// boost

#endif
