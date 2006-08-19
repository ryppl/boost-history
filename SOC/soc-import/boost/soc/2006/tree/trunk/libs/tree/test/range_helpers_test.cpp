//  Copyright (c) 2006, Bernhard Reiter
//
//  This code may be used under either of the following two licences:
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy 
//  of this software and associated documentation files (the "Software"), to deal 
//  in the Software without restriction, including without limitation the rights 
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
//  copies of the Software, and to permit persons to whom the Software is 
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in 
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
//  THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/detail/range_helpers.hpp>

#include <boost/test/minimal.hpp>

#include <vector>

//TODO: Make this a test suite.

void test_binary_lower_bound()
{
	using boost::tree::binary_lower_bound;
	
	std::vector<int> vec(1, 9);
	
	std::vector<int>::const_iterator ci = 
		binary_lower_bound(vec.begin(), vec.end(), 9, std::less<int>());
	BOOST_CHECK(ci == vec.begin());
	
	ci = binary_lower_bound(vec.begin(), vec.end(), 7, std::less<int>());
	BOOST_CHECK(ci == vec.begin());
	
	ci = binary_lower_bound(vec.begin(), vec.end(), 17, std::less<int>());
	BOOST_CHECK(ci == vec.end());
}

void test_binary_upper_bound()
{
	using boost::tree::binary_upper_bound;
	std::vector<int> vec(1, 9);
	
	std::vector<int>::const_iterator ci = 
		binary_upper_bound(vec.begin(), vec.end(), 7, std::less<int>());
	BOOST_CHECK(ci == vec.begin());
	
	ci = binary_upper_bound(vec.begin(), vec.end(), 9, std::less<int>());
	BOOST_CHECK(ci == vec.end());
	
	ci = binary_upper_bound(vec.begin(), vec.end(), 17, std::less<int>());
	BOOST_CHECK(ci == vec.end());
}

static std::vector<int>&
vec()
{
	static std::vector<int> myvec(7);
	myvec[0] = -6;
	myvec[1] = -2;
	myvec[2] = -2;
	myvec[3] =  0;
	myvec[4] =  7;
	myvec[5] =  7;
	myvec[6] = 12;
	return myvec;
}

void test_linear_lower_bound()
{
	using boost::tree::linear_lower_bound;
	
	std::vector<int>::const_iterator ci = 
		linear_lower_bound(vec().begin(), vec().end(), 7, std::less<int>());
	BOOST_CHECK(ci == (vec().begin() + 4));
	
	ci = linear_lower_bound(vec().begin(), vec().end(), 9, std::less<int>());
	BOOST_CHECK(ci == (vec().begin() + 6));
	
	ci = linear_lower_bound(vec().begin(), vec().end(), 17, std::less<int>());
	BOOST_CHECK(ci == vec().end());
}

void test_linear_upper_bound()
{
	using boost::tree::linear_upper_bound;
	
	std::vector<int>::const_iterator ci = 
		linear_upper_bound(vec().begin(), vec().end(), 7, std::less<int>());
	BOOST_CHECK(ci == (vec().begin() + 6));
	
	ci = linear_upper_bound(vec().begin(), vec().end(), 9, std::less<int>());
	BOOST_CHECK(ci == (vec().begin() + 6));
	
	ci = linear_upper_bound(vec().begin(), vec().end(), 17, std::less<int>());
	BOOST_CHECK(ci == vec().end());
}

int test_main(int, char* [])
{
	test_binary_lower_bound();
	test_binary_upper_bound();
	
	test_linear_lower_bound();
	test_linear_upper_bound();
	return 0;
}
