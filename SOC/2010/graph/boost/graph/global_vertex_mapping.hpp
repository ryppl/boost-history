#ifndef BOOST_GRAPH_GLOBAL_VERTEX_MAPPING_HPP
#define BOOST_GRAPH_GLOBAL_VERTEX_MAPPING_HPP

#include <utility>
#include <map>
#include <boost/bimap.hpp>
#include <boost/graph/graph_traits.hpp>

namespace boost {
  template <class Graph, typename id_type = int, typename graph_name_type = std::string>
  class GlobalVertexMapping
  {
  private:
    typedef typename graph_traits<const Graph>::vertex_descriptor Vertex;
    typedef typename boost::bimap < Vertex, id_type > bm_type;
    typedef typename bm_type::value_type association;

    struct labelling {
      graph_name_type name;
      bm_type mapping;
    };

    std::map < const Graph *, labelling > graph_labelling;
    id_type dummy_id;
    Vertex dummy_vertex;
    
  public:
    typedef id_type global_id_type;

    GlobalVertexMapping (Vertex dummy_vertex_arg, id_type dummy_id_arg)
      : dummy_vertex(dummy_vertex_arg), dummy_id(dummy_id_arg) { }

    void add_graph(const Graph &G, graph_name_type name)
    {
      graph_labelling[&G].name = name;
    }

    void associate(const Graph &G, Vertex v, id_type id)
    {
      graph_labelling[&G].mapping.insert( association( v, id ));
    }

    std::pair < Vertex, bool > find_vertex(const Graph &G, Vertex v, const Graph &g_out)
    {
      std::pair < id_type, bool > id = get_id(G, v);
      if (id.second == false)
        return std::pair<Vertex, bool> (dummy_vertex, false);
      return get_vertex(g_out, id.first);
    }

    std::pair < Vertex, bool > get_vertex(const Graph &G, id_type id)
    {
      try {
        return std::pair<Vertex, bool> (graph_labelling[&G].mapping.right.at(id), true);
      }
      catch ( std::out_of_range & e ) {
        return std::pair<Vertex, bool> (dummy_vertex, false);
      }
    }

    std::pair < id_type, bool > get_id(const Graph &G, Vertex v)
    {
      try {
        return std::pair<id_type, bool> (graph_labelling[&G].mapping.left.at(v), true);
      }
      catch ( std::out_of_range & e ) {
        return std::pair<id_type, bool> (dummy_id, false);
      }
    }

    bool equal(const Graph &G1, Vertex v1, const Graph &G2, Vertex v2)
    {
      typename std::map < const Graph *, labelling >::const_iterator
        i1 = graph_labelling.find(&G1), i2 = graph_labelling.find(&G2);
      if (i1 == graph_labelling.end() || i2 == graph_labelling.end())
        return false;
      bm_type m1 = i1->second.mapping, m2 = i2->second.mapping;
      typename bm_type::left_const_iterator b1 = m1.left.find(v1), b2 = m2.left.find(v2);
      if (b1 == m1.left.end() || b2 == m2.left.end())
        return false;
      return b1->second == b2->second;
    }

    void show_associations()
    {
      typename std::map < const Graph *, labelling >::const_iterator iter;
      for ( iter = graph_labelling.begin(); iter != graph_labelling.end(); iter++) {
        labelling l = iter->second;
        std::cout << "Graph " << l.name << ": (" << l.mapping.size() << " associations)" << std::endl;
        typename bm_type::const_iterator liter, lend = l.mapping.end();
        for ( liter = l.mapping.begin(); liter != lend; liter++ )
          std::cout << "  " << liter->left << " <--> " << liter->right << std::endl;
      }
    }
  };
}

#endif
