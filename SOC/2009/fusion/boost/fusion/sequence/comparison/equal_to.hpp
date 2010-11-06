/*==============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_COMPARISON_EQUAL_TO_HPP
#define BOOST_FUSION_SEQUENCE_COMPARISON_EQUAL_TO_HPP

#define BOOST_FUSION_OPERATOR_ABREV equal_to
#define BOOST_FUSION_OPERATOR ==
#define BOOST_FUSION_COMPARE_ALL
#include <boost/fusion/sequence/comparison/detail/operator_impl.hpp>
#undef BOOST_FUSION_COMPARE_ALL
#undef BOOST_FUSION_OPERATOR
#undef BOOST_FUSION_OPERATOR_ABREV

#endif
