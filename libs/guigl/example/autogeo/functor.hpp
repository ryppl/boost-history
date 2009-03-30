#ifndef BOOST_GUIGL_EXAMPLE_FUNCTORS_HPP
#define BOOST_GUIGL_EXAMPLE_FUNCTORS_HPP

#include <boost/guigl/types.hpp>
#include <boost/array.hpp>
#include <boost/ref.hpp>
#include <boost/tuple/tuple.hpp>

#include <geometry/geometries/polygon.hpp>
#include <boost/guigl/ggl.hpp>

template<class T>
struct SmartResult
  {
  typedef T result_type;

  int id;
  result_type result;

  explicit SmartResult(int id_):id(id_), result(){}
  explicit SmartResult(int id_, T const& r):id(id_), result(r){}
  };

struct EmptyData {};

template<class Result, class Args, class Data = EmptyData>
struct Functor : public SmartResult<Result>
  {
  typedef Data data_type;
  typedef Args args_type;

  Args args;
  Data data;

  Functor(int id, Args const& args_, Data const& data_)
    : SmartResult<Result>(id), args(args_), data(data_) {}
  };

template<std::size_t N, class HT, class TT>
inline typename boost::tuples::access_traits<typename boost::tuples::element<N, boost::tuples::cons<HT, TT> >::type>::const_type&
arg(boost::tuples::cons<HT, TT> const& t)
  {
  return boost::get<i>(t.args);
  }

template<std::size_t N, class HT, class TT>
inline typename boost::tuples::access_traits<typename boost::tuples::element<N, boost::tuples::cons<HT, TT> >::type>::non_const_type&
arg(boost::tuples::cons<HT, TT>& t)
  {
  return boost::get<i>(t.args);
  }

//template<std::size_t N, class HT, class TT>
//inline typename boost::tuples::access_traits<typename boost::tuples::element<N, boost::tuples::cons<HT, TT> >::type>::const_type::result_type&
//result(boost::tuples::cons<HT, TT> const& t)
//  {
//  return arg<N>(t).result;
//  }
//
//template<std::size_t N, class HT, class TT>
//inline typename boost::tuples::access_traits<typename boost::tuples::element<N, boost::tuples::cons<HT, TT> >::type>::non_const_type::result_type&
//result(boost::tuples::cons<HT, TT>& t)
//  {
//  return arg<N>(t).result;
//  }


typedef double distance_type;
typedef boost::guigl::position_type point_type;
typedef boost::guigl::position_type vector_type;
typedef boost::guigl::segment_type line_type;
typedef geometry::polygon<point_type> plane_type;

typedef SmartResult<point_type> const& SmartPoint;
typedef SmartResult<line_type> const& SmartLine;
typedef SmartResult<plane_type> const& SmartPlane;

typedef
Functor<point_type, boost::tuple<>, point_type>
source_point;

typedef
Functor<line_type, boost::tuple<SmartPoint, SmartPoint> >
line_from_two_points;

//typedef
//Functor<line_type, boost::tuple<SmartPoint, SmartVector, SmartDistance> >
//line_from_point_vector_distance;

typedef
Functor<plane_type, boost::tuple<SmartPoint, SmartPoint, SmartPoint> >
plane_from_three_points;

#endif // BOOST_GUIGL_EXAMPLE_FUNCTORS_HPP
