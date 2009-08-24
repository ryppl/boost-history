/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_TUPLE_TUPLE_FWD_HPP
#define BOOST_FUSION_TUPLE_TUPLE_FWD_HPP

#include <boost/fusion/container/vector/limits.hpp>
#include <boost/fusion/support/internal/template.hpp>

namespace boost { namespace fusion
{
    VARIADIC_TEMPLATE_WITH_DEFAULT(FUSION_MAX_VECTOR_SIZE)
    struct tuple;
}}

#endif
