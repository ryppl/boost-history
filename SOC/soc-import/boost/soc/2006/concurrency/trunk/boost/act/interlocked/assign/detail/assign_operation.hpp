#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_OPERATION_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_OPERATION_HPP

namespace boost { namespace act { namespace interlocked { namespace detail {

struct assign_operation
{
  struct new_from_old
  {
    template< typename ValueType, typename OperandType >
    static ValueType execute( ValueType& value, OperandType& operand )
    {
      return operand;
    }
  };

  struct old_from_new
  {
    template< typename ValueType, typename OperandType >
    static ValueType execute( ValueType& value, OperandType& operand )
    {
      return operand;
    }
  };
};

} } } }

#endif
