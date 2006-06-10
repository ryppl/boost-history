//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

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
  typedef typename ::boost::mpl::eval_if< ::boost::is_volatile< SourceType >
                                        , ::boost::add_volatile< TargetType >
                                        , ::boost::mpl::identity< TargetType >
                                        >
                                        ::type partially_qualified_type;
public:
  typedef typename ::boost::mpl::eval_if
          <
            ::boost::is_const< SourceType >
          , ::boost::add_const< partially_qualified_type >
          , ::boost::mpl::identity< partially_qualified_type >
          >
          ::type type;
};

}
}
}

#endif
