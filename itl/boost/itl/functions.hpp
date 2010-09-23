/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_FUNCTIONS_HPP_JOFA_090803
#define BOOST_ITL_FUNCTIONS_HPP_JOFA_090803

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/itl/type_traits/domain_type_of.hpp>
#include <boost/itl/type_traits/interval_type_of.hpp>
#include <boost/itl/type_traits/element_type_of.hpp>
#include <boost/itl/type_traits/segment_type_of.hpp>
#include <boost/itl/type_traits/is_interval_container.hpp>
#include <boost/itl/type_traits/is_interval_separator.hpp>
#include <boost/itl/type_traits/is_interval_splitter.hpp>
#include <boost/itl/type_traits/is_interval_joiner.hpp>
#include <boost/itl/type_traits/is_element_container.hpp>
#include <boost/itl/type_traits/is_combinable.hpp>
#include <boost/itl/type_traits/is_fragment_type_of.hpp>
#include <boost/itl/detail/interval_map_algo.hpp>

#include <boost/itl/concept/abstract/container.hpp>

#include <boost/itl/concept/interval/base.hpp>
#include <boost/itl/concept/interval/set.hpp>
#include <boost/itl/concept/interval/map.hpp>
#include <boost/itl/concept/interval/set_or_map.hpp>


namespace boost{namespace itl
{

//==============================================================================
//= Containedness
//==============================================================================
//==============================================================================
//= Equivalences and Orderings
//==============================================================================
//==============================================================================
//= Addition
//==============================================================================
//==============================================================================
//= Insertion
//==============================================================================
//==============================================================================
//= Erasure
//==============================================================================
//==============================================================================
//= Subtraction
//==============================================================================
//------------------------------------------------------------------------------
//- set (selective update)
//------------------------------------------------------------------------------
//==============================================================================
//= Intersection
//==============================================================================
//==============================================================================
//= Symmetric difference
//==============================================================================
//==============================================================================
//= Domain
//==============================================================================
//==============================================================================
//= Streaming
//==============================================================================

}} // namespace itl boost

#endif


