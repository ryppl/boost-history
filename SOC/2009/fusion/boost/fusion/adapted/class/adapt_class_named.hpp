/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_CLASS_ADAPT_CLASS_NAMED_HPP
#define BOOST_FUSION_ADAPTED_CLASS_ADAPT_CLASS_NAMED_HPP

#include <boost/fusion/adapted/class/adapt_class.hpp>
#include <boost/fusion/adapted/detail/struct/proxy_type.hpp>

#define BOOST_FUSION_ADAPT_CLASS_NAMED(WRAPPED_TYPE, NAME, SEQ)\
    BOOST_FUSION_ADAPT_CLASS_NAMED_NS(\
        WRAPPED_TYPE,\
        (boost)(fusion)(adapted),\
        NAME,\
        SEQ)

#define BOOST_FUSION_ADAPT_CLASS_NAMED_NS(WRAPPED_TYPE, NAMESPACE, NAME, SEQ)\
    BOOST_FUSION_DEFINE_PROXY_TYPE(WRAPPED_TYPE,NAMESPACE,NAME)\
    BOOST_FUSION_ADAPT_CLASS(\
        BOOST_FUSION_NAMESPACE_DECLARATION(NAMESPACE)NAME,\
        SEQ)

#endif
