/**
 * \file examples/serialization/cube.cpp
 * Example of registering namespaces, types and classes
 * and cooperation with the Boost.Serialization library
 *
 * NOTE: if You are not familiar with namespace and type
 * registration and reflection, You should probably 
 * see examples/registering/namespaces.cpp and
 * examples/registering/types.cpp first.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <assert.h>
#include <math.h>

#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/char_type_switch/iostream.hpp>

#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/meta_class.hpp>

#include <boost/mirror/utils/name_to_stream.hpp>


/** First declare some namespaces and classes
 */

namespace Graphics {

	// coordinates
	template <typename a_float_type>
	class CoordsTempl
	{
	public:
		typedef a_float_type float_type;
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
		float_type length_squared(void){return x*x + y*y + z*z;}
		float_type length(void){return sqrt(length_squared());}
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
	private:
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
	};

} // namespace Graphics



namespace boost { 
namespace mirror {

/** Register the 3D namespace
 */
BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(Graphics)

/** Register the types and classes
 */
BOOST_MIRROR_REG_META_TYPE(_Graphics, ::Graphics, Coords)
BOOST_MIRROR_REG_META_TYPE(_Graphics, ::Graphics, Vector)


BOOST_MIRROR_REG_SINGLE_BASE_CLASS(
	::Graphics::Vector, 
	public, ::Graphics::Coords
)

/** Class attributes
 */
// register the attributes of Coords
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Graphics::Coords)	
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_SETTER(
		0, ::Graphics::Coords::float_type, 
		x, get_x, set_x
	)
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_SETTER(
		1, ::Graphics::Coords::float_type, 
		y, get_y, set_y
	)
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_SETTER(
		2, ::Graphics::Coords::float_type, 
		z, get_z, set_z
	)
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_SETTER(
		3, ::Graphics::Coords::float_type, 
		w, get_w, set_w
	)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

// register the attributes of Vector
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Graphics::Vector)	
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_ONLY(
		0, ::Graphics::Vector::float_type, 
		length_squared, length_squared
	)
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_ONLY(
		1, ::Graphics::Vector::float_type, 
		length, length
	)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** Support for serialization 
 */

template <class Class>
struct to_be_loaded
{
	to_be_loaded(Class& _inst):inst(_inst){ }
	Class& inst;
};

template <class Class>
struct to_be_saved
{
	to_be_saved(const Class& _inst):inst(_inst){ }
	const Class& inst;
};

template<class Archive, class Class>
void load(Archive & ar, Class & c, const unsigned int version)
{
	typedef BOOST_MIRROR_REFLECT_CLASS(Class) meta_Class;
}

template<class Archive, class Class>
void save(Archive & ar, const Class & c, const unsigned int version)
{
	typedef BOOST_MIRROR_REFLECT_CLASS(Class) meta_Class;
}


} // namespace mirror

namespace serialization {

template<class Archive, class Class>
void serialize(Archive & ar, mirror::to_be_saved<Class> dst, const unsigned int version)
{
	mirror::save(ar, dst.inst, version);
}

template<class Archive, class Class>
void serialize(Archive & ar, mirror::to_be_loaded<Class> src, const unsigned int version)
{
	mirror::load(ar, src.inst, version);
}


} // namespace serialization
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
	Cube c1(2.0f, 2.0f, 2.0f), c2;
	//
	boost::archive::xml_oarchive oa(cout);
	//
	oa << BOOST_SERIALIZATION_NVP(to_be_saved<Cube>(c1));
	//
	//
	bcout << "Finished" << endl;
	//
	return 0;
}

