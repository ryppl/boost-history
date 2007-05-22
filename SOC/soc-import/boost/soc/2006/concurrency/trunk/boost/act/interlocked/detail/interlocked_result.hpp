#ifndef BOOST_ACT_INTERLOCKED_DETAIL_INTERLOCKED_RESULT_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_INTERLOCKED_RESULT_HPP

#include <boost/act/detail/execute_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/assert.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

struct old_value;
struct new_value;

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
  BOOST_MPL_ASSERT_MSG( ( mpl::or_< is_same< ValueInfo, old_value >
                                  , is_same< ValueInfo, new_value >
                                  >
                                  ::value
                        )
                      , INVALID_VALUE_INFO_PASSED_TO_INTERLOCKED_RESULT
                      , ( ValueInfo )
                      );
private:
  typedef typename remove_volatile< ValueType >::type value_type;
public:
  explicit unary_interlocked_result( value_type const& value_init )
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

template< typename Operation, typename ValueInfo
        , typename ValueType, typename OperandType
        >
class binary_interlocked_result
{
  BOOST_MPL_ASSERT_MSG( ( mpl::or_< is_same< ValueInfo, old_value >
                                  , is_same< ValueInfo, new_value >
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
  explicit binary_interlocked_result( value_type const& value_init
                                    , operand_type const& operand_init
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

template< typename Operation, typename ValueInfo, typename ValueType >
unary_interlocked_result< Operation, ValueInfo, ValueType >
make_interlocked_result( ValueType const& value )
{
  return unary_interlocked_result< Operation, ValueInfo, ValueType >( value );
}

template< typename Operation, typename ValueInfo
        , typename ValueType, typename OperandType
        >
binary_interlocked_result< Operation, ValueInfo, ValueType, OperandType >
make_interlocked_result( ValueType const& value, OperandType const& operand )
{
  return binary_interlocked_result< Operation, ValueInfo
                                  , ValueType, OperandType
                                  >
                                  ( value, operand );
}

} } } }

#endif
