/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_DETAIL_GIVE_CV_TO_HPP
#define BOOST_ACT_DETAIL_GIVE_CV_TO_HPP

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_volatile.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_volatile.hpp>
#include <boost/type_traits/add_const.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename SourceType
        , typename TargetType
        >
struct give_cv_to
{
private:
  typedef typename mpl::eval_if< is_volatile< SourceType >
                               , add_volatile< TargetType >
                               , mpl::identity< TargetType >
                               >
                               ::type partially_qualified_type;
public:
  typedef typename mpl::eval_if
          <
            is_const< SourceType >
          , add_const< partially_qualified_type >
          , mpl::identity< partially_qualified_type >
          >
          ::type type;
};

}
}
}

#endif
