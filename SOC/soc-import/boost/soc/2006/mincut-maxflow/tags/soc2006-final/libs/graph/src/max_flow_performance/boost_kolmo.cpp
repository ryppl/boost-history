//  Copyright (c) 2006, Stephan Diederich
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <boost/graph/kolmogorov_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/read_dimacs.hpp>

//use DIMACS max-flow file as stdin
#include <sys/time.h>
#include <sys/resource.h>
#include <valgrind/callgrind.h>
#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional/optional.hpp>
#include <map>

//define following to use an optimized version of read_dimacs_max_flow
// #define USE_OPTIMIZED_GRAPH 1 

template<typename Graph, typename CapacityMap, typename ReverseEdgeMap>
    void new_read_dimacs_max_flow(Graph& g,
                                  CapacityMap cap,
                                  ReverseEdgeMap rev,
                                  typename boost::graph_traits<Graph>::vertex_descriptor& source_vertex,
                                  typename boost::graph_traits<Graph>::vertex_descriptor& sink_vertex,
                                  std::istream& in = std::cin)
{
  using namespace std;
  using namespace boost;

  //some useful typedefs
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
  
  //vars to store information about the graph
  typedef typename graph_traits<Graph>::vertices_size_type vertices_size_type;
  typedef typename graph_traits<Graph>::edges_size_type edges_size_type;
  
  vertices_size_type n_vertices;
  edges_size_type n_edges;
  
  vertices_size_type source_vertex_index = 0; //just to get rid of warnings
  vertices_size_type sink_vertex_index = 0;
  
  typedef pair<vertices_size_type, vertices_size_type> edge_storage;
  typedef map<edge_storage, typename property_traits<CapacityMap>::value_type> edge_map_t;
  edge_map_t edge_map;

  //vars for parsing
  std::size_t n_current_line = 0;
  string temp_string;
  bool problem_description_found = false;
  edges_size_type n_of_arc_descriptions = 0;
  while(getline(in, temp_string,'\n')){
    ++n_current_line;
    stringstream current_line(temp_string);
    char line_description;
    current_line >> line_description;
    switch (line_description){
      case 'c':
      case '\n':
      case '\0':
        break;
      case 'p':
      {
        string problem_description;
        current_line >> problem_description;
        if(current_line.fail())
          throw runtime_error("Bad problem description Line:" + lexical_cast<std::string>(n_current_line));
        if(problem_description.compare("max") != 0)
          throw runtime_error("Not a maximum flow problem!  Line:" + lexical_cast<std::string>(n_current_line));
        current_line >> n_vertices;
        if(current_line.fail())
          throw runtime_error("Bad vertex index  Line:" + lexical_cast<std::string>(n_current_line));
        current_line >> n_edges;
        if(current_line.fail())
          throw runtime_error("Bad vertex index  Line:" + lexical_cast<std::string>(n_current_line));
        problem_description_found = true;
        break;
      }
      case 'n':
      {
        if(!problem_description_found)
          throw runtime_error("No problem description found yet! Line:" + lexical_cast<std::string>(n_current_line));
        vertices_size_type index;
        current_line >> index;
        if(current_line.fail())
          throw runtime_error("Bad vertex index in source/sink node description Line:" + lexical_cast<std::string>(n_current_line));
        char source_or_sink;
        current_line >> source_or_sink;
        if(current_line.fail())
          throw runtime_error("Not a valid source/sink node description Line:" + lexical_cast<std::string>(n_current_line));
        switch(source_or_sink){
          case 's':
            source_vertex_index = index - 1;
            break;
          case 't':
            sink_vertex_index = index - 1;
            break;
          default:
            throw runtime_error("Not a valid source/sink node description Line:" + lexical_cast<std::string>(n_current_line));
        }
        break;
      }
      case 'a':
      {
        if(!problem_description_found)
          throw runtime_error("No problem description found yet! Line:" + lexical_cast<std::string>(n_current_line));
        if(++n_of_arc_descriptions > n_edges)
          throw runtime_error("Too many arc-descriptions found Line:" + lexical_cast<std::string>(n_current_line));
        vertices_size_type from_index;
        vertices_size_type to_index;
        typename property_traits<CapacityMap>::value_type capacity;
        current_line >> from_index;
        if(current_line.fail())
          throw runtime_error("Not a valid vertex index Line:" + lexical_cast<std::string>(n_current_line));
        if(from_index > n_vertices)
          throw runtime_error("Vertex index out of bounds Line:" + lexical_cast<std::string>(n_current_line));
        current_line >> to_index;
        if(current_line.fail())
          throw runtime_error("Not a valid vertex index Line:" + lexical_cast<std::string>(n_current_line));
        if(from_index > n_vertices)
          throw runtime_error("Vertex index out of bounds Line:" + lexical_cast<std::string>(n_current_line));
        current_line >> capacity;
        if(current_line.fail())
          throw runtime_error("Not a valid capacity value Line:" + lexical_cast<std::string>(n_current_line));

        //insert this found edge only if there isn't already one to which we can add this capacity
        typename edge_map_t::iterator it = edge_map.find(make_pair(from_index - 1, to_index - 1));
        if(it == edge_map.end())
          edge_map.insert(make_pair(make_pair(from_index - 1, to_index - 1), capacity));
        else
          it->second += capacity;
        break;
      }
      default:
        throw runtime_error("Unexpected tokens found Line:" + lexical_cast<std::string>(n_current_line));
    }//switch
  }//while(getlines)
//   std::cout << "n_verts: " << n_vertices << endl;
//   std::cout << "n_edges: " << n_edges << endl;
//   std::cout << "source: " << source_vertex_index << endl;
//   std::cout << "sink: " << sink_vertex_index << endl;
//   std::cout << "edges_size: " << edge_map.size() << endl;
  //do plausibility checks
  if(n_of_arc_descriptions != n_edges)
    throw runtime_error("Not enough arc descriptions found");

  //create a vector of vertices and add the vertices to the graph
  vector<vertex_descriptor> vertices;
  vertices.resize(n_vertices);
  for(vertices_size_type i = 0; i < n_vertices; ++i){
    vertices[i] = add_vertex(g);
  }
  //store source and sink vertex for return
  source_vertex = vertices[source_vertex_index];
  sink_vertex = vertices[sink_vertex_index];
  typename edge_map_t::iterator eit = edge_map.begin();
  typename edge_map_t::iterator e_end = edge_map.end();
  for(; eit != e_end; ++eit){
    vertices_size_type src_vertex_index = eit->first.first;
    vertices_size_type tgt_vertex_index = eit->first.second;
    typename property_traits<CapacityMap>::value_type capacity = eit->second;
    //check if this edge has already a reverse edge, if not, just add one
    edge_descriptor e;
    bool inserted;
    tie(e, inserted) = add_edge(vertices[src_vertex_index], vertices[tgt_vertex_index], g);
    if(!inserted)
      throw runtime_error("could not add edge to graph");
    cap[e] = capacity;
    typename edge_map_t::iterator it = edge_map.find(make_pair(tgt_vertex_index, src_vertex_index));
    if(it == edge_map.end()){
      tie(e, inserted) = add_edge(vertices[tgt_vertex_index], vertices[src_vertex_index], g);
      if(!inserted)
        throw runtime_error("could not add edge to graph");
      cap[e] = 0;
    }
  }

  //adjust reverse_edge_map
  typename graph_traits<Graph>::edge_iterator edges_it, edges_end;
  for(tie(edges_it, edges_end) = edges(g); edges_it != edges_end; ++edges_it){
    edge_descriptor e;
    bool found;
    tie(e, found) = edge(target(*edges_it, g), source(*edges_it, g), g);
    if(!found)
      throw runtime_error("no reverse edge present");
    rev[*edges_it] = e;
    rev[e] = *edges_it;
//     if(rev[rev[*edges_it]] != *edges_it){
//       throw runtime_error("reverse edges error");
//     }
  }
}

