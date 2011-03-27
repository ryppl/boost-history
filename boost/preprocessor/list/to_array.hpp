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
# ifndef BOOST_PREPROCESSOR_LIST_TO_ARRAY_HPP
# define BOOST_PREPROCESSOR_LIST_TO_ARRAY_HPP
#
#include <boost/preprocessor/tuple/to_array.hpp>
#include <boost/preprocessor/list/to_tuple.hpp>
#include <boost/preprocessor/list/size.hpp>
#
# /* BOOST_PP_LIST_TO_ARRAY */
#
#define BOOST_PP_LIST_TO_ARRAY(list) \
  BOOST_PP_TUPLE_TO_ARRAY(BOOST_PP_LIST_SIZE(list),BOOST_PP_LIST_TO_TUPLE(list)) \
/**/
# endif // BOOST_PREPROCESSOR_LIST_TO_ARRAY_HPP
