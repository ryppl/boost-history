//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

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

BOOST_PP_REPEAT( BOOST_ACT_MAX_ACTIVE_CALL_PARAMS
               , BOOST_ACT_DETAIL_TUPLE_FROM_SEQUENCE_SPEC
               , BOOST_PP_NIL
               )

}
}
}

#endif
