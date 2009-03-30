#ifndef BOOST_GUIGL_EXAMPLE_GRAPH_HPP
#define BOOST_GUIGL_EXAMPLE_GRAPH_HPP

#include "functor.hpp"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/ref.hpp>

template<class T1, class T2, class F>
inline void for_each(boost::tuples::cons<T1, T2>& t, F const& f)
  {
  f(t.head);
  for_each(t.tail, f);
  }

template<class T1, class F>
inline void for_each(boost::tuples::cons<T1, boost::tuples::null_type>& t, F const& f) {
  f(t.head);
  }

template<class F>
inline void for_each(boost::tuple<>& t, F const& f) {}

template<class T1, class T2, class F>
inline void for_each(boost::tuples::cons<T1, T2> const& t, F const& f)
  {
  f(t.head);
  for_each(t.tail, f);
  }

template<class T1, class F>
inline void for_each(boost::tuples::cons<T1, boost::tuples::null_type> const& t, F const& f) {
  f(t.head);
  }

template<class F>
inline void for_each(boost::tuple<> const& t, F const& f) {}

class DependencyGraph
  {
  public:
    std::map<int, boost::shared_ptr<IAcceptor> > objects;

  public:
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;
    Graph g;

    struct edge_creator {
      int v;
      Graph& g;
      edge_creator(int v_, Graph& g_):v(v_), g(g_){}
      template<class T>
      void operator()(SmartResult<T> const& r) const { boost::add_edge(r.id, v, g);}
      };

    template<class F>
    SmartResult<typename F::result_type> const& add_functor(
      typename F::args_type const& args,
      typename F::data_type const& data = F::data_type())
      {
      boost::shared_ptr<Acceptor<F> >
        obj(new Acceptor<F>(F(boost::add_vertex(g), args, data)));
      for_each(args, edge_creator(obj->id, g));
      objects[obj->id] = obj;
      SmartResult<typename F::result_type> const& r = *obj;
      return r;
      }

  };

  SmartPoint make_source_point(DependencyGraph& g, point_type const& pt)
  {
    return g.add_functor<source_point>(boost::make_tuple(), pt);
  }

#define FUNCTOR_IMPL_1(FunctionName, F, T1)\
  SmartResult<typename F::result_type> const& FunctionName(DependencyGraph& g,\
    SmartResult<T1> const& arg1)\
    {\
    return g.add_functor<F>(boost::make_tuple(boost::cref(arg1)));\
    }

#define FUNCTOR_IMPL_2(FunctionName, F, T1, T2)\
  SmartResult<typename F::result_type> const& FunctionName(DependencyGraph& g,\
    SmartResult<T1> const& arg1,\
    SmartResult<T2> const& arg2)\
    {\
    return g.add_functor<F>(boost::make_tuple(boost::cref(arg1), boost::cref(arg2)));\
    }

#define FUNCTOR_IMPL_3(FunctionName, F, T1, T2, T3)\
  SmartResult<typename F::result_type> const& FunctionName(DependencyGraph& g,\
    SmartResult<T1> const& arg1,\
    SmartResult<T2> const& arg2,\
    SmartResult<T3> const& arg3)\
    {\
    return g.add_functor<F>(\
    boost::make_tuple(boost::cref(arg1), boost::cref(arg2), boost::cref(arg3)));\
    }

FUNCTOR_IMPL_2(make_line, line_from_two_points, point_type, point_type);
//FUNCTOR_IMPL_3(make_line, line_from_point_vector_distance, point_type, vector_type, distance_type);

FUNCTOR_IMPL_3(make_plane, plane_from_three_points, point_type, point_type, point_type);

#endif
