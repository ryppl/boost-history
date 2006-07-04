//=======================================================================
// Copyright 2006 University of Mannheim.
// Author: Stephan Diederich
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

//TODO: use edges for predecessor map to avoid find
//TODO: augment and adopt
//TODO: has_sink_connect and has_source_connect <- use breadth first, add visitor 
//TODO: add timestamp map to get rid of double checking source and sink connects
//TODO: add second queque (swapping), to get at least for first search a minimum path
//TODO: add paremeter versions
//TODO: why is there a double delete on one of the vertex-vecs
//TODO: work out needed graph-types and parameters
//TODO: add concept checks
//

#ifndef BOOST_KOLMOGOROV_MAX_FLOW_HPP
#define BOOST_KOLMOGOROV_MAX_FLOW_HPP

#include <boost/config.hpp>
#include <cassert>
#include <vector>
#include <list>
#include <utility>
#include <iosfwd>
#include <algorithm> // for std::min and std::max

#include <boost/pending/queue.hpp>
#include <boost/limits.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/named_function_params.hpp>

namespace boost {
  namespace detail {
    template <class Graph,
              class EdgeCapacityMap,
              class ResidualCapacityEdgeMap,
              class ReverseEdgeMap,
              class ColorMap>
    class kolmogorov{
      typedef typename property_traits< typename property_map<Graph, edge_capacity_t>::const_type>::value_type tEdgeVal;
      typedef graph_traits<Graph> tGraphTraits;
      typedef typename tGraphTraits::vertex_descriptor tVertex;
      typedef typename tGraphTraits::vertex_iterator tVertexIterator;
      typedef typename tGraphTraits::edge_descriptor tEdge;
      typedef typename tGraphTraits::out_edge_iterator tOutEdgeIterator;
      
      typedef boost::queue<tVertex> tQueue; //queue of vertices
      typedef typename property_traits<ColorMap>::value_type tColorValue;
      typedef color_traits<tColorValue> tColorTraits;
        public:
          kolmogorov(Graph& g,
                     EdgeCapacityMap cap,
                     ResidualCapacityEdgeMap res,    
                     ReverseEdgeMap rev,
                     ColorMap color,                 
                     typename graph_traits<Graph>::vertex_descriptor src,
                     typename graph_traits<Graph>::vertex_descriptor sink):
          m_g(g),
          m_cap_map(cap),
          m_res_cap_map(res),
          m_rev_edge_map(rev),
          m_tree_map(color),
            m_source(src),
            m_sink(sink),
            m_active_override(num_vertices(m_g),false),
            m_pre_map(num_vertices(g)),
            m_flow(0){
              
            //populate the active_nodes queue with the two terminals
              add_active_node(m_source);
              add_active_node(m_sink);
            
            // Initialize flow to zero which means initializing
            // the residual capacity to equal the capacity.
            // additionally initialize the color-map with gray-values
            // and the parent map
            tVertexIterator vi,v_end;
            tOutEdgeIterator ei, e_end;
            for (tie(vi, v_end) = vertices(m_g); vi != v_end; ++vi){
              if(*vi != m_source && *vi != m_sink) //TODO remove this if
                set_tree(*vi,tColorTraits::gray());
              set_parent(*vi,tGraphTraits::null_vertex());
              for (tie(ei, e_end) = out_edges(*vi, m_g); ei != e_end; ++ei) {
                m_res_cap_map[*ei] = m_cap_map[*ei];
              }
            }
            //init the search trees with the two terminals
            m_tree_map[m_source] = tColorTraits::white();
            m_tree_map[m_sink] = tColorTraits::black();
//             set_tree(m_source,tColorTraits::white()); //TODO: remove me for 2 lines above
//             set_tree(m_sink,tColorTraits::black());
          }
          ~kolmogorov(){}
      
          tEdgeVal max_flow(){
            while(true){
              tVertex end_of_path,beginning_of_path; 
              tie(end_of_path,beginning_of_path) = grow(); //find a path from source to sink, the returning pair is the link
              if( end_of_path == tGraphTraits::null_vertex())
                break; //we're finished, no path was found
              augment(end_of_path,beginning_of_path); //augment that path
//               std::cout << "max_flow " << m_flow << std::endl;
              adopt(); //rebuild search tree structure
            }
            return m_flow;
          }
          
