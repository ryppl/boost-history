# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# include <boost/preprocessor/debug.hpp>
# include <libs/preprocessor/test/test.h>

# if BOOST_PP_VARIADICS

BOOST_PP_ASSERT_IS_TUPLE((x,3,e,2)ty)
BOOST_PP_ASSERT_IS_ARRAY((y,(x,3,e,2)))
BOOST_PP_ASSERT_IS_LIST((4,(x)))

# endif
