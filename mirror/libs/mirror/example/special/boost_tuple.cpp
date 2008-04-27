/**
 * \file examples/special/boost_tuple.cpp
 * 
 * Example showing reflection of boost::tuples 
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#include <boost/char_type_switch/string.hpp>
#include <boost/char_type_switch/iostream.hpp>

#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/meta_class.hpp>

#include <boost/mirror/utils/name_to_stream.hpp>

#include <boost/mirror/meta_types/std_pair.hpp>
#include <boost/mirror/meta_types/boost_tuple.hpp>
#include <boost/mirror/meta_types/std_vector.hpp>
#include <boost/mirror/meta_types/std_list.hpp>
#include <boost/mirror/meta_types/std_map.hpp>
#include <boost/mirror/meta_types/std_set.hpp>

int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	// hold on to yer butts ;)
	typedef tuple<int, double, const string*> T1;
	typedef tuple<const bool, volatile float, void * const, char> T2;
	typedef pair<T1, T2> T3;
	typedef tuple<void*, const wstring& , const string&> T4;
	typedef tuple<char, wchar_t, short int const> T5;
	typedef pair<T4, T5> T6;
	typedef vector<tuple<T1, T2, T3, T4, T5, T6> > T7;
	typedef set<map<list<T1>, T7> > T;
	//
	typedef BOOST_MIRROR_REFLECT_TYPE(T) meta_T;
	//
	//
	bcout << "The type name length = " << meta_T::base_name_length << " characters" << endl;
	bcout << "---------------------------------------------------" << endl;
	bcout << "The type name is: "<< meta_T::base_name() << endl;
	bcout << "---------------------------------------------------" << endl;
	bcout << "The full type name length = " << meta_T::full_name_length << " characters" << endl;
	bcout << "---------------------------------------------------" << endl;
	bcout << "The full type name is: "<< meta_T::full_name() << endl;
	bcout << "---------------------------------------------------" << endl;
	//
	return 0;
}

