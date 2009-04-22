/**
 * \file examples/traversal/sample_visitor.cpp
 *
 * Example of class structure deep and flat traversal
 *
 * NOTE: if You are not familiar with registration
 * and reflection, You should probably 
 * see examples in examples/registering/ first.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/char_type_switch/iostream.hpp>

#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/meta_class.hpp>

#include <boost/mirror/meta_types/std/pair.hpp>
#include <boost/mirror/meta_types/boost/tuples/tuple.hpp>
#include <boost/mirror/meta_classes/boost/tuples/tuple.hpp>

#include <boost/mirror/meta_path/ancestors.hpp>
#include <boost/mirror/meta_path/size.hpp>

#include <boost/mirror/visitors/sample.hpp>
#include <boost/mirror/traversal.hpp>

/** First declare some namespaces and classes
 */

namespace Test {

	struct A 
	{
		long l;
	};

	struct B : virtual A
	{
		int i;
	};

	struct C : virtual A
	{
		double d;
	};

	struct D : virtual A
	{
		short s;
	};

	struct E : B, C, D
	{
		float f;
	};

	struct F : virtual E
	{
		bool b;
	};

	struct G : virtual E
	{
		char c;
	};

	struct H : F, G
	{
		wchar_t w;
		unsigned short u;
	};


} // namespace Test

namespace boost { 
namespace mirror {

/** Register the Test namespace
 */
BOOST_MIRROR_REG_NAMESPACE((Test))

/** Register the classes
 */
// class with no base classes
BOOST_MIRROR_QREG_CLASS_NO_BASE(::Test, A, (l))
// class with only virtual base classes
BOOST_MIRROR_QREG_CLASS_V_BASES(::Test, B, (::Test::A), (i))
BOOST_MIRROR_QREG_CLASS_V_BASES(::Test, C, (::Test::A), (d))
BOOST_MIRROR_QREG_CLASS_V_BASES(::Test, D, (::Test::A), (s))
// class with both regular base classes and member attributes
BOOST_MIRROR_QREG_CLASS(::Test, E, (::Test::B)(::Test::C)(::Test::D), (f))
BOOST_MIRROR_QREG_CLASS_V_BASES(::Test, F, (::Test::E), (b))
BOOST_MIRROR_QREG_CLASS_V_BASES(::Test, G, (::Test::E), (c))
BOOST_MIRROR_QREG_CLASS(::Test, H, (::Test::F)(::Test::G), (w)(u))

} // namespace mirror
} // namespace boost


int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	cts::bostream &bcout = cts::bcout();
	//
	using namespace ::Test;
	//
	typedef BOOST_MIRRORED_CLASS(H) meta_H;
	H h;
	h.l = 1234567890;
	h.i = 123;
	h.d = 456.7890123;
	h.s = 456;
	h.f = 78.9f;
	h.b = false;
	h.c = '1';
	h.w = L'2';
	//
	bcout << "--------------------------------------------" << endl;
	deep_traversal_of<meta_H>::accept(sample_visitor<true>(), &h);
	bcout << "--------------------------------------------" << endl;
	flat_traversal_of<meta_H>::accept(sample_visitor<true>(), &h);
	bcout << "--------------------------------------------" << endl;
	//
	//
	typedef pair<int volatile, long const> T1; 
	typedef pair<double * const *, float> T2; 
	typedef pair<char const [12], wchar_t [4]> T3; 
	typedef pair<bool, bool> T4; 
	typedef pair<T1, T2> T5; 
	typedef pair<T3, T4> T6; 
	typedef pair<T5, T6> T7; 
	typedef tuple<T1, T2, T3, T4, T5, T6, T7> T;
	typedef BOOST_MIRRORED_CLASS(T) meta_T;
	//
	//
	bcout << "--------------------------------------------" << endl;
	deep_traversal_of<meta_T>::accept(sample_visitor<false>());
	bcout << "--------------------------------------------" << endl;
	flat_traversal_of<meta_T>::accept(sample_visitor<false>());
	bcout << "--------------------------------------------" << endl;
	//
	return 0;
}
