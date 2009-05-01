/**
 * \file boost/char_type_switch/char.hpp
 * Narrow/Wide character type switching.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_CHAR_TYPE_SWITCH_CHAR_HPP
#define BOOST_CHAR_TYPE_SWITCH_CHAR_HPP


#include <boost/config.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/char_type_switch/char_types.hpp>

namespace boost {
namespace cts {


// define the bchar type
typedef BOOST_CTS_BCHAR() bchar;

// define macro expanding into a compile time const length
// of the given string literal
#define BOOST_CTS_LIT_LENGTH(STR) \
	((sizeof(BOOST_CTS_LIT(STR))/sizeof(::boost::cts::bchar))-1)

#define BOOST_CTS_LIT_CHAR_T(CHAR_T, STR) \
	BOOST_PP_CAT(BOOST_CTS_PREFIX_CHAR_T(CHAR_T), STR)
// define macro for string literal type selection
#define BOOST_CTS_LIT(STR) BOOST_CTS_LIT_CHAR_T(BOOST_CTS_BCHAR(), STR)
//

#define BOOST_CTS_STRINGIZE_CHAR_T(CHAR_T, PARAM) \
	BOOST_CTS_DO_STRINGIZE_CHAR_T(CHAR_T, PARAM)
#define BOOST_CTS_DO_STRINGIZE_CHAR_T(CHAR_T, PARAM) \
	BOOST_CTS_LIT_CHAR_T(CHAR_T, #PARAM)


// stringization macro for bchar based literals
#define BOOST_CTS_STRINGIZE(PARAM) \
	BOOST_CTS_STRINGIZE_CHAR_T(BOOST_CTS_BCHAR(), PARAM)

} // namespace cts
} // namespace boost

#endif //include guard


