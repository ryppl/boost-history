// Boost.Print library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
//#include <boost/print.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <map>

template<typename T>
void print(T t, std::stringstream &str_out)
{
	str_out << "!!! fail test !!!";
}

BOOST_AUTO_TEST_CASE( basic_map_test )
{
	std::stringstream str_out;
	
	std::map<int,std::string> mis;
	print(mis, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[]");
	
	str_out.str("");
	
	mis.insert(std::make_pair(1, "first"));
	print(mis, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[1:first]");
	
	str_out.str("");
	
	mis.insert(std::make_pair(2, "second"));
	mis.insert(std::make_pair(3, "third"));
	print(mis, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[1:first, 2:second, 3:third]");
}

BOOST_AUTO_TEST_CASE( vector_in_map_test )
{
	std::stringstream str_out;
	
	std::vector<int> vi;
	vi.push_back(1);
	vi.push_back(2);
	vi.push_back(3);
	
	std::map<int,std::vector<int> > mivi;
	print(mivi, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[]");
	
	str_out.str("");
	
	mivi.insert(std::make_pair(1, vi));
	print(mivi, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[1:[1,2,3]]");
	
	str_out.str("");
	
	mivi.insert(std::make_pair(2, vi));
	mivi.insert(std::make_pair(3, vi));
	print(mivi, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[1:[1,2,3], 2:[1,2,3], 3:[1,2,3]]");
}