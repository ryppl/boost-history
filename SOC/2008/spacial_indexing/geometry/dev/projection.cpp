#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>

#include <boost/numeric/conversion/bounds.hpp>
#include <boost/limits.hpp>
#include <boost/function_output_iterator.hpp>


#include <geometry/geometry.hpp>
#include <geometry/latlong.hpp>
#include <geometry/streamwkt.hpp>
#include <geometry/distance.hpp>
#include <geometry/length.hpp>

typedef int int32;
typedef int32 GL_PROJPARAM_TYPE;
typedef int32 GL_PROJECTION_TYPE;


struct PROJECTION_PARAM
{
	// int-parameters
	int32  Zone;
	int32  i1;							// General parameter

	// double-parameters
	double FalseEast;
	double FalseNorth;
	double MajorAxis;
	double MinorAxis;
	double CenterLatitude;			// radian
	double CenterLongitude;			// radian
	double FirstParallel;			// radian
	double SecondParallel;			// radian
	double ScaleFactor;
	double Azimuth;					// radian
	double d1;							// General parameter
	double d2;							// General parameter

public:
	double Get(GL_PROJPARAM_TYPE Param) const;
	void Set(GL_PROJPARAM_TYPE Param, double value);
};

//static const double d2r = M_PI / 180.0;
//static const double TWO_PI = M_PI + M_PI;




/* Coordinates of Amersfoort in degrees: 5 23' 15.5'', 52 9' 22.178'' */
// Original geolib:
//const double AmersfoortXll = 5.0 + (23.0 / 60.0) + (15.500 / 3600.0);
//const double AmersfoortYll = 52.0 + ( 9.0 / 60.0) + (22.178 / 3600.0);

// Second approach:
//const double AmersfoortXll = from_dms(5, 23, 15.5);
//const double AmersfoortYll = from_dms(52.0, 9.0, 22.178);

// Third approach (now in namespace geometry), explicitly initialized with lat/long, safer:
/*
 const geometry::point_ll<double> AmersfoortLL(geometry::graticule<geometry::longitude, double>(geometry::dms(5, 23, 15.5)),
   geometry::graticule<geometry::latitude, double>(geometry::dms(52.0, 9.0, 22.178)));
*/

// Fourth approach, same, less verbose
namespace 
{
	// Note thate "using geometry" will result in a globally available namespace, but stated like this it is only here
	using geometry::graticule;
	using geometry::dms;
	using geometry::latitude;
	using geometry::longitude;
	const geometry::point_ll<geometry::degree, double> AmersfoortLL(
			longitude<>(dms( 5.0, 23.0, 15.500)),
			latitude<>(dms(52.0,  9.0, 22.178)));
}


