#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <fstream>

#include <boost/test/minimal.hpp>
#include <boost/graph/kolmogorov_max_flow.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
//boost utilities we use
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/random.hpp>
#include <boost/property_map.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost;

template <typename Graph, typename CapacityMap, typename ReverseEdgeMap>
std::pair< typename graph_traits<Graph>::vertex_descriptor,typename graph_traits<Graph>::vertex_descriptor>
fill_random_max_flow_graph(Graph& g, CapacityMap cap, ReverseEdgeMap rev, typename graph_traits<Graph>::vertices_size_type n_verts, 
                           typename graph_traits<Graph>::edges_size_type n_edges, std::size_t seed)
{
  typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  const int cap_low = 1;
  const int cap_high = 1000;
  
  //init random numer generator
  minstd_rand gen(seed);
  //generate graph
  generate_random_graph(g, n_verts, n_edges, gen);
  
  //init an uniform distribution int generator
  typedef variate_generator<minstd_rand, uniform_int<int> > tIntGen;
  tIntGen int_gen(gen, uniform_int<int>(cap_low, cap_high));
  //randomize edge-capacities
  //randomize_property<edge_capacity, Graph, tIntGen> (g,int_gen); //we cannot use this, as we have no idea how properties are stored, right?
  typename graph_traits<Graph>::edge_iterator ei, e_end;
  for(tie(ei,e_end) = edges(g); ei != e_end; ++ei)
    cap[*ei] = int_gen();

  //get source and sink node
  vertex_descriptor s = random_vertex(g, gen);
  vertex_descriptor t = graph_traits<Graph>::null_vertex();
  while(t == graph_traits<Graph>::null_vertex() || t == s)
    t = random_vertex(g, gen);
  
  //add reverse edges (ugly... how to do better?!)
  std::list<edge_descriptor> edges_copy;
  tie(ei, e_end) = edges(g);  
  std::copy(ei, e_end, std::back_insert_iterator< std::list<edge_descriptor> >(edges_copy));
  while(!edges_copy.empty()){
    edge_descriptor old_edge = edges_copy.front();
    edges_copy.pop_front();
    vertex_descriptor source_vertex = target(old_edge, g);  
    vertex_descriptor target_vertex = source(old_edge, g);
    bool inserted;
    edge_descriptor  new_edge;
    tie(new_edge,inserted) = add_edge(source_vertex, target_vertex, g); 
    assert(inserted);
    rev[old_edge] = new_edge;
    rev[new_edge] = old_edge ;
    cap[new_edge] = 0;
  }
  return std::make_pair(s,t);
}

long test_adjacency_list_vecS(int n_verts, int n_edges, std::size_t seed){
  typedef adjacency_list_traits<vecS, vecS, directedS> tVectorTraits;
  typedef adjacency_list<vecS, vecS, directedS,
  property<vertex_index_t, long,
  property<vertex_predecessor_t, tVectorTraits::edge_descriptor,
  property<vertex_color_t, boost::default_color_type,
  property<vertex_distance_t, long> > > >,
  property<edge_capacity_t, long,
  property<edge_residual_capacity_t, long,
  property<edge_reverse_t, tVectorTraits::edge_descriptor > > > > tVectorGraph;
  
  tVectorGraph g;
  
  graph_traits<tVectorGraph>::vertex_descriptor src,sink;
  tie(src,sink) = fill_random_max_flow_graph(g, get(edge_capacity,g), get(edge_reverse, g), n_verts, n_edges, seed);
  
  return kolmogorov_max_flow(g, get(edge_capacity, g),
                             get(edge_residual_capacity, g),
                             get(edge_reverse, g),
                             get(vertex_predecessor, g),
                             get(vertex_color, g),
                             get(vertex_distance, g),
                             get(vertex_index, g),
                             src, sink);
}

