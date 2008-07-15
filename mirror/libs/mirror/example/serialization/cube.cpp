/**
 * \file examples/serialization/cube.cpp
 * Example of registering namespaces, types and classes
 * and cooperation with the Boost.Serialization library.
 *
 * NOTE: if You are not familiar with namespace and type
 * registration and reflection, You should probably 
 * see examples/registering/namespaces.cpp and
 * examples/registering/types.cpp first.
 *
 * NOTE: this example will currently not compile with g++.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <assert.h>
#include <math.h>

#include <boost/type_traits/is_fundamental.hpp>

#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/char_type_switch/iostream.hpp>

#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/meta_class.hpp>


/** First declare some namespaces and classes
 */
namespace Graphics {

	// coordinates
	template <typename FloatType>
	class CoordsTempl
	{
	public:
		typedef FloatType float_type;
		CoordsTempl(
			float_type _x = 0.0, 
			float_type _y = 0.0, 
			float_type _z = 0.0, 
			float_type _w = 1.0
		)
			: x(_x)
			, y(_y)
			, z(_z)
			, w(_w)
		{ }
		// getters (space coordinates)
		float_type get_x(void) const {return x;}
		float_type get_y(void) const {return y;}
		float_type get_z(void) const {return z;}
		float_type get_w(void) const {return w;}
		// setters
		void set_x(float_type _x){x = _x;}
		void set_y(float_type _y){y = _y;}
		void set_z(float_type _z){z = _z;}
		void set_w(float_type _w){w = _w;}
		//
		friend bool operator == (const CoordsTempl& a, const CoordsTempl& b) 
		{
			return 
				a.x == b.x && 
				a.y == b.y &&
				a.z == b.z &&
				a.w == b.w;
		}
	protected:
		float_type x, y, z, w;
	};

	typedef CoordsTempl<float> Coords;

	// a 3D vector
	class Vector : public Coords
	{
	public:
		Vector(
			float_type _x = 0.0, 
			float_type _y = 0.0, 
			float_type _z = 0.0
		)
			: Coords(_x, _y, _z){ }
		float_type length_squared(void) const {return x*x + y*y + z*z;}
		float_type length(void) const {return sqrt(length_squared());}
		//
		friend bool operator == (const Vector& a, const Vector& b) 
		{
			return static_cast<const Coords&>(a) == static_cast<const Coords&>(b);
		}
	private:
	};

	// a cube
	class Cube
	{
	public:
		typedef Vector::float_type float_type;
		//
		Cube(float_type _width = 1.0f, float_type _height = 1.0f, float_type _depth = 1.0f)
			: lbb(-_width/2.0f, -_height/2.0f, -_depth/2.0f)
			, lbf(-_width/2.0f, -_height/2.0f,  _depth/2.0f)
			, ltb(-_width/2.0f,  _height/2.0f, -_depth/2.0f)
			, ltf(-_width/2.0f,  _height/2.0f,  _depth/2.0f)
			, rbb( _width/2.0f, -_height/2.0f, -_depth/2.0f)
			, rbf( _width/2.0f, -_height/2.0f,  _depth/2.0f)
			, rtb( _width/2.0f,  _height/2.0f, -_depth/2.0f)
			, rtf( _width/2.0f,  _height/2.0f,  _depth/2.0f)
		{ }
		// the attribs are public which is actually not a good idea
		// but this is just an example
		//
		// left-bottom-back
		Vector lbb;
		// left-bottom-front
		Vector lbf;
		// left-top-back
		Vector ltb;
		// left-top-front
		Vector ltf;
		// right-bottom-back
		Vector rbb;
		// right-bottom-front
		Vector rbf;
		// right-top-back
		Vector rtb;
		// right-top-front
		Vector rtf;
		//
		friend bool operator == (const Cube& a, const Cube& b) 
		{
			return 
				a.lbb == b.lbb && 
				a.lbf == b.lbf && 
				a.ltb == b.ltb && 
				a.ltf == b.ltf && 
				a.rbb == b.rbb && 
				a.rbf == b.rbf && 
				a.rtb == b.rtb && 
				a.rtf == b.rtf;
		}
	};

} // namespace Graphics



namespace boost { 
namespace mirror {

/** Register the Graphics namespace
 */
BOOST_MIRROR_REG_NAMESPACE((Graphics))

/** Register the types and classes
 */
BOOST_MIRROR_REG_TYPE(::Graphics, Coords)
BOOST_MIRROR_REG_TYPE(::Graphics, Vector)
BOOST_MIRROR_REG_TYPE(::Graphics, Cube)


BOOST_MIRROR_REG_SINGLE_BASE_CLASS(
	::Graphics::Vector, 
	public, ::Graphics::Coords
)

/** Class attributes
 */
// register the attributes of Coords
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Graphics::Coords)	
	BOOST_MIRROR_REG_CLASS_ATTRIB_GET_SET(
		_, ::Graphics::Coords::float_type, x,
		get_x(), set_x(value)
	)
	BOOST_MIRROR_REG_CLASS_ATTRIB_GET_SET(
		_, ::Graphics::Coords::float_type, y,
		get_y(), set_y(value)
	)
	BOOST_MIRROR_REG_CLASS_ATTRIB_GET_SET(
		_, ::Graphics::Coords::float_type, z,
		get_z(), set_z(value)
	)
	BOOST_MIRROR_REG_CLASS_ATTRIB_GET_SET(
		_, ::Graphics::Coords::float_type, w,
		get_w(), set_w(value)
	)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

// register the attributes of Vector
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Graphics::Vector)	
	BOOST_MIRROR_REG_CLASS_ATTRIB(
		_, ::Graphics::Vector::float_type, length_squared, 
		{return instance.length_squared();},
		{dest = DestType(instance.length_squared()); return dest;},
		{ } // no setting
	)
	BOOST_MIRROR_REG_CLASS_ATTRIB(
		_, ::Graphics::Vector::float_type, length, 
		{return instance.length();},
		{dest = DestType(instance.length()); return dest;},
		{ } // no setting
	)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