/***
class datum_transformation
{
	point_ll to_wgs84(point_ll)
	point_ll from_wgs84(point_ll)
}

class amersfoort_dt :public datum_transformation
{
	// geen parameters
}

class vector7_dt :public datum_transformation
{
	parameter[7]
	//matrixbereking: schaling, rotatie en translatie
}

class vector3_dt :public datum_transformation
{
	parameter[3]
	//matrixbereking: translatie
}


class coordinate_system
{
	point_ll to_wgs84(point_ll)
	point_ll from_wgs84(point_ll)
}

class gg_coordinate_system:public coordinate_system
{
	std::vector<datum_transformation> m_dts;

	point_ll to_wgs84(point_ll)
	{
		// for iterator etc
		point_ll = dts[0].to_wgs85(point_ll);
		point_ll = dts[1].to_wgs85(point_ll);
	}
	point_ll from_wgs84(point_ll)
	{
		// backward
	}
};

class project_parameter
{
}

class ellipsoid : public  project_parameter
{
	double MajorAxis;
	double MinorAxis;
	2 -> 3 getallen?
}

class zone : public  project_parameter
{
	int32  Zone;
}

class offsets : public  project_parameter
	double FalseEast;
	double FalseNorth;

class centers : public  project_parameter
{
	radian CenterLatitude;			// radian
	radian CenterLongitude;			// radian
}

class parallells (?) : public  project_parameter
{
	// lat long?
	radian FirstParallel;			// radian
	radian SecondParallel;			// radian
}

class azimuth? : public  project_parameter
{
	radian Azimuth;					// radian
};
class scalefactor? : public  project_parameter
{
	// double-parameters
	double ScaleFactor;
}



class projection_method
{
	// parameters
	point_ll unproject(point_xy)
	point_xy project(point_ll)
};


template <typename P1, typename C1, typename P2, typename C2>
public static Transform (const P1&, const C1 cs2, P2&, const C2 cs2)
{
	coordinate_system s1 = coordinatesystem_factory(cs1);
	coordinate_system s2 = coordinatesystem_factory(cs2);

	s1.from_wgs84();
	s2.to_wgs84();
}

public static Transform (point_ll, int cs1, point_ll, int cs2)
public static Transform (point_xy, int cs1, point_xy, int cs2)
public static Transform (point_ll, int cs1, point_xy, int cs2)



//class projected_cs:public coordinate_system
class projection
{
	gg_coordinate_system m_gg_coordinate_system;
	project_method m_method;

	point_ll to_wgs84(pointxy)
	{
		pointll = m_method.unproject(pointxy);
		return m_gg_coordinate_system.to_wgs84(pointll);
	}

	point_xy from_wgs84(point_ll)
	{
		pointll=m_gg_coordinate_system.from_wgs84(point_ll);
		return m_method.project(pointll);
	}
};

***/											

/* Coordinates of Amersfoort in Rijksdriehoek meting */
//const double AmersfoortXrd = 155000.0;
//const double AmersfoortYrd = 463000.0;
const geometry::point_xy<double> AmersfoortRD(155000.0, 463000.0);


// Lat-long in DEGREE
template <geometry::dr_selector D, typename T>
inline void BesselToWGS84(const geometry::point_ll<D, T>& in, geometry::point_ll<D, T>& out)
{
	// delta(WGSlat, WGSlong) = (c1, c2) + A * delta(Blat, Blong)
	// in which A is the matrix:
	//     a11 a12
	//     a21 a22
	// and the deltas are offsets from (52, 5)

	// formula from an article by ir.T.G.Schut
	// published in NGT Geodesia, june 1992.

	geometry::point_ll<geometry::degree, T> in_deg = in.convert<geometry::degree>();

	const double c1  = -96.862/100000.0;
	const double c2  = -37.902/100000.0;
	const double a11 = -11.714/100000.0 + 1.0;
	const double a12 = - 0.125/100000.0;
	const double a21 =   0.329/100000.0;
	const double a22 = -14.667/100000.0 + 1.0;

	//const double Lat0  = 52.0;	// North
	//const double Long0 =  5.0; // East
	//const LL ll0(geometry::longitude<>(5), geometry::latitude<>(52));
	//const LL ll1(5.0, 52.0); // won't compile
	const geometry::point_ll<geometry::degree, T> ll0(geometry::longitude<>(5), geometry::latitude<>(52));

	double DeltaLat  = in_deg.lat()  - ll0.lat();
	double DeltaLong = in_deg.lon() - ll0.lon();

	geometry::point_ll<geometry::degree, T> out_deg;
	out_deg.lat( c1 + ll0.lat() + a11 * DeltaLat + a12 * DeltaLong);
	out_deg.lon (c2 + ll0.lon() + a21 * DeltaLat + a22 * DeltaLong);
	out = out_deg.convert<D>();
}

