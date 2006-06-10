//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_AS_FUTURE_IMPL_HPP
#define BOOST_ACT_DETAIL_AS_FUTURE_IMPL_HPP

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Come up with a way to get a raw reference to an action's contained type
//       in order to prevent an extra copy construction when used as a future.
//       Also, eliminate all copies for immediate-style models.
template< typename Type >
class future_type
{
public:
  explicit future_type( Type& ref_init )
    : ref_m( ref_init )
  {
  }
public:
  operator Type&() const
  {
    return ref_m;
  }
private:
  Type& ref_m;
};

template< typename Type, typename ActModel >
struct remove_future_type
{
  typedef Type type;
};

// ToDo: Change to add scoped action
template< typename Type, typename ActModel >
struct remove_future_type< future_type< Type > >
{
  typedef typename add_action< Type, ActModel >::type type;
};

struct as_future_type
{
  as_future_type()
  {
  }

  as_future_type operator ()() const
  {
    return *this;
  }

  template< typename Type >
  future_type< Type > operator ()( Type& ref_init ) const
  {
    return future_type< Type >( ref_init );
  }
};

}
}
}

#endif
