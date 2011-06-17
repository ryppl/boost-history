//  Boost checks/isbn.hpp header file  ------------------------------------//
//  (C) Copyright Murilo Adriano Vasconcelos 2011.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_ISBN_INCLUDED
#define BOOST_ISBN_INCLUDED

#include <string>

namespace boost {

/**
 * This function checks if a `isbn' is a valid ISBN 
 */
bool is_isbn(const std::string& isbn)
{
	if (isbn.size() != 10) return false;
	
	int check = 0;
	for (int i = 0; i < 9; ++i) {
		if (!isdigit(isbn[i])) {
			return false;
		}
		
		check += (10 - i) * isbn[i];
	}
	
	check += (isbn[9] == 'x' || isbn[9] == 'X') ? 10 : isbn[9];
	
	return (check % 11 == 0);
}

/**
 * This function computes the check digit for a given ISBN in `isbn'
 */
char isbn_check_digit(const std::string& isbn)
{	
	int check = 0;
	for (int i = 0; i < 9; ++i) {
		if (!isdigit(isbn[i])) {
			return false;
		}
		
		check += (10 - i) * isbn[i];
	}
	
	check += (isbn[9] == 'x' || isbn[9] == 'X') ? 10 : isbn[9];
	check %= 11;
	
	if (check == 0) {
		return '0';
	}
	check = 11 - check;
	
	return (check == 10) ? 'X' : (check + '0');
}

} // namespace boost

#endif