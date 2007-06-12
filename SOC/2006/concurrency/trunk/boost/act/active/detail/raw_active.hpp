/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_DETAIL_RAW_ACTIVE_HPP
#define BOOST_ACT_DETAIL_RAW_ACTIVE_HPP

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#include <boost/mpl/deque.hpp>

#include <boost/act/active_interface.hpp>

//#include <boost/act/active/detail/active_helper.hpp>

#include <boost/act/active/detail/action_return_meta.hpp>

#include <boost/act/config/max_params.hpp>

#include <boost/act/detail/function_package.hpp>

#include <boost/act/active/detail/constructor_caller.hpp>

#include <boost/act/detail/tie.hpp>

namespace boost { namespace act { namespace detail {

template< typename UnqualifiedType
        , typename ConsumerPolicy
        , bool is_safe
        , bool is_scoped
        >
struct raw_active_consumer_from_traits
{
private:
  // ToDo: Make version which unwraps references on half-bound functions
  class call_function_on_object
  {
  public:
    template< typename FunctionType >
    struct result
      : result_of< typename function_traits< FunctionType >::type, arg1_type >
        {};

    template< typename FunctionType > 
    typename result< call_function_by_index( FunctionType ) >::type
    operator ()( FunctionType& function ) const
    {
      return function( object_m );
    }
  private:
    // Todo: Use raw storage to make construction and destruction via calls
    value_type object_m;
  };

  struct normalize_to_variant
  {
    typedef typename make_variant_over
            < bound_function_type_sequence >
            ::type result_type;

    template< typename ParamType >
    result_type operator ()( ParamType const& arg ) const
    {
      // ToDo: Convert half bound arguments to bound arguments

      return result;
    }
  };

  struct denormalize_from_variant
  {
    template< typename ParamType, typename VisitorType >
    void operator ()( ParamType& arg, VisitorType& visitor ) const
    {
      boost::apply_visitor( arg, visitor );
    }
  };
public:
  // ToDo: Make normalization elidible (optimizes non-multi-threaded calls)
  typedef typename boost::consumer_chooser
          < consumer_policy
          , is_safe, is_scoped
          , constraint< is_one_of_over< half_bound_function_type_sequence > >
          , processor< call_function_on_object >
          , normalizer< normalize_to_variant, denormalize_from_variant >
          >
          ::type type;
};

// ToDo: Error if Type is qualified
template< typename UnqualifiedType
        , typename ConsumerPolicy
        , bool is_safe   // ToDo: Change to type
        , bool is_scoped // ToDo: Change to type
        >
class raw_active
  : public active_interface< UnqualifiedType
                           , active_details
                             <
                               raw_active< UnqualifiedType, ConsumerPolicy >
                             , ConsumerPolicy
                             , action_return_meta
                             >
                           >/*
  , public active_helper< raw_active< UnqualifiedType, ConsumerPolicy >
                        , UnqualifiedType
                        , ConsumerPolicy
                        > // ToDo: Change to private
*/
{
public:
  typedef UnqualifiedType represented_type;
  typedef ConsumerPolicy consumer_policy;
private:/*
  typedef active_helper< raw_active< UnqualifiedType, ConsumerPolicy >
                       , UnqualifiedType
                       , ConsumerPolicy
                       >
                       active_helper_base;*/
/*
public: // ToDo: Make private
  // Change to yield internal consumer
  active_impl_base_type& impl()
  {
    return static_cast< active_impl_base_type& >( *this );
  }

  active_impl_base_type const& impl() const
  {
    return static_cast< active_impl_base_type const& >( *this );
  }*/
public:
  raw_active()
  {
    consumer_m( detail::active_default_constructor() );
  }

  raw_active()
  {
    consumer_m( detail::active_destructor() );
  }

  //#include <boost/act/active/detail/raw_active/active_constructors.hpp>

 // #include <boost/act/active/detail/raw_active/active_impl_constructors.hpp>

private:
  struct detail_get_value
  {
    typedef UnqualifiedType result_type;

    result_type operator ()( UnqualifiedType const& target ) const
    {
      return target;
    }
  };
public:
  UnqualifiedType value() const
  {
    return scoped_action< UnqualifiedType, policy< consumer_policy > >
             ( as_target( consumer_m ), detail_get_value() ).value();
  }
public:
  //BOOST_ACT_DETAIL_PROMOTE_ACTIVE_MEMBERS( active_helper_base )
private:
  typedef typename raw_active_consumer_from_traits
          <
            unqualified_type
          , consumer_policy
          , is_safe, is_scoped
          >
          ::type consumer_type;
private:
  consumer_type consumer_m;
};

}
}
}

#endif
