//  boost::rotate_left and boost::rotate_right example file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#include <iostream>
#include <boost/integer/rotate.hpp>

int main()
{
	using boost::rotate_left;
	using boost::rotate_right;
	
	std::cout << std::hex << std::uppercase;
	
	std::cout << rotate_left(0xABCDEF12u, 24) << std::endl; // 12ABCDEF
	std::cout << rotate_left(0x12345678u,  4) << std::endl; // 23456781
	std::cout << rotate_left(0x0FF00000u,  6) << std::endl; // FC000003
	std::cout << rotate_left(0x00000000u, 31) << std::endl; // 0
	std::cout << rotate_left(0xF0F0F0F0u,  4) << std::endl; // F0F0F0F
	
	std::cout << rotate_right(0xABCDEF12u,  8) << std::endl; // 12ABCDEF
	std::cout << rotate_right(0x0000000Fu,  1) << std::endl; // 80000007
	std::cout << rotate_right(0xFF00FF00u,  8) << std::endl; // FF00FF
	std::cout << rotate_right(0xAABBCCDDu, 16) << std::endl; // CCDDAABB
	std::cout << rotate_right(0xFFFFFFFFu, 31) << std::endl; // FFFFFFFF
}