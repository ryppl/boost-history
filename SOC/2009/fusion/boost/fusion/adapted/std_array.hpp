/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_STD_ARRAY_HPP
#define BOOST_FUSION_ADAPTED_STD_ARRAY_HPP

#include <boost/config.hpp>
#if !defined(BOOST_NO_0X_HDR_ARRAY) || defined(BOOST_HAS_TR1_ARRAY)
#   include <boost/fusion/support/internal/ref.hpp>

#   define BOOST_FUSION_ADAPTED_ARRAY_TYPE struct
#   ifndef BOOST_NO_0X_HDR_ARRAY
#       define BOOST_FUSION_ADAPTED_ARRAY_NAMESPACE std
#   else
#       define BOOST_FUSION_ADAPTED_ARRAY_NAMESPACE std::tr1
#   endif
#   include <boost/fusion/adapted/detail/array/tag_of.hpp>
#   undef BOOST_FUSION_ADAPTED_ARRAY_NAMESPACE
#   undef BOOST_FUSION_ADAPTED_ARRAY_TYPE

#   include <boost/fusion/adapted/detail/array/is_view_impl.hpp>
#   include <boost/fusion/adapted/detail/array/is_sequence_impl.hpp>
#   include <boost/fusion/adapted/detail/array/category_of_impl.hpp>
#   include <boost/fusion/adapted/detail/array/begin_impl.hpp>
#   include <boost/fusion/adapted/detail/array/end_impl.hpp>
#   include <boost/fusion/adapted/detail/array/size_impl.hpp>
#   include <boost/fusion/adapted/detail/array/at_impl.hpp>
#   include <boost/fusion/adapted/detail/array/value_at_impl.hpp>
#endif

#endif
