/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
 
#if !defined(BOOST_FUSION_SEQ_NAME) && !defined(BOOST_FUSION_CONTAINER_DETAIL_FORWARD_CTOR_HPP)
#   define BOOST_FUSION_CONTAINER_DETAIL_FORWARD_CTOR_HPP
#   include <boost/config.hpp>
#endif

#ifdef BOOST_FUSION_USE_BASE_TYPE
#   define BOOST_FUSION_INIT_BASE base_type
#else
#   define BOOST_FUSION_INIT_BASE data
#endif

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/detail/pp/forward_ctor.hpp>
#else
#   include <boost/fusion/container/detail/variadic_templates/forward_ctor.hpp>
#endif

#undef BOOST_FUSION_INIT_BASE
