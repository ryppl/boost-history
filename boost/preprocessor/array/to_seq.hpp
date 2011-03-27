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
# ifndef BOOST_PREPROCESSOR_ARRAY_TO_SEQ_HPP
# define BOOST_PREPROCESSOR_ARRAY_TO_SEQ_HPP
#
#include <boost/preprocessor/tuple/to_seq.hpp>
#include <boost/preprocessor/array/size.hpp>
#include <boost/preprocessor/array/data.hpp>
#
# /* BOOST_PP_ARRAY_TO_SEQ */
#
#define BOOST_PP_ARRAY_TO_SEQ(array) \
  BOOST_PP_TUPLE_TO_SEQ(BOOST_PP_ARRAY_SIZE(array),BOOST_PP_ARRAY_DATA(array)) \
/**/
# endif // BOOST_PREPROCESSOR_ARRAY_TO_SEQ_HPP