        private:
          /**
          * returns main parts of a found path from s->t , read "the link"
          * the first tVertex is the end of the path found in the source-tree
          * the second tVertex is the beginning of the path found in the sink-tree
          */
          std::pair<tVertex,tVertex> grow(){
            tVertex current_node = get_next_active_node();//get first vertex from active list and remove it
            while(current_node != tGraphTraits::null_vertex()){ //if there is one
              if(get_tree(current_node) == tColorTraits::white()){ 
                //source tree growing
                tOutEdgeIterator ei,e_end;
                for(tie(ei,e_end) = out_edges(current_node,m_g);ei != e_end;++ei){
                  if(m_res_cap_map[*ei]>0){
                    const tVertex& other_node = target(*ei,m_g);
                    if(get_tree(other_node) == tColorTraits::gray()){ //it's a free node
                      set_tree(other_node,get_tree(current_node)); //aquire other node to our search tree
                      set_parent(other_node,current_node);
                      add_active_node(other_node);
                    }
                    else if(get_tree(other_node) != get_tree(current_node)){ 
                    //kewl, we found a path from one to the other search tree
                      return std::make_pair(current_node,other_node);
                    }
                  }
                }//for all out-edges
              } //source-tree-growing
              else{
                //sink-tree growing
                assert(get_tree(current_node) == tColorTraits::black()); 
                tOutEdgeIterator ei,e_end;
                for(tie(ei,e_end) = out_edges(current_node,m_g);ei != e_end;++ei){
                  //get the edge back to me
                  tEdge rev_edge = m_rev_edge_map[*ei];
                  if(m_res_cap_map[rev_edge]>0){
                    const tVertex& other_node = source(rev_edge,m_g);
                    if(get_tree(other_node) == tColorTraits::gray()){ //it's a free node
                      set_tree(other_node, tColorTraits::black()); //aquire other node to our search tree
                      set_parent(other_node,current_node);
                      add_active_node(other_node);
                    }
                    else if(get_tree(other_node) == tColorTraits::white()){ 
                    //kewl, we found a path from one to the other search tree
                      return std::make_pair(other_node,current_node);
                    }
                  }
                }//for all out-edges                 
              }//sink-tree growing
              current_node = get_next_active_node(); //get next active node
            } //while active_nodes not empty
            return std::make_pair(tGraphTraits::null_vertex(),tGraphTraits::null_vertex()); //no active nodes anymore and no path found, we're done
          }

          /**
          * augments path from s->t and updates residual graph
          * the first tVertex is the end of the path found in the source-tree
          * the second tVertex is the beginning of the path found in the sink-tree
          */
          void augment(const tVertex& end_of_path,const tVertex& beginning_of_path){
            assert(m_orphans.empty());
            const tEdgeVal bottleneck = find_bottleneck(end_of_path,beginning_of_path);
            //now we push the found flow through the path
            //for each edge we saturate we have to look for the verts that belong to that edge, they become orphans
            //first the connecting edge
            process_edge(end_of_path,beginning_of_path,bottleneck);
            tVertex current_node = end_of_path;
            //now back to the sink
            while(current_node != m_source){
              tVertex pred = get_parent(current_node);
              process_edge(pred,current_node,bottleneck);
              current_node = pred;
            }
            //then go forward in the sink-tree
            current_node = beginning_of_path;
            while(current_node != m_sink){
              tVertex pred = get_parent(current_node);
              process_edge(current_node,pred,bottleneck);
              current_node = pred;
            }
            //and add it to the max-flow
            m_flow += bottleneck;
          }

