# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_FACILITIES_REMOVE_PARENS_HPP
# define BOOST_PREPROCESSOR_FACILITIES_REMOVE_PARENS_HPP
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/control/iif.hpp>
# include <boost/preprocessor/facilities/is_tuple_begin.hpp>
# include <boost/preprocessor/tuple/enum.hpp>
#
# /* BOOST_PP_REMOVE_PARENS */
#
# if BOOST_PP_VARIADICS
# define BOOST_PP_REMOVE_PARENS(x) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_TUPLE_BEGIN(x), \
      BOOST_PP_REMOVE_PARENS_DETAIL, \
      BOOST_PP_IDENTITY \
      ) \
    (x)() \
/**/
#
# define BOOST_PP_REMOVE_PARENS_DETAIL(x) \
    BOOST_PP_IDENTITY(BOOST_PP_TUPLE_ENUM(x)) \
/**/
#
# endif /* BOOST_PP_VARIADICS */
# endif /* BOOST_PREPROCESSOR_FACILITIES_REMOVE_PARENS_HPP */
