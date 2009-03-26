#ifndef BOOST_GUIGL_EXAMPLE_GRAPH_HPP
#define BOOST_GUIGL_EXAMPLE_GRAPH_HPP

#include "node_types.hpp"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/dijkstra_shortest_paths.hpp>

template<class Key, class T>
class GraphVertex
  {
  public:
    Key key;
    boost::shared_ptr<base_result<T> > obj;

    GraphVertex(
      Key key_,
      boost::shared_ptr<base_result<T> > const& obj_)
      :key(key_), obj(obj_) {}
  };


class dependency_graph
  {
  public:
    std::map<int, boost::shared_ptr<IAcceptor> > objects;
    std::map<int, int> key_map;

  public:
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;

    Graph g;

    boost::shared_ptr<IAcceptor> get(int key)
      {
      return objects[key_map[key]];
      }

    int add_vertex(boost::shared_ptr<IAcceptor> const& ptr)
      {
      int v = boost::add_vertex(g);
      objects[v] = ptr;
      return v;
      }

    void add_key(int key, int v)
      {
      if(key_map.end() != key_map.find(key))
        throw std::runtime_error("vertex already exists");
      key_map[key] = v;
      }

    boost::optional<int> get_key(int v) const
      {
      typedef std::pair<int, int> pair_type;
      BOOST_FOREACH(pair_type const& p, key_map)
        if(p.second == v)
          return p.first;
      return boost::optional<int>();
      }

    GraphVertex<int, point_type> make_source_point(point_type const& pt)
      {
      boost::shared_ptr<Acceptor<source_point> >
        obj(new Acceptor<source_point>(source_point(pt)) );
      int v = add_vertex(obj);
      return GraphVertex<int, point_type>(v, obj);
      }

    GraphVertex<int, point_type> make_source_point(int key, point_type const& pt)
      {
      GraphVertex<int, point_type> result = make_source_point(pt);
      add_key(key, result.key);
      return result;
      }

    GraphVertex<int, line_type> make_line(
      GraphVertex<int, point_type> const& pt1,
      GraphVertex<int, point_type> const& pt2)
      {
      boost::shared_ptr<Acceptor<line_from_two_points> >
        obj(new Acceptor<line_from_two_points>(line_from_two_points(pt1.obj->result, pt2.obj->result)) );

      int v = add_vertex(obj);

      boost::add_edge(pt1.key, v, g);
      boost::add_edge(pt2.key, v, g);

      return GraphVertex<int, line_type>(v, obj);
      }

    GraphVertex<int, line_type> make_line(int key,
      GraphVertex<int, point_type> const& pt1,
      GraphVertex<int, point_type> const& pt2)
      {
      GraphVertex<int, line_type> result = make_line(pt1, pt2);
      add_key(key, result.key);
      return result;
      }

    GraphVertex<int, plane_type> make_plane(
      GraphVertex<int, point_type> const& pt1,
      GraphVertex<int, point_type> const& pt2,
      GraphVertex<int, point_type> const& pt3)
      {
      boost::shared_ptr<Acceptor<plane_from_three_points> >
        obj(new Acceptor<plane_from_three_points>(plane_from_three_points(pt1.obj->result, pt2.obj->result, pt3.obj->result)) );

      int v = add_vertex(obj);

      boost::add_edge(pt1.key, v, g);
      boost::add_edge(pt2.key, v, g);
      boost::add_edge(pt3.key, v, g);

      return GraphVertex<int, plane_type>(v, obj);
      }

    GraphVertex<int, plane_type> make_plane(int key,
      GraphVertex<int, point_type> const& pt1,
      GraphVertex<int, point_type> const& pt2,
      GraphVertex<int, point_type> const& pt3)
      {
      GraphVertex<int, plane_type> result = make_plane(pt1, pt2, pt3);
      add_key(key, result.key);
      return result;
      }
  };

#endif
