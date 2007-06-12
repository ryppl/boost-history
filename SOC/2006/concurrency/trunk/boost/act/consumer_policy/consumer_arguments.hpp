/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_CONSUMER_ARGUMENTS_HPP
#define BOOST_ACT_CONSUMER_POLICY_CONSUMER_ARGUMENTS_HPP

#include <boost/act/consumer_policy/consumer_arguments_fwd.hpp>

#include <boost/act/consumer_policy/default_normalization_info.hpp>
#include <boost/act/detail/has_single_result_type.hpp>
#include <boost/act/detail/result_type.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/pair.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/assert.hpp>

#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

// ToDo: Use more specific header
#include <boost/parameter.hpp>

namespace boost { namespace act { namespace detail {

#define BOOST_ACT_DETAIL_REVISED_CONSUMER_ARGS()                               \
  BOOST_PP_ENUM_BINARY_PARAMS                                                  \
  ( BOOST_ACT_NUM_CONSUMER_PARAMS                                              \
  , typename ::boost::act::detail::revise_argument< Arg                        \
  , >::type BOOST_PP_INTERCEPT                                                 \
  )

template< BOOST_ACT_CONSUMER_PARAMS_DECL() >
struct consumer_traits
{
private:
  // ToDo: Make constraints for types
  //       Make shared_data
  typedef parameter::parameters
          <
            parameter::required< tag::constraint >
          , parameter::required< tag::processor_info >
          , parameter::optional< tag::normalization_info >
          >
          consumer_signature;
private:
  typedef typename consumer_signature::bind
          < BOOST_ACT_DETAIL_REVISED_CONSUMER_ARGS() >::type
          boost_act_detail_args;
private:
  typedef typename parameter::binding
          <
            boost_act_detail_args
          , tag::processor_info
          >
          ::type processor_info;
public:
  typedef typename mpl::first< processor_info >::type processor_type;

  typedef typename mpl::second< processor_info >::type
            processor_shared_data_type;

  typedef typename parameter::binding
          <
            boost_act_detail_args
          , tag::constraint
          >
          ::type constraint;
private:
  struct no_deducible_normalization;
public:
  typedef typename parameter::binding
          <
            boost_act_detail_args
          , tag::normalization_info
          , typename mpl::if_
            <
              detail::has_default_normalization_info< constraint >
            , default_normalization_info< constraint >
            , no_deducible_normalization
            >
            ::type
          >
          ::type normalization_info;
private:
  BOOST_MPL_ASSERT_MSG( ( mpl::not_< is_same< normalization_info
                                            , no_deducible_normalization
                                            >
                                   >
                                   ::value
                        )
                      , NORMALIZER_NOT_DEDUCIBLE_FOR_CONSUMER
                      , ( constraint )
                      );
public:
  typedef typename normalization_info::normalize   normalizer_type;
  typedef typename normalization_info::denormalize denormalizer_type;
  // ToDo: Add helper deduction for normalized_type
private:
  BOOST_MPL_ASSERT_MSG( has_single_result_type< normalizer_type >::value
                      , NORMALIZER_DOES_NOT_HAVE_GLOBALLY_DEDUCIBLE_RESULT_TYPE
                      , ( normalizer_type )
                      );
public:
  typedef typename detail::result_type< normalizer_type >::type normalized_type;
};

} } }

#define BOOST_ACT_CONSUMER_TRAITS()                                            \
::boost::act::detail::consumer_traits< BOOST_ACT_CONSUMER_ARGS() >

#define BOOST_ACT_CONSUMER_TYPE_MEMBERS()                                      \
typedef typename BOOST_ACT_CONSUMER_TRAITS()::processor_type processor_type;   \
typedef typename BOOST_ACT_CONSUMER_TRAITS()::processor_shared_data_type       \
                                              processor_shared_data_type;      \
typedef typename BOOST_ACT_CONSUMER_TRAITS()::constraint constraint;           \
typedef typename BOOST_ACT_CONSUMER_TRAITS()::normalizer_type normalizer_type; \
typedef typename BOOST_ACT_CONSUMER_TRAITS()::denormalizer_type                \
                                              denormalizer_type;               \
typedef typename BOOST_ACT_CONSUMER_TRAITS()::normalized_type normalized_type;

#endif
