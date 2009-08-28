/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
 
#ifndef BOOST_FUSION_SEQ_NAME
#   ifndef BOOST_FUSION_CONTAINER_DETAIL_VARIADIC_TEMPLATES_FORWARD_CTOR_HPP
#   define BOOST_FUSION_CONTAINER_DETAIL_VARIADIC_TEMPLATES_FORWARD_CTOR_HPP
#       ifdef BOOST_NO_RVALUE_REFERENCES
#           include <boost/call_traits.hpp>
#       endif
#   endif
#else
#   ifdef BOOST_NO_RVALUE_REFERENCES
    explicit
    BOOST_FUSION_SEQ_NAME(
        typename call_traits<Arguments>::param_type... arguments)
      : BOOST_FUSION_INIT_BASE(arguments)
    {}

    template<typename T>
    BOOST_FUSION_SEQ_NAME(T const& t)
      : BOOST_FUSION_INIT_BASE(t)
    {}
#   else
    template <typename... OtherArguments>
    explicit
    BOOST_FUSION_SEQ_NAME(OtherArguments&&... arguments)
      : BOOST_FUSION_INIT_BASE(std::forward<OtherArguments>(arguments)...)
    {}

    template<typename T>
    BOOST_FUSION_SEQ_NAME(T&& t)
      : BOOST_FUSION_INIT_BASE(std::forward<T>(t))
    {}
#   endif
#endif
