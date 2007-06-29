/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_MODIFY_MODIFY_ACQ_REL_HPP
#define BOOST_ACT_INTERLOCKED_MODIFY_MODIFY_ACQ_REL_HPP

#include <boost/act/interlocked/assign_if_was/assign_if_was_acq_rel.hpp>
#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/act/interlocked/semantics/acquire.hpp>
#include <boost/act/interlocked/semantics/acq_rel.hpp>
#include <boost/act/interlocked/load/load_acquire.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Semantics, typename TargetType, typename OperationType >
typename lazy_enable_if
<
  mpl::and_< is_same< Semantics, acq_rel >
           , detail::are_valid_store_style_params< TargetType >
           >
, remove_cv< TargetType >
>
::type
modify( TargetType& destination, OperationType operation )
{
  typedef typename remove_cv< TargetType >::type unqualified_type;
  unqualified_type new_value;

  for( unqualified_type curr_value = interlocked::load< acquire >( destination )
     ;    ( new_value = interlocked::assign_if_was< acq_rel >
                        ( destination
                        , operation( curr_value )
                        , curr_value
                        )
          )
       != curr_value
     ; curr_value = new_value
     );

  // Note: new_value is the old value here
  return new_value;
}

} } }

#endif
