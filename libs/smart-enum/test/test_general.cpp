//
// Test program to verify smart_enum<> and friends.
//
// Copyright (c) 2002-2007 Peter Simons <simons@cryp.to>
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/static_assert.hpp>
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

template<typename enumT>
struct UncheckedEnumTraits
{
  typedef enumT enum_type;

  static enum_type cast(const int value)
  {
    return static_cast<enum_type>(value);
  }

  static void increment(enum_type& value, signed int offset)
  {
    value = static_cast<enum_type>(value + offset);
  }
};

template< typename EnumT
        , typename EnumTraits = UncheckedEnumTraits<EnumT>
        >
class smart_enum
{
public:
  typedef EnumT enum_type;
  typedef EnumTraits enum_traits;
  typedef smart_enum self_type;

  smart_enum(const enum_type value) : m_value(value)
  {
  }
  self_type& operator= (const enum_type value)
  {
    m_value = value;
    return *this;
  }

  smart_enum(const int value) : m_value(enum_traits::cast(value))
  {
  }
  self_type& operator= (const int value)
  {
    m_value = enum_traits::cast(value);
    return *this;
  }

  self_type& operator += (signed int offset)
  {
    enum_traits::increment(m_value, offset);
    return *this;
  }
  self_type& operator -= (signed int offset)
  {
    return (*this += (-offset));
  }

  operator const enum_type () const
  {
    return m_value;
  }

  friend self_type& operator++ (self_type& value)
  {
    return (value += 1);
  }
  friend self_type& operator-- (self_type& value)
  {
    return (value -= 1);
  }
  friend const self_type operator++ (self_type& value, int)
  {
    self_type tmp(value);
    value += 1;
    return tmp;
  }
  friend const self_type operator-- (self_type& value, int)
  {
    self_type tmp(value);
    value -= 1;
    return tmp;
  }
#if 0
  friend bool operator== (self_type& lhs, self_type& rhs)
  {
    return (lhs.m_value == rhs.m_value);
  }
  friend bool operator!= (self_type& lhs, self_type& rhs)
  {
    return (lhs.m_value != rhs.m_value);
  }
#endif
private:
  enum_type m_value;
};


enum myEnum { STATE1, STATE2, STATE3, STATE4 };

BOOST_STATIC_ASSERT(sizeof(smart_enum<myEnum>) == sizeof(myEnum));

BOOST_AUTO_TEST_CASE( test_state_enum )
{
  smart_enum<myEnum> state( 0 );
  BOOST_REQUIRE_EQUAL((state += 1), STATE2);
  BOOST_REQUIRE_EQUAL((++state), STATE3);
  BOOST_REQUIRE_EQUAL((--state), STATE2);
  BOOST_REQUIRE_EQUAL((state++), STATE2);
  BOOST_REQUIRE_EQUAL((state--), STATE3);
  BOOST_REQUIRE_EQUAL((state), STATE2);
  BOOST_REQUIRE_EQUAL((state -= 1), STATE1);

  state = STATE4;
  BOOST_REQUIRE_EQUAL(state, STATE4);

  smart_enum<myEnum> tmp( state );
  BOOST_REQUIRE_EQUAL(state, tmp);
  BOOST_REQUIRE(state < tmp+1);

  tmp = state - 1;
  BOOST_REQUIRE_EQUAL(tmp, STATE3);
}
