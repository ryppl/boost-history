# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* Revised by Edward Diener (2011) */
#
# /* See http://www.boost.org for most recent version. */
#
# include <boost/preprocessor/array.hpp>
# include <libs/preprocessor/test/test.h>
# include <boost/preprocessor/list/at.hpp>
# include <boost/preprocessor/seq/elem.hpp>
# include <boost/preprocessor/tuple/elem.hpp>
# if BOOST_PP_VARIADICS
# include <boost/preprocessor/variadic/size.hpp>
# include <boost/preprocessor/variadic/elem.hpp>
# endif

# define ARRAY (3, (0, 1, 2))

// element access

BEGIN BOOST_PP_ARRAY_ELEM(1, ARRAY) == 1 END
BEGIN BOOST_PP_ARRAY_ELEM(2, (5, (0, 1, 2, 3, 4))) == 2 END

// size

BEGIN BOOST_PP_ARRAY_SIZE(ARRAY) == 3 END
BEGIN BOOST_PP_ARRAY_SIZE((5, (0, 1, 2, 3, 4))) == 5 END

// enum

# if BOOST_PP_VARIADICS

BEGIN BOOST_PP_VARIADIC_ELEM(2,BOOST_PP_ARRAY_ENUM(ARRAY)) == 2 END
BEGIN BOOST_PP_VARIADIC_ELEM(3,BOOST_PP_ARRAY_ENUM((5, (0, 1, 2, 3, 4)))) == 3 END
BEGIN BOOST_PP_VARIADIC_SIZE(BOOST_PP_ARRAY_ENUM((5, (0, 1, 2, 3, 4)))) == 5 END

# endif

// to_list

BEGIN BOOST_PP_LIST_AT(BOOST_PP_ARRAY_TO_LIST(ARRAY), 1) == 1 END
BEGIN BOOST_PP_LIST_AT(BOOST_PP_ARRAY_TO_LIST((5, (0, 1, 2, 3, 4))), 4) == 4 END

// to_seq

BEGIN BOOST_PP_SEQ_ELEM(0, BOOST_PP_ARRAY_TO_SEQ(ARRAY)) == 0 END
BEGIN BOOST_PP_SEQ_ELEM(3, BOOST_PP_ARRAY_TO_SEQ((5, (0, 1, 2, 3, 4)))) == 3 END

// to_tuple

# if BOOST_PP_VARIADICS

BEGIN BOOST_PP_TUPLE_ELEM(2, BOOST_PP_ARRAY_TO_TUPLE(ARRAY)) == 2 END
BEGIN BOOST_PP_TUPLE_ELEM(1, BOOST_PP_ARRAY_TO_TUPLE((5, (0, 1, 2, 3, 4)))) == 1 END

# else

BEGIN BOOST_PP_TUPLE_ELEM(3, 2, BOOST_PP_ARRAY_TO_TUPLE(ARRAY)) == 2 END
BEGIN BOOST_PP_TUPLE_ELEM(5, 1, BOOST_PP_ARRAY_TO_TUPLE((5, (0, 1, 2, 3, 4)))) == 1 END

# endif
