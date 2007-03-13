/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_ALLOCATOR_CONSUMER_ARGUMENTS_HPP
#define BOOST_ACT_CONSUMER_ALLOCATOR_CONSUMER_ARGUMENTS_HPP

#include <boost/mpl/always.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/size_t.hpp>

#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/mpl/equal_to.hpp>

#include <boost/variant.hpp>

#include <boost/parameter.hpp>

#include <boost/ref.hpp>

#include <boost/act/detail/has_result_type.hpp>
#include <boost/act/detail/result_type.hpp>

// ToDo: Make specialization for variant for 1 type

// ToDo: Verify greater than 0 or account for 0 in implementation
#define BOOST_ACT_MAX_IS_PARAMS 10

namespace boost
{
namespace act
{

template< BOOST_PP_ENUM_BINARY_PARAMS( BOOST_ACT_MAX_IS_PARAMS, typename Type
                                     , = mpl::na BOOST_PP_INTERCEPT
                                     )
        >
struct is;

struct no_argument;

namespace detail
{

template< typename Type >
struct is_is : mpl::false_ {};

template< BOOST_PP_ENUM_PARAMS( BOOST_ACT_MAX_IS_PARAMS, typename Type ) >
struct is_is< is< BOOST_PP_ENUM_PARAMS( BOOST_ACT_MAX_IS_PARAMS, Type ) > >
  : mpl::true_ {};

template< typename Type >
struct reference_to_reference_wrapper
  : reference_wrapper< typename remove_reference< Type >::type >
{
};

template< typename IsType >
struct has_unary_is_sequence
  : mpl::equal_to< typename mpl::size< typename IsType::type_sequence >::type
                 , mpl::size_t< 1 >
                 >
{
};

}

// ToDo: Make default normalizer convert to any

// ToDo: Move to another file
//       Eliminate and use variant when specialization is made
template< typename TargetType >
struct convert_to
{
  typedef typename mpl::eval_if
          <
            is_reference< TargetType >
          , detail::reference_to_reference_wrapper< TargetType >
          , mpl::identity< TargetType >
          >
          ::type result_type;

  template< typename InputType >
  result_type operator ()( InputType& input ) const
  {
    return result_type( input );
  }
};

template< typename TypeSequence >
struct convert_to_variant_over
{
  typedef typename make_variant_over< TypeSequence >::type result_type;

  template< typename InputType >
  result_type operator ()( InputType& input ) const
  {
    return result_type( input );
  }
};

template< typename ProcessorType, typename VisitorType, typename Type >
void boost_act_consumer_visit( VisitorType& visitor, Type& target )
{
  visitor( target
         , mpl::identity< typename result_of< ProcessorType(Type) >::type >()
         );
}

namespace detail
{

template< typename FunctionType, typename ReturnType >
class variant_consumer_visitor
  : static_visitor<>
{
public:
  explicit variant_consumer_visitor( FunctionType& function_init )
    : function_m( function_init ) {}
public:
  template< typename Type >
  void operator ()( Type& target )
  {
    function_m( target, mpl::identity< ReturnType >() );
  }
private:
  FunctionType& function_m;
};

template< typename ReturnType, typename FunctionType >
variant_consumer_visitor< FunctionType, ReturnType >
make_variant_consumer_visitor( FunctionType& function_init )
{
  return variant_consumer_visitor< FunctionType, ReturnType >( function_init );
}

}

template< typename ProcessorType, typename VisitorType
        , BOOST_VARIANT_ENUM_PARAMS( typename Type )
        >
void boost_act_consumer_visit( VisitorType& visitor
                             , variant<BOOST_VARIANT_ENUM_PARAMS(Type)>& target
                             )
{
  // Qualified to avoid Koenig lookup
  ::boost::apply_visitor( variant_consumer_visitor( visitor ), target );
}

template< typename ProcessorType, typename VisitorType, typename Type >
void consumer_visit( VisitorType& visitor, Type& target )
{
  return boost_act_consumer_visit< ProcessorType >( visitor, target );
}

namespace detail
{

template< typename IsType >
struct convert_to_meta
{
private:
  typedef typename mpl::front< typename IsType::type_sequence >::type front;
public:
  typedef convert_to< front > type;
};

template< typename IsType >
struct convert_to_variant_meta
{
private:
  typedef typename IsType::type_sequence type_sequence;
public:
  typedef convert_to_variant_over< type_sequence > type;
};

}

BOOST_PARAMETER_TEMPLATE_KEYWORD( processor )
BOOST_PARAMETER_TEMPLATE_KEYWORD( normalizer )
BOOST_PARAMETER_TEMPLATE_KEYWORD( restraint )
BOOST_PARAMETER_TEMPLATE_KEYWORD( shared_data )
BOOST_PARAMETER_TEMPLATE_KEYWORD( allow_parallel_consumption_info )

typedef allow_parallel_consumption_info<mpl::true_> allow_parallel_consumption;

namespace detail
{
typedef parameter::parameters
        <
          parameter::required< tag::processor >
        , parameter::optional< tag::restraint >
        , parameter::optional< tag::normalizer >
        , parameter::optional< tag::shared_data >
        , parameter::optional< tag::allow_parallel_consumption_info >
        >
        consumer_signature;

struct no_argument_normalizer
{
  typedef void result_type;

