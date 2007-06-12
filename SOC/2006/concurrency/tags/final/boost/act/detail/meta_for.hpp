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
