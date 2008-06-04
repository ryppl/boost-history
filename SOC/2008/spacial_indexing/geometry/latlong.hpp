// Geometry Library
//
// Copyright XXX  Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_LATLONG_HPP
#define _GEOMETRY_LATLONG_HPP

#include <geometry/geometry.hpp>
#include <geometry/util/math.hpp>

#include <sstream>


namespace geometry
{
	/*!
		\brief Unit of plan angles: degrees or radians
	*/
	enum dr_selector {degree, radian};

	/*!
		\brief Cardinal directions. 
		\details They are used in the dms-class. When specified by the library user,
		north/east/south/west is, in general, enough. When parsed or received by an algorithm,
		the user knows it it is lat/long but not more
	*/
	enum cd_selector {/*cd_none, */ north, east, south, west, cd_lat, cd_lon};

	/*!
		\brief Utility class to construct latlong points point_ll with degree,minute,second
		\note Normally combined with latitude and longitude classes
		\par Template parameters:
		- \a CD selects if it is north/south/west/east
		- \a coordinate value, double/float, should correspond to latitude/longitude or point_ll class
		\par Example:
		Example showing how to use the dms class
		\dontinclude doxygen_examples.cpp
		\skip example_dms
		\line {
		\until }
	*/
	template <cd_selector CD, typename T = double>
	class dms
	{
		public :
			/// Constructs with a value
			inline explicit dms(T v) 
				: m_value(v) 
			{}
			/// Constructs with a degree, minute, optional second
			inline explicit dms(int d, int m, T s = 0.0) 
				: m_value(T(
					(CD == west || CD == south) ? -1.0 : 1.0) 
					* (d + (m / 60.0) + (s / 3600.0)))
			{
			}

			// Prohibit automatic conversion to T
			// because this would enable lon(dms<south>)
			// inline operator T() const { return m_value; }

			/// Explicit conversion to T (double/float)
			inline const T& as_value() const 
			{ 
				return m_value; 
			}

			/// Get degrees as integer, minutes as integer, seconds as double.
			inline void get_dms(int& d, int& m, double& s, 
				bool& positive, char& cardinal) const 
			{ 
				double value = m_value;

				// Set to normal earth latlong coordinates
				while (value < -180)
				{
					value += 360;
				}
				while (value > 180)
				{
					value -= 360;
				}
				// Make positive and indicate this
				positive = value > 0;

				// Todo: we might implment template/specializations here
				// Todo: if it is "west" and "positive", make east? or keep minus sign then?

				cardinal = ((CD == cd_lat && positive) ? 'N'
					:  (CD == cd_lat && !positive) ? 'S'
					:  (CD == cd_lon && positive) ? 'E'
					:  (CD == cd_lon && !positive) ? 'W' 
					:  (CD == east) ? 'E' 
					:  (CD == west) ? 'W' 
					:  (CD == north) ? 'N' 
					:  (CD == south) ? 'S' 
					: ' ');

				value = fabs(value);

				// Calculate the values
				double fraction, integer;
				fraction = modf(value, &integer);
				d = int(integer);
				s = 60.0 * modf(fraction * 60.0, &integer);
				m = int(integer);
			}

			/// Get degrees, minutes, seconds as a string, separators can be specified optionally
			inline std::string get_dms(const std::string& ds = " ", 
				const std::string& ms = "'", 
				const std::string& ss = "\"") const 
			{ 
				double s;
				int d, m;
				bool positive;
				char cardinal;
				get_dms(d, m, s, positive, cardinal);
				std::ostringstream out;
				out << d << ds << m << ms << s << ss << " " << cardinal;


				return out.str();
			}

		private :
			T m_value;
	};


	namespace impl
	{
		/*!
			\brief internal base class for latitude and longitude classes
			\details The latitude longitude classes define different types for lat and lon. This is convenient
			to construct latlong class without ambiguity.
			\note It is called graticule, after <em>"This latitude/longitude "webbing" is known as the common 
			graticule" (http://en.wikipedia.org/wiki/Geographic_coordinate_system)</em>
			\par Template parameters:
			- \a S latitude/longitude
			- \a T coordinate type, double float or int, should correspond to point_ll type
		*/
		template <typename T>
		class graticule
		{
			public :
				inline explicit graticule(T v) : m_v(v) {}
				inline operator T() const { return m_v; }
			private :
				T m_v;
		};
	}