  result_type operator ()() const {}
};

}

#define BOOST_ACT_NUM_CONSUMER_PARAMS 5

#define BOOST_ACT_CONSUMER_TEMPLATE_PARAMS_DECL()                              \
  BOOST_PP_ENUM_BINARY_PARAMS( BOOST_ACT_NUM_CONSUMER_PARAMS                   \
                             , typename Arg                                    \
                             , = ::boost::parameter::void_ BOOST_PP_INTERCEPT  \
                             )

#define BOOST_ACT_CONSUMER_TEMPLATE_PARAMS()                                   \
  BOOST_PP_ENUM_PARAMS( BOOST_ACT_NUM_CONSUMER_PARAMS, typename Arg )

#define BOOST_ACT_DETAIL_CONSUMER_TEMPLATE_ARGS()                              \
  BOOST_PP_ENUM_PARAMS( BOOST_ACT_NUM_CONSUMER_PARAMS, Arg )

namespace detail
{

template< BOOST_ACT_CONSUMER_TEMPLATE_PARAMS_DECL() >
struct consumer_traits
{
private:
  typedef typename detail::consumer_signature::bind
          < BOOST_ACT_DETAIL_CONSUMER_TEMPLATE_ARGS() >::type
          boost_act_detail_args;
public:
  typedef typename parameter::binding
          <
            boost_act_detail_args
          , tag::processor
          >
          ::type processor_type;

  typedef typename ::boost::parameter::binding
          <
            boost_act_detail_args
          , tag::restraint
          , no_argument
          >
          ::type restraint;

  typedef typename ::boost::parameter::binding
          <
            boost_act_detail_args
          , tag::shared_data
          , void
          >
          ::type shared_data;

  typedef typename ::boost::parameter::binding
          <
            boost_act_detail_args
          , tag::allow_parallel_consumption_info
          , mpl::not_< is_void< shared_data > >
          >
          ::type allow_parallel_consumption;
private:
  struct use_convert_to;
  struct use_convert_to_variant;
  struct no_deducible_normalizer;
  struct no_required_normalizer;

  typedef typename mpl::eval_if
          <
            detail::is_is< restraint >
          , mpl::if_< detail::has_unary_is_sequence< restraint >
                    , use_convert_to
                    , use_convert_to_variant
                    >
          , mpl::if_< is_same< restraint, no_argument >
                    , no_required_normalizer
                    , no_deducible_normalizer
                    >
          >
          ::type default_normalizer_info;
public:
  typedef typename parameter::binding
          <
            boost_act_detail_args
          , tag::normalizer
          , typename mpl::eval_if
            <
              is_same< default_normalizer_info, no_deducible_normalizer >
            , mpl::identity< no_deducible_normalizer >
            , mpl::eval_if
              <
                is_same< default_normalizer_info, use_convert_to >
              , detail::convert_to_meta< restraint >
              , mpl::eval_if
                <
                  is_same< default_normalizer_info, use_convert_to_variant >
                , detail::convert_to_variant_meta< restraint >
                , mpl::identity< detail::no_argument_normalizer >
                >
              >
            >
            ::type
          >
          ::type normalizer_type;
private:
  BOOST_MPL_ASSERT_MSG( ( mpl::not_< is_same< normalizer_type
                                            , no_deducible_normalizer
                                            >
                                   >
                                   ::value
                        )
                      , NORMALIZER_NOT_DEDUCIBLE_FOR_CONSUMER
                      , ()
                      );

  BOOST_MPL_ASSERT_MSG( detail::has_result_type< normalizer_type >::value
                      , NORMALIZER_DOES_NOT_HAVE_GLOBALLY_DEDUCIBLE_RESULT_TYPE
                      , ( normalizer_type )
                      );
public:
  typedef typename detail::result_type< normalizer_type >::type normalized_type;
};

#define BOOST_ACT_CONSUMER_TRAITS()                                            \
::boost::act::detail::consumer_traits<BOOST_ACT_DETAIL_CONSUMER_TEMPLATE_ARGS()>

#define BOOST_ACT_CONSUMER_TYPE_MEMBERS()                                      \
typedef typename BOOST_ACT_CONSUMER_TRAITS()::processor_type processor_type;   \
typedef typename BOOST_ACT_CONSUMER_TRAITS()::restraint restraint;             \
typedef typename BOOST_ACT_CONSUMER_TRAITS()::normalizer_type normalizer_type; \
typedef typename BOOST_ACT_CONSUMER_TRAITS()::normalized_type normalized_type;

template< typename Type, typename Arg >
struct is_impl_basic
  : mpl::and_< is_convertible< typename remove_cv< Type >::type*
                             , typename remove_cv< Arg >::type*
                             >
             , is_convertible< Arg, Type >
             > {};

// ToDo: Possibly rewrite to use unwrap_reference
template< typename Type, typename Arg >
struct is_impl_ref
  : mpl::and_< is_reference_wrapper< Arg >, is_convertible< Arg, Type > > {};

template< typename Type, typename Arg >
struct is_impl
  : mpl::eval_if< is_reference< Type >
                , detail::is_impl_ref< Type, Arg >
                , detail::is_impl_basic< Type, Arg >
                > {};

}

// ToDo: Make variable argument list size toggleable
template< BOOST_PP_ENUM_PARAMS( BOOST_ACT_MAX_IS_PARAMS, typename Type ) >
struct is
{
  typedef mpl::vector
          <
            BOOST_PP_ENUM_PARAMS( BOOST_ACT_MAX_IS_PARAMS, Type )
          >
          type_sequence;

  template< typename Arg >
  struct apply
    : mpl::not_
      <
        is_same< typename mpl::find_if< type_sequence
                                      , detail::is_impl< mpl::_1, Arg >
                                      >
                                      ::type
               , typename mpl::end< type_sequence >::type
               >
      > {};
};

}
}

#endif
