// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_STRATEGIES_POINT_LL_HPP
#define _GEOMETRY_STRATEGIES_POINT_LL_HPP

#include <utility>

#include <geometry/geometry.hpp>
#include <geometry/latlong.hpp>
#include <geometry/util/math.hpp>
#include <geometry/util/return_types.hpp>

namespace geometry
{
	// To be moved somewhere.
	namespace constants
	{
		static const double average_earth_radius = 6372795.0;
	}

	namespace strategy
	{
		namespace distance
		{

			/*!
				\brief Distance calculation for spherical coordinates on a perfect sphere using haversine
				\par Template parameters:
				- \a P1 first point type
				- \a P2 optional second point type
				\author Adapted from: http://williams.best.vwh.net/avform.htm
				\note See also http://en.wikipedia.org/wiki/Great-circle_distance
				\note It says: <em>The great circle distance d between two points with coordinates {lat1,lon1} and {lat2,lon2} is given by:
							d=acos(sin(lat1)*sin(lat2)+cos(lat1)*cos(lat2)*cos(lon1-lon2))
						A mathematically equivalent formula, which is less subject to rounding error for short distances is:
							d=2*asin(sqrt((sin((lat1-lat2)/2))^2 + cos(lat1)*cos(lat2)*(sin((lon1-lon2)/2))^2))</em>
			*/
			template <typename P1, typename P2 = P1>
			class haversine
			{
				public :
					inline haversine(double r = constants::average_earth_radius)
						: m_radius(r)
					{}

					inline bool squared() const { return false; }

					inline distance_result operator()(const P1& p1, const P2& p2) const
					{
						// TODO: be sure we calc in radian's
						//return calc(p1.template convert<radian>(), p2.template convert<radian>());
						return calc(p1, p2);
					}

				private :
					double m_radius;
					typedef point_ll<radian, typename point_traits<P1>::coordinate_type> PR;

					inline distance_result calc(const P1& p1, const P2& p2) const
					{
						double a = math::hav(get<1>(p2) - get<1>(p1)) 
							+ cos(get<1>(p1)) * cos(get<1>(p2)) * math::hav(get<0>(p2) - get<0>(p1));
						double c = 2.0 * asin(sqrt(a));
						return distance_result(m_radius * c, false);
					}
			};


			/*!
				\brief Defines ellipsoid values for use in distance calculations
				\details They have a constructor with the earth radius
				\par Template parameters:
				- \a P1 first point type
				- \a P2 optional second point type
				\note Might be moved to separated projection:: namespace, or elsewhere
				\todo Optionally specify earth model, defaulting to WGS84
				- See http://en.wikipedia.org/wiki/Figure_of_the_Earth
				- and http://en.wikipedia.org/wiki/World_Geodetic_System#A_new_World_Geodetic_System:_WGS84
				\note 
			*/
			class ellipsoid
			{
				public :
					ellipsoid(double a, double b)
						: m_a(a)
						, m_b(b)
						, m_f((a - b) / a)
					{}
					ellipsoid()
						: m_a(6378137.0)
						, m_b(6356752.314245)
						, m_f((m_a - m_b) / m_a)
					{}
					// Unit sphere 
					ellipsoid(double f)
						: m_a(1.0)
						, m_f(f)
					{}

					double a() const { return m_a; }
					double b() const { return m_b; }
					double f() const { return m_f; }

				private :
					double m_a, m_b, m_f; // equatorial radius, polar radius, flattening
			};


			/*!
				\brief Point-point distance approximation taking flattening into account
				\par Template parameters:
				- \a P1 first point type
				- \a P2 optional second point type
				\author After Andoyer, 19xx, republished 1950, republished by Meeus, 1999
				\note Although not so well-known, the approximation is very good: in all cases the results
				are about the same as Vincenty. In my (Barend's) testcases the results didn't differ more than 6 m
				\note See also:
					- http://nacc.upc.es/tierra/node16.html
					- http://sci.tech-archive.net/Archive/sci.geo.satellite-nav/2004-12/2724.html
				\note See also the implementations
					- http://home.att.net/~srschmitt/great_circle_route.html
					- http://www.codeguru.com/Cpp/Cpp/algorithms/article.php/c5115
					- http://futureboy.homeip.net/frinksamp/navigation.frink
					- http://www.voidware.com/earthdist.htm
			*/
			template <typename P1, typename P2 = P1>
			class andoyer 
			{
				public :
					andoyer() 
						: m_ellipsoid() 
					{}
					andoyer(double f) 
						: m_ellipsoid(f) 
					{}

