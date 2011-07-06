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
# include <boost/preprocessor/cat.hpp>
# include <boost/preprocessor/facilities.hpp>
# include <libs/preprocessor/test/test.h>

# if BOOST_PP_VARIADICS

# define TUPLE (0, 1, 2, 3, 4, 5)
# define TUPLE_LARGE (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32)
# define TUPLE_VERY_LARGE (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63)

#endif

BEGIN BOOST_PP_APPLY(BOOST_PP_NIL) 0 == 0 END
BEGIN BOOST_PP_APPLY((0)) == 0 END

BEGIN BOOST_PP_APPLY((BOOST_PP_EMPTY))() 0 == 0 END

# define MACRO(x, y, z) 1
# define ARGS (1, 2, 3)

BEGIN BOOST_PP_EXPAND(MACRO ARGS) == 1 END

BEGIN BOOST_PP_IDENTITY(1)() == 1 END

BEGIN BOOST_PP_CAT(BOOST_PP_INTERCEPT, 2) 1 == 1 END

# if BOOST_PP_VARIADICS

BEGIN BOOST_PP_IS_TUPLE_BEGIN(TUPLE) == 1 END
BEGIN BOOST_PP_IS_TUPLE_BEGIN(TUPLE_LARGE) == 1 END
BEGIN BOOST_PP_IS_TUPLE_BEGIN(TUPLE_VERY_LARGE) == 1 END
BEGIN BOOST_PP_IS_TUPLE_BEGIN((4,8,ty,2)) == 1 END
BEGIN BOOST_PP_IS_TUPLE_BEGIN(A green test) == 0 END
BEGIN BOOST_PP_IS_TUPLE_BEGIN(TUPLE_VERY_LARGE A green test) == 1 END
BEGIN BOOST_PP_IS_TUPLE_BEGIN((4,8,ty,2)yz) == 1 END
BEGIN BOOST_PP_IS_TUPLE_BEGIN(A green test(3.x.t)) == 0 END

#endif
