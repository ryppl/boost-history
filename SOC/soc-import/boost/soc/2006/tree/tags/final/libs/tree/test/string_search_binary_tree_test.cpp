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

#include <boost/tree/comparators/string.hpp>

#include <boost/multi_index/identity.hpp>

#include <string>

#include <boost/test/minimal.hpp>

//TODO: Make this a test suite.

//Words from Austern et al., p1290

//TODO: add real tests. (what is where?)
//does boost have timers? what does the austern et al one look like?
// TODO: get timings. that makes that no testcase anymore, right?
void test_normal_string_search_binary_tree()
{
	using boost::tree::searcher;
	using boost::tree::binary_tree;
	
	typedef searcher<false, binary_tree<std::string> > searcher_t;
	searcher_t my_tree;
	
	my_tree.insert("anthology");
	my_tree.insert("anagram");
	my_tree.insert("anodyne");
	my_tree.insert("anthrax");
	my_tree.insert("anteater");	
	
	//FIXME: const_iterator doesn't work properly yet
	searcher_t::iterator ci = my_tree.begin(); 
	BOOST_CHECK(*ci++ == "anagram");
	BOOST_CHECK(*ci++ == "anodyne");
	BOOST_CHECK(*ci++ == "anteater");
	BOOST_CHECK(*ci++ == "anthology");
	BOOST_CHECK(*ci++ == "anthrax");
	BOOST_CHECK(ci == my_tree.end());
}

void test_optimized_string_search_binary_tree()
{
	
	using boost::tree::searcher;
	using boost::tree::binary_tree;
	using boost::multi_index::identity;
	using boost::tree::container_lexicographical_compare;	

	typedef searcher<false, binary_tree<std::string>, 
					 identity<std::string>,
					 container_lexicographical_compare<std::string, std::string>
					> searcher_t;
	searcher_t my_tree;
	
	my_tree.insert("anthology");
	my_tree.insert("anagram");
	my_tree.insert("anodyne");
	my_tree.insert("anthrax");
	my_tree.insert("anteater");
	
	//FIXME: const_iterator doesn't work properly yet
	searcher_t::iterator ci = my_tree.begin(); 
	BOOST_CHECK(*ci++ == "anagram");
	BOOST_CHECK(*ci++ == "anodyne");
	BOOST_CHECK(*ci++ == "anteater");
	BOOST_CHECK(*ci++ == "anthology");
	BOOST_CHECK(*ci++ == "anthrax");
	BOOST_CHECK(ci == my_tree.end());
}


int test_main(int, char* [])
{
	test_normal_string_search_binary_tree();
	test_optimized_string_search_binary_tree();
	return 0;
}