					inline bool squared() const { return false; }

					inline distance_result operator()(const P1& p1, const P2& p2) const
					{
						return calc(p1.template convert<radian>(), p2.template convert<radian>());
					}


				private :
					typedef point_ll<radian, typename point_traits<P1>::coordinate_type> PR;
					ellipsoid m_ellipsoid;

					inline distance_result calc(const PR& p1, const PR& p2) const
					{
						double F = (p1.lat() + p2.lat()) / 2.0;
						double G = (p1.lat() - p2.lat()) / 2.0;
						double lambda = (p1.lon() - p2.lon()) / 2.0;

						double sinG2 = math::sqr(sin(G));
						double cosG2 = math::sqr(cos(G));
						double sinF2 = math::sqr(sin(F));
						double cosF2 = math::sqr(cos(F));
						double sinL2 = math::sqr(sin(lambda));
						double cosL2 = math::sqr(cos(lambda));

						double S = sinG2 * cosL2 + cosF2 * sinL2;
						double C = cosG2 * cosL2 + sinF2 * sinL2;

						double omega = atan(sqrt(S / C));
						double r = sqrt(S * C) / omega; // not sure if this is r or greek nu

						double D = 2.0 * omega * m_ellipsoid.a();
						double H1 = (3 * r - 1.0) / (2.0 * C);
						double H2 = (3 * r + 1.0) / (2.0 * S);

						return distance_result(D * (1.0 + m_ellipsoid.f() * H1 * sinF2 * cosG2 - m_ellipsoid.f() * H2 * cosF2 * sinG2), false);
					}
			};


			/*!
				\brief Distance calculation formulae on latlong coordinates, after Vincenty, 1975
				\par Template parameters:
				- \a P1 first point type
				- \a P2 optional second point type
				\author See http://www.ngs.noaa.gov/PUBS_LIB/inverse.pdf
				\author Adapted from various implementations to get it close to the original document
					- http://www.movable-type.co.uk/scripts/LatLongVincenty.html
					- http://exogen.case.edu/projects/geopy/source/geopy.distance.html
					- http://futureboy.homeip.net/fsp/colorize.fsp?fileName=navigation.frink

			*/
			template <typename P1, typename P2 = P1>
			class vincenty
			{
				public :
					inline bool squared() const { return false; }

					inline distance_result operator()(const P1& p1, const P2& p2) const
					{
						return calc(p1.template convert<radian>(), p2.template convert<radian>());
					}

