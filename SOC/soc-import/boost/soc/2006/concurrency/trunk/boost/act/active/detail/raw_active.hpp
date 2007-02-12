/*=============================================================================
    Copyright (c) 2006-2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
/*
#ifndef BOOST_ACT_DETAIL_ACTIVE_TYPE_HPP
#define BOOST_ACT_DETAIL_ACTIVE_TYPE_HPP

#include <boost/tuple/tuple.hpp> // ToDo: Remove

#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#include <boost/mpl/deque.hpp>


#include "../active/active_interface.hpp"

#include "active_helper.hpp"

#include "action_return_meta.hpp"

#include "../act_model.hpp"

#include <boost/bind.hpp>

#include "../config/max_active_call_params.hpp"

#include "function_package.hpp"
#include "tie.hpp"
#include "../active/detail/constructor_caller.hpp"

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Error if Type is qualified
template< typename UnqualifiedType
        , typename ActModel
        >
class active_type
  : ActModel::template active_impl< UnqualifiedType >
  , public active_interface< UnqualifiedType
                           , active_details
                             <
                               active_type< UnqualifiedType, ActModel >
                             , ActModel
                             , action_return_meta
                             >
                           >
  , public active_helper< active_type< UnqualifiedType, ActModel >
                        , UnqualifiedType
                        , ActModel
                        > // ToDo: Change to private
{
public:
  typedef UnqualifiedType represented_type;
  typedef ActModel act_model;
private:
  typedef typename ActModel::template active_impl< UnqualifiedType >
            active_impl_base_type;

  typedef active_helper< active_type< UnqualifiedType, ActModel >
                       , UnqualifiedType
                       , ActModel
                       >
                       active_helper_base;
public: // ToDo: Make private
  active_impl_base_type& impl()
  {
    return static_cast< active_impl_base_type& >( *this );
  }

  active_impl_base_type const& impl() const
  {
    return static_cast< active_impl_base_type const& >( *this );
  }
public:
  // ToDo: Make constructors
public:
  active_type()
    : active_impl_base_type
      (
        detail::make_function_package
        <
          void, ActModel
        , mpl::deque<>
        >
        (
          constructor_caller< UnqualifiedType >()
        , detail::tie()
        )
      )
  {
  }

  explicit active_type( active_impl_constructor_tag )
    : active_impl_base_type( active_impl_constructor_tag() )
  {
  }

// ToDo: Unwrap references
#define BOOST_ACT_DETAIL_ACTIVE_IMPL_CONSTRUCTOR_MACRO( z, num_params, dummy ) \
  template< BOOST_PP_ENUM_PARAMS_Z( z, num_params, typename Param ) >          \
  active_type( active_impl_constructor_tag                                     \
               BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z( z, num_params           \
                                                     , Param                   \
                                                     , const & arg             \
                                                     )                         \
             )                                                                 \
    : active_impl_base_type( active_impl_constructor_tag()                     \
                             BOOST_PP_ENUM_TRAILING_PARAMS_Z( z, num_params    \
                                                            , arg              \
                                                            )                  \
                           )                                                   \
  {                                                                            \
  }

  BOOST_PP_REPEAT_FROM_TO( 1, BOOST_PP_INC( BOOST_ACT_MAX_PARAMS )
                         , BOOST_ACT_DETAIL_ACTIVE_IMPL_CONSTRUCTOR_MACRO
                         , BOOST_PP_NIL
                         )

#undef BOOST_ACT_DETAIL_ACTIVE_IMPL_CONSTRUCTOR_MACRO

// ToDo: Unwrap references
#define BOOST_ACT_DETAIL_ACTIVE_CONSTRUCTOR_MACRO( z, num_params, dummy )      \
  template< BOOST_PP_ENUM_PARAMS_Z( z, num_params, typename Param ) >          \
  active_type( BOOST_PP_ENUM_BINARY_PARAMS_Z( z, num_params                    \
                                            , Param                            \
                                            , const & arg                      \
                                            )                                  \
             )                                                                 \
    : active_impl_base_type                                                    \
      (                                                                        \
        detail::make_function_package                                          \
        <                                                                      \
          void, ActModel                                                       \
        , mpl::deque                                                           \
            < BOOST_PP_ENUM_PARAMS_Z( z, num_params, const Param ) >           \
        >                                                                      \
        (                                                                      \
          constructor_caller< UnqualifiedType >()                              \
        , detail::tie( BOOST_PP_ENUM_PARAMS_Z( z, num_params, arg ) )          \
        )                                                                      \
      )                                                                        \
  {                                                                            \
  }

  BOOST_PP_REPEAT_FROM_TO( 1, BOOST_PP_INC( BOOST_ACT_MAX_PARAMS )
                         , BOOST_ACT_DETAIL_ACTIVE_CONSTRUCTOR_MACRO
                         , BOOST_PP_NIL
                         )

#undef BOOST_ACT_DETAIL_ACTIVE_CONSTRUCTOR_MACRO

public:
  UnqualifiedType inactive_value() const
  {
    return active_impl_base_type::inactive_value();
  }
public:
  BOOST_ACT_DETAIL_PROMOTE_ACTIVE_MEMBERS( active_helper_base )
};

}
}
}

#endif
*/
