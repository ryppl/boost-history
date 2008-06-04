// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_GEOMETRY_HPP
#define _GEOMETRY_GEOMETRY_HPP

#include <vector>
#include <limits>
#include <cmath>
#include <stdexcept>

#include <geometry/concepts/point_traits.hpp>
#include <geometry/util/math.hpp>

#include <boost/numeric/conversion/bounds.hpp>
#include <boost/static_assert.hpp>
#include <boost/assert.hpp>

namespace geometry
{

	/// Enumeration to initialize in constructor for point/envelope
	enum init {init_zero, init_min_infinite, init_max_infinite, init_inverse};




	/*!
		\brief Basic point class, having coordinates dfined in a neutral way
		\par Template parameters:
		- \a T numeric type, for example double, float, int
		- \a D number of coordinates, for example 2
	*/
	template<typename T, size_t D>
	class point 
	{
		public :
			// Member type/const 
			typedef T coordinate_type;
			static const size_t coordinate_count = D;


			/// Default constructor, no initialization at all
			inline point()
			{}

			/// Constructs with one, or optionally two or three values
			inline point(const T& v0, const T& v1 = 0, const T& v2 = 0)
			{
				if (D >= 1) m_values[0] = v0;
				if (D >= 2) m_values[1] = v1;
				if (D >= 3) m_values[2] = v2;
			}

			/// Constructs with an option
			inline point(init option)
			{
				switch(option)
				{
					case init_zero :
						{
							for (register size_t i = 0; i < D; i++)
							{
								// Use the template-init-zero-construction to init at zero
								m_values[i] = T();
							}
						}
						break;
					case init_min_infinite :
						{
							for (register size_t i = 0; i < D; i++)
							{
								m_values[i] = boost::numeric::bounds<T>::lowest();
							}
						}
						break;
					case init_max_infinite :
						{
							for (register size_t i = 0; i < D; i++)
							{
								m_values[i] = boost::numeric::bounds<T>::highest();
							}
						}
						break;
					case init_inverse :
							throw std::invalid_argument("The init_reverse option cannot be used here");
						break;
				}
			}

			/// Compile time access to coordinate values
			template <size_t K>
			//inline void value(T const& v)
			inline T& get()
			{
				BOOST_STATIC_ASSERT(K < D);
				//m_values[K] = v;
				return m_values[K];
			}

			template <size_t K>
			inline const T& get() const
			{
				BOOST_STATIC_ASSERT(K < D);
				return m_values[K];
			}


			/// Examine if point is equal to other point
			inline bool operator==(const point& other) const
			{
				for (register size_t i = 0; i < D; i++)
				{
					if (! equals(m_values[i], other.m_values[i]))
					{
						return false;
					}
				}
				return true;
			}

			/// Examine if points are NOT equal
			inline bool operator!=(const point& other) const
			{
				return ! operator==(other);
			}

		private :
			T m_values[D];
	};


	/*!
		\brief 2D point in Cartesian coordinate system
		\par Template parameters:
		- \a T numeric type, arguments can be, for example, double, float, int
	*/
	template<typename T>
	class point_xy : public point<T, 2>
	{
		public :
			/// Default constructor, does not initialize anything
			inline point_xy() : point<T, 2>() {}
			/// Constructor with x/y values
			inline point_xy(const T& x, const T& y) : point<T, 2>(x, y) {}
			/// Constructor with an option
			inline point_xy(init option) : point<T, 2>(option) {}

			// Note that the this->template qualifiers are not necessary for MS VS 2005, but they
			// are required for GCC, and are standard (C++ templates, The Complete Guide, page 132)

			/// Get x-value
			inline const T& x() const { return this->template get<0>(); }
			/// Get y-value
			inline const T& y() const { return this->template get<1>(); }

			/// Set x-value
			inline void x(T const & v) { this->template get<0>() = v; }
			/// Set y-value
			inline void y(T const & v) { this->template get<1>() = v; }

			/// Compare two points
			inline bool operator<(const point_xy& other) const
			{
				return equals(x(), other.x()) ? y() < other.y() : x() < other.x();
			}
	};


	/*!
		\brief Helper class for derived classes or algorithms to conveniently define member characteristics
		\details Defines the following:
		- point_type
		- coordinate_type
		- coordinate_count (size_t)
		Linestring, polygon, etc can be derived from this class and, hence, define these characteristics
		\par Template parameters:
		- \a P point type
	*/
	template<typename P>
	class geometry_traits
	{
		public :
			typedef P point_type;
			typedef typename point_traits<P>::coordinate_type coordinate_type;
			static const size_t coordinate_count = point_traits<P>::coordinate_count;
	};


	/*!
		\brief A linestring (named so by OGC) is a collection (probably a vector) of points.
		\par Template parameters:
		- \a P point type
		- \a V optional container type, for example std::vector, std::list, std::deque
		- \a A optional container-allocator-type 
		(see http://accu.org/index.php/journals/427#ftn.d0e249 )
		\note This small but complex class is enough to define a linestring
	*/
	template<typename P,
			template<typename,typename> class V = std::vector,
			template<typename> class A = std::allocator>
	class linestring : public V<P, A<P> >, public geometry_traits<P>
	{
	};



	/*!
		\brief A linear_ring (linear linear_ring) is a closed line which should not be selfintersecting
		\par Template parameters:
		- \a P point type
		- \a V optional container type, for example std::vector, std::list, std::deque
		- \a A optional container-allocator-type 
	*/
	template<typename P,
			template<typename,typename> class V = std::vector,
			template<typename> class A = std::allocator>
	class linear_ring : public V<P, A<P> >, public geometry_traits<P>
	{
	};


	/*!
		\brief The \b polygon contains an outer ring and zero or more inner rings.
		\par Template parameters:
		- \a P point type
		- \a VR optional container type for inner rings, for example std::vector, std::list, std::deque
		- \a VP optional container type for points, for example std::vector, std::list, std::deque
		- \a AR container-allocator-type 
		- \a AP container-allocator-type 
		\note The container collecting the points in the rings can be different from the 
		container collecting the inner rings. They all default to vector.
	*/
	template<typename P,
			template<typename,typename> class VP = std::vector,
			template<typename,typename> class VR = std::vector,
			template<typename> class AP = std::allocator,
			template<typename> class AR = std::allocator>
	class polygon : public geometry_traits<P>
	{
		public :
			// Member types
			typedef linear_ring<P, VP, AP> ring_type;
			typedef VR<ring_type , AR<ring_type > > inner_container_type;

			inline const ring_type& outer() const { return m_outer; }
			inline const inner_container_type & inners() const { return m_inners; }

			inline ring_type& outer() { return m_outer; }
			inline inner_container_type & inners() { return m_inners; }

			/// Utility method, clears outer and inner rings
			inline void clear()
			{
				m_outer.clear();
				m_inners.clear();
			}

		private :
			ring_type m_outer;
			inner_container_type m_inners;
	};



} // namespace geometry


#endif // _GEOMETRY_GEOMETRY_HPP