				private :
					typedef point_ll<radian, typename point_traits<P1>::coordinate_type> PR;
					ellipsoid m_ellipsoid;
					inline distance_result calc(const PR& p1, const PR& p2) const
					{
						// lambda: difference in longitude on an auxiliary sphere
						double L = p2.lon() - p1.lon();
						double lambda = L;

						if (L < -math::pi) L += math::two_pi;
						if (L > math::pi) L -= math::two_pi;

						if (p1.lat() == p2.lat() && p1.lon() == p2.lon())
						{
						  return distance_result(0, false);
						}

						// U: reduced latitude, defined by tan U = (1-f) tan phi
						double U1 = atan((1-m_ellipsoid.f()) * tan(p1.lat())); // above (1)
						double U2 = atan((1-m_ellipsoid.f()) * tan(p2.lat())); // above (1)

						double cos_U1 = cos(U1);
						double cos_U2 = cos(U2);
						double sin_U1 = sin(U1);
						double sin_U2 = sin(U2);

						// alpha: azimuth of the geodesic at the equator
						double cos2_alpha;
						double sin_alpha;

						// sigma: angular distance p1,p2 on the sphere
						// sigma1: angular distance on the sphere from the equator to p1
						// sigma_m: angular distance on the sphere from the equator to the midpoint of the line
						double sigma;
						double sin_sigma;
						double cos2_sigma_m;

						double previous_lambda;

						do
						{
							previous_lambda = lambda; // (13)
							double sin_lambda = sin(lambda);
							double cos_lambda = cos(lambda);
							sin_sigma = sqrt(math::sqr(cos_U2 * sin_lambda) + math::sqr(cos_U1 * sin_U2 - sin_U1 * cos_U2 * cos_lambda)); // (14)
							double cos_sigma = sin_U1 * sin_U2 + cos_U1 * cos_U2 * cos_lambda; // (15)
							sin_alpha = cos_U1 * cos_U2 * sin_lambda / sin_sigma; // (17)
							cos2_alpha = 1.0 - math::sqr(sin_alpha);
							cos2_sigma_m = cos2_alpha == 0 ? 0 : cos_sigma - 2.0 * sin_U1 * sin_U2 / cos2_alpha; // (18)

							double C = m_ellipsoid.f()/16.0 * cos2_alpha * (4.0 + m_ellipsoid.f() * (4.0 - 3.0 * cos2_alpha)); // (10)
							sigma = atan2(sin_sigma, cos_sigma); // (16)
							lambda = L + (1.0 - C) * m_ellipsoid.f() * sin_alpha *
								(sigma + C * sin_sigma * ( cos2_sigma_m + C * cos_sigma * (-1.0 + 2.0 * math::sqr(cos2_sigma_m)))); // (11)
												
						} while (fabs(previous_lambda - lambda) > 1e-12 && fabs(lambda) < math::pi);

						double sqr_u = cos2_alpha * (math::sqr(m_ellipsoid.a()) - math::sqr(m_ellipsoid.b())) / math::sqr(m_ellipsoid.b()); // above (1)

						double A = 1.0 + sqr_u/16384.0 * (4096 + sqr_u * (-768.0 + sqr_u * (320.0 - 175.0 * sqr_u))); // (3)
						double B = sqr_u/1024.0 * (256.0 + sqr_u * ( -128.0 + sqr_u * (74.0 - 47.0 * sqr_u))); // (4)
						double delta_sigma = B * sin_sigma * ( cos2_sigma_m + (B/4.0) * (cos(sigma)* (-1.0 + 2.0 * cos2_sigma_m) 
								- (B/6.0) * cos2_sigma_m * (-3.0 + 4.0 * math::sqr(sin_sigma)) * (-3.0 + 4.0 * cos2_sigma_m))); // (6)

						double dist = m_ellipsoid.b() * A * (sigma - delta_sigma); // (19)

						return distance_result(dist, false);
					}
			};


			/*!
				\brief Strategy functor for distance point to segment calculation
				\details Class which calculates the distance of a point to a segment, using latlong points
				\par Template parameters:
				- \a P point type
				- \a PS segment-point type
			*/
			template <typename P, typename PS>
			class ll_point_segment
			{
				public :
					inline ll_point_segment(double r = constants::average_earth_radius) : m_radius(r)
					{}

					inline bool squared() const { return false; }

					inline distance_result operator()(const P& p, const const_segment<PS>& s) const
					{

						return calc(p.template convert<radian>(), 
							s.first.template convert<radian>(), 
							s.second.template convert<radian>());
					}

				private :
					typedef point_ll<radian, typename point_traits<P>::coordinate_type> PR;
					double m_radius;

					/// Calculate course (bearing) between two points. Might be moved to a "course formula" ...
					inline double course(const PR& p1, const PR& p2) const
					{
						/***
							Course between points

							We obtain the initial course, tc1, (at point 1) from point 1 to point 2 by the following. The formula fails if the initial point is a pole. We can special case this with:

							IF (cos(lat1) < EPS)   // EPS a small number ~ machine precision
							  IF (lat1 > 0): tc1= pi        //  starting from N pole
							  ELSE: tc1= 2*pi         //  starting from S pole
							  ENDIF
							ENDIF

							For starting points other than the poles:
							IF sin(lon2-lon1)<0: tc1=acos((sin(lat2)-sin(lat1)*cos(d))/(sin(d)*cos(lat1)))
							ELSE: tc1=2*pi-acos((sin(lat2)-sin(lat1)*cos(d))/(sin(d)*cos(lat1)))
							ENDIF

							An alternative formula, not requiring the pre-computation of d, the distance between the points, is:
							   tc1=mod(atan2(sin(lon1-lon2)*cos(lat2),
									   cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon1-lon2))
									   , 2*pi)
						 ***/
						double dlon = p2.lon() - p1.lon();
						double cos_p2lat = cos(p2.lat());
						return atan2(sin(dlon) * cos_p2lat,	cos(p1.lat()) * sin(p2.lat()) - sin(p1.lat()) * cos_p2lat * cos(dlon));
					}

