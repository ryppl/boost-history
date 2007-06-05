/* (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "../word.hpp"
#include <string>

// info class for word interface
struct word_description
{
	std::string language;
	std::string english_translation;

	word_description(std::string language, std::string english_translation)
		: language(language), english_translation(english_translation) {}
};
