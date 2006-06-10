#ifndef SURGE_ACT_MAKE_ACTIVE_FUNCTION_HPP
#define SURGE_ACT_MAKE_ACTIVE_FUNCTION_HPP

namespace surge
{
  template< typename LogicalFuncType
          , typename ThreadAllocator
          , typename FunctionObjectType
          >
  active_function< LogicalFuncType
                 , FunctionObjectType
                 , ThreadAllocator
                 >
  make_active_function( FunctionObjectType function )
  {
    return active_function< LogicalFuncType
                          , FunctionObjectType
                          , ThreadAllocator
                          >
                          ( function );
  };
}

#endif
