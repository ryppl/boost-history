#ifndef BOOST_ACT_DETAIL_FUNCTION_PACKAGE_HPP
#define BOOST_ACT_DETAIL_FUNCTION_PACKAGE_HPP

#include "queued_function_caller.hpp"
#include "active_function_caller.hpp"
#include "params_to_stored_params.hpp"
#include "tuple_from_sequence.hpp"
#include <boost/mpl/empty.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename ResultType
        , typename ActModel
        , typename ParamSequence
        , typename FunctionType
        , typename TupleType
        >
class function_package
{
public:
  typedef ResultType result_type;
  typedef mpl::empty< ParamSequence > param_sequence_is_empty;
public:
  function_package( FunctionType const& function_init
                  , TupleType const& arguments_init
                  )
    : function_m( function_init )
    , arguments_m( arguments_init )
  {
  }
public:
  class stored_function_type
  {
  public:
    typedef ResultType result_type;
  private:
    typedef typename detail::params_to_stored_params
              < ActModel, ParamSequence >::type
                stored_params_sequence;

    typedef typename detail::tuple_from_sequence
              < stored_params_sequence >::type
                stored_params_type;
  public:
    stored_function_type( FunctionType const& function_init
                        , TupleType const& arguments_init
                        )
      : function_m( function_init )
      , arguments_m( arguments_init )
    {
    }
  public:
    result_type operator ()() const
    {
      return detail::active_function_caller< result_type >
               ( function_m, arguments_m );
    }

    template< typename TargetType >
    result_type operator ()( TargetType* const target ) const
    {
      return detail::queued_function_caller< result_type >
               ( target, function_m, arguments_m );
    }
  private:
    FunctionType function_m;
    stored_params_type arguments_m;
  };
public:
  stored_function_type store_function() const
  {
    return stored_function_type( function_m, arguments_m );
  }
private:
  FunctionType const& function_m;
  TupleType const& arguments_m;
};

template< typename ResultType
        , typename ActModel
        , typename ParamSequence
        , typename FunctionType
        , typename ArgumentsType
        >
function_package< ResultType, ActModel
                , ParamSequence, FunctionType, ArgumentsType
                >
make_function_package( FunctionType const& function
                     , ArgumentsType const& arguments
                     )
{
  return function_package< ResultType, ActModel
                         , ParamSequence, FunctionType, ArgumentsType
                         >( function, arguments );
}

}
}
}

#endif
