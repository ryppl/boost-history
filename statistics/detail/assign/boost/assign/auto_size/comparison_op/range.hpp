

//////////////////////////////////////////////////////////////////////////////
// assign::detail::comparison_op::range.hpp                                 //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_COMPARISON_OP_RANGE_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_COMPARISON_OP_RANGE_ER_2010_HPP
#include <boost/assign/auto_size/comparison_op/crtp.hpp>
#include <boost/assign/auto_size/comparison_op/completed_policy.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace range_comparison_op{

    // Let D1, D2 and R model Range, and for D in {D1,D2}, D derives from
    //    base_of<D>::type
    // then defines for Op in {==,!=,<,>,<=,>=}, 
    //    Op(D1&,D2&)
    //    Op(D1&,R&)
    //    Op(R, D2&)
    // etc.    
    struct fundamental_policy{
    
        template<typename L,typename R>
        static bool equal(const L& l,const R& r){
            return ::boost::iterator_range_detail::equal( l, r );
        }
        template<typename L,typename R>
        static bool less(const L& l,const R& r){
            return ::boost::iterator_range_detail::less_than( l, r );
        }

    };

    template<typename D>
    struct base_of{
        typedef comparison_op::crtp<
            D,
            comparison_op::completed_policy<
                range_comparison_op::fundamental_policy
            >
        > type;
    };

}// range_comparison_op
}// detail
}// assign
}// boost

#endif

