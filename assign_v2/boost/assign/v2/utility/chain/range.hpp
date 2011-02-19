//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_RANGE_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHAIN_RANGE_ER_2010_HPP
#include <boost/concept/assert.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/iterator.hpp>
#include <boost/assign/v2/utility/chain/iterator.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace chain_aux{

    template<typename R1, typename R2, template<typename> class F, typename Tag>
    struct range{

        typedef typename chain_aux::iterator<
            typename F<R1>::type,
            typename F<R2>::type,
            Tag
        >::type iterator_t;
        typedef boost::iterator_range< iterator_t > type;

        static type call(R1& r1, R2& r2)
        {
            BOOST_CONCEPT_ASSERT((SinglePassRangeConcept<R1>));
            BOOST_CONCEPT_ASSERT((SinglePassRangeConcept<R2>));
            namespace ns = boost::range_detail;
//            typedef ns::chain_iterator_begin_tag begin_tag_;
//            typedef ns::chain_iterator_end_tag end_tag_;
            typedef ns::join_iterator_begin_tag begin_tag_;
            typedef ns::join_iterator_end_tag end_tag_;
            return type(
                iterator_t(r1, r2, begin_tag_()),
                iterator_t(r1, r2, end_tag_())
            );
        }
    };

    template<typename R1, typename R2, typename Tag>
    struct range_l
        : chain_aux::range< R1, R2, boost::range_iterator,Tag>{};

    template<typename R1, typename R2, typename Tag>
    struct range_r
        : chain_aux::range<R1, R2, boost::range_const_iterator,Tag>{};

}// chain_aux
}// v2
}// assign
}// boost

#endif
