# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.  Permission to copy, use,        *
#  *     modify, sell, and distribute this software is granted provided       *
#  *     this copyright notice appears in all copies.  This software is       *
#  *     provided "as is" without express or implied warranty, and with       *
#  *     no claim at to its suitability for any purpose.                      *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_ARRAY_DATA_HPP
# define BOOST_PREPROCESSOR_ARRAY_DATA_HPP
#
# include <boost/preprocessor/config/config.hpp>
#
# /* BOOST_PP_ARRAY_DATA */
#
# if ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_EDG && ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_MWCW
#    define BOOST_PP_ARRAY_DATA BOOST_PP_ARRAY_DATA_I
# elif ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_MWCW
#    define BOOST_PP_ARRAY_DATA(array) BOOST_PP_ARRAY_DATA_I(array)
# else
#    define BOOST_PP_ARRAY_DATA(array) BOOST_PP_EVIL_ARRAY_DATA_D((array))
#    define BOOST_PP_EVIL_ARRAY_DATA_D(par) BOOST_PP_ARRAY_DATA_I ## par
# endif
#
# if ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_MWCW
#    define BOOST_PP_ARRAY_DATA_I(array) BOOST_PP_ARRAY_DATA_X array
# else
#    define BOOST_PP_ARRAY_DATA_I(array) BOOST_PP_ARRAY_DATA_X ## array
# endif
#
# define BOOST_PP_ARRAY_DATA_X(s, d) d
#
# endif
