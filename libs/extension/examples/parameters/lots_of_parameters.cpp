/* (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "lots_of_parameters_interface.hpp"
#include <boost/extension/factory_map.hpp>

#include <iostream>

class six_parameters : public lots_of_parameters_interface
{
public:
	six_parameters(bool b, unsigned int i, char c, std::string s, A a, boost::shared_ptr<A> ptr_a) 
		: lots_of_parameters_interface(b, i, c, s, a, ptr_a)
	{
		std::cout << "Constructor called." << std::endl << std::endl;
		std::cout << "Parameters:" << std::endl;
		std::cout << "\tbool: " << b << std::endl;
		std::cout << "\tunsigned int: " << i << std::endl;
		std::cout << "\tchar: " << c << std::endl;
		std::cout << "\tstring: " << s << std::endl;
		std::cout << "\tA.i: " << a.i << std::endl;
		std::cout << "\tptr_a->i: " << ptr_a->i << std::endl;
		std::cout << std::endl;
	}

	virtual ~six_parameters(void) {}
};


extern "C" void BOOST_EXTENSION_EXPORT_DECL extension_export(boost::extensions::factory_map & fm)
{
	fm.add< six_parameters, lots_of_parameters_interface, int, bool, unsigned int, char, std::string, A, boost::shared_ptr<A> >(6);
}
