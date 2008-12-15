/**
 * \file examples/factories/tetrahedron.cpp
 *
 *  This example shows how to use the configurable factory
 *  with a input user interface implementation to generically 
 *  construct an instance of classes with non default constructors.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <math.h>
#include <cstdlib>

#include <boost/char_type_switch/iostream.hpp>
#include <boost/mirror/factory.hpp>
#include <boost/mirror/meta_type.hpp>

namespace test {
	
struct vector
{
	double x,y,z;

	vector(double _x, double _y, double _z)
	 : x(_x)
	 , y(_y)
	 , z(_z)
	{ }

	vector(void)
	 : x(0.0)
	 , y(0.0)
	 , z(0.0)
	{ }

	friend vector operator + (const vector& a, const vector& b)
	{
		return vector(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	friend vector operator - (const vector& a, const vector& b)
	{
		return vector(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	// cross product
	friend vector operator % (const vector& a, const vector& b)
	{
		return vector(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}

	// dot product
	friend double operator * (const vector& a, const vector& b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}

	double length(void) const
	{
		return sqrt(x*x + y*y + z*z);
	}
};

struct triangle
{
	vector a, b, c;

	triangle(const vector& _a, const vector& _b, const vector& _c)
	 : a(_a)
	 , b(_b)
	 , c(_c)
	{ }


	double area(void) const 
	{
		return ((b-a)%(c-a)).length()/2.0;
	}
};

struct tetrahedron
{
	triangle base;
	vector apex;

	tetrahedron(const triangle& _base, const vector& _apex)
	 : base(_base)
	 , apex(_apex)
	{ }

	const vector& a(void) const {return base.a;}
	const vector& b(void) const {return base.b;}
	const vector& c(void) const {return base.c;}
	const vector& d(void) const {return apex;}

	double volume(void) const
	{
		return fabs(((a()-d())*((b()-d())%(c()-d()))))/6.0;
	}
};


/** The general implementation of the input user interface template.
 *  Upon construction prints-out a banner and uses a factory
 *  configured to use the same input user interface to construct
 *  the Product.
 */
template <class Product>
struct input_ui
{
	struct banner
	{
		template <class Context, class ConstrIndex, class ParamIndex>
		banner(Context* pc, ConstrIndex ci, ParamIndex pi)
		{
			::std::cout << 
				"Construct " << 
				BOOST_MIRRORED_TYPE(Product)::full_name() << 
				" " << 
				boost::mirror::meta_constructors<
					Context
				>::base_param_name(ci, pi) <<
				::std::endl;
		}
	} b;

	typename ::boost::mirror::make_factory< input_ui, Product >::type f;

	template <class Context, class ConstrIndex, class ParamIndex>
	input_ui(int _x, Context* pc, ConstrIndex ci, ParamIndex pi)
	 : b(pc, ci, pi)
	 , f(_x)
	{ }

	inline Product operator()(void)
	{
		return f();
	}
};

/** Specialization of the input interface, used to produce
 *  doubles by prompting the user to enter a value on the 
 *  console.
 */
template <> 
struct input_ui<double>
{
	double x;

	template <class Context, class ConstrIndex, class ParamIndex>
	input_ui(int _x, Context* pc, ConstrIndex ci, ParamIndex pi)
	{
                ::std::cout <<
                        "Enter " << 
			BOOST_MIRRORED_TYPE(double)::full_name() << 
			" " << 
			boost::mirror::meta_constructors<
				Context
			>::base_param_name(ci, pi) <<
			" = " <<
                        ::std::flush;
		::std::cin >> x;
	}

	inline double operator()(void)
	{
		return x;
	}
};

/** A manager of this input user interface, which picks the 
 *  constructor that will be used by the means of the result
 *  of the index member function (i.e. the zero-th registered
 *  constructor will be always used with this manager).
 */
template <>
struct input_ui<void>
{

	input_ui(int _x, int factory_index)
	{ }

	input_ui(const char* names[], int factory_index)
	{
		::std::cout << "Create " << names[factory_index]  << ::std::endl;
	}

	inline int param(void) const 
	{
		// no params
		return 0;
	}

	inline int index(void) 
	{
		return 0;
	}
};

} // namespace test 


namespace boost {
namespace mirror {

// register the ::test namespace
BOOST_MIRROR_REG_NAMESPACE( (test) )

// register the vector, triangle and tetrahedron classes
BOOST_MIRROR_REG_TYPE( ::test, vector)
BOOST_MIRROR_REG_TYPE( ::test, triangle)
BOOST_MIRROR_REG_TYPE( ::test, tetrahedron)

// register the constructors of ::test::vector
BOOST_MIRROR_REG_CONSTRUCTORS_BEGIN( ::test::vector )
	BOOST_MIRROR_REG_CONSTRUCTOR(0, 
		((double)(x))((double)(y))((double)(z))
	)
	BOOST_MIRROR_REG_DEFAULT_CONSTRUCTOR(1)
BOOST_MIRROR_REG_CONSTRUCTORS_END

// register the constructor of ::test::triangle
BOOST_MIRROR_REG_CONSTRUCTORS_BEGIN( ::test::triangle )
	BOOST_MIRROR_REG_CONSTRUCTOR(0, 
		((::test::vector)(a))((::test::vector)(b))((::test::vector)(c))
	)
BOOST_MIRROR_REG_CONSTRUCTORS_END

// register the constructor of ::test::tetrahedron
BOOST_MIRROR_REG_CONSTRUCTORS_BEGIN( ::test::tetrahedron )
	BOOST_MIRROR_REG_CONSTRUCTOR(0, 
		((::test::triangle)(base))((::test::vector)(apex))
	)
BOOST_MIRROR_REG_CONSTRUCTORS_END

} // namespace mirror
} // namespace boost


int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	// create a factory plugged with the input ui
	const char* param_names[] = {"a tetrahedron"};
	factory< ::test::input_ui, ::test::tetrahedron > f(param_names, 0);
	// use the factory to construct a tetrahedron and calculate 
	// it's volume and base area
	::test::tetrahedron t(f());
	// ... and print them out
	cout << "the volume is " << t.volume() << endl;
	cout << "the area of the base is " << t.base.area() << endl;
	//
	return 0;
}

