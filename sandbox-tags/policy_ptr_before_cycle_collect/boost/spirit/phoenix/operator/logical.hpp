/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_OPERATOR_LOGICAL_HPP
#define PHOENIX_OPERATOR_LOGICAL_HPP

#include <boost/spirit/phoenix/core/composite.hpp>
#include <boost/spirit/phoenix/core/compose.hpp>
#include <boost/spirit/phoenix/utility/type_deduction.hpp>
#include <boost/spirit/phoenix/operator/impl/unary_eval.hpp>
#include <boost/spirit/phoenix/operator/impl/unary_compose.hpp>
#include <boost/spirit/phoenix/operator/impl/binary_eval.hpp>
#include <boost/spirit/phoenix/operator/impl/binary_compose.hpp>

namespace boost { namespace phoenix
{
    struct logical_not_eval;
    struct logical_and_eval;
    struct logical_or_eval;

    BOOST_UNARY_RESULT_OF(!x, result_of_logical_not);
    BOOST_BINARY_RESULT_OF(x && y, result_of_logical_and);
    BOOST_BINARY_RESULT_OF(x || y, result_of_logical_or);

#define x a0.eval(args, env)
#define y a1.eval(args, env)

    PHOENIX_UNARY_EVAL(logical_not_eval, result_of_logical_not, !x);
    PHOENIX_BINARY_EVAL(logical_and_eval, result_of_logical_and, x && y);
    PHOENIX_BINARY_EVAL(logical_or_eval, result_of_logical_or, x || y);

    PHOENIX_UNARY_COMPOSE(logical_not_eval, !);
    PHOENIX_BINARY_COMPOSE(logical_and_eval, &&);
    PHOENIX_BINARY_COMPOSE(logical_or_eval, ||);

#undef x
#undef y
}}

#endif
