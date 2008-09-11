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

#include <boost/mirror/algorithms.hpp>
#include <boost/mirror/functions.hpp>

#include <boost/mirror/meta_types/boost_tuple.hpp>
#include <boost/mirror/meta_types/std_pair.hpp>
#include <boost/mirror/meta_types/std_vector.hpp>
#include <boost/mirror/meta_types/std_list.hpp>
#include <boost/mirror/meta_types/std_map.hpp>
#include <boost/mirror/meta_types/std_set.hpp>

#include <boost/mirror/meta_classes/boost_tuple.hpp>
#include <boost/mirror/meta_classes/boost_fusion_vector.hpp>

template <class Class>
class attrib_value_printer
{
public:
                attrib_value_printer(Class& _inst)
                : inst(_inst){ }
	
                template <typename MetaAttribute>
                void operator()(MetaAttribute ma) const
                {
                        using namespace ::std;
                        using namespace ::boost;
                        using namespace ::boost::mirror;
						cts::bcout() <<
                                " " <<
                                ma.base_name() <<
                                " = " <<
                                ma.get(inst) <<
                                endl;
                }
private:
	Class& inst;
};

struct str_printer
{
	void operator()(const ::boost::cts::bstring& str) const
	{
		::boost::cts::bcout() << str << ", ";
	}
	~str_printer(void)
	{
		::boost::cts::bcout() << ::std::endl;
	}
};

int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	cts::bostream& bcout = cts::bcout();
	//
	typedef int (*A)(double);
	typedef A (*B)(string);
	typedef B (C)(char, wchar_t);
	typedef A D[2][3];
	typedef tuple<int, double, const string *> T1;
	typedef tuple<const A, volatile B, C&, D> T2;
	typedef pair<T1, T2> T3;
	typedef tuple<void*, const wstring& , const string&> T4;
	typedef tuple<char, wchar_t, short int const> T5;
	typedef pair<T4, T5> T6;
	typedef vector<tuple<T1, T2, T3, T4, T5, T6> > T7;
	typedef set<map<list<T1>, T7> > T;
	//
	typedef BOOST_MIRRORED_CLASS(T) meta_T;
	//
	//
	//
	bcout << "---------------------------------------------------" << endl;
	bcout << "The type name is: "<< meta_T::base_name() << endl;
	bcout << "---------------------------------------------------" << endl;
	bcout << "The full type name is: "<< meta_T::full_name() << endl;
	bcout << "---------------------------------------------------" << endl;
	//
	T1 t1(12, 34.56, 0);
	typedef BOOST_MIRRORED_CLASS(T1) meta_T1;
	bcout << "---------------------------------------------------" << endl;
	bcout << "The full type name is: "<< meta_T1::full_name() << endl;
	bcout << "---------------------------------------------------" << endl;
	bcout << "The class has "<< meta_T1::all_attributes::size::value << " members" << endl;
	bcout << "---------------------------------------------------" << endl;
	bcout << meta_T1::all_attributes::get_name(mpl::int_<0>(), mpl::false_(), cts::bchar_traits()) << " = ";
	bcout << meta_T1::all_attributes::get(t1, mpl::int_<0>()) << endl;
	bcout << meta_T1::all_attributes::get_name(mpl::int_<1>(), mpl::false_(), cts::bchar_traits()) << " = ";
	bcout << meta_T1::all_attributes::get(t1, mpl::int_<1>()) << endl;
	bcout << meta_T1::all_attributes::get_name(mpl::int_<2>(), mpl::false_(), cts::bchar_traits()) << " = ";
	bcout << meta_T1::all_attributes::get(t1, mpl::int_<2>()) << endl;
	bcout << "---------------------------------------------------" << endl;
	//
	meta_T1::all_attributes::set(t1, mpl::int_<0>(), 23);
	meta_T1::all_attributes::set(t1, mpl::int_<1>(), 45.67);
	//
	assert(meta_T1::all_attributes::get(t1, mpl::int_<0>()) == tuples::get<0>(t1));
	assert(meta_T1::all_attributes::get(t1, mpl::int_<1>()) == tuples::get<1>(t1));
	//
	//
	fusion::vector<int, int, int, int, int, int, int, int, int, int> x(0,1,2,3,4,5,6,7,8,9);
	typedef BOOST_MIRRORED_CLASS(BOOST_TYPEOF(x)) meta_X;
	attrib_value_printer<meta_X::reflected_type> p(x);
	//
	bcout << "The type name is: "<< meta_X::base_name() << endl;
	bcout << "The class has "<< meta_X::all_attributes::size::value << " members" << endl;
	bcout << "---------------------------------------------------" << endl;
	for_each<meta_X::all_attributes>(p);
	bcout << "---------------------------------------------------" << endl;
	reverse_for_each<meta_X::all_attributes>(p);
	bcout << "---------------------------------------------------" << endl;
	for_each<meta_X::all_attributes>(cref(select_base_name()), cref(str_printer()));
	bcout << "---------------------------------------------------" << endl;
	for_each<
		begin<meta_X::all_attributes>::type,
		end<meta_X::all_attributes>::type
	>(cref(select_full_name()), cref(str_printer()));
	bcout << "---------------------------------------------------" << endl;
	reverse_for_each<
		begin<meta_X::all_attributes>::type,
		end<meta_X::all_attributes>::type
	>(cref(select_base_name()), cref(str_printer()));
	bcout << "---------------------------------------------------" << endl;
	bcout << "Finished" << endl;

	return 0;
}

