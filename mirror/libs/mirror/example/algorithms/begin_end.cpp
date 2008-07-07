/**
 * \file examples/special/boost_tuple.cpp
 * 
 * Example showing meta-object iterator usage 
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
#include <boost/mirror/meta_classes/boost_tuple.hpp>

#include <boost/static_assert.hpp>
#include <boost/mpl/modulus.hpp>
#include <boost/type_traits/is_integral.hpp> 

namespace boost {
namespace mirror {

template <class Modulus>
struct is_even_attrib_templ
{
	template <class MetaAttribute>
	struct apply : mpl::equal_to<
		mpl::modulus<
			typename MetaAttribute::position,
			mpl::int_<2>
		>,
		Modulus
	>::type	{ };
};

typedef is_even_attrib_templ<mpl::int_<0> > is_even_attrib;
typedef is_even_attrib_templ<mpl::int_<1> > is_odd_attrib;

struct is_integral_attrib
{

	template <class MetaAttribute>
	struct apply : is_integral<
		typename MetaAttribute::type
	>::type { };
};

} // namespace mirror
} // namespace boost

int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	typedef tuple<bool, unsigned char, char, wchar_t, short, int, long, float, double> T;
	//
	typedef BOOST_MIRRORED_CLASS(T) meta_T;
	//
	//
	BOOST_STATIC_ASSERT((iterator_not_equal<
		begin<meta_T::all_attributes>::type,
		end<meta_T::all_attributes>::type
	>::value));
	BOOST_STATIC_ASSERT((iterator_less<
		begin<meta_T::all_attributes>::type,
		end<meta_T::all_attributes>::type
	>::value));
	BOOST_STATIC_ASSERT((iterator_greater<
		end<meta_T::all_attributes>::type,
		begin<meta_T::all_attributes>::type
	>::value));
	BOOST_STATIC_ASSERT((iterator_less_equal<
		begin<meta_T::all_attributes>::type,
		end<meta_T::all_attributes>::type
	>::value));
	BOOST_STATIC_ASSERT((iterator_greater_equal<
		end<meta_T::all_attributes>::type,
		begin<meta_T::all_attributes>::type
	>::value));
	//
	BOOST_STATIC_ASSERT((iterator_not_equal<
		rbegin<meta_T::all_attributes>::type,
		rend<meta_T::all_attributes>::type
	>::value));
	BOOST_STATIC_ASSERT((iterator_less<
		rbegin<meta_T::all_attributes>::type,
		rend<meta_T::all_attributes>::type
	>::value));
	BOOST_STATIC_ASSERT((iterator_greater<
		rend<meta_T::all_attributes>::type,
		rbegin<meta_T::all_attributes>::type
	>::value));
	BOOST_STATIC_ASSERT((iterator_less_equal<
		rbegin<meta_T::all_attributes>::type,
		rend<meta_T::all_attributes>::type
	>::value));
	BOOST_STATIC_ASSERT((iterator_greater_equal<
		rend<meta_T::all_attributes>::type,
		rbegin<meta_T::all_attributes>::type
	>::value));
	//
	//
	typedef begin<meta_T::all_attributes>::type i_0;
	bcout << deref<i_0>::type::base_name() << ',' ;
	typedef next<i_0>::type i_1;
	bcout << deref<i_1>::type::base_name() << ',' ;
	typedef next<i_1>::type i_2;
	bcout << deref<i_2>::type::base_name() << ',' ;
	typedef next<i_2>::type i_3;
	bcout << deref<i_3>::type::base_name() << ',' ;
	typedef next<i_3>::type i_4;
	bcout << deref<i_4>::type::base_name() << ',' ;
	typedef next<i_4>::type i_5;
	bcout << deref<i_5>::type::base_name() << ',' ;
	typedef next<i_5>::type i_6;
	bcout << deref<i_6>::type::base_name() << ',' ;
	typedef next<i_6>::type i_7;
	bcout << deref<i_7>::type::base_name() << ',' ;
	typedef next<i_7>::type i_8;
	bcout << deref<i_8>::type::base_name() << endl ;
	typedef next<i_8>::type i_end;
	//
	BOOST_STATIC_ASSERT((iterator_equal<
		end<meta_T::all_attributes>::type, i_end
	>::value));
	//
	bcout << "---------------------------------------------------" << endl;
	typedef prior<i_end>::type j_8;
	bcout << deref<j_8>::type::base_name() << ',' ;
	typedef prior<j_8>::type j_7;
	bcout << deref<j_7>::type::base_name() << ',' ;
	typedef prior<j_7>::type j_6;
	bcout << deref<j_6>::type::base_name() << ',' ;
	typedef prior<j_6>::type j_5;
	bcout << deref<j_5>::type::base_name() << ',' ;
	typedef prior<j_5>::type j_4;
	bcout << deref<j_4>::type::base_name() << ',' ;
	typedef prior<j_4>::type j_3;
	bcout << deref<j_3>::type::base_name() << ',' ;
	typedef prior<j_3>::type j_2;
	bcout << deref<j_2>::type::base_name() << ',' ;
	typedef prior<j_2>::type j_1;
	bcout << deref<j_1>::type::base_name() << ',' ;
	typedef prior<j_1>::type j_0;
	bcout << deref<j_0>::type::base_name() << endl ;
	typedef j_0 j_begin;
	//
	BOOST_STATIC_ASSERT((iterator_equal<
		begin<meta_T::all_attributes>::type, j_begin
	>::value));
	//
	bcout << "---------------------------------------------------" << endl;
	typedef rbegin<meta_T::all_attributes>::type r_8;
	bcout << deref<r_8>::type::base_name() << ',' ;
	typedef next<r_8>::type r_7;
	bcout << deref<r_7>::type::base_name() << ',' ;
	typedef next<r_7>::type r_6;
	bcout << deref<r_6>::type::base_name() << ',' ;
	typedef next<r_6>::type r_5;
	bcout << deref<r_5>::type::base_name() << ',' ;
	typedef next<r_5>::type r_4;
	bcout << deref<r_4>::type::base_name() << ',' ;
	typedef next<r_4>::type r_3;
	bcout << deref<r_3>::type::base_name() << ',' ;
	typedef next<r_3>::type r_2;
	bcout << deref<r_2>::type::base_name() << ',' ;
	typedef next<r_2>::type r_1;
	bcout << deref<r_1>::type::base_name() << ',' ;
	typedef next<r_1>::type r_0;
	bcout << deref<r_0>::type::base_name() << endl ;
	typedef next<r_0>::type r_end;
	//
	BOOST_STATIC_ASSERT((iterator_equal<
		rend<meta_T::all_attributes>::type, r_end
	>::value));
	//
	bcout << "---------------------------------------------------" << endl;
	typedef prior<r_end>::type p_0;
	bcout << deref<p_0>::type::base_name() << ',' ;
	typedef prior<p_0>::type p_1;
	bcout << deref<p_1>::type::base_name() << ',' ;
	typedef prior<p_1>::type p_2;
	bcout << deref<p_2>::type::base_name() << ',' ;
	typedef prior<p_2>::type p_3;
	bcout << deref<p_3>::type::base_name() << ',' ;
	typedef prior<p_3>::type p_4;
	bcout << deref<p_4>::type::base_name() << ',' ;
	typedef prior<p_4>::type p_5;
	bcout << deref<p_5>::type::base_name() << ',' ;
	typedef prior<p_5>::type p_6;
	bcout << deref<p_6>::type::base_name() << ',' ;
	typedef prior<p_6>::type p_7;
	bcout << deref<p_7>::type::base_name() << ',' ;
	typedef prior<p_7>::type p_8;
	bcout << deref<p_8>::type::base_name() << endl ;
	typedef p_8 p_rbegin;
	//
	BOOST_STATIC_ASSERT((iterator_equal<
		rbegin<meta_T::all_attributes>::type, p_rbegin
	>::value));
	//
	bcout << "---------------------------------------------------" << endl;
	//
	typedef mirror::find_if<meta_T::all_attributes, is_even_attrib>::type n_0;
	bcout << deref<n_0>::type::base_name() << ',' ;
	typedef next<n_0>::type n_2;
	bcout << deref<n_2>::type::base_name() << ',' ;
	typedef next<n_2>::type n_4;
	bcout << deref<n_4>::type::base_name() << ',' ;
	typedef next<n_4>::type n_6;
	bcout << deref<n_6>::type::base_name() << ',' ;
	typedef next<n_6>::type n_8;
	bcout << deref<n_8>::type::base_name() << endl ;
	typedef next<n_8>::type n_end;
	//
	BOOST_STATIC_ASSERT((iterator_equal<
		end<meta_T::all_attributes>::type, n_end
	>::value));
	//
	bcout << "---------------------------------------------------" << endl;
	//
	typedef mirror::find_if<meta_T::all_attributes, is_odd_attrib >::type m_1;
	bcout << deref<m_1>::type::base_name() << ',' ;
	typedef next<m_1>::type m_3;
	bcout << deref<m_3>::type::base_name() << ',' ;
	typedef next<m_3>::type m_5;
	bcout << deref<m_5>::type::base_name() << ',' ;
	typedef next<m_5>::type m_7;
	bcout << deref<m_7>::type::base_name() << endl ;
	typedef next<m_7>::type m_end;
	//
	BOOST_STATIC_ASSERT((iterator_equal<
		end<meta_T::all_attributes>::type, m_end
	>::value));
	//
	bcout << "---------------------------------------------------" << endl;
	//
	/*
	typedef mirror::reverse_find_if<meta_T::all_attributes, is_odd_attrib >::type q_7;
	bcout << deref<q_7>::type::base_name() << ',' ;
	typedef next<q_7>::type q_5;
	bcout << deref<q_5>::type::base_name() << ',' ;
	typedef next<q_5>::type q_3;
	bcout << deref<q_3>::type::base_name() << ',' ;
	typedef next<q_3>::type q_1;
	bcout << deref<q_1>::type::base_name() << endl ;
	typedef next<q_1>::type q_end;
	//
	BOOST_STATIC_ASSERT((iterator_equal<
		rend<meta_T::all_attributes>::type, q_end
	>::value));
	*/
	//
	bcout << "---------------------------------------------------" << endl;
	//
	typedef mirror::find_if<meta_T::all_attributes, is_integral_attrib >::type o_0;
	bcout << deref<o_0>::type::base_name() << ',' ;
	typedef next<o_0>::type o_1;
	bcout << deref<o_1>::type::base_name() << ',' ;
	typedef next<o_1>::type o_2;
	bcout << deref<o_2>::type::base_name() << ',' ;
	typedef next<o_2>::type o_3;
	bcout << deref<o_3>::type::base_name() << ',' ;
	typedef next<o_3>::type o_4;
	bcout << deref<o_4>::type::base_name() << ',' ;
	typedef next<o_4>::type o_5;
	bcout << deref<o_5>::type::base_name() << ',' ;
	typedef next<o_5>::type o_6;
	bcout << deref<o_6>::type::base_name() << endl ;
	typedef next<o_6>::type o_end;
	//
	BOOST_STATIC_ASSERT((iterator_equal<
		end<meta_T::all_attributes>::type, o_end
	>::value));
	//
	bcout << "---------------------------------------------------" << endl;
	//
	BOOST_STATIC_ASSERT((iterator_equal<
		begin<meta_T::base_classes>::type,
		end<meta_T::base_classes>::type
	>::value));
	//
	bcout << "---------------------------------------------------" << endl;
	bcout << "Finished" << endl;
	return 0;
}

