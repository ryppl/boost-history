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

#include <boost/test/minimal.hpp>

#include <boost/tree/searcher.hpp>

#include <vector>

//TODO: Make this a test suite.


static void search_ordered_vector_test()
{
	using boost::tree::searcher;
	using std::vector;
	
	typedef searcher<false, vector<int> > searcher_t;
	searcher_t my_searcher;

	searcher_t::cursor c, c1, c2, c3, c4, c5;
	
	c = my_searcher.end();

	c1 = my_searcher.insert(c, 18);
	c1 = my_searcher.insert(c1, 7);		//FIXME: crash if pos hint == c
	c1 = my_searcher.insert(c1, 6);
	c1 = my_searcher.insert(c1, 8);

	c1 = my_searcher.begin();
	BOOST_CHECK(*c1++ == 6);
	BOOST_CHECK(*c1++ == 7);
	BOOST_CHECK(*c1++ == 8);
	BOOST_CHECK(*c1++ == 18);
	BOOST_CHECK(c1 == my_searcher.end());
}

//boost::unit_test::test_suite*
//init_unit_test_suite( int argc, char* argv[] )
//{
//    boost::unit_test::test_suite* ordered_vector_test = 
//    		BOOST_TEST_SUITE( "Ordered vector test" );
//
//    ordered_vector_test->add( BOOST_TEST_CASE( &search_ordered_vector_test ) );
//
//    return ordered_vector_test;
//}


int test_main(int, char* [])
{
	search_ordered_vector_test();
	return 0;
}