          /**
          * rebuild search paths
          * empty the queue of orphans, and find new parents for them or just drop them from the search trees
          */
          void adopt(){
            while(!m_orphans.empty()){
              tVertex current_node = m_orphans.front(); //get the first orphan
              m_orphans.pop();
              if(get_tree(current_node) == tColorTraits::white()){ 
                //we're in the source-tree
                //first check its neighbors
                tOutEdgeIterator ei,e_end;
                bool parent_found = false;
                for(tie(ei,e_end) = out_edges(current_node,m_g);ei != e_end;++ei){
                  tEdge current_edge = m_rev_edge_map[*ei];
                  assert(target(current_edge,m_g) == current_node); //we should be the target of this edge
                  tVertex other_node = source(current_edge,m_g);
                  if(get_tree(other_node) == tColorTraits::white() && m_res_cap_map[current_edge]>0 && has_source_connect(current_node)){
                    set_parent(current_node,other_node);
                    parent_found = true;
                    break;
                  }
                }
                if(!parent_found){
                  for(tie(ei,e_end) = out_edges(current_node,m_g);ei != e_end;++ei){
                    tEdge current_edge = m_rev_edge_map[*ei];
                    tVertex other_node = source(current_edge,m_g);
                    if(get_tree(other_node) == tColorTraits::white() && m_res_cap_map[current_edge]>0 ){
                      add_active_node(other_node);                    
                    }
                    if(get_parent(other_node) == current_node){
                      set_parent(other_node,tGraphTraits::null_vertex());
                      m_orphans.push(other_node);
                    }
                  }
                  set_tree(current_node,tColorTraits::gray());
                  remove_active_node(current_node);
                }//no parent found
              }//source-tree-adoption
              else{
                //now we should be in the sink-tree, check that...
                assert(get_tree(current_node) == tColorTraits::black());
                //first check its neighbors
                tOutEdgeIterator ei,e_end;
                bool parent_found = false;
                for(tie(ei,e_end) = out_edges(current_node,m_g);ei != e_end;++ei){
                  const tEdge& current_edge = *ei;
                  assert(source(current_edge,m_g) == current_node); //we should be the source of this edge
                  const tVertex& other_node = target(current_edge,m_g);
                  if(get_tree(other_node) == tColorTraits::black() && m_res_cap_map[current_edge]>0 && has_sink_connect(current_node)){
                    set_parent(current_node,other_node);
                    parent_found = true;
                    break;
                  }
                }
                if(!parent_found){
                  for(tie(ei,e_end) = out_edges(current_node,m_g);ei != e_end;++ei){
                    const tEdge& current_edge = *ei;
                    const tVertex& other_node = target(current_edge,m_g);
                    if(get_tree(other_node) == tColorTraits::black() && m_res_cap_map[current_edge]>0 ){
                      add_active_node(other_node);
                    }
                    if(get_parent(other_node) == current_node){
                        set_parent(other_node,tGraphTraits::null_vertex());
                        m_orphans.push(other_node);
                    }
                  }
                  set_tree(current_node,tColorTraits::gray());
                  remove_active_node(current_node);
                }                
              }
            }
          }

