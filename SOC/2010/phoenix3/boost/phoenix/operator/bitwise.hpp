/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_OPERATOR_BITWISE_HPP
#define PHOENIX_OPERATOR_BITWISE_HPP

#include <boost/phoenix/core/meta_grammar.hpp>

namespace boost { namespace phoenix
{
	
	PHOENIX_BINARY_OPERATORS(
		(bitwise_and_assign)
		(bitwise_or_assign)
		(bitwise_xor_assign)
		(shift_left_assign)
		(shift_right_assign)
		(bitwise_and)
		(bitwise_or)
		(bitwise_xor)
		(shift_left)
		(shift_right)
	)
}}

#endif
