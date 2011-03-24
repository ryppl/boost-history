//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_META_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHAIN_META_ER_2010_HPP
#include <boost/assign/v2/chain/iterator.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/config.hpp>
#include <boost/concept/assert.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/reference.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
//[syntax_chain_meta
namespace chain_aux{

//<-
    template<typename R1, typename R2, template<typename> class F>
    struct range{

        typedef typename chain_aux::iterator<
            typename F<R1>::type,
            typename F<R2>::type
        >::type iterator_t;
        typedef boost::iterator_range< iterator_t > type;

        static type call(R1& r1, R2& r2)
        {
            BOOST_CONCEPT_ASSERT((SinglePassRangeConcept<R1>));
            BOOST_CONCEPT_ASSERT((SinglePassRangeConcept<R2>));
            namespace ns = boost::range_detail;
            typedef ns::join_iterator_begin_tag begin_tag_;
            typedef ns::join_iterator_end_tag end_tag_;
            return type(
                iterator_t(r1, r2, begin_tag_()),
                iterator_t(r1, r2, end_tag_())
            );
        }
    };

    template<typename R1, typename R2>
    struct range_l
        : chain_aux::range< R1, R2, boost::range_iterator>{};

    template<typename R1, typename R2>
    struct range_r
        : chain_aux::range<R1, R2, boost::range_const_iterator>{};

    template<typename U1, typename U2,
        bool is_r = boost::is_reference<U1>::value,
        bool is_c = boost::is_const<
            typename boost::remove_reference<U1>::type
        >::value
    >
    struct dont_use_const_impl : ::boost::mpl::false_{};

    template<typename U>
    struct dont_use_const_impl<U, U, true, false> : ::boost::mpl::true_{};

    template<typename R1, typename R2>
    struct dont_use_const : dont_use_const_impl<
        typename boost::range_reference<R1>::type,
        typename boost::range_reference<R2>::type
    >{};

//->

    template<
        typename R1        // Range
        , typename R2    // Range
    >
    struct use_const/*<-*/ : ::boost::mpl::bool_<
        !dont_use_const<R1, R2>::value
    >{}/*->*/;
    
namespace result_of{

    template<typename R1, typename R2>
    struct chain/*<-*/
    {
        typedef typename  ::boost::mpl::eval_if<
            chain_aux::use_const<R1, R2>,
            ::boost::mpl::identity< chain_aux::range_r<R1, R2> >,
            ::boost::mpl::identity< chain_aux::range_l<R1, R2> >
        >::type caller_;

        typedef typename caller_::type type;

        static type call(R1& r1, R2& r2)
        {
            return caller_::call( r1, r2 );
        }
    }/*->*/;
    
}// result_of
}// chain_aux
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_CHAIN_RESULT_ER_2010_HPP
