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

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/searcher.hpp>

#include <boost/numeric/interval.hpp>
//#include <boost/numeric/interval/compare/lexicographic.hpp>
#include <boost/multi_index/identity.hpp>


#include <boost/test/minimal.hpp>

//TODO: Make this a test suite.

//TODO: add real tests. (what is where?)
// test overlaps stuff.

// Interval example data as in GCC libstdc++'s pb_ds

// FIXME: Still buggy.
// Is the following really what we want?
using namespace boost::numeric::interval_lib::compare::lexicographic;
using boost::numeric::interval_lib::cerlt;
using boost::numeric::interval;

template <typename T>
struct cerless {
	inline bool operator() (T const& a, T const& b)
	{
		return boost::numeric::interval_lib::cerlt(a,b);
	}
};

void test_interval_search_binary_tree()
{
	using boost::tree::searcher;
	using boost::tree::binary_tree;
	
	using boost::multi_index::identity;
	
	typedef searcher<false, binary_tree<interval<int> >, identity<interval<int> >,
					 cerless<interval<int> > > searcher_t;
	searcher_t my_tree;
	
	my_tree.insert(interval<int>(20,36));	
	my_tree.insert(interval<int>( 3,41));
	my_tree.insert(interval<int>(10,15));	
	my_tree.insert(interval<int>( 0, 1));	
	my_tree.insert(interval<int>(29,99));	

	searcher_t::iterator ci = my_tree.begin(); 
	BOOST_CHECK(*ci++ == interval<int>( 0, 1));
//	BOOST_CHECK(*ci++ == interval<int>( 3,41));
	BOOST_CHECK(*ci++ == interval<int>(10,15));
//	BOOST_CHECK(*ci++ == interval<int>(20,36));
//	BOOST_CHECK(*ci++ == interval<int>(29,99));
//	BOOST_CHECK(ci == my_tree.end());
	
}


int test_main(int, char* [])
{
	test_interval_search_binary_tree();
	return 0;
}
