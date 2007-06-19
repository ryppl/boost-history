/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_INTERLOCKED_RESULT_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_INTERLOCKED_RESULT_HPP

#include <boost/act/detail/execute_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/assert.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

struct old_value;
struct new_value;
struct success_value;

struct interlocked_identity
{
  template< typename ValueType >
  static ValueType execute( ValueType const& value ) { return value; }

  template< typename ValueType, typename OperandType >
  static ValueType execute( ValueType const& value, OperandType const& )
  {
    return value;
  }
};

template< typename Operation, typename ValueInfo, typename ValueType >
class unary_interlocked_result
{
  BOOST_MPL_ASSERT_MSG( ( mpl::or_< is_same< ValueInfo, detail::old_value >
                                  , is_same< ValueInfo, detail::new_value >
                                  >
                                  ::value
                        )
                      , INVALID_VALUE_INFO_PASSED_TO_INTERLOCKED_RESULT
                      , ( ValueInfo )
                      );
private:
  typedef typename remove_volatile< ValueType >::type value_type;
public:
  template< typename Type >
  unary_interlocked_result( Type const& value_init )
    : value_m( value_init ) {}
public:
  value_type old_value() const
  {
    return act::detail::execute_if
           <
             is_same< ValueInfo, detail::old_value >
           , interlocked_identity
           , typename Operation::old_from_new
           >
           ( value_m );
  }

  value_type new_value() const
  {
    return act::detail::execute_if
           <
             is_same< ValueInfo, detail::old_value >
           , typename Operation::new_from_old
           , interlocked_identity
           >
           ( value_m );
  }
private:
  value_type value_m;
};

template< typename Operation, typename ValueType >
class unary_interlocked_result_returns_old
{
private:
  typedef typename remove_cv< ValueType >::type   value_type;
public:
  typedef unary_interlocked_result< Operation, detail::old_value
                                  , value_type
                                  >
                                  type;
};

template< typename Operation, typename ValueInfo
        , typename ValueType, typename OperandType = ValueType
        >
class binary_interlocked_result
{
  BOOST_MPL_ASSERT_MSG( ( mpl::or_< is_same< ValueInfo, detail::old_value >
                                  , is_same< ValueInfo, detail::new_value >
                                  >
                                  ::value
                        )
                      , INVALID_VALUE_INFO_PASSED_TO_INTERLOCKED_RESULT
                      , ( ValueInfo )
                      );
private:
  typedef typename remove_volatile< ValueType >::type   value_type;
  typedef typename remove_volatile< OperandType >::type operand_type;
public:
  template< typename Type0, typename Type1 >
  explicit binary_interlocked_result( Type0 const& value_init
                                    , Type1 const& operand_init
                                    )
    : value_m( value_init ), operand_m( operand_init ) {}
public:
  value_type old_value() const
  {
    return act::detail::execute_if
           <
             is_same< ValueInfo, detail::old_value >
           , interlocked_identity
           , typename Operation::old_from_new
           >
           ::execute( value_m, operand_m );
  }

  value_type new_value() const
  {
    return act::detail::execute_if
           <
             is_same< ValueInfo, detail::old_value >
           , typename Operation::new_from_old
           , interlocked_identity
           >
           ::execute( value_m, operand_m );
  }
private:
  value_type value_m;
  operand_type operand_m;
};

template< typename Operation
        , typename ValueType, typename OperandType = ValueType
        >
struct binary_interlocked_result_returns_old
{
private:
  typedef typename remove_cv< ValueType >::type   value_type;
  typedef typename remove_cv< OperandType >::type operand_type;
public:
  typedef binary_interlocked_result< Operation, detail::old_value
                                   , value_type, operand_type
                                   >
                                   type;
};

template< typename Operation, typename ValueInfo
        , typename ValueType, typename OperandType = ValueType
        >
struct binary_interlocked_result_returns_new
{
private:
  typedef typename remove_cv< ValueType >::type   value_type;
  typedef typename remove_cv< OperandType >::type operand_type;
public:
  typedef binary_interlocked_result< Operation, detail::new_value
                                   , value_type, operand_type
                                   >
                                   type;
};

template< typename ValueInfo, typename ValueType >
class assign_if_was_interlocked_result
{
  BOOST_MPL_ASSERT_MSG( ( mpl::or_< is_same< ValueInfo, detail::old_value >
                                  , is_same< ValueInfo, detail::success_value >
                                  >
                                  ::value
                        )
                      , INVALID_VALUE_INFO_PASSED_TO_INTERLOCKED_RESULT
                      , ( ValueInfo )
                      );
private:
  typedef typename remove_volatile< ValueType >::type value_type;
public:
  template< typename Type0, typename Type1, typename Type2 >
  explicit assign_if_was_interlocked_result
           ( Type0 const& result_init
           , Type1 const& attempted_new_value_init
           , Type2 const& expected_value_init
           )
    : result_m( result_init )
    , attempted_new_value_m( attempted_new_value_init )
    , expected_value_m( expected_value_init ) {}
public:
  value_type old_value() const
  {
    BOOST_MPL_ASSERT_MSG( ( is_same< ValueInfo, detail::old_value >::value )
                        , CANNOT_RETRIEVE_OLD_VALUE_ON_THIS_OPERATING_SYSTEM
                        , ()
                        );

    return result_m;
  }

  value_type new_value() const
  {
    BOOST_MPL_ASSERT_MSG( ( is_same< ValueInfo, detail::old_value >::value )
                        , CANNOT_RETRIEVE_NEW_VALUE_ON_THIS_OPERATING_SYSTEM
                        , ()
                        );

    // ToDo: Allow toggle for regular assert, not static assert

    return was_successful() ? attempted_new_value_m : result_m;
  }

  struct was_successful_checker
  {
    static bool execute( value_type const& result
                       , value_type const& expected_value
                       )
    {
      return result == expected_value;
    }
  };

  bool was_successful() const
  {
    return mpl::if_< is_same< ValueInfo, detail::old_value >
                   , was_successful_checker
                   , interlocked_identity
                   >
                   ::type::execute( result_m, expected_value_m );
  }
private:
  value_type result_m,
             attempted_new_value_m,
             expected_value_m;
};

template< typename ValueType >
class assign_if_was_interlocked_result_returns_old
{
private:
  typedef typename remove_cv< ValueType >::type   value_type;
public:
  typedef assign_if_was_interlocked_result< detail::old_value
                                          , value_type
                                          >
                                          type;
};

template< typename ValueType >
class assign_if_was_interlocked_result_returns_success
{
private:
  typedef typename remove_cv< ValueType >::type   value_type;
public:
  typedef assign_if_was_interlocked_result< detail::success_value
                                          , value_type
                                          >
                                          type;
};

} } } }

#endif
