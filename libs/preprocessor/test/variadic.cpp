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
# include <boost/preprocessor/variadic.hpp>
# include <boost/preprocessor/array/size.hpp>
# include <boost/preprocessor/array/elem.hpp>
# include <boost/preprocessor/list/at.hpp>
# include <boost/preprocessor/list/size.hpp>
# include <boost/preprocessor/seq/elem.hpp>
# include <boost/preprocessor/seq/size.hpp>
# include <boost/preprocessor/tuple/size.hpp>
# include <boost/preprocessor/tuple/elem.hpp>
# include <libs/preprocessor/test/test.h>

#if BOOST_PP_VARIADICS

#define VDATA 0,1,2,3,4,5,6

BEGIN BOOST_PP_VARIADIC_ELEM(4,VDATA) == 4 END
BEGIN BOOST_PP_VARIADIC_ELEM(6,7,11,3,8,14,85,56,92,165) == 56 END
BEGIN BOOST_PP_VARIADIC_SIZE(VDATA) == 7 END
BEGIN BOOST_PP_VARIADIC_SIZE(7,11,3,8,14,85,56,92,165) == 9 END
BEGIN BOOST_PP_ARRAY_SIZE(BOOST_PP_VARIADIC_TO_ARRAY(VDATA)) == 7 END
BEGIN BOOST_PP_ARRAY_ELEM(4,BOOST_PP_VARIADIC_TO_ARRAY(7,11,3,8,14,85,56,92,165)) == 14 END
BEGIN BOOST_PP_LIST_AT(BOOST_PP_VARIADIC_TO_LIST(VDATA),3) == 3 END
BEGIN BOOST_PP_LIST_SIZE(BOOST_PP_VARIADIC_TO_LIST(7,11,3,8,14,85,56,92,165)) == 9 END
BEGIN BOOST_PP_SEQ_ELEM(5,BOOST_PP_VARIADIC_TO_SEQ(VDATA)) == 5 END
BEGIN BOOST_PP_SEQ_SIZE(BOOST_PP_VARIADIC_TO_SEQ(7,11,3,8,14,85,56,92,165)) == 9 END
BEGIN BOOST_PP_TUPLE_SIZE(BOOST_PP_VARIADIC_TO_TUPLE(VDATA)) == 7 END
BEGIN BOOST_PP_TUPLE_ELEM(8,BOOST_PP_VARIADIC_TO_TUPLE(7,11,3,8,14,85,56,92,165)) == 165 END

#endif
