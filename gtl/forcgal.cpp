#define CGAL_YES
#ifdef CGAL_YES

#include <vector>
#include <CGAL/Cartesian.h>
#include <CGAL/Cartesian_d.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

#include <CGAL/box_intersection_d.h>
#include <CGAL/Bbox_2.h>
//#include <iostream>

typedef CGAL::Box_intersection_d::Box_d<double,2> Box;
typedef CGAL::Bbox_2                              Bbox;



// instead of
//typedef CGAL::Cartesian<Number_type>               Kernel;
// workaround for VC++
typedef double FT;

struct Kernel : public CGAL::Cartesian<FT> {};

typedef Kernel::Point_2                            Point_2;
typedef CGAL::Polygon_2<Kernel>                    Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>         Polygon_with_holes_2;
typedef CGAL::Polygon_set_2<Kernel>                Polygon_set_2;


const int D = 2;                          // dimension of points
typedef CGAL::Cartesian_d<FT>             K;
typedef CGAL::Min_sphere_of_spheres_d_traits_d<K,FT,D> Traits;
typedef CGAL::Min_sphere_of_spheres_d<Traits> Min_sphere;
typedef K::Point_d                        Point;
typedef Traits::Sphere                    Sphere;

typedef std::list<Polygon_with_holes_2>            Pwh_list_2;

#endif // CGAL_YES

using namespace std;

//#include "../generate.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

namespace generate{
/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0df   /* constant vector a */
#define UPPER_MASK 0x80000000 /* most significant w-r bits */
#define LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */   
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y)  (y >> 11)
#define TEMPERING_SHIFT_S(y)  (y << 7)
#define TEMPERING_SHIFT_T(y)  (y << 15)
#define TEMPERING_SHIFT_L(y)  (y >> 18)

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializing the array with a NONZERO seed */
void
sgenrand(unsigned long seed)
{
    /* setting initial seeds to mt[N] using         */
    /* the generator Line 25 of Table 1 in          */
    /* [KNUTH 1981, The Art of Computer Programming */
    /*    Vol. 2 (2nd Ed.), pp102]                  */
    mt[0]= seed & 0xffffffff;
    for (mti=1; mti<N; mti++)
        mt[mti] = (69069 * mt[mti-1]) & 0xffffffff;
}

double /* generating reals */
/* unsigned long */ /* for integer generation */
genrand()
{
    unsigned long y;
    static unsigned long mag01[2]={0x0, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if sgenrand() has not been called, */
            sgenrand(4357); /* a default initial seed is used   */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];

        mti = 0;
    }
  
    y = mt[mti++];
    y ^= TEMPERING_SHIFT_U(y);
    y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
    y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
    y ^= TEMPERING_SHIFT_L(y);

    return ( (double)y / (unsigned long)0xffffffff ); /* reals */
    /* return y; */ /* for integer generation */
}
#if 0
/* this main() outputs first 1000 generated numbers  */
int main(void)
{ 
    int j;

    sgenrand(4357); /* any nonzero integer can be used as a seed */
    for (j=0; j<1000; j++) {
        printf("%5f ", genrand());
        if (j%8==7) printf("\n");
    }
    printf("\n");
    return 0;
}
#endif

using namespace std;


class GRect{
public:
  int xl, yl, xh, yh;
  GRect(int xxl, int yyl, int xxh, int yyh) : xl(xxl), yl(yyl), xh(xxh), yh(yyh) {}
  static const int dx = 100;
  static const int dy = 100;
};


class GTri{
public:
  typedef int TT[3][2];
  TT pts; // the 3 points of the triangle
  GTri(TT & p)
  {
    for(int i = 0; i < 3; ++i){
      pts[i][0]  = p[i][0];
      pts[i][1]  = p[i][1];
    }
  }
  static const int fortyFivePatternCount = 4;
  static const int randomPatternCount = 6;

  static const int d = 100;
  static const double dd = d; // just for internal multiplies

 
};

const int D = GTri::d;
const double DD = GTri::d;

typedef int TriData[2][2];

