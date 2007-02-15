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

#include <boost/act/active/detail/active_helper.hpp>

#include <boost/act/active/detail/action_return_meta.hpp>

#include <boost/act/config/max_params.hpp>

#include <boost/act/detail/function_package.hpp>

#include <boost/act/active/detail/constructor_caller.hpp>

#include <boost/act/detail/tie.hpp>

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
class raw_active
  : ActModel::template active_impl< UnqualifiedType >
  , public active_interface< UnqualifiedType
                           , active_details
                             <
                               raw_active< UnqualifiedType, ActModel >
                             , ActModel
                             , action_return_meta
                             >
                           >
  , public active_helper< raw_active< UnqualifiedType, ActModel >
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

  typedef active_helper< raw_active< UnqualifiedType, ActModel >
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
  raw_active()
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

  #include <boost/act/active/detail/raw_active/active_constructors.hpp>

  #include <boost/act/active/detail/raw_active/active_impl_constructors.hpp>

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
