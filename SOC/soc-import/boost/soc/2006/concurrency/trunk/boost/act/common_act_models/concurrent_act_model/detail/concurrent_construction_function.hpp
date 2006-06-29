#ifndef BOOST_ACT_COMMON_ACT_MODELS_DETAIL_CONCURRENT_CONSTRUCTION_FUNCTION_HPP
#define BOOST_ACT_COMMON_ACT_MODELS_DETAIL_CONCURRENT_CONSTRUCTION_FUNCTION_HPP

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_void.hpp>

// ToDo: Don't include
#include "concurrent_action_internals.hpp"

namespace boost
{
namespace act
{
namespace detail
{


template< typename FunctionPackageType >
class concurrent_construction_function_base
{
public:
  concurrent_construction_function_base
  (
    FunctionPackageType const& function_init
  )
    : function_m( function_init.store_function() )
  {
  }
public:
  void operator ()() const
  {
    function_m();
  }

  template< typename TargetType >
  void operator ()( TargetType* const target ) const
  {
    function_m( target );
  }
private:
  typedef typename FunctionPackageType::stored_function_type
            stored_function_type;
private:
  mutable stored_function_type function_m; // ToDo: Possibly remove mutable
};


template< typename FunctionPackageType >
class concurrent_construction_function
{
private:
  typedef concurrent_construction_function_base< FunctionPackageType >
            function_type_base;
private:
  class function_type
    : public function_type_base // ToDo: Change to private
  {
  public:
    function_type( FunctionPackageType const& function_init )
      : function_type_base( function_init )
    {
    }
  public:
    using function_type_base::operator ();
  };
public:
  concurrent_construction_function( FunctionPackageType const& function_init )
    : function_m( new function_type( function_init ) )
  {
  }
public:
  void operator ()() const
  {
    (*function_m)();
  }

  template< typename TargetType >
  void operator ()( TargetType* const target ) const
  {
    (*function_m)( target );
  }
private:
  shared_ptr< function_type > function_m;
};

template< typename FunctionPackageType >
concurrent_construction_function< FunctionPackageType >
make_concurrent_construction_function( FunctionPackageType const& function )
{
  return concurrent_construction_function< FunctionPackageType >( function );
}



/////////////////////////////////////////////



template< typename ObjectType, typename FunctionPackageType >
class concurrent_action_construction_function_base
{
private:
  typedef concurrent_action_internals< ObjectType > internals_type;
  typedef shared_ptr< internals_type > shared_internals_type;
public:
  concurrent_action_construction_function_base
  (
    shared_ptr< internals_type > const& concurrent_action_internals_init
  , FunctionPackageType const& function_init
  )
    : action_internals_m( concurrent_action_internals_init )
    , function_m( function_init.store_function() )
  {
  }
public:
  void operator ()() const
  {
    function_m();

    update_internals();
  }

  template< typename TargetType >
  void operator ()( TargetType* const target ) const
  {
    function_m( target );

    update_internals();
  }
private:
  void update_internals() const
  {
    shared_internals_type const internals( action_internals_m );

    if( internals )
      internals->completion_trigger = true;
  }
private:
  typedef typename FunctionPackageType::stored_function_type
            stored_function_type;
private:
  weak_ptr< internals_type > const action_internals_m;
  mutable stored_function_type function_m; // ToDo: Possibly remove mutable
};


template< typename ObjectType, typename FunctionPackageType >
class concurrent_action_construction_function
{
private:
  typedef concurrent_action_internals< ObjectType > internals_type;

  typedef shared_ptr< internals_type > shared_internals_type;

  typedef concurrent_action_construction_function_base
               < ObjectType, FunctionPackageType > function_type_base;
private:
  class function_type
    : public function_type_base // ToDo: Change to private
  {
  public:
    function_type( typename concurrent_action_construction_function
                     ::shared_internals_type
                       const& concurrent_action_internals_init
                 , FunctionPackageType const& function_init
                 )
      : function_type_base( concurrent_action_internals_init
                          , function_init
                          )
    {
    }
  public:
    using function_type_base::operator ();
  };
public:
  concurrent_action_construction_function
  ( shared_internals_type const& concurrent_action_internals_init
  , FunctionPackageType const& function_init
  )
    : function_m( new function_type( concurrent_action_internals_init
                                   , function_init
                                   )
                )
  {
  }
public:
  void operator ()() const
  {
    (*function_m)();
  }

  template< typename TargetType >
  void operator ()( TargetType* const target ) const
  {
    (*function_m)( target );
  }
private:
  shared_ptr< function_type > function_m;
};

template< typename ObjectType, typename FunctionPackageType >
concurrent_action_construction_function< ObjectType, FunctionPackageType >
make_concurrent_action_construction_function
(
  shared_ptr
  <
    concurrent_action_internals< ObjectType >
  >
  const& internals
, FunctionPackageType const& function
)
{
  return concurrent_action_construction_function< ObjectType
                                         , FunctionPackageType
                                         >( internals
                                          , function
                                          );
}

}
}
}

#endif
