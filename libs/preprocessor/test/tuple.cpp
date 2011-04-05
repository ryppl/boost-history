# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# include <boost/preprocessor/tuple.hpp>
# include <boost/preprocessor/arithmetic/add.hpp>
# include <boost/preprocessor/cat.hpp>
# include <boost/preprocessor/control/if.hpp>
# include <libs/preprocessor/test/test.h>

# define TUPLE (0, 1, 2, 3, 4, 5)

BEGIN BOOST_PP_TUPLE_ELEM(6, 3, TUPLE) == 3 END
BEGIN BOOST_PP_TUPLE_ELEM(6, 5, TUPLE) == 5 END

#if BOOST_PP_VARIADICS

BEGIN BOOST_PP_TUPLE_ELEM(3, TUPLE) == 3 END
BEGIN BOOST_PP_TUPLE_ELEM(5, TUPLE) == 5 END

#endif

# define CALC(x) BOOST_PP_TUPLE_ELEM(3, 0, x) BOOST_PP_TUPLE_ELEM(3, 1, x) BOOST_PP_TUPLE_ELEM(3, 2, x)
# define T2 (+3, /2, +6)

BEGIN CALC(T2) == 7 END
BEGIN CALC(BOOST_PP_TUPLE_REVERSE(3, T2)) == 6 END

#if BOOST_PP_VARIADICS

#undef CALC

# define CALC(x) BOOST_PP_TUPLE_ELEM(0, x) BOOST_PP_TUPLE_ELEM(1, x) BOOST_PP_TUPLE_ELEM(2, x)

BEGIN CALC(T2) == 7 END
BEGIN CALC(BOOST_PP_TUPLE_REVERSE(T2)) == 6 END

#endif

#if BOOST_PP_VARIADICS

#define TEST_EAT BOOST_PP_TUPLE_EAT()(1, 2) 4

#else

#define TEST_EAT BOOST_PP_TUPLE_EAT(2)(1, 2) 4

#endif

BEGIN TEST_EAT == 4 END
