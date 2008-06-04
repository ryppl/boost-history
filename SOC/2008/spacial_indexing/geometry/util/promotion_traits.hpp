// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_PROMOTION_TRAITS_HPP
#define _GEOMETRY_PROMOTION_TRAITS_HPP


namespace geometry
{
	/*!
		\brief Traits class to select, of two types, the most accurate type for calculations
		\details The promotion traits classes, base class and specializations, compares two types on compile time.
		For example, if an addition must be done with a double and an integer, the result must be a double.
		If both types are integer, the results can be an integer.
		The select_type_traits class and its specializations define the appropriate type in the member type <em>type</em>.
		\note Might be replaced by the new promotion_traits class of boost.
	*/
	template<typename T1, typename T2>
	struct select_type_traits
	{
		typedef T1 type;
	};

	// (Partial) specializations

	// Any combination with double will define a double
	template<typename T> struct select_type_traits<double, T> { typedef double type; };
	template<typename T> struct select_type_traits<T, double> { typedef double type; };
	// Avoid ambiguity for the double/double case
	template<> struct select_type_traits<double, double> { typedef double type; };

	// List other cases
	template<> struct select_type_traits<int, float> { typedef float type; };
	template<> struct select_type_traits<float, int> {	typedef float type; };

	// to be extended



	/*!
		\brief Traits class to select a larger type to avoid overflow in some calculations
		\details Converts small integer classes to long int, and float to double
		\note Maybe this class can be replaced by the new (accepted?) promotion_traits class of boost.
	*/
	template<typename T>
	struct large_type_traits
	{
		typedef T type;
	};

	template<> struct large_type_traits<float> { typedef double type; };
	template<> struct large_type_traits<char> { typedef long int type; };
	template<> struct large_type_traits<short int> { typedef long int type; };

	/*!
		\brief Combination of select_type_traits and large_type_traits for convenience
		\details Selects the most appropriate type for calculation on two values of
		possibly different type, and making them larger if necessary to avoid overflow.
		\note Maybe this class can be replaced by the new (accepted?) promotion_traits class of boost.
	*/
	template <typename T1, typename T2>
	struct calculation_traits
	{
		typedef large_type_traits<typename select_type_traits<T1, T2>::type> type;
	};

}

#endif // _GEOMETRY_PROMOTION_TRAITS_HPP
