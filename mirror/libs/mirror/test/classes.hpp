/**
 * \file test/classes.hpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Several test classes 
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_MIRROR_LIBS_MIRROR_TEST_CLASSES_HPP
#define BOOST_MIRROR_LIBS_MIRROR_TEST_CLASSES_HPP

#include "./namespaces.hpp"

namespace test {

	struct A 
	{
		long a1;
		long a2;
		long a3;
		A(void): a1(1L), a2(2L), a3(3L){ }
	};

	struct B : virtual A
	{
		static int b1;
		int b2;
		B(void):b2(4){ }
	};
	int B::b1 = 0;

	struct C : virtual A
	{
		mutable double c1;
		double c2;
		C(void):c1(5.0),c2(6.0){ }
	};

	struct D : virtual A
	{
		const short d1;
		volatile short d2;
		D(void):d1(7), d2(8){ }
	};

	struct E : B, C, D
	{
		static const float e;
	};
	const float E::e = 1.0f;

	struct F : virtual E
	{
		bool f1;
		bool f2;
		F(void):f1(true),f2(false){ }
	};

	struct G : virtual E
	{
		char g1;
		char g2;
		char g3;
		G(void):g1('a'),g2('b'),g3('c'){ }
	};

	struct H : F, G
	{
		wchar_t h1;
		wchar_t h2;
		wchar_t h3;
		H(void):h1(L'a'),h2(L'b'),h3(L'c'){ }
	};

	class I
	{
	public:
		const ::std::string& get_i(void) const
		{
			return i;
		}

		void set_i(const ::std::string& _i)
		{
			i = _i;
		}
	private:
		::std::string i;
	};

	class J
	{
	public:
		const ::std::wstring& get_j(void) const
		{
			return j;
		}

		void set_j(const ::std::wstring& _j)
		{
			j = _j;
		}
	private:
		::std::wstring j;
	};

	class K : public I, public J
	{
	public:
		// 'generated', read only attribute without
		// any actual storage
		int get_k_ro(void) const
		{
			return 1;
		}
		// 'consumed', write only attribute without
		// any actual storage
		void set_k_wo(int _k) 
		{
		}
	};

} // namespace test


namespace boost { 
namespace mirror {

/** class ::test::A */
BOOST_MIRROR_REG_TYPE(::test, A)
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::A)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, long, a1)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, long, a2)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, long, a3)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** class ::test::B */
BOOST_MIRROR_REG_TYPE(::test, B)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::B)
	BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(0,  public, ::test::A)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::B)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(static, int, b1)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, int, b2)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** class ::test::C */
BOOST_MIRROR_REG_TYPE(::test, C)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::C)
	BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(0,  public, ::test::A)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::C)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(mutable, double, c1)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, double, c2)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** class ::test::D */
BOOST_MIRROR_REG_TYPE(::test, D)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::D)
	BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(0,  public, ::test::A)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::D)
	BOOST_MIRROR_REG_CLASS_ATTRIB(
		_, const short, d1,
		{return instance.d1;},
		{dest = DestType(instance.d1);},
		{ } // no setting
	)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, volatile short, d2)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** class ::test::E */
BOOST_MIRROR_REG_TYPE(::test, E)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::E)
	BOOST_MIRROR_REG_BASE_CLASS(0,  public, ::test::B)
	BOOST_MIRROR_REG_BASE_CLASS(1,  public, ::test::C)
	BOOST_MIRROR_REG_BASE_CLASS(2,  public, ::test::D)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::E)
	BOOST_MIRROR_REG_CLASS_ATTRIB(
		static, const float, e,
		{return instance.e;},
		{dest = DestType(instance.e);},
		{ } // no setting
	)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** class ::test::F */
BOOST_MIRROR_REG_TYPE(::test, F)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::F)
	BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(0,  public, ::test::E)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::F)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, bool, f1)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, bool, f2)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** class ::test::G */
BOOST_MIRROR_REG_TYPE(::test, G)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::G)
	BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(0,  public, ::test::E)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::G)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, char, g1)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, char, g2)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, char, g3)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** class ::test::H */
BOOST_MIRROR_REG_TYPE(::test, H)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::H)
	BOOST_MIRROR_REG_BASE_CLASS(0,  public, ::test::F)
	BOOST_MIRROR_REG_BASE_CLASS(1,  public, ::test::G)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::H)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, wchar_t, h1)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, wchar_t, h2)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, wchar_t, h3)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** class ::test::I */
BOOST_MIRROR_REG_TYPE(::test, I)
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::I)
	BOOST_MIRROR_REG_CLASS_ATTRIB(
		_, ::std::string, i,
		{return instance.get_i();},
		{dest = DestType(instance.get_i());},
		{instance.set_i(value);} 
	)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** class ::test::J */
BOOST_MIRROR_REG_TYPE(::test, J)
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::J)
	BOOST_MIRROR_REG_CLASS_ATTRIB(
		_, ::std::wstring, j,
		{return instance.get_j();},
		{dest = DestType(instance.get_j());},
		{instance.set_j(value);} 
	)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** class ::test::K */
BOOST_MIRROR_REG_TYPE(::test, K)
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::K)
	BOOST_MIRROR_REG_BASE_CLASS(0,  public, ::test::I)
	BOOST_MIRROR_REG_BASE_CLASS(1,  public, ::test::J)
BOOST_MIRROR_REG_BASE_CLASSES_END
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::K)
	BOOST_MIRROR_REG_CLASS_ATTRIB(
		_, int, k_ro,
		{return instance.get_k_ro();},
		{dest = DestType(instance.get_k_ro());},
		{ } // no setting
	)
	BOOST_MIRROR_REG_CLASS_ATTRIB(
		_, int, k_wo,
		{return 0;}, // dummy default
		{dest = DestType(0);},
		{instance.set_k_wo(value);} 
	)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END


} // namespace mirror
} // namespace boost

#endif // include guard