template <typename T>
inline void WGS84ToBessel(const geometry::point_ll<geometry::degree, T>& in, geometry::point_ll<geometry::degree, T>& out)
{
	// delta(Blat, Blong) = D * { delta(WGSlat, WGSlong) - (c1, c2) }
	// in which D is the inverse van matrix A
	// and the deltas are offsets from (52, 5)

	const double c1  = -96.862/100000.0;
	const double c2  = -37.902/100000.0;
	const double a11 = -11.714/100000.0 + 1.0;
	const double a12 = - 0.125/100000.0;
	const double a21 =   0.329/100000.0;
	const double a22 = -14.667/100000.0 + 1.0;

	const geometry::point_ll<geometry::degree, T> ll0(geometry::longitude<>(5), geometry::latitude<>(52));

	const double NormA = 1.0 / (a11 * a22 - a12 * a21);
	const double d11 = + a22 * NormA;
	const double d12 = - a12 * NormA;
	const double d21 = - a21 * NormA;
	const double d22 = + a11 * NormA;

	double DeltaLat  = in.lat() - ll0.lat() - c1;
	double DeltaLong = in.lon() - ll0.lon() - c2;

	out.lat (ll0.lat() + d11 * DeltaLat + d12 * DeltaLong);
	out.lon (ll0.lon() + d21 * DeltaLat + d22 * DeltaLong);
}


template <typename P, typename LL>
class projection_rd 
{
	public :

		typedef typename LL::coordinate_type T;

		GL_PROJECTION_TYPE ParamsGet(PROJECTION_PARAM &Params) const
		{
			Params.MajorAxis = 6377397.155000;
			Params.MinorAxis = 6356078.962818;
			Params.CenterLongitude = 0.094032;
			Params.CenterLatitude = 0.910297;
			Params.ScaleFactor = 0.999908;
			Params.FalseEast = 155000.000000;
			Params.FalseNorth = 463000.000000;
			return 28992;
		}
		bool Project(const LL& lola, P& p) const
		{
			BOOST_STATIC_ASSERT(LL::angle_type == geometry::radian);
			double Phi1, Lambda1, Phi2, Lambda2, Phi3, Lambda3, Phi4, Lambda4, Lambda5;

			double TWO_PI = geometry::math::two_pi;
			if (lola.lon() < -TWO_PI || lola.lon() > TWO_PI || lola.lat() < -TWO_PI || lola.lat() > TWO_PI)
			{
				p.x(0.0);
				p.y(0.0);
				return false;
			}

			// Calculate x and y in units of 10000 seconds
			// (lat, lon might be converted to degrees (0-360))
			geometry::point_ll<geometry::degree, LL::coordinate_type> lola_deg = lola.convert<geometry::degree>();
			Lambda1 = (lola_deg.lon() - AmersfoortLL.lon() ) * 3600.0 / 10000.0;
			Phi1 = (lola_deg.lat() - AmersfoortLL.lat() ) * 3600.0 / 10000.0;

			/* Precompute powers */
			Phi2 = Phi1 * Phi1;
			Phi3 = Phi2 * Phi1;
			Phi4 = Phi3 * Phi1;
			Lambda2 = Lambda1 * Lambda1;
			Lambda3 = Lambda2 * Lambda1;
			Lambda4 = Lambda3 * Lambda1;
			Lambda5 = Lambda4 * Lambda1;

			/* Calculate coordinates in meters
			 * "DG Rijkswaterstaat. Coordinaattransformaties en kaartprojecties. 1993, p. 19"
			 */
			p.x(AmersfoortRD.x()
				+ 190066.98903 *        Lambda1
				-  11830.85831 * Phi1 * Lambda1
				-    144.19754 * Phi2 * Lambda1
				-     32.38360 *        Lambda3
				-      2.34078 * Phi3 * Lambda1
				-      0.60639 * Phi1 * Lambda3
				+      0.15774 * Phi2 * Lambda3
				-      0.04158 * Phi4 * Lambda1
				-      0.00661 *        Lambda5);

			p.y(AmersfoortRD.y()
				+ 309020.31810 * Phi1
				+   3638.36193 *        Lambda2
				-    157.95222 * Phi1 * Lambda2
				+     72.97141 * Phi2
				+     59.79734 * Phi3
				-      6.43481 * Phi2 * Lambda2
				+      0.09351 *        Lambda4
				-      0.07379 * Phi3 * Lambda2
				-      0.05419 * Phi1 * Lambda4
				-      0.03444 * Phi4);

			return true;
		}

