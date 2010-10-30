/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_BOOST_ARRAY_HPP
#define BOOST_FUSION_ADAPTED_BOOST_ARRAY_HPP
 
#include <boost/fusion/support/internal/base.hpp>

#define BOOST_FUSION_ADAPTED_ARRAY_TYPE class
#define BOOST_FUSION_ADAPTED_ARRAY_NAMESPACE boost
#include <boost/fusion/adapted/detail/array/tag_of.hpp>
#undef BOOST_FUSION_ADAPTED_ARRAY_NAMESPACE
#undef BOOST_FUSION_ADAPTED_ARRAY_TYPE

#include <boost/fusion/adapted/detail/array/is_view_impl.hpp>
#include <boost/fusion/adapted/detail/array/is_sequence_impl.hpp>
#include <boost/fusion/adapted/detail/array/category_of_impl.hpp>
#include <boost/fusion/adapted/detail/array/begin_impl.hpp>
#include <boost/fusion/adapted/detail/array/end_impl.hpp>
#include <boost/fusion/adapted/detail/array/size_impl.hpp>
#include <boost/fusion/adapted/detail/array/at_impl.hpp>
#include <boost/fusion/adapted/detail/array/value_at_impl.hpp>
#include <boost/fusion/adapted/detail/array/deref_impl.hpp>
#include <boost/fusion/adapted/detail/array/value_of_impl.hpp>

#endif
