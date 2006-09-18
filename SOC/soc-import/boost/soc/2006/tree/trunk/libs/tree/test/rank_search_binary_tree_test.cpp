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

//#include <boost/test/unit_test.hpp>
#include <boost/test/minimal.hpp>

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/balancers/unbalanced.hpp>
#include <boost/tree/augmentors/rank.hpp>

#include <boost/tree/searcher.hpp>
#include <boost/tree/algorithm.hpp>

#include "helpers.hpp"

//TODO: Make this a test suite.


void test_rank_search_binary_tree()
{
	using namespace boost::tree;
//	
//	typedef binary_tree<int, balancers::unbalanced, augmentors::rank<> > tree_t;
//	typedef searcher<false, tree_t> searcher_t;
//	searcher_t my_searcher;
//	
//	test_data_searcher(my_searcher);
//	tree_t& my_tree = my_searcher.container();
//	
//	tree_t::cursor c = select(my_tree, 3);
//	BOOST_CHECK(*c < 14);

	typedef binary_tree<int, balancers::unbalanced, augmentors::rank<> > tree_t;
	typedef test_searcher<false, tree_t> searcher_t;
	searcher_t my_searcher;
	
	test_data_searcher(my_searcher);
	tree_t& my_tree = my_searcher.container();

	tree_t::cursor c = select_rank(my_tree, 3);
	//BOOST_CHECK(*c < 14);

}

//boost::unit_test::test_suite*
//init_unit_test_suite( int argc, char* argv[] )
//{
//    boost::unit_test::test_suite* rank_search_test = 
//    		BOOST_TEST_SUITE( "Key search binary vector test" );
//
//    rank_search_test->add( BOOST_TEST_CASE( &rank_search_binary_tree_test ) );
//
//    return rank_search_test;
//}

int test_main(int, char* [])
{
	//test_rank_search_binary_tree();

	return 0;
}
