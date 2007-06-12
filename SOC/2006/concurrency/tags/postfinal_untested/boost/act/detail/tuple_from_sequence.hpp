//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_DETAIL_TUPLE_FROM_SEQUENCE_HPP
#define BOOST_ACT_DETAIL_TUPLE_FROM_SEQUENCE_HPP

#include <boost/mpl/at.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include "../config/max_active_call_params.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename Sequence
        , typename Enabler = void
        >
struct tuple_from_sequence;

#define BOOST_ACT_DETAIL_SEQUENCE_ELEM( z, element_index, sequence_name )      \
typename ::boost::mpl::at_c< sequence_name, element_index >::type 

#define BOOST_ACT_DETAIL_TUPLE_FROM_SEQUENCE_SPEC( z, sequence_size, dummy )   \
template< typename Sequence >                                                  \
struct tuple_from_sequence                                                     \
       <                                                                       \
         Sequence                                                              \
       , typename ::boost::enable_if                                           \
         <                                                                     \
           ::boost::mpl::equal_to                                              \
           <                                                                   \
             typename ::boost::mpl::size< Sequence >::type                     \
           , ::boost::mpl::size_t< sequence_size >                             \
           >                                                                   \
         >::type                                                               \
       >                                                                       \
{                                                                              \
  typedef ::boost::tuple                                                       \
          <                                                                    \
            BOOST_PP_CAT( BOOST_PP_ENUM_, z )( sequence_size                   \
                                             , BOOST_ACT_DETAIL_SEQUENCE_ELEM  \
                                             , Sequence                        \
                                             )                                 \
          >                                                                    \
          type;                                                                \
}; 

BOOST_PP_REPEAT( BOOST_ACT_MAX_PARAMS
               , BOOST_ACT_DETAIL_TUPLE_FROM_SEQUENCE_SPEC
               , BOOST_PP_NIL
               )

}
}
}

#endif
