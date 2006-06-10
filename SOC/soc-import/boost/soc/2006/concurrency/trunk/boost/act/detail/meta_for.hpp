//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_META_FOR_HPP
#define BOOST_ACT_DETAIL_META_FOR_HPP

#include <boost/mpl/apply.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename State
        , typename Predicate
        , typename Operation
        , typename Body
        >
struct meta_for;

template< typename State
        , typename Predicate
        , typename Operation
        , typename Body
        >
struct meta_for_impl
  : meta_for< typename ::boost::mpl::apply
              <
                Body
              , typename ::boost::mpl::apply< Operation, State >::type
              >
              ::type
            , Predicate
            , Operation
            , Body
            >
{
};

template< typename State
        , typename Predicate
        , typename Operation
        , typename Body
        >
struct meta_for
  : ::boost::mpl::if_< typename ::boost::mpl::apply< Predicate, State >::type
                     , meta_for_impl< State, Predicate, Operation, Body >
                     , ::boost::mpl::identity< State >
                     >
                     ::type
{
};

}
}
}

#endif
