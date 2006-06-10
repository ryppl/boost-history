//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ACT_REF_HPP
#define BOOST_ACT_DETAIL_ACT_REF_HPP

namespace boost
{
namespace act
{
namespace detail
{

template< typename Type >
class ref_type
{
public:
  explicit ref_type( Type& ref_init )
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

template< typename Type >
struct remove_act_ref
{
  typedef Type type;
};

template< typename Type >
struct remove_act_ref< ref_type< Type > >
{
  typedef Type& type;
};

struct act_ref_type
{
  act_ref_type()
  {
  }

  act_ref_type operator ()() const
  {
    return *this;
  }

  template< typename Type >
  ref_type< Type > operator ()( Type& ref_init ) const
  {
    return ref_type< Type >( ref_init );
  }
};

struct act_cref_type
{
  act_cref_type()
  {
  }

  act_cref_type operator ()() const
  {
    return *this;
  }

  template< typename Type >
  ref_type< Type const > operator ()( Type const& ref_init ) const
  {
    return ref_type< Type const >( ref_init );
  }
};

}
}
}

#endif