long test_adjacency_list_listS(int n_verts, int n_edges, std::size_t seed){
  typedef adjacency_list_traits<listS, listS, directedS> tListTraits;
  typedef adjacency_list<listS, listS, directedS,
  property<vertex_index_t, long,
  property<vertex_predecessor_t, tListTraits::edge_descriptor,
  property<vertex_color_t, boost::default_color_type,
  property<vertex_distance_t, long> > > >,
  property<edge_capacity_t, long,
  property<edge_residual_capacity_t, long,
  property<edge_reverse_t, tListTraits::edge_descriptor > > > > tListGraph;
  
  tListGraph g;
  
  graph_traits<tListGraph>::vertex_descriptor src,sink;
  tie(src,sink) = fill_random_max_flow_graph(g, get(edge_capacity,g), get(edge_reverse, g), n_verts, n_edges, seed);
  
  //initialize vertex indices
  graph_traits<tListGraph>::vertex_iterator vi,v_end;
  graph_traits<tListGraph>::vertices_size_type index = 0;
  for(tie(vi, v_end) = vertices(g); vi != v_end; ++vi, ++index){
    put(vertex_index, g, *vi, index);
  }
  
  return kolmogorov_max_flow(g, get(edge_capacity, g),
                             get(edge_residual_capacity, g),
                             get(edge_reverse, g),
                             get(vertex_predecessor, g),
                             get(vertex_color, g),
                             get(vertex_distance, g),
                             get(vertex_index, g),
                             src, sink);
}

template<typename EdgeDescriptor>
    struct Node{
  boost::default_color_type vertex_color;
  long vertex_distance;
  EdgeDescriptor vertex_predecessor;
};

template<typename EdgeDescriptor>
    struct Link{
  long edge_capacity;
  long edge_residual_capacity;
  EdgeDescriptor edge_reverse;
};

long test_bundled_properties(int n_verts, int n_edges, std::size_t seed){
  typedef adjacency_list_traits<vecS, vecS, directedS> tTraits;
  typedef Node<tTraits::edge_descriptor> tVertex;
  typedef Link<tTraits::edge_descriptor> tEdge;
  typedef adjacency_list<vecS, vecS, directedS, tVertex, tEdge> tBundleGraph;
  
  tBundleGraph g;
  
  graph_traits<tBundleGraph>::vertex_descriptor src,sink;
  tie(src,sink) = fill_random_max_flow_graph(g, get(&tEdge::edge_capacity,g), get(&tEdge::edge_reverse, g), n_verts, n_edges, seed);

  return kolmogorov_max_flow(g, get(&tEdge::edge_capacity, g),
                             get(&tEdge::edge_residual_capacity, g),
                             get(&tEdge::edge_reverse, g),
                             get(&tVertex::vertex_predecessor, g),
                             get(&tVertex::vertex_color, g),
                             get(&tVertex::vertex_distance, g),
                             get(vertex_index, g),
                             src, sink);
}

long test_overloads(int n_verts, int n_edges, std::size_t seed){
  typedef adjacency_list_traits<vecS, vecS, directedS> tTraits;
  typedef property <edge_capacity_t, long,
     property<edge_residual_capacity_t, long,
     property<edge_reverse_t, tTraits::edge_descriptor> > >tEdgeProperty;
  typedef adjacency_list<vecS, vecS, directedS, no_property, tEdgeProperty> tGraph;
  
  tGraph g;
  
  graph_traits<tGraph>::vertex_descriptor src,sink;
  tie(src,sink) = fill_random_max_flow_graph(g, get(edge_capacity,g), get(edge_reverse, g), n_verts, n_edges, seed);

  std::vector<graph_traits<tGraph>::edge_descriptor> predecessor_vec(n_verts);
  std::vector<default_color_type> color_vec(n_verts);
  std::vector<graph_traits<tGraph>::vertices_size_type> distance_vec(n_verts);

  long flow_overload_1 = kolmogorov_max_flow(g, get(edge_capacity,g), get(edge_residual_capacity,g), get(edge_reverse,g), get(vertex_index,g), src, sink);
  long flow_overload_2 = kolmogorov_max_flow(g, get(edge_capacity,g), get(edge_residual_capacity,g), get(edge_reverse,g),
                                             &(color_vec[0]), get(vertex_index,g), src, sink);
  BOOST_REQUIRE(flow_overload_1 == flow_overload_2);
  return flow_overload_1;
}