		bool LatLong(const P& p, LL& lola) const
		{
			BOOST_STATIC_ASSERT(LL::angle_type == geometry::radian);
			double x2, x3, x4, x5, y2, y3, y4, y5, Phi, Lambda;

			/* Calculate coordinates in 100 kilometers units
			 * with Amersfoort(155,463) as origin
			 */
			typename P::coordinate_type x = (p.x() - AmersfoortRD.x()) / 100000.0;
			typename P::coordinate_type y = (p.y() - AmersfoortRD.y()) / 100000.0;

			/* Precompute second, third and fourth power */
			x2 = x * x;
			x3 = x2 * x;
			x4 = x3 * x;
			x5 = x4 * x;

			y2 = y * y;
			y3 = y2 * y;
			y4 = y3 * y;
			y5 = y4 * y;

			/*
			 * Calculate coordinates in lat-long seconds
			 * "DG Rijkswaterstaat. Coordinaattransformaties en kaartprojecties. 1993, p. 20"
			 * (Lambda = X direction, phi is Y direction)
			 */
			Lambda =
						 + 5261.3028966 * x
						 +  105.9780241 * x  * y
						 +    2.4576469 * x  * y2
						 -    0.8192156 * x3
						 -    0.0560092 * x3 * y
						 +    0.0560089 * x  * y3
						 -    0.0025614 * x3 * y2
						 +    0.0012770 * x  * y4
						 +    0.0002574 * x5
						 -    0.0000973 * x3 * y3
						 +    0.0000293 * x5 * y
						 +    0.0000291 * x  * y5;

			Phi =
					+ 3236.0331637 *      y
					-   32.5915821 * x2
					-    0.2472814 *      y2
					-    0.8501341 * x2 * y
					-    0.0655238 *      y3
					-    0.0171137 * x2 * y2
					+    0.0052771 * x4
					-    0.0003859 * x2 * y3
					+    0.0003314 * x4 * y
					+    0.0000371 *      y4
					+    0.0000143 * x4 * y2
					-    0.0000090 * x2 * y4;

			/* x and y are in seconds from Amersfoort. Recompute degrees
			 * and add Amersfoort in degrees
			 */
			lola.lon(geometry::math::d2r * (AmersfoortLL.lon() + (Lambda / 3600.0)));
			lola.lat(geometry::math::d2r * (AmersfoortLL.lat() + (Phi /  3600.0)));
			return true;
		}

		bool TransformToWGS84(const LL& in, LL& out) const
		{
			//if (m_transformations.size()==0)
			{
				//out.lat() = in.lat();
				//out.lon() = in.lon();

				//in.lon( in.lon()*180/M_PI );
				//in.lat( in.lat()*180/M_PI );
				geometry::point_ll<geometry::degree, LL::coordinate_type> in_deg = in.convert<geometry::degree>();

				LL out_deg;
				BesselToWGS84(in_deg, out_deg);
				// out_deg.as_<geometry::in>
				out = out_deg.as_radian();

				//out.lon( out.lon() *M_PI / 180);
				//out.lat( out.lat() *M_PI / 180);

			}
			/***
			else
			{
				BASE_PROJECTION::TransformToWGS84(in.lon(), in.lat(), out.lon(), out.lat());
			}
			***/

			return true;
		}