TriData fortyFivePattern[GTri::fortyFivePatternCount] = 
  {
    { {0, D}, {D, 0} },
    { {D, 0}, {0, -D} },
    { {0, -D}, {-D, 0} },
    { {-D, 0}, {0, D} }
  };


TriData randomPattern[GTri::randomPatternCount] = 
  {
    { { 0.9*DD,  0.6*DD}, {-0.9*DD,  0.3*DD} },
    { { 0.8*DD,  0.8*DD}, {-0.4*DD,  0.9*DD} },
    { { 1.0*DD,  0.1*DD}, {-0.3*DD,  1.0*DD} },
    { { 0.7*DD,  0.3*DD}, {-0.8*DD,  0.9*DD} },
    { { 0.3*DD,  0.8*DD}, {-1.0*DD,  0.1*DD} },
    { {-1.0*DD, -0.3*DD}, { 0.8*DD, -0.6*DD} }
  };

ofstream glvf("out.glv");

//#define GLVIEW_OUT

char const * stipples[] = {
    "(USE_STIPPLE \"stipple_halfcross\")",
    "(USE_STIPPLE \"stipple_halfleftdiagonal\")",
    "(USE_STIPPLE \"stipple_halfrightdiagonal\")",
    "(USE_STIPPLE \"stipple_grid\")"
  };

void generateRandomRectangles(vector<GRect> & results, int boxsize)
{
  static int cnt = 0;
  ++cnt;
  sgenrand(cnt); // seed the random generator
#ifdef GLVIEW_OUT
  glvf << "(ELEMENT \"/rects_" << cnt << "\"\n";
  glvf << stipples[cnt] << "\n";
  glvf << "(R \n";
#endif
  
  int count = int(0.9 * (boxsize * boxsize) / (GRect::dx * GRect::dy)); // total area divided by individual area;
  for(int i = 0; i < count; ++i){
    int x = int(genrand() * (double(boxsize)));
    int y = int(genrand() * (double(boxsize)));

    results.push_back(GRect(x, y, (x + GRect::dx), (y + GRect::dy)));
#ifdef GLVIEW_OUT
    glvf << x << "," << y << ",0 " << (x+GRect::dx) << "," << (y+GRect::dy) << ",0 ";
#endif

  }
#ifdef GLVIEW_OUT
  glvf << "\n)\n)";
#endif
}
    

void generateRandomTriangles(vector<GTri> & results, int boxsize, bool fortyFive)
{
  static int fvcount = 0;
  static int rcount = 0;
  if(fortyFive) { ++fvcount; sgenrand(fvcount); }
  else { ++rcount; sgenrand(rcount); }

#ifdef GLVIEW_OUT
  if(fortyFive){
    glvf << "\n(ELEMENT \"/triangles_45_" << fvcount << "\"\n";
    glvf << stipples[fvcount] << "\n";
  }
  else{
    glvf << "\n(ELEMENT \"/triangles_random_" << rcount << "\"\n";
    glvf << stipples[rcount] << "\n";
  }
#endif
  
  int count = int((fortyFive ? 1.1 : 0.9) * ((boxsize * boxsize) / ((GTri::d * GTri::d) / 2)));

  for(int i = 0; i < count; ++i){

    int pts[3][2];

    int x  = int(genrand() * (double(boxsize)));
    int y  = int(genrand() * (double(boxsize)));

    int index;
    const TriData * cachePtr; 

    if(fortyFive){
      index = i % GTri::fortyFivePatternCount;
      cachePtr = fortyFivePattern;
    }
    else{
      index = i % GTri::randomPatternCount;
      cachePtr = randomPattern;
    }
    
    pts[0][0] = x;
    pts[0][1] = y;
    
    pts[1][0] = x + cachePtr[index][0][0];
    pts[1][1] = y + cachePtr[index][0][1];

    pts[2][0] = x + cachePtr[index][1][0];
    pts[2][1] = y + cachePtr[index][1][1];

    results.push_back(GTri(pts));

#ifdef GLVIEW_OUT
    glvf << "(P ";
    for(int j = 0; j < 3; ++j){
      glvf << pts[j][0] << "," << pts[j][1] << "," << 0 << " ";
    }
    glvf << ")\n";
#endif

  }
#ifdef GLVIEW_OUT
  glvf << "\n)";
#endif


}
}// end of namespace generate



