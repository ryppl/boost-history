// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <geometry/geometry.hpp>
#include <geometry/distance.hpp>
#include <geometry/streamwkt.hpp>

#include <iostream>
#include <vector>
#include <algorithm>



// Derived class
class GP : public geometry::point_xy<float>
{
	public :
		GP()
			: geometry::point_xy<float>()
		{}

		GP(const float& x, const float& y)
			: geometry::point_xy<float>(x,y)
			, _spatial_reference_id(0)
		{}
		GP(const float& x, const float& y, int id)
			: geometry::point_xy<float>(x,y)
			, _spatial_reference_id(id)
		{}
	private :
		int _spatial_reference_id;
};

// Own class
class point_with_value
{
	private :
		double m_x, m_y, m_value;
	public :
		typedef double coordinate_type;
		point_with_value(const double& anx, const double& any, double v)
			: m_x(anx)
			, m_y(any)
			, m_value(v)
		{}
		inline const double x() const { return m_x; }
		inline const double y() const { return m_y; }
};



int main()
{
/*
REWRITE
lola a(5.9, 52.4); // Amsterdam 52° 23' N  4° 55' E
	lola b(13.3, 52.45); // Berlin (West) 52° 27' N 13° 18' E

	std::cout << "distance " << a << " to " << b << " is " << geometry::distance(a,b) / 1000.0 << " km" << std::endl;

	// You can use your own points, derived from geometry::point
	GP dp1(1,1);
	GP dp2(2,2);

	std::cout << "distance derived: " << geometry::distance(dp1, dp2) << std::endl;

	// Or use your own point NOT derived from points
	point_with_value op1(1,2,3);
	point_with_value op2(4,5,6);

	std::cout << "distance own: " << geometry::distance(op1, op2) << std::endl;

	// You can also calculate the distance between "mixed" points
	std::cout << "distance mixed: " << geometry::distance(dp1, op2) << std::endl;

	geometry::point_xy<double> c(1,1);
	std::cout << "distance " << a << " to " << b << " is " << geometry::distance(a,c) << std::endl;
	*/

	return 0;
}
