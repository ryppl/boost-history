/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_PP_VECTOR_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_PP_VECTOR_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/container/vector/detail/pp/vector_n_chooser.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/add_const.hpp>

#include <boost/fusion/container/detail/forward_interface.hpp>

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;

    VARIADIC_TEMPLATE(FUSION_MAX_VECTOR_SIZE)
    struct vector
      : detail::vector_n_chooser<
            EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_VECTOR_SIZE)
        >::type
    {
    private:
        typedef typename
            detail::vector_n_chooser<
                EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_VECTOR_SIZE)
            >::type
        base_type;

    public:
#define BOOST_FUSION_USE_BASE_TYPE
#define BOOST_FUSION_SEQ_NAME vector
#define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_VECTOR_SIZE
#include <boost/fusion/container/detail/forward_interface.hpp>
#undef BOOST_FUSION_MAX_SEQ_SIZE
#undef BOOST_FUSION_SEQ_NAME
#undef BOOST_FUSION_USE_BASE_TYPE
    };
}}

#endif