#include <boost/polygon/polygon.hpp>
namespace gtl = boost::polygon;
using namespace boost::polygon::operators;
#include <ctime>
#include <cassert>

namespace boost { namespace polygon {
  typedef int Unit;
  typedef point_data<int> Point;
  typedef interval_data<int> Interval;
  typedef rectangle_data<int> Rectangle;
  typedef polygon_90_data<int> Polygon;
  typedef polygon_90_with_holes_data<int> PolygonWithHoles;
  typedef polygon_45_data<int> Polygon45;
  typedef polygon_45_with_holes_data<int> Polygon45WithHoles;
  typedef polygon_90_set_data<int> PolygonSet;
  typedef polygon_45_set_data<int> Polygon45Set;
  typedef polygon_data<int> PolygonRandom;
  typedef polygon_with_holes_data<int> PolygonRandomWithHoles;
  typedef polygon_set_data<int> PolygonRandomSet;
  typedef axis_transformation AxisTransform;
  typedef transformation<int> Transform;
} }

using namespace std;

int main(void)
{

  const int K = 2;
  //const int BOXX = 5000;
  const int BBOX_INITIAL_SIZE = 6000;
  const int BBOX_FINAL_SIZE = 22000;
  const int BBOX_INCREMENT = 2000;



#if 1
    /// BEGIN RANDOM TEST ////////////////////////
      {
        gtl::PolygonRandomSet gtlptr[K];   // gtl polygon 
        vector<generate::GTri>  tr[K];

#ifdef CGAL_YES
        Polygon_set_2 cgalptr[2];
#endif // CGAL_YES

        cout << "\n\n $$$$$$$$$$$$ RANDOM TEST $$$$$$$$$$$$$$$$\n";
        for(int BOXX = BBOX_INITIAL_SIZE; BOXX < BBOX_FINAL_SIZE; BOXX += BBOX_INCREMENT){
          cout << "\n\nSquare BOXX size: " << BOXX << " *************************\n\n" << endl;
          // Random triangles
          for(int i = 0; i < K; ++i){
            generate::generateRandomTriangles(tr[i], BOXX, false); // generate random shaped triangles


            for(vector<generate::GTri>::iterator vit = tr[i].begin(); vit != tr[i].end(); ++vit){
              // getting one random triangle
              vector<gtl::Point> triangle_points;
              for(int j = 0; j < 3; ++j){
                triangle_points.push_back(gtl::Point(vit->pts[j][0], vit->pts[j][1]));
              }
              gtl::PolygonRandom temp;
              temp.set(triangle_points.begin(), triangle_points.end()); // now we have 1 triangle in gtl
        
              gtlptr[i].insert(temp); // adding one triangle to the gtl random polygonset
            }

            // feed the filled polygons first
            vector<gtl::PolygonRandomWithHoles> prwh;
            gtlptr[i].get(prwh);
            for(vector<gtl::PolygonRandomWithHoles>::iterator vit = prwh.begin(); vit != prwh.end(); ++vit){
         
#ifdef CGAL_YES
              Polygon_2 cgalp2;
              vector<Polygon_2> cgalpholes;
#endif // CGAL_YES   
   
              int j = 0;
              for(gtl::PolygonRandomWithHoles::iterator_type vertex_it = vit->begin(); vertex_it != vit->end(); ++vertex_it, ++j){
#ifdef CGAL_YES
                if(distance(vit->begin(), vertex_it) < (vit->size()-1)){ // avoid outputting end vertex (that coincides first)
                  //cout << "Adding to cgalPolygon one vertex: " << vertex_it->get(gtl::HORIZONTAL) << ", " <<  vertex_it->get(gtl::VERTICAL) << endl;
                  cgalp2.push_back(Point_2( vertex_it->get(gtl::HORIZONTAL), vertex_it->get(gtl::VERTICAL)));
                }
#endif // CGAL_YES

              }

              // now get the holes 
        
              for(gtl::PolygonRandomWithHoles::iterator_holes_type hit = vit->begin_holes(); hit != vit->end_holes(); ++hit){
#ifdef CGAL_YES
                Polygon_2 cgalOneHole;
#endif // CGAL_YES  
                int k = 0;
                for(gtl::PolygonRandomWithHoles::iterator_type vertex_it = hit->begin(); vertex_it != hit->end(); ++vertex_it, ++k){
#ifdef CGAL_YES
                  if(distance(hit->begin(), vertex_it) < (hit->size()-1)){ 
                    //cout << "Adding to cgalOneHole one vertex: " << vertex_it->get(gtl::HORIZONTAL) << ", " <<  vertex_it->get(gtl::VERTICAL) << endl;
                    cgalOneHole.push_back(Point_2(vertex_it->get(gtl::HORIZONTAL), vertex_it->get(gtl::VERTICAL)));
                  }
#endif // CGAL_YES

                }
#ifdef CGAL_YES
                // adding one hole to the holes vector
                cgalpholes.push_back(cgalOneHole);
#endif // CGAL_YES

              }
#ifdef CGAL_YES
              cgalptr[i].insert(Polygon_with_holes_2(cgalp2, cgalpholes.begin(), cgalpholes.end()));
#endif // CGAL_YES

            }  
          }

          // GTL clip operation
          //gtl::PolygonRandomSet gtl_result = gtlptr[0] * gtlptr[1];

          //gtlptr[0].clear();
          //gtlptr[1].clear();
          //gtl_result.clear();

#ifdef CGAL_YES   

          // CGAL clip operation
          Polygon_set_2 cgal_result;
          cgal_result.intersection(cgalptr[0], cgalptr[1]); // do the intersection
          cgalptr[0].clear();
          cgalptr[1].clear();
          cgal_result.clear();
#endif // CGAL_YES

        }
      }
      /// END RANDOM TEST ///////////////////////////
#endif

#if 1
#ifdef CGAL_YES
        Polygon_2 p;
        p.push_back(Point_2(0, 0));
        p.push_back(Point_2(1, 0));
        p.push_back(Point_2(1, 1));
        p.push_back(Point_2(0, 1));

        Polygon_set_2 ps;
        //Polygon_set_2::OutputItr psit;
        Polygon_with_holes_2 cgpwh;
        //CGAL::Oneset_iterator<Polygon_with_holes_2> psit(cgpwh);
        list<Polygon_with_holes_2> res;
        ps.polygons_with_holes(back_inserter(res));

#endif // CGAL_YES
#endif
      
        return 0;   
}


/* 
   compile line I used:
 g++ -I ~/boostcheckout/gtl/ '-I/nfs/ltdn/disks/tcad_lmg_db_exchange_01/CGAL-3.3.1/include/CGAL/config/x86-64_Linux-2.6_g++-3.4.2' -I/nfs/ltdn/disks/tcad_lmg_db_exchange_01/CGAL-3.3.1/include -I/usr/intel/pkgs/boost/1.35.0/include/boost-1_35 -I/usr/X11/include -I/usr/lib/qt3/include forcgal.cpp -lrt '-L/nfs/ltdn/disks/tcad_lmg_db_exchange_01/CGAL-3.3.1/lib/x86-64_Linux-2.6_g++-3.4.2' -L/usr/X11/lib64 -L/usr/lib/qt3/lib64 -Wl,-R/nfs/ltdn/disks/tcad_lmg_db_exchange_01/CGAL-3.3.1/lib/x86-64_Linux-2.6_g++-3.4.2:/usr/X11/lib64:/usr/lib/qt3/lib64 -lCGAL -lCGALcore++ -lCGALQt -lCGALimageIO -lCGALPDB -lqt-mt -lGL -lGLU -lz -lX11 -lm

*/