	/*!
		\brief Class to construct point_ll with latitude value (north/south)
		\par Template parameters:
		- \a T coordinate type, double / float, should correspond to point_ll type
		\note Often combined with dms class
	*/
	template <typename T = double> class latitude : public impl::graticule<T>
	{
		public : 
			/// Can be constructed with a value
			inline explicit latitude(T v) : impl::graticule<T>(v) {}
			/// Can be constructed with a NORTH dms-class
			inline explicit latitude(const dms<north,T>& v) : impl::graticule<T>(v.as_value()) {}
			/// Can be constructed with a SOUTH dms-class
			inline explicit latitude(const dms<south,T>& v) : impl::graticule<T>(v.as_value()) {}
	};
	/*!
		\brief Class to construct point_ll with longitude value (west/east)
		\par Template parameters:
		- \a T coordinate type, double / float, should correspond to point_ll type
		\note Often combined with dms class
	*/
	template <typename T = double> class longitude : public impl::graticule<T>
	{
		public : 
			/// Can be constructed with a value
			inline explicit longitude(T v) : impl::graticule<T>(v) {}
			/// Can be constructed with a WEST dms-class
			inline explicit longitude(const dms<west, T>& v) : impl::graticule<T>(v.as_value()) {}
			/// Can be constructed with an EAST dms-class
			inline explicit longitude(const dms<east, T>& v) : impl::graticule<T>(v.as_value()) {}
	};



	/*!
		\brief Point using spherical coordinates \a lat and \a lon, on Earth
		\details The point_ll class implements a point with lat and lon functions. It can be constructed 
		using latitude and longitude classes. The latlong class can be defined in degrees or in radians.
		There is a conversion method from degree to radian, and from radian to degree.
		\par Template parameters:
		- \a D degree/radian enumeration
		- \a T coordinate type, double (the default) or float (it might be int as well)
		\note There is NO constructor with two values to avoid exchanging lat and long
		\note Construction with latitude and longitude can be done in both orders, so lat/long and long/lat
		\par Example:
		Example showing how the point_ll class can be constructed. Note that it can also be constructed using
		decimal degrees (43.123).
		\dontinclude doxygen_examples.cpp
		\skip example_point_ll_construct
		\line {
		\until }
	*/
	template <dr_selector D, typename T = double>
	class point_ll : public point<T, 2>
	{
		public :
			static const dr_selector angle_type = D;

			/// Default constructor, does not initialize anything
			inline point_ll() : point<T, 2>() {}

			/// Constructor with an option, e.g. init_zero
			inline point_ll(init option) : point<T, 2>(option) {}

			/// Constructor with longitude/latitude
			inline point_ll(const longitude<T>& lo, const latitude<T>& la) : point<T, 2>(lo, la) {}

			/// Constructor with latitude/longitude
			inline point_ll(const latitude<T>& la, const longitude<T>& lo) : point<T, 2>(lo, la) {}



			/// Get longitude
			inline const T& lon() const { return this->template get<0>(); }
			/// Get latitude
			inline const T& lat() const { return this->template get<1>(); }

			/// Set longitude
			inline void lon(const T& v) { this->template get<0>() = v; }
			/// Set latitude
			inline void lat(const T& v) { this->template get<1>() = v; }

			// Set longitude using DMS classes
			inline void lon(const dms<east, T>& v) { this->template get<0>() = v.as_value(); }
			inline void lon(const dms<west, T>& v) { this->template get<0>() = v.as_value(); }

			inline void lat(const dms<north, T>& v) { this->template get<1>() = v.as_value(); }
			inline void lat(const dms<south, T>& v) { this->template get<1>() = v.as_value(); }

			/*!
				\brief Method to convert degree or radian
				\details Template method to convert to a new point_ll class instance.
				Degree/radian is specified as a template parameter. Latlong points are usually 
				specified in degrees. They are often stored in degrees as well. Algorithms
				usually use points in radians. The convert method conveniently does the conversion
				from degree to radian, or from radian to degree, or, if it is already in the
				spherical coordinate type specified, it returns just a copy.
				\par Example:
				Example showing how to convert a point in degrees to latlong.
				\dontinclude doxygen_examples.cpp
				\skip example_point_ll_convert
				\line {
				\until }
			*/
			template <dr_selector CD>
			inline point_ll<CD, T> convert() const
			{
				static const double c = (CD == D) ? 1.0 : CD == radian ? geometry::math::d2r : geometry::math::r2d;
				return point_ll<CD, T>(longitude<T>(T(c * this->lon())), latitude<T>(T(c * this->lat())));
			}

			/* for latlong comparison is not so obvious!
			inline bool operator<(const point_xy& other) const
			{
				return equals(x(), other.x()) ? y() < other.y() : x() < other.x();
			}
			*/

	};



} // namespace geometry


#endif // _GEOMETRY_LATLONG_HPP