int test_main(int argc, char* argv[])
{
  int n_verts = 5; 
  int n_edges = 10;
  std::size_t seed = 1;

  /*
  * are checks for different calls to kolmogorov_max_flow and different graph-types
  */
  //checks support of vecS storage
  long flow_vecS = test_adjacency_list_vecS(n_verts, n_edges, seed);
  //checks support of listS storage (especially problems with vertex indices) 
  long flow_listS = test_adjacency_list_listS(n_verts, n_edges, seed);
  BOOST_REQUIRE(flow_vecS == flow_listS);
  //checks bundled properties
  long flow_bundles = test_bundled_properties(n_verts, n_edges, seed);
  BOOST_REQUIRE(flow_listS == flow_bundles);
  //checks overloads
  long flow_overloads = test_overloads(n_verts, n_edges, seed);
  BOOST_REQUIRE(flow_bundles == flow_overloads);

  return 0;
}

/*
}
          
          void invariant_four(tVertex v) const{
            //passive nodes in S or T
            if(v==m_source || v==m_sink)
              return;
            typename std::list<tVertex>::const_iterator it = find(m_orphans.begin(),m_orphans.end(), v);
            // a node is active, if its in the active_list AND (is has_a_parent, or its already in the orphans_list or its the sink, or its the source)
            bool is_active = (m_in_active_list_map[v] && (has_parent(v) || it != m_orphans.end() || (v==m_source || v==m_sink)));
                        
            if(get_tree(v) != tColorTraits::gray() && !is_active(v)){
              tOutEdgeIterator ei,e_end;
              for(tie(ei, e_end) = out_edges(v,m_fg);ei != e_end; ++ei){
                const tVertex& other_node = target(*ei, m_fg);
                if(get_tree(other_node) != get_tree(v)){ 
                  if(get_tree(v) == tColorTraits::white())
                    assert(m_res_cap_map[*ei] == 0);
                  else
                    assert(m_res_cap_map[m_rev_edge_map[*ei]] == 0);
}
}
}
}
          
          void invariant_five(const tVertex& v) const{
            assert(get_tree(v) != tColorTraits::gray() || m_time_map[v] <= m_time);
}
          
          void invariant_six(const tVertex& v) const{
            if(get_tree(v) == tColorTraits::gray() || m_time_map[v] != m_time)
              return;
            else{
              tVertex current_node = v;
              tDistanceVal distance = 0;
              tColorValue color = get_tree(v);
              tVertex terminal = (color==tColorTraits::white())? m_source:m_sink;
              while(current_node != terminal){
                tEdge e = get_edge_to_parent(current_node);
                ++distance;
                current_node = (color==tColorTraits::white())? source(e, m_fg) : target(e, m_fg);
                if(distance > m_dist_map[v])
                  break;
}
              assert(distance == m_dist_map[v]);
              return;     
}
}
          
          void invariant_seven(const tVertex& v) const{
            if(get_tree(v) == tColorTraits::gray())
              return;
            else{
              tEdge e = get_edge_to_parent(v);   
              tColorValue color = get_tree(v);
              long time = m_time_map[v];
              while(e){
                tVertex current_node = (color==tColorTraits::white()) ? source(*e, m_fg):target(*e, m_fg);
                assert(m_time_map[current_node] >= time);
                e = get_edge_to_parent(current_node);   
}
              return;
}
}//invariant_seven
          
          void invariant_eight(const tVertex& v) const{
            if(get_tree(v) == tColorTraits::gray())
              return;
            else{
              tEdge e = get_edge_to_parent(v);   
              tColorValue color= get_tree(v);
              long time = m_time_map[v];
              tDistanceVal distance = m_dist_map[v];
              while(e){
                tVertex current_node = (color == tColorTraits::white())? source(*e, m_fg):target(*e, m_fg);
                if(m_time_map[current_node] == time)
                  assert(m_dist_map[current_node] < distance);
                e = get_edge_to_parent(current_node);   
}
              return;
}
}
          */

