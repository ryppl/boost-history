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
# ifndef BOOST_PREPROCESSOR_FACILITIES_IS_TUPLE_BEGIN_HPP
# define BOOST_PREPROCESSOR_FACILITIES_IS_TUPLE_BEGIN_HPP
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/arithmetic/dec.hpp>
# include <boost/preprocessor/variadic/size.hpp>
#
# /* BOOST_PP_IS_TUPLE */
#
# if BOOST_PP_VARIADICS
# define BOOST_PP_IS_TUPLE_BEGIN(x) \
    BOOST_PP_DEC \
      ( \
      BOOST_PP_VARIADIC_SIZE \
        ( \
        BOOST_PP_IS_TUPLE_BEGIN_DETAIL_EXPAND x \
        ) \
      ) \
/**/
#
# define BOOST_PP_IS_TUPLE_BEGIN_DETAIL_EXPAND(...) \
    1,1 \
/**/
#
# endif /* BOOST_PP_VARIADICS */
# endif /* BOOST_PREPROCESSOR_IS_TUPLE_BEGIN_HPP */
