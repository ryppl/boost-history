// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PP_IS_ITERATING
#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_PP_ZIP_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_PP_ZIP_HPP

#include <boost/fusion/container/generation/vector_tie.hpp>
#include <boost/fusion/view/zip_view.hpp>
#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/template.hpp>
#include <boost/fusion/support/assert.hpp>

#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/preprocessor/cat.hpp>
#   include <boost/preprocessor/repetition/repeat.hpp>
#endif

namespace boost { namespace fusion
{
    namespace result_of
    {
        VARIADIC_TEMPLATE_WITH_DEFAULT(FUSION_MAX_ZIP_SEQUENCES)
        struct zip;
    }

#define BOOST_PP_FILENAME_1 <boost/fusion/algorithm/transformation/detail/pp/zip.hpp>
#define BOOST_PP_ITERATION_LIMITS (2, FUSION_MAX_ZIP_SEQUENCES)
#include BOOST_PP_ITERATE()

}}

#endif
#else

#define N BOOST_PP_ITERATION()

    namespace result_of
    {
        VARIADIC_TEMPLATE(N)
        struct zip
            EXPAND_TEMPLATE_ARGUMENTS_SPECIALIZATION(FUSION_MAX_ZIP_SEQUENCES,N)
        {
#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERS
#define BOOST_FUSION_IS_FORWARD_ASSERT(Z,N,_)\
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<BOOST_PP_CAT(T,N)>));

            BOOST_PP_REPEAT(N,BOOST_FUSION_IS_FORWARD_ASSERT,_)

#undef BOOST_FUSION_IS_FORWARD_ASSERT
#endif

            typedef
                zip_view<
                    typename result_of::vector_tie<
                        EXPAND_TEMPLATE_ARGUMENTS(N)
                    >::type
                >
            type;
        };
    }

    VARIADIC_TEMPLATE_A(N)
    inline
#if N
        typename
#endif
        result_of::zip<EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_CLREF(N)>::type
    zip(EXPAND_TEMPLATE_ARGUMENTS_PARAMETERS_A_R_ELSE_CLREF(N))
    {
        return
#if N
            typename
#endif
            result_of::zip<EXPAND_TEMPLATE_ARGUMENTS_A_R_ELSE_CLREF(N)>::type(
                    fusion::vector_tie(EXPAND_PARAMETERS_A(N)));
    }

#undef N
#endif
