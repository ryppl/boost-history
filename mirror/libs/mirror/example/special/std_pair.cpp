/**
 * \file examples/special/std_pair.cpp
 * 
 * Example showing reflection of std::pairs
 *
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

#include <boost/mirror/meta_types/std_pair.hpp>
#include <boost/mirror/meta_classes/std_pair.hpp>

int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	//
	typedef pair<const int*, double [321]> T1;
	typedef pair<float, const ::std::string&> T2;
	typedef pair<T1 const volatile, T2 volatile const> T3;
	typedef pair<T2, T1 volatile> T4;
	typedef pair<T3 const * volatile *, const T4&> T;
	//
	typedef BOOST_MIRROR_REFLECT_CLASS(T) meta_T;
	//
	bcout << "The type name length = " << meta_T::base_name_length << " characters" << endl;
	bcout << "---------------------------------------------------" << endl;
	bcout << "The type name is: "<< meta_T::base_name() << endl;
	bcout << "---------------------------------------------------" << endl;
	bcout << "The full type name length = " << meta_T::full_name_length << " characters" << endl;
	bcout << "---------------------------------------------------" << endl;
	bcout << "The full type name is: "<< meta_T::full_name() << endl;
	bcout << "---------------------------------------------------" << endl;
	bcout << "The class has "<< meta_T::all_attributes::size::value << " members" << endl;
	bcout << "---------------------------------------------------" << endl;
	//
	return 0;
}

