/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQ_NAME
#   ifndef BOOST_FUSION_CONTAINER_DETAIL_PP_FORWARD_INTERFACE_HPP
#       define BOOST_FUSION_CONTAINER_DETAIL_PP_FORWARD_INTERFACE_HPP

#       include <boost/fusion/support/internal/template.hpp>

#       include <boost/config.hpp>
#       include <boost/preprocessor/iterate.hpp>
#       include <boost/preprocessor/repetition/enum_params.hpp>
#       include <boost/preprocessor/repetition/enum_binary_params.hpp>
#   endif
#else
#   define BOOST_PP_FILENAME_1 <boost/fusion/container/detail/pp/forward_interface_n.hpp>
#   define BOOST_PP_ITERATION_LIMITS (0, BOOST_FUSION_MAX_SEQ_SIZE)
#   include BOOST_PP_ITERATE()
#endif
