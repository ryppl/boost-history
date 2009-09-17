/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_BOOST_TUPLE_HPP
#define BOOST_FUSION_ADAPTED_BOOST_TUPLE_HPP

#include <boost/fusion/support/internal/ref.hpp>

#include <boost/tuple/tuple.hpp>

#define BOOST_FUSION_ADAPTED_TUPLE_NAMESPACE(X) tuples::X
#define BOOST_FUSION_ADAPTED_TUPLE_TAG boost_tuple_tag
#define BOOST_FUSION_ADAPTED_TUPLE_ITERATOR_TAG boost_tuple_iterator_tag

#include <boost/fusion/adapted/detail/tuple/tag_of.hpp>
#include <boost/fusion/adapted/detail/tuple/at_impl.hpp>
#include <boost/fusion/adapted/detail/tuple/begin_impl.hpp>
#include <boost/fusion/adapted/detail/tuple/category_of_impl.hpp>
#include <boost/fusion/adapted/detail/tuple/deref_impl.hpp>
#include <boost/fusion/adapted/detail/tuple/end_impl.hpp>
#include <boost/fusion/adapted/detail/tuple/is_sequence_impl.hpp>
#include <boost/fusion/adapted/detail/tuple/is_view_impl.hpp>
#include <boost/fusion/adapted/detail/tuple/size_impl.hpp>
#include <boost/fusion/adapted/detail/tuple/value_at_impl.hpp>
#include <boost/fusion/adapted/detail/tuple/value_of_impl.hpp>

#undef BOOST_FUSION_ADAPTED_TUPLE_ITERATOR_TAG
#undef BOOST_FUSION_ADAPTED_TUPLE_TAG
#undef BOOST_FUSION_ADAPTED_TUPLE_NAMESPACE

#endif
