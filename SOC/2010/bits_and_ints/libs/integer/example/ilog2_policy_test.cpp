//  boost::ilog2 test file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#define BOOST_TEST_MAIN
#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <boost/test/included/unit_test.hpp>
#include <boost/integer/ilog2.hpp>

namespace boost { namespace math { namespace policies {

template <class T>
T user_indeterminate_result_error(const char* function, const char* message, const T& val)
{
	std::cout << "User Error on function: " << function 
		<< " with message: " << message << std::endl;
	
	(void)val;
	
	// Returning a custom value
	return -2;
}

} } }
	
BOOST_AUTO_TEST_CASE( test )
{
	// Checking default behavior
	BOOST_CHECK_EQUAL(boost::ilog2(0u), -1);
	
	using namespace boost::math::policies;
	policy< indeterminate_result_error<throw_on_error> > throw_pol;
	policy< indeterminate_result_error<errno_on_error> > errno_pol;
	policy< indeterminate_result_error<ignore_error> >   ignore_pol;
	policy< indeterminate_result_error<user_error> >     user_pol;
	
	
	// Checking throw_on_error policy
	BOOST_CHECK_THROW(boost::ilog2(0u, throw_pol) , std::domain_error);
	
	// Checking errno_on_error policy
	errno = 0;
	boost::ilog2(0u, errno_pol);
	BOOST_CHECK_EQUAL(errno, EDOM);
	
	// Checking ignore_error policy
	BOOST_CHECK_EQUAL(boost::ilog2(0u, ignore_pol), -1);
	
	// Checking user_error policy
	BOOST_CHECK_EQUAL(boost::ilog2(0u, user_pol), -2);
}