// template< typename Graph> 
//     std::pair<typename graph_traits<Graph>::vertex_descriptor, typename graph_traits<Graph>::vertex_descriptor>
//     generate_max_flow_graph(Graph& g, typename graph_traits<Graph>::vertices_size_type n_verts, typename graph_traits<Graph>::edges_size_type n_edges, long seed){
//   typedef typename graph_traits<Graph>::edge_descriptor tEdge;
//   typedef typename graph_traits<Graph>::vertex_descriptor tVertex;
// 
//   return std::make_pair(s,t);
// }

// namespace boost{
// namespace detail{
//   template <class Graph,
//   class EdgeCapacityMap,
//   class ResidualCapacityEdgeMap,
//   class ReverseEdgeMap,
//   class PredecessorMap,
//   class ColorMap,
//   class DistanceMap,
//   class IndexMap>
//     class kolmogorov_test:public kolmogorov<Graph,EdgeCapacityMap,ResidualCapacityEdgeMap,ReverseEdgeMap,PredecessorMap,ColorMap,DistanceMap,IndexMap>{
//       
//     typedef typename graph_traits<Graph>::edge_descriptor tEdge;
//     typedef typename property_traits< typename property_map<Graph, edge_capacity_t>::const_type>::value_type tEdgeVal;
//     
//       public:
//         kolmogorov_test(Graph& g, typename graph_traits<Graph>::vertex_descriptor src, typename graph_traits<Graph>::vertex_descriptor sink):
//           kolmogorov<Graph,EdgeCapacityMap,ResidualCapacityEdgeMap,ReverseEdgeMap,PredecessorMap,ColorMap,DistanceMap,IndexMap>(g, get(edge_capacity,g), get(edge_residual_capacity,g), get(edge_reverse, g), get(vertex_predecessor, g), get(vertex_color, g), get(vertex_distance, g),  get(vertex_index, g), src, sink){}
//         
//         tEdgeVal test(){    
// 
//           add_active_node(this->m_sink);
// //           this->augment_direct_paths();                       
//             //start the main-loop
//           
//           while(true){
//             std::cout << this->m_flow <<std::endl;
//             bool path_found;
//             tEdge connecting_edge;
//             tie(connecting_edge, path_found) = this->grow(); //find a path from source to sink
//             if(!path_found){
//                 //we're finished, no more paths were found
//               break; 
//             }
//             ++(this->m_time);
//             this->augment(connecting_edge); //augment that path
//             this->adopt(); //rebuild search tree structure
//           }
//           return this->m_flow;                      
//         }
//   };
//   
//   template <class Graph>
//   typename property_traits< typename property_map<Graph, edge_capacity_t>::const_type>::value_type 
//   test_kolmogorov(Graph g, typename graph_traits<Graph>::vertex_descriptor source, typename graph_traits<Graph>::vertex_descriptor sink){
//     typedef typename property_map<Graph, edge_capacity_t>::type tEdgeCapMap;
//     typedef typename property_map<Graph, edge_residual_capacity_t>::type tEdgeResCapMap;
//     typedef typename property_map<Graph, edge_reverse_t>::type tRevEdgeMap;
//     typedef typename property_map<Graph, vertex_predecessor_t>::type tVertexPredMap;
//     typedef typename property_map<Graph, vertex_color_t>::type tVertexColorMap;
//     typedef typename property_map<Graph, vertex_distance_t>::type tDistanceMap;
//     typedef typename property_map<Graph, vertex_index_t>::type tIndexMap;
//     kolmogorov_test<Graph, tEdgeCapMap, tEdgeResCapMap, tRevEdgeMap, tVertexPredMap, tVertexColorMap, tDistanceMap, tIndexMap> instance(g,source,sink);
//     return instance.test();
//   }
// }//namespace detail
// }//namespace boost