					inline distance_result calc(const PR& p, const PR& sp1, const PR& sp2) const
					{
						/***
						Cross track error:
						Suppose you are proceeding on a great circle route from A to B (course =crs_AB) and end up at D, perhaps off course.
						(We presume that A is ot a pole!) You can calculate the course from A to D (crs_AD) and the distance from A to D (dist_AD)
						using the formulae above. In shifteds of these the cross track error, XTD, (distance off course) is given by

								   XTD =asin(sin(dist_AD)*sin(crs_AD-crs_AB))

						(positive XTD means right of course, negative means left)
						(If the point A is the N. or S. Pole replace crs_AD-crs_AB with
						lon_D-lon_B or lon_B-lon_D, respectively.)
						 ***/

						// Calculate distances, in radians, on the unit sphere
						// It seems not useful to let this strategy be templatized, it should be in radians and on the unit sphere
						strategy::distance::haversine<PR, PR> strategy(1.0);
						double d1 = strategy(sp1, p);

						// Actually, calculation of d2 not necessary if we know that the projected point is on the great circle...
						double d2 = strategy(sp2, p);


						double crs_AD = course(sp1, p);
						double crs_AB = course(sp1, sp2);
						double XTD = fabs(asin(sin(d1) * sin(crs_AD - crs_AB)));

						// Return shortest distance, either to projected point on segment sp1-sp2, or to sp1, or to sp2
						return distance_result(m_radius * std::min(std::min(d1, d2), XTD), false);
					}
			};



			// We might add a vincenty-like strategy also for point-segment distance, but to calculate the projected point is not trivial

		} // namespace distance



		namespace area
		{
			/*!
				\brief Area calculation by spherical excess
				\par Template parameters:
				- \a P type of points of rings/polygons
				\author Barend Gehrels. Adapted from:
				- http://www.soe.ucsc.edu/~pang/160/f98/Gems/GemsIV/sph_poly.c
				- http://williams.best.vwh.net/avform.htm
				\note The version in Gems didn't account for polygons crossing the 180 meridian.
				\note This version works for convex and non-convex polygons, for 180 meridian
				crossing polygons and for polygons with holes. However, some cases (especially
				180 meridian cases) must still be checked.
				\note The version which sums angles, which is often seen, doesn't handle non-convex
				polygons correctly.
				\note The version which sums longitudes, see 
				http://trs-new.jpl.nasa.gov/dspace/bitstream/2014/40409/1/07-03.pdf, is simple
				and works well in most cases but not in 180 meridian crossing cases. This probably
				could be solved.
			*/
			template<typename P>
			class by_spherical_excess
			{
				private :
					struct excess_sum
					{
						double sum;
						inline excess_sum() : sum(0) {}
						inline double area() const
						{
							return - sum * constants::average_earth_radius * constants::average_earth_radius;
						}
					};

					// Distances are calculated on unit sphere here
					strategy::distance::haversine<P, P> m_unit_sphere;

				public :
					typedef excess_sum state_type;

					by_spherical_excess()
						: m_unit_sphere(1)
					{}

					inline bool operator()(const const_segment<P>& segment, state_type& state) const
					{
						if (segment.first.lon() != segment.second.lon())
						{

							typedef point_ll<radian, typename point_traits<P>::coordinate_type> PR;
							PR p1 = segment.first.template convert<radian>();
							PR p2 = segment.second.template convert<radian>();

							// Distance p1 p2
							double a = m_unit_sphere(segment.first, segment.second);
							// Sides on unit sphere to south pole
							double b = 0.5 * math::pi - p2.lat();
							double c = 0.5 * math::pi - p1.lat();
							// Semi parameter
							double s = 0.5 * (a + b + c);

							// E: spherical excess, using l’Huiller’s formula
							// [tg(e / 4)]2   =   tg[s / 2]  tg[(s-a) / 2]  tg[(s-b) / 2]  tg[(s-c) / 2]
							double E = 4.0 * atan(sqrt(fabs(tan(s / 2) * tan((s - a) / 2) * tan((s - b) / 2) * tan((s - c) / 2))));

							E = fabs(E);

							// In right direction: positive, add area. In left direction: negative, subtract area.
							// Longitude comparisons are not so obvious. If one is negative, other is positive,
							// we have to take the date into account.
							// TODO: check this / enhance this, should be more robust. See also the "grow" for ll
							double lon1 = p1.lon() < 0 ? p1.lon() + math::two_pi : p1.lon();
							double lon2 = p2.lon() < 0 ? p2.lon() + math::two_pi : p2.lon();

							if (lon2 < lon1)
							{
								E= -E;
							}

							state.sum += E;
						}
						return true;
					}
			};

		} // namespace area


