///////////////////////////////////////////////////////////////////////////////
// binary_op::algorithm::sort_on_head.hpp                                    //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_BINARY_OP_ALGORITHM_SORT_ON_HEAD_HPP_ER_2009
#define BOOST_BINARY_OP_ALGORITHM_SORT_ON_HEAD_HPP_ER_2009
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <boost/function.hpp>
#include <boost/lambda/construct.hpp>
#include <boost/bind.hpp>
#include <boost/range.hpp>
#include <boost/iterator/iterator_traits.hpp>

namespace boost{
namespace binary_op{

       // TODO : sort_on_element<i=0,1> and sort_on_head or tail .

      // sort_on_head<P>(s1,s2) reorders s1 based on the ordering 
      // predicate P, and applies the same rearrangement of elements
      // to s2.
      template<
        typename It,
        typename It1,
        template<typename> class Pred
    >
      void
      sort_on_head(
        It b,
        It e,
        It1 b_1
      );
      
      template<typename It,typename It1>
      void sort_on_head_greater(
            It b,
            It e,
            It1 b1
      );



    // Implementation //
      
    template<
        typename It,
        typename It1,
        template<typename> class Pred
    >
    void sort_on_head(
        It b,
        It e,
        It1 b_1
      )
      {
            typedef typename iterator_value<It>::type   val_t;
            typedef typename iterator_value<It1>::type  val1_t;
            typedef std::pair<val_t,val1_t>             pair_t;
            typedef std::vector<pair_t>                 pairs_t;
            typedef typename range_size<pairs_t>::type size_type;

            pairs_t pairs;
            pairs.reserve(
                static_cast<size_type>(
                    std::distance(b,e)
                )
            );
            std::transform(
                b,
                e,
                b_1,
                back_inserter(pairs),
                bind<pair_t>(
                    boost::lambda::constructor<pair_t>(),
                    _1,
                    _2
                )
            );

            function<val1_t(pair_t)> first = &pair_t::first;

            sort(
                begin(pairs),
                end(pairs),
                bind<bool>(
                    Pred<val1_t>(),
                    bind<val1_t>(
                        first,
                        _1
                    ),
                    bind<val1_t>(
                        first,
                        _2
                    )
                )
            );
            transform(
                begin(pairs),
                end(pairs),
                b,
                bind<val1_t>(
                    &pair_t::first,
                    _1
                )
            );
            std::transform(
                boost::begin(pairs),
                boost::end(pairs),
                b_1,
                bind<val1_t>(
                    &pair_t::second,
                    _1
                )
            );
      }

      template<typename It,typename It1>
      void sort_on_head_greater(
            It b,
            It e,
            It1 b1
      ){
        return sort_on_head<It,It1,std::greater>(
            b,
            e,
            b1
        );
      }
}// algorithm
}// boost

#endif