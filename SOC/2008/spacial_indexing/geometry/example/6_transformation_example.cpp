// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <geometry/geometry.hpp>
#include <geometry/streamwkt.hpp>
#include <geometry/fromwkt.hpp>
#include <geometry/foreach.hpp>

#include <geometry/assign.hpp>


#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>



// get_value, and specialization, get a value of a point
template <typename P, size_t I, size_t N>
struct get_value
{
	static const typename P::coordinate_type get(P& p, size_t i)
	{
		if (i == I)
		{
			return p.template get<I>();
		}
		return get_value<P, I+1, N>::get(p, i);
	}
};

template <typename P, size_t N>
struct get_value<P, N, N>
{
	static const typename P::coordinate_type get(P& p, size_t i)
	{
		return 0;
	}
};



void transform1(geometry::point_xy<double>& p, boost::numeric::ublas::matrix<double>& m)
{
	// First method, not so effective but clear
	// Copy the point to vector, do matrix calculation, copy back.
	boost::numeric::ublas::vector<double> vec1(3);
	vec1[0] = p.x();
	vec1[1] = p.y();
	vec1[2] = 1.0;
	boost::numeric::ublas::vector<double> vec2(boost::numeric::ublas::prod(m, vec1));
	p.x(vec2[0]);
	p.y(vec2[1]);
}


void transform2(geometry::point_xy<double>& p, boost::numeric::ublas::matrix<double>& m)
{
	// Second method.
	// More effective, using the matrix directly
	p.x(p.x() * m(0,0) + p.y() * m(0,1) + m(0,2));
	p.y(p.x() * m(1,0) + p.y() * m(1,1) + m(1,2));
}


template<typename P, typename M>
class transform3
{

	public :
		transform3(const M& m)
			: _matrix(m)
		{}

		inline void operator()(P& p) const
		{
			// Transform the point, using any method. 
			// Here the more neutral "value" methods are used.
			// (value cannot be used in loops, the "assign" method is used).
			const size_t n = P::coordinate_count;
			BOOST_ASSERT(n + 1 == _matrix.size1());
			BOOST_ASSERT(n + 1 == _matrix.size2());

			for (size_t i = 0; i < n; i++)
			{
				typename P::coordinate_type s = _matrix(i, n);
				for (size_t j = 0; j < n; j++)
				{
					//s += p.ivalue(j) * _matrix(i, j);
					s += get_value<P, 0, n>::get(p, j) * _matrix(i, j);
				}
				//p.ivalue(i, s);
				geometry::assign_coordinate(p, i, s);
			}
		}

	private :
		M _matrix;
};


void transform4(geometry::point_xy<double>& p, boost::numeric::ublas::matrix<double>& m)
{
	// There might be another method, defining an ublas "vector_expression<E2>" using
	// the point. This is not worked out here.
}


int main()
{
	geometry::point_xy<double> p(1, 1);


	// First matrix: translation over (5,5) in (x,y) direction
	boost::numeric::ublas::matrix<double> t1(3,3);
	t1(0,0) = 1;   t1(0,1) = 0;   t1(0,2) = 5;
	t1(1,0) = 0;   t1(1,1) = 1;   t1(1,2) = 5;
	t1(2,0) = 0;   t1(2,1) = 0;   t1(2,2) = 1;


	// Second matrix: scaling over x*10, y*10 in (x,y) direction
	boost::numeric::ublas::matrix<double> s(3, 3);
	s(0,0) = 10;   s(0,1) = 0;   s(0,2) = 0;
	s(1,0) = 0;    s(1,1) = 10;  s(1,2) = 0;
	s(2,0) = 0;    s(2,1) = 0;   s(2,2) = 1;


	// Third matrix: translation over (-20,-15) in (x,y) direction
	boost::numeric::ublas::matrix<double> t2(3, 3);
	t2(0,0) = 1;   t2(0,1) = 0;   t2(0,2) = -20;
	t2(1,0) = 0;   t2(1,1) = 1;   t2(1,2) = -15;
	t2(2,0) = 0;   t2(2,1) = 0;   t2(2,2) = 1;

	// Of course we could rotate as well.

	// Transform the point, using the three matrices one by one
	transform1(p, t1);
	transform1(p, s);
	transform1(p, t2);
	std::cout << "after 3 transformations: " << p << std::endl;

	// Combination matrix
	boost::numeric::ublas::matrix<double> c(3,3);
	c = boost::numeric::ublas::prod(s, t1);
	c = boost::numeric::ublas::prod(t2, c);

	// Transform with combined matrix
	p.x(1);
	p.y(1);
	transform1(p, c);
	std::cout << "combined: " << p << std::endl;

	// Idem, using method 2
	p.x(1);
	p.y(1);
	transform2(p, c);
	std::cout << "method 2: " << p << std::endl;

	// Transform a point with a transformation functor
	p.x(1);
	p.y(1);
	transform3<geometry::point_xy<double>, boost::numeric::ublas::matrix<double> > trans3(c);
	geometry::for_each_point(p, trans3);
	std::cout << "method 3: " << p << std::endl;

	// Transform a polygon
	geometry::polygon<geometry::point_xy<double>, std::deque> poly;
	geometry::from_wkt("POLYGON((0 0,0 7,4 2,2 0,0 0))", poly);
	geometry::for_each_point(poly, trans3);
	std::cout << "transformed polygon " << poly << std::endl;

	return 0;
}