		namespace envelope
		{
			// envelope calculation strategy for latlong-points
			namespace shift
			{
				template <dr_selector D>
				struct shifted
				{
				};

				template<>
				struct shifted<radian>
				{
					inline static double shift() { return math::two_pi; }
				};
				template<>
				struct shifted<degree>
				{
					inline static double shift() { return 360.0; }
				};

			}

			/*!
				\par Algorithm:
				The envelope of latlong-points cannot be implemented as for xy-points. Suppose the
				envelope of the Aleutian Islands must be calculated. The span from 170E to 170W, from -170 to 170.
				Of course the real envelope is not -170..170 but 170..-170.
				On the other hand, there might be geometries that indeed span from -170 to 170. If there are
				two points, it is not known. If there are points in between, we probably should take the shorter
				range. So we do that for the moment.
				We shift coordinates and do as if we live in a world with longitude coordinates from 0 - 360,
				where 0 is still at Greenwich. Longitude ranges are then calculated twice: one for real world,
				one for the shifted world.
				The shortest range is taken for the bounding box. This might have coordinates > 180
			*/

			template <typename PB, typename P = PB>
			struct grow_ll
			{

				struct state
				{
					typedef typename point_traits<PB>::coordinate_type T;
					bool has_west;
					T min_lat, max_lat;
					T min_lon1, min_lon2, max_lon1, max_lon2;

					state()
						: has_west(false)
						, min_lat(boost::numeric::bounds<T>::highest())
						, min_lon1(boost::numeric::bounds<T>::highest())
						, min_lon2(boost::numeric::bounds<T>::highest())
						, max_lat(boost::numeric::bounds<T>::lowest())
						, max_lon1(boost::numeric::bounds<T>::lowest())
						, max_lon2(boost::numeric::bounds<T>::lowest())
					{}

					template <typename T>
					void take_minmax(const T& value, T& min, T& max)
					{
						if (value < min)
						{
							min = value;
						}
						if (value > max)
						{
							max = value;
						}
					}

					void grow(const P& p)
					{
						// For latitude, we can take the min/max
						take_minmax(p.lat(), min_lat, max_lat);


						// For longitude, we do the same...
						take_minmax(p.lon(), min_lon1, max_lon1);

						// But we also add 360 (2pi) if it is negative
						T value = p.lon();
						while(value < 0)
						{
							has_west = true;
							value += shift::shifted<PB::angle_type>::shift();
						}
						while (value > math::two_pi)
						{
							value -= shift::shifted<PB::angle_type>::shift();
						}
						take_minmax(value, min_lon2, max_lon2);
					}

					void envelope(box<PB>& mbr)
					{
						// For latitude it is easy
						mbr.min().lat(min_lat);
						mbr.max().lat(max_lat);
						if (! has_west)
						{
							mbr.min().lon(min_lon1);
							mbr.max().lon(max_lon1);
						}
						else
						{
							// Get both ranges
							T diff1 = max_lon1 - min_lon1;
							T diff2 = max_lon2 - min_lon2;

							//std::cout << "range 1: " << min_lon1 * math::r2d << ".." << max_lon1 * math::r2d << std::endl;
							//std::cout << "range 2: " << min_lon2 * math::r2d << ".." << max_lon2 * math::r2d << std::endl;

							if (diff1 <= diff2)
							{
								mbr.min().lon(min_lon1);
								mbr.max().lon(max_lon1);
							}
							else
							{
								mbr.min().lon(min_lon2);
								mbr.max().lon(max_lon2);
							}
						}
					}
				};

				typedef state state_type;

				void operator()(const P& p, state_type& s) const
				{
					s.grow(p);
				}
			};
		} // namespace envelope


	} // namespace strategy

} // namespace geometry


#endif // _GEOMETRY_STRATEGIES_POINT_LL_HPP