          /**
          * return next active vertex if there is one, otherwise a null_vertex
          * active nodes are stored in a boost::queue. To avoid searching in it, if we want to remove a node,
          * we set an override flag. so check here if it is set, if so, dont return that node as an active one
          */	
          inline tVertex get_next_active_node(){
            while(!m_active_nodes.empty()){
              tVertex v = m_active_nodes.front();
              m_active_nodes.pop();
              if(m_active_override[v]){
                m_active_override[v] = false;
              }
              else{
                return v;
              }
            }
            return tGraphTraits::null_vertex();
          }
          /**
          * adds v as an active vertex, and reset the non-active-override flag if set
          */		
          inline void add_active_node(const tVertex& v){
            m_active_override[v] = false;
            m_active_nodes.push(v);
          }
          /**
          * removes a vertex from the queue of active nodes by setting an override flag
          */		
          inline void remove_active_node(const tVertex& v){
            m_active_override[v] = true;
          }
          /**
          * returns the search tree of v; tColorValue::white() for source tree, black() for no sink tree, gray() for no tree
          */		
          inline tColorValue get_tree(const tVertex& v) const {
            return m_tree_map[v];
          }
          /**
          * sets search tree of v; tColorValue::white() for source tree, black() for no sink tree, gray() for no tree
          */		
          inline void set_tree(const tVertex& v, tColorValue t){
            assert(v != m_source && v != m_sink);
            m_tree_map[v] = t;
          }
          /**
          * returns parent vertex of v; TODO: switch to edges here
          */		
          inline tVertex get_parent(const tVertex& v) const {
            return m_pre_map[v];
          }
          /**
          * sets parent vertex of v; TODO: switch to edges here
          */		
          inline void set_parent(const tVertex& v, const tVertex& parent){
            m_pre_map[v] = parent;
          }
          /**
          * returns the bottleneck of a s->t path (end_of_path is last vertex in source-tree, begin_of_path is first vertex in sink-tree)
          */		
          inline tEdgeVal find_bottleneck(const tVertex& end_of_path,const tVertex& begin_of_path) const{
            BOOST_USING_STD_MIN();
            tEdge e; bool has_edge;
            tie(e,has_edge) = edge(end_of_path,begin_of_path,m_g); //this edge must exist, otherwise path would have not been found
            assert(has_edge);
            tEdgeVal minimum_cap = m_res_cap_map[e];
            tVertex current_node = end_of_path;
            //first go back in the source tree
            while(current_node != m_source){
              tVertex pred = get_parent(current_node);
              tie(e,has_edge) = edge(pred,current_node,m_g);
              assert(has_edge);
              minimum_cap = min BOOST_PREVENT_MACRO_SUBSTITUTION(minimum_cap,m_res_cap_map[e]);
              current_node = pred;
            }
            //then go forward in the sink-tree
            current_node = begin_of_path;
            while(current_node != m_sink){
              tVertex pred = get_parent(current_node);
              tie(e,has_edge) = edge(current_node,pred,m_g);
              assert(has_edge);
              minimum_cap = min BOOST_PREVENT_MACRO_SUBSTITUTION(minimum_cap,m_res_cap_map[e]);
              current_node = pred;
            }
            return minimum_cap;
          }
          /**
          * processes an edge from source to target, updates residual and checks if we generate orphans
          */		
          inline void process_edge(tVertex source_vertex, tVertex target_vertex, tEdgeVal bottleneck){
            tEdge current_edge;
            bool has_edge;
            tie(current_edge,has_edge) = edge(source_vertex,target_vertex,m_g);
            assert(has_edge); //this edge MUST be there (otherwise there would be no path)
            m_res_cap_map[current_edge] -= bottleneck;
            assert(m_res_cap_map[current_edge] >= 0);
            if(m_res_cap_map[current_edge] == 0){
              tColorValue source_color = get_tree(source_vertex);
              tColorValue target_color = get_tree(target_vertex);
              if(source_color == target_color){
                if(source_color == tColorTraits::white()){
                  set_parent(target_vertex,tGraphTraits::null_vertex());
                  m_orphans.push(target_vertex);
                }
                else{
                  assert(source_color == tColorTraits::black()) ; //now both of them have to be in the sink tree
                  set_parent(source_vertex,tGraphTraits::null_vertex());
                  m_orphans.push(source_vertex);
                }
              }
            }
          }
          /**
          * returns true if we found a valid path back to the source TODO: use depth-first-search and store in nodes with timestamp
          */			
          inline bool has_sink_connect(const tVertex& v) const{
            tVertex parent = get_parent(v);
            while(parent != tGraphTraits::null_vertex()){
              if(parent == m_sink)
                return true;
              else
                parent = get_parent(parent);
            }
            return false;
          }
          /**
          * returns true if we found a valid path to the sink TODO: use depth-first-search and store in nodes with timestamp
          */			
          inline bool has_source_connect(const tVertex& v) const{
            tVertex parent = get_parent(v);
            while(parent != tGraphTraits::null_vertex()){
              if(parent == m_source)
                return true;
              else
                parent = get_parent(parent);
            }
            return false;
          }          
          Graph& m_g;
          EdgeCapacityMap m_cap_map;
          ResidualCapacityEdgeMap m_res_cap_map;
          ReverseEdgeMap m_rev_edge_map;
          ColorMap m_tree_map; //maps each vertex into the two search tree or none 
          tVertex m_source;
          tVertex m_sink;
          tQueue m_active_nodes;
          tQueue m_orphans;
          std::vector<bool> m_active_override;
          std::vector<tVertex> m_pre_map; //stores paths found in the growth stage
          tEdgeVal m_flow;
    };
  } //namespace detail
  
  /**
   * initial version...
   */			
  template <class Graph, class CapacityEdgeMap, class ResidualCapacityEdgeMap, class ReverseEdgeMap, class ColorMap>
      typename property_traits<
      typename property_map<Graph, edge_capacity_t>::const_type
      >::value_type
    kolmogorov_max_flow
      (Graph& g, 
       CapacityEdgeMap cap,
       ResidualCapacityEdgeMap res_cap,
       ReverseEdgeMap rev_map,
       ColorMap color,
       typename graph_traits<Graph>::vertex_descriptor src,
       typename graph_traits<Graph>::vertex_descriptor sink
        )
      {
        detail::kolmogorov<Graph,CapacityEdgeMap,ResidualCapacityEdgeMap,ReverseEdgeMap,ColorMap> algo(g,cap,res_cap,rev_map,color,src,sink);
         return algo.max_flow();
      }          
				
} // namespace boost

#endif // BOOST_KOLMOGOROV_MAX_FLOW_HPP