float timer ()
{
  struct rusage r;

  getrusage(0, &r);
  return (float)(r.ru_utime.tv_sec+r.ru_utime.tv_usec/(float)1000000);
}

template <typename EdgeDescriptor>
struct Pixel{
  boost::default_color_type color;
  long distance;
  EdgeDescriptor predecessor;
};

template <typename EdgeDescriptor>
struct Link{
  long capacity;
  long residual_capacity;
  EdgeDescriptor edge_reverse;
};

int
main()
{
  using namespace boost;
  using namespace std;
  typedef adjacency_list_traits <vecS, vecS, directedS> Traits;
  typedef Traits::edge_descriptor edge_descriptor;

  typedef property<vertex_color_t, default_color_type,
          property<vertex_distance_t, long,
          property<vertex_predecessor_t, edge_descriptor> > > tInternalVertexProperties;
  typedef property<edge_capacity_t, long ,
          property<edge_residual_capacity_t, long,
          property<edge_reverse_t, edge_descriptor> > > tInternalEdgeProperties;
 
  typedef adjacency_list <vecS, vecS, directedS, no_property, tInternalEdgeProperties> Graph;

  Graph g;
  typedef property_map<Graph, edge_capacity_t>::type tCapacityMap;
  tCapacityMap cap_map = get(edge_capacity, g);
  typedef property_map<Graph, edge_reverse_t>::type tReverseEdgeMap;
  tReverseEdgeMap rev_edge_map = get(edge_reverse, g);    

  Traits::vertex_descriptor s, t;
#ifdef USE_OPTIMIZED_GRAPH
  new_read_dimacs_max_flow(g, cap_map, rev_edge_map, s, t, std::cin);
#else
  read_dimacs_max_flow(g, cap_map, rev_edge_map, s, t, std::cin);
#endif

  vector<boost::default_color_type> color(num_vertices(g));
  vector<long> distance(num_vertices(g));
  vector<edge_descriptor> predecessor(num_vertices(g));
  
  float time = timer();
//   CALLGRIND_START_INSTRUMENTATION();

  long flow = kolmogorov_max_flow(g, s, t, predecessor_map(&predecessor[0]).color_map(&color[0]).distance_map(&distance[0]));

//   CALLGRIND_STOP_INSTRUMENTATION();
  double elapsed = timer() - time;
  std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
  std::cout << "flow: " << flow << std::endl;
  std::cout << "time: " << elapsed << std::endl;
  return EXIT_SUCCESS;
}
