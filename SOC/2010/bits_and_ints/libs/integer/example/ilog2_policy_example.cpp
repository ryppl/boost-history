//  boost::ilog2 example file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <boost/integer/ilog2.hpp>

namespace boost { namespace math { namespace policies {
	
template <class T>
T user_indeterminate_result_error(const char* function, const char* message, const T& val)
{
	std::cout << "User Error on function: " << function << " retunrning -2" << std::endl;
	
	(void)val;
	
	// Returning a custom value
	return -2;
}
	
} } }

int main()
{
	// Checking default behavior
	std::cout << boost::ilog2(13265u) << std::endl; // 13
	std::cout << boost::ilog2(0u) << std::endl; // -1
	
	using namespace boost::math::policies;
	policy< indeterminate_result_error<throw_on_error> > throw_pol;
	policy< indeterminate_result_error<errno_on_error> > errno_pol;
	policy< indeterminate_result_error<ignore_error> >   ignore_pol;
	policy< indeterminate_result_error<user_error> >     user_pol;
	
	
	// Checking throw_on_error policy (throw a std::domain_error exception on error)
	try {
		std::cout << boost::ilog2(13265u, throw_pol) << std::endl; // 13
	}
	catch (std::domain_error& e) {
		std::cout << e.what() << std::endl; // never comes here
	}
	catch (...) {
		// never comes here
	}
	
	try {
		// Here an exception is threw
		std::cout << boost::ilog2(0u, throw_pol) << std::endl;
	}
	catch (std::domain_error& e) {
		std::cout << e.what() << std::endl; // Error in function boost::ilog2(i): ilog2 is indeterminate for value 0, returning -1
	}
	catch (...) {
		// never comes here
	}
	
	// Checking errno_on_error policy (ilog2(0) sets errno as EDOM on error)
	std::cout << boost::ilog2(13265u, errno_pol) << std::endl; // 13
		
	errno = 0;
	std::cout << boost::ilog2(0u, errno_pol) << std::endl; // -1
	std::cout << (errno == EDOM) << std::endl; // 1 (or true)
	
	// Checking ignore_error policy (ilog2(0) returns -1 on error)
	std::cout << boost::ilog2(13265u, ignore_pol) << std::endl; // 13
	std::cout << boost::ilog2(0u, ignore_pol) << std::endl; // -1
	
	// Checking user_error policy (as we defined at the beginning of this file, 
	//		on error, our user function prints an error message and returns -2)
	std::cout << boost::ilog2(13265u, user_pol) << std::endl; // 13
	
	// User Error on function: boost::ilog2(%1%) retunrning -2
	// -2
	std::cout << boost::ilog2(0u, user_pol) << std::endl; 
		
	return 0;
}