/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_VECTOR10_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_VECTOR10_HPP

#include <boost/config.hpp>

#if defined(BOOST_NO_VARIADIC_TEMPLATES) || !defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/vector/vector_fwd.hpp>
#   include <boost/fusion/iterator/deref.hpp>
#   include <boost/fusion/iterator/next.hpp>
#   include <boost/fusion/support/sequence_base.hpp>
#   include <boost/fusion/support/ref.hpp>
#   include <boost/fusion/support/sequence_assign.hpp>

#   include <boost/preprocessor/cat.hpp>
#   include <boost/preprocessor/dec.hpp>
#   include <boost/preprocessor/repetition/enum.hpp>
#   include <boost/preprocessor/repetition/enum_params.hpp>
#   include <boost/preprocessor/repetition/enum_binary_params.hpp>
#   include <boost/preprocessor/repetition/repeat.hpp>
#   include <boost/preprocessor/repetition/repeat_from_to.hpp>
#   include <boost/preprocessor/iteration/iterate.hpp>

#   include <boost/mpl/int.hpp>
#   include <boost/mpl/bool.hpp>
#   include <boost/mpl/vector.hpp>
#   include <boost/mpl/at.hpp>

#   include <boost/type_traits/add_reference.hpp>
#   include <boost/type_traits/add_const.hpp>

#   include <boost/fusion/container/vector/detail/at_impl.hpp>
#   include <boost/fusion/container/vector/detail/value_at_impl.hpp>
#   include <boost/fusion/container/vector/detail/vector_iterator.hpp>
#   include <boost/fusion/container/vector/detail/begin_impl.hpp>
#   include <boost/fusion/container/vector/detail/end_impl.hpp>
#   include <boost/fusion/container/vector/detail/deref_impl.hpp>
#   include <boost/fusion/container/vector/detail/value_of_impl.hpp>
#   include <boost/fusion/container/vector/detail/next_impl.hpp>
#   include <boost/fusion/container/vector/detail/prior_impl.hpp>
#   include <boost/fusion/container/vector/detail/equal_to_impl.hpp>
#   include <boost/fusion/container/vector/detail/distance_impl.hpp>
#   include <boost/fusion/container/vector/detail/advance_impl.hpp>
#else
#   include <boost/fusion/container/vector/vector.hpp>
#endif

namespace boost { namespace fusion
{
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   define BOOST_PP_FILENAME_1 <boost/fusion/container/vector/detail/pp/vector_n.hpp>
#else
#   define BOOST_PP_FILENAME_1 <boost/fusion/container/vector/detail/variadic_templates/vector_n.hpp>
#endif

#   define BOOST_PP_ITERATION_LIMITS (0, 10)
#   include BOOST_PP_ITERATE()
}}
#else
#   include <boost/fusion/container/vector/vector.hpp>
#endif

#endif
