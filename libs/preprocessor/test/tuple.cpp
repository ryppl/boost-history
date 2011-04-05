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
# include <boost/preprocessor/array/elem.hpp>
# include <boost/preprocessor/list/at.hpp>
# include <boost/preprocessor/seq/elem.hpp>
# if BOOST_PP_VARIADICS
# include <boost/preprocessor/variadic/size.hpp>
# include <boost/preprocessor/variadic/elem.hpp>
# endif
# include <libs/preprocessor/test/test.h>

# define TUPLE (0, 1, 2, 3, 4, 5)
# define T2 (+3, /2, +6)

#if BOOST_PP_VARIADICS

# define CALC(x) BOOST_PP_TUPLE_ELEM(0, x) BOOST_PP_TUPLE_ELEM(1, x) BOOST_PP_TUPLE_ELEM(2, x)
# define TEST_EAT BOOST_PP_TUPLE_EAT()(1, 2) 4

#else

# define CALC(x) BOOST_PP_TUPLE_ELEM(3, 0, x) BOOST_PP_TUPLE_ELEM(3, 1, x) BOOST_PP_TUPLE_ELEM(3, 2, x)
# define TEST_EAT BOOST_PP_TUPLE_EAT(2)(1, 2) 4

#endif

BEGIN BOOST_PP_TUPLE_ELEM(6, 3, TUPLE) == 3 END
BEGIN BOOST_PP_TUPLE_ELEM(6, 5, TUPLE) == 5 END
BEGIN CALC(T2) == 7 END
BEGIN CALC(BOOST_PP_TUPLE_REVERSE(3, T2)) == 6 END
BEGIN TEST_EAT == 4 END
BEGIN BOOST_PP_ARRAY_ELEM(3,BOOST_PP_TUPLE_TO_ARRAY(6,TUPLE)) == 3 END
BEGIN BOOST_PP_LIST_AT(BOOST_PP_TUPLE_TO_LIST(6,TUPLE), 2) == 2 END
BEGIN BOOST_PP_SEQ_ELEM(4,BOOST_PP_TUPLE_TO_SEQ(6,TUPLE)) == 4 END

#if BOOST_PP_VARIADICS

BEGIN BOOST_PP_TUPLE_ELEM(3, TUPLE) == 3 END
BEGIN BOOST_PP_TUPLE_ELEM(5, TUPLE) == 5 END
BEGIN CALC(BOOST_PP_TUPLE_REVERSE(T2)) == 6 END
BEGIN BOOST_PP_VARIADIC_ELEM(2,BOOST_PP_TUPLE_ENUM(BOOST_PP_TUPLE_REVERSE(TUPLE))) == 3 END
BEGIN BOOST_PP_VARIADIC_SIZE(BOOST_PP_TUPLE_ENUM(TUPLE)) == 6 END
BEGIN BOOST_PP_TUPLE_SIZE(TUPLE) == 6 END
BEGIN BOOST_PP_ARRAY_ELEM(3,BOOST_PP_TUPLE_TO_ARRAY(TUPLE)) == 3 END
BEGIN BOOST_PP_LIST_AT(BOOST_PP_TUPLE_TO_LIST(TUPLE), 2) == 2 END
BEGIN BOOST_PP_SEQ_ELEM(4,BOOST_PP_TUPLE_TO_SEQ(TUPLE)) == 4 END

#endif
