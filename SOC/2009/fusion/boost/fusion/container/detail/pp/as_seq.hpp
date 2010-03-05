/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/next.hpp>

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/dec.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <int Size>
    struct BOOST_PP_CAT(BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME),_impl);

#define BOOST_PP_FILENAME_1 <boost/fusion/container/detail/pp/as_seq_n.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_FUSION_MAX_SEQ_SIZE)
#include BOOST_PP_ITERATE()
}}}