// register the attributes of Cube
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Graphics::Cube)	
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(
		_, ::Graphics::Vector, 
		lbb
	)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(
		_, ::Graphics::Vector, 
		lbf
	)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(
		_, ::Graphics::Vector,
		ltb
	)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(
		_, ::Graphics::Vector,
		ltf
	)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(
		_, ::Graphics::Vector,
		rbb
	)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(
		_, ::Graphics::Vector,
		rbf
	)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(
		_, ::Graphics::Vector,
		rtb
	)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(
		_, ::Graphics::Vector,
		rtf
	)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END


/** Support for serialization 
 */

template <typename Type>
struct to_be_loaded
{
	typedef typename mpl::if_<
		is_fundamental<Type>,
		Type&,
		to_be_loaded<Type>
	>::type type;
	to_be_loaded(Type& _inst):inst(_inst){ }
	Type& inst;
};


template <class MetaClass, class Position>
struct single_attrib_loader
{
	template <class Archive, class Class>
	single_attrib_loader(Archive & ar, Class& c)
	{
		// first load the previous
		single_attrib_loader<MetaClass, mpl::int_<Position::value - 1> >(ar, c);
		// query the value of the current member
		typedef typename mpl::at<
			typename MetaClass::all_attributes::type_list, 
			Position
		>::type member_type; 
		member_type value;
		// load it
		typename to_be_loaded<member_type>::type dst(value);
		ar >> dst;
		// and set it
		MetaClass::all_attributes::set(c, Position(), value);
	}
};

template <class MetaClass>
struct single_attrib_loader<MetaClass, mpl::int_<0> >
{
	template <class Archive, class Class>
	single_attrib_loader(Archive & ar, Class& c)
	{
		typedef mpl::int_<0> position;
		// we are on the first so just load it
		typedef typename mpl::at<
			typename MetaClass::all_attributes::type_list, 
			position
		>::type member_type;
		member_type value;
		typename to_be_loaded<member_type>::type dst(value);
		ar >> dst;
		MetaClass::all_attributes::set(c, position(), value);
	}
};


template<class Archive, class Class>
void do_load(Archive & ar, Class & c)
{
	typedef BOOST_MIRRORED_CLASS(Class) meta_Class;
	typedef mpl::int_<meta_Class::all_attributes::size::value - 1> last;
	single_attrib_loader<meta_Class, last>(ar, c);
}

template <typename Type>
struct to_be_saved
{
	typedef typename mpl::if_<
		is_fundamental<Type>,
		Type&,
		to_be_saved<Type>
	>::type type;
	to_be_saved(Type& _inst):inst(_inst){ }
	Type& inst;
};


template <class MetaClass, class Position>
struct single_attrib_saver
{
	template <class Archive, class Class>
	single_attrib_saver(Archive & ar, Class& c)
	{
		// first save the previous
		single_attrib_saver<MetaClass, mpl::int_<Position::value - 1> >(ar, c);
		// query the value of the current member
		typedef typename mpl::at<
			typename MetaClass::all_attributes::type_list, 
			Position
		>::type member_type;
		member_type value;
		// and save it
		typename to_be_saved<member_type>::type src(
			MetaClass::all_attributes::query(c, Position(), value)
		);
		ar << src;
	}
};

template <class MetaClass>
struct single_attrib_saver<MetaClass, mpl::int_<0> >
{
	template <class Archive, class Class>
	single_attrib_saver(Archive & ar, Class& c)
	{
		typedef mpl::int_<0> position;
		// we are on the first so just save it
		// query the value of the current member
		typedef typename mpl::at<
			typename MetaClass::all_attributes::type_list, 
			position
		>::type member_type;
		member_type value;
		typename to_be_saved<member_type>::type src(
			MetaClass::all_attributes::query(c, position(), value)
		);
		ar << src;
	}
};

template<class Archive, class Class>
void do_save(Archive & ar, Class & c)
{
	typedef BOOST_MIRRORED_CLASS(Class) meta_Class;
	typedef mpl::int_<meta_Class::all_attributes::size::value - 1> last;
	single_attrib_saver<meta_Class, last>(ar, c);
}


} // namespace mirror
namespace archive {

template<class Archive, class Class>
void save(Archive & ar, mirror::to_be_saved<Class> & c)
{
	mirror::do_save(ar, c.inst);
}

template<class Archive, class Class>
void load(Archive & ar, mirror::to_be_loaded<Class> & c)
{
	mirror::do_load(ar, c.inst);
}


} // namespace archive
} // namespace boost

int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	using ::Graphics::Coords;
	using ::Graphics::Vector;
	using ::Graphics::Cube;
	//
	// create two different cubes
	Cube c1(2.0f, 2.0f, 2.0f), c2;
	//
	// save the first one into a text file
	{
		std::fstream out("./Cube.txt", ios_base::out);
		boost::archive::text_oarchive oa(out);
		//
		to_be_saved<Cube> sc1(c1);
		oa << sc1;
	}
	//
	// load the second from the same file
	{
		std::fstream in("./Cube.txt", ios_base::in);
		boost::archive::text_iarchive ia(in);
		//
		to_be_loaded<Cube> lc2(c2);
		ia >> lc2;
	}
	// compare them
	assert(c1 == c2);
	//
	cts::bcout() << "Finished" << endl;
	//
	return 0;
}