		bool TransformFromWGS84(const LL& in, LL& out) const
		{
			//if (m_transformations.size()==0)
			{
				//out.lat() = in.lat();
				//out.lon() = in.lon();
			
				 in.lon( in.lon()*180/M_PI );
				 in.lat( in.lat()*180/M_PI );
				 WGS84ToBessel(in, out);
				 out.lon(out.lon() *M_PI / 180);
				 out.lat(out.lat() *M_PI / 180);
			}
 			/*
			else
			{
				BASE_PROJECTION::TransformFromWGS84(in.lon(), in.lat(), out.lon(), out.lat());
			}
			*/
			return true;
		}
};



void PrintGrad(double Angle)
{
	int Degree = (int)Angle;
	Angle -= Degree;
	Angle *= 60;
	int Minutes = (int)Angle;
	Angle -= Minutes;
	Angle *= 60;

	printf("%d %d' %lf%c", Degree, Minutes, Angle, '"');
}



int main() 
{
	/*
	double Lat = 53.0;
	double Long = 6.0;

	double LatTemp;
	double LongTemp;

	double LatResult;
	double LongResult;
	*/
	geometry::point_ll<geometry::degree, double> latlon(geometry::longitude<>(6), geometry::latitude<>(53));
	geometry::point_ll<geometry::degree, double> ll_temp;
	geometry::point_ll<geometry::degree, double> ll_result;

	BesselToWGS84(latlon, ll_temp);
	WGS84ToBessel(ll_temp, ll_result);
	printf("lat  = ");
	PrintGrad(latlon.lat());
	printf(" -> ");
	PrintGrad(ll_temp.lat());
	printf(" -> ");
	PrintGrad(ll_result.lat());
	printf("\n");
	printf("long = ");
	PrintGrad(latlon.lon());
	printf(" -> ");
	PrintGrad(ll_temp.lon());
	printf(" -> ");
	PrintGrad(ll_result.lon());
	printf("\n");

	projection_rd<geometry::point_xy<double>, geometry::point_ll<geometry::radian, double> > rd;

	geometry::point_xy<double> p;
	geometry::point_ll<geometry::radian, double> ll;

	ll.lat(52 * geometry::math::d2r);
	ll.lon(5 * geometry::math::d2r);

	rd.Project(ll, p);
	std::cout << p.x() << " " << p.y() << std::endl;

	geometry::point_ll<geometry::degree, double> lld = ll.convert<geometry::degree>();
	//rd.Project(lld, p); //will NOT compile because it is a latlong!

	geometry::point_ll<geometry::radian, double> ll2;
	rd.LatLong(p, ll2);
	std::cout << ll2.lon() / geometry::math::d2r << " " << ll2.lat() / geometry::math::d2r << std::endl;

	//latitude lt = as_lat(3);
	//double d = lt();
	// will not compile!
	//lon ln = lt;

	{
		typedef geometry::point_ll<geometry::degree, double> lola;
		lola a, r;
		// Amsterdam 52 22'23"N 4 53'32"E
		a.lat(geometry::dms(52, 22, 23));
		a.lon(geometry::dms(4, 53, 32));

		// Rotterdam 51 55'51"N 4 28'45"E
		r.lat(geometry::dms(51, 55, 51));
		r.lon(geometry::dms(4, 28, 45));

		// Project points to Dutch coordinate-system, which is in meters
		typedef geometry::point_xy<double> XY;
		XY a1, r1;
		rd.Project(a.convert<geometry::radian>(), a1);
		rd.Project(r.convert<geometry::radian>(), r1);

		std::cout << "Amsterdam: " << a1 << " Rotterdam: " << r1 << std::endl;

		std::cout << "Distance Amsterdam-Rotterdam: " << std::endl;
		std::cout << "vincenty:               " << 0.001 * geometry::distance(a, r, geometry::formulae::distance::vincenty<>() ) << " km" << std::endl;
		std::cout << "RD, pythagoras:         " << 0.001 * geometry::distance(a1, r1) << " km" << std::endl;


	}


  return 0;
}

