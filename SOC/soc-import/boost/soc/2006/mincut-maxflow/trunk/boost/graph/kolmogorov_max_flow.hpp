//=======================================================================
// Copyright 2006 University of Mannheim.
// Author: Stephan Diederich
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

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
#include <boost/optional.hpp> //edge-predecessor map is modeled with boost::optional
#include "boost/none_t.hpp"
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
      typedef typename tGraphTraits::edge_iterator tEdgeIterator;
      typedef typename tGraphTraits::out_edge_iterator tOutEdgeIterator;
      
      typedef typename std::list<tVertex>::iterator tActiveNodesIterator;
      
      typedef typename boost::optional<tEdge> tOptionalEdge;
          
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
            m_pre_map(num_vertices(g),tOptionalEdge()),
            m_time_map(num_vertices(g),0);
            m_flow(0),
            m_time(0){
              
            // initialize the color-map with gray-values
            tVertexIterator vi,v_end;
            for (tie(vi, v_end) = vertices(m_g); vi != v_end; ++vi){
              set_tree(*vi,tColorTraits::gray());
            }
            // Initialize flow to zero which means initializing
            // the residual capacity equal to the capacity.            
            tEdgeIterator ei,e_end;
            for (tie(ei, e_end) = edges(m_g); ei != e_end; ++ei) {
              m_res_cap_map[*ei] = m_cap_map[*ei];
            }
            //init the search trees with the two terminals
            set_tree(m_source,tColorTraits::white());
            set_tree(m_sink,tColorTraits::black());
            //populate the active_nodes queue with the two terminals
            add_active_node(m_source);
            add_active_node(m_sink);            
          }
          
          ~kolmogorov(){}
      
          tEdgeVal max_flow(){
            while(true){
              tOptionalEdge e = grow(); //find a path from source to sink, the returning pair is the link
              if( !e ){
                //we're finished, no path was found
                break; 
              }
              ++m_time;
              augment(*e); //augment that path
              std::cout << "max_flow " << m_flow << std::endl;
              adopt(); //rebuild search tree structure
            }
            return m_flow;
          }
          
        private:
          /**
          * returns the connecting edge of a found path from s->t , read "the link"
          * if returnVal is initialized (convertible to true):
          *   source(returnVal, m_g) is the end of the path found in the source-tree
          *   target(returnVal, m_g) is the beginning of the path found in the sink-tree
          * else :
          *   no path was found
          */
          tOptionalEdge grow(){
            tVertex current_node;
            while((current_node = get_next_active_node()) != tGraphTraits::null_vertex()){ //if there is one
              if(get_tree(current_node) == tColorTraits::white()){ 
                //source tree growing
                tOutEdgeIterator ei,e_end;
                for(tie(ei,e_end) = out_edges(current_node,m_g);ei != e_end;++ei){
                  if(m_res_cap_map[*ei]>0){
                    const tVertex& other_node = target(*ei,m_g);
                    if(get_tree(other_node) == tColorTraits::gray()){ //it's a free node
                      set_tree(other_node,tColorTraits::white()); //aquire other node to our search tree
                      set_edge_to_parent(other_node,*ei);
                      add_active_node(other_node);
                    }
                    else if(get_tree(other_node) == tColorTraits::black()){ 
                    //kewl, we found a path from one to the other search tree
                      return *ei;
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
                  tEdge in_edge = m_rev_edge_map[*ei];
                  if(m_res_cap_map[in_edge]>0){
                    const tVertex& other_node = source(in_edge,m_g);
                    if(get_tree(other_node) == tColorTraits::gray()){ //it's a free node
                      set_tree(other_node, tColorTraits::black()); //aquire other node to our search tree
                      set_edge_to_parent(other_node,in_edge);
                      add_active_node(other_node);
                    }
                    else if(get_tree(other_node) == tColorTraits::white()){ 
                    //kewl, we found a path from one to the other search tree
                      return in_edge;
                    }
                  }
                }//for all out-edges                 
              }//sink-tree growing
              remove_active_node( current_node );
            } //while active_nodes not empty
            return tOptionalEdge(); //no active nodes anymore and no path found, we're done
          }

          /**
          * augments path from s->t and updates residual graph
          * source(e,m_g) is the end of the path found in the source-tree
          * target(e,m_g) is the beginning of the path found in the sink-tree
          */
          void augment(const tEdge& e){
            assert(m_orphans.empty());
            const tEdgeVal bottleneck = find_bottleneck(e);
            //now we push the found flow through the path
            //for each edge we saturate we have to look for the verts that belong to that edge, they become orphans
            //first the connecting edge
            process_edge(e,bottleneck);
            tVertex current_node = source(e,m_g);
            //now back to the source
            while(current_node != m_source){
              tEdge pred = *get_edge_to_parent(current_node);
              process_edge(pred,bottleneck);
              current_node = source(pred,m_g);
            }
            //then go forward in the sink-tree
            current_node = target(e,m_g);
            while(current_node != m_sink){
              tEdge pred = *get_edge_to_parent(current_node);
              process_edge(pred,bottleneck);
              current_node = target(pred,m_g);
            }
            //and add it to the max-flow
            m_flow += bottleneck;
          }

          /**
          * rebuild search trees
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
                  tEdge in_edge = m_rev_edge_map[*ei];
                  assert(target(in_edge,m_g) == current_node); //we should be the target of this edge
                  tVertex other_node = source(in_edge,m_g);
                  if(get_tree(other_node) == tColorTraits::white() && m_res_cap_map[in_edge]>0 && has_source_connect(other_node)){
                    set_edge_to_parent(current_node,in_edge);
                    parent_found = true;
                    break;
                  }
                }
                if(!parent_found){              
                  set_tree(current_node,tColorTraits::gray());
                  remove_active_node(current_node);
                  for(tie(ei,e_end) = out_edges(current_node,m_g);ei != e_end;++ei){
                    tEdge in_edge = m_rev_edge_map[*ei];
                    tVertex other_node = source(in_edge,m_g);
                    if(get_tree(other_node) == tColorTraits::white()){
                      if (m_res_cap_map[in_edge]>0 ){
                        add_active_node(other_node);
                      }
                      tOptionalEdge e=get_edge_to_parent(other_node);
                      if(e && source(*e,m_g) == current_node){
                        set_edge_to_parent(other_node,tOptionalEdge());
                        m_orphans.push(other_node);
                      }
                    }                    
                  }
                }//no parent found
              }//source-tree-adoption
              else{
                //now we should be in the sink-tree, check that...
                assert(get_tree(current_node) == tColorTraits::black());
                //first check its neighbors
                tOutEdgeIterator ei,e_end;
                bool parent_found = false;
                for(tie(ei,e_end) = out_edges(current_node,m_g);ei != e_end;++ei){
                  const tVertex& other_node = target(*ei,m_g);
                  if(get_tree(other_node) == tColorTraits::black() && m_res_cap_map[*ei]>0 && has_sink_connect(other_node)){
                    set_edge_to_parent(current_node,*ei);
                    parent_found = true;
                    break;
                  }
                }
                if(!parent_found){
                  set_tree(current_node,tColorTraits::gray());
                  remove_active_node(current_node);
                  for(tie(ei,e_end) = out_edges(current_node,m_g);ei != e_end;++ei){
                    const tVertex& other_node = target(*ei,m_g);
                    if(get_tree(other_node) == tColorTraits::black()){ 
                      tOptionalEdge e=get_edge_to_parent(other_node);
                      if(e && target(*e,m_g) == current_node){
                        //check if we generate more orphans
                        set_edge_to_parent(other_node,tOptionalEdge());
                        m_orphans.push(other_node);
                      }
                      if(m_res_cap_map[*ei]>0 ){
                        add_active_node(other_node);
                      }  
                    }
                  }
                }//no parent found                
              }//sink-tree adoption
            }//while !oprhans.empty()
          }//adopt

          /**
          * return next active vertex if there is one, otherwise a null_vertex
          */	
          inline tVertex get_next_active_node(){
            if(m_active_nodes.empty())
              return tGraphTraits::null_vertex();
            else{
              tVertex v = m_active_nodes.front();
              assert(get_tree(v)==tColorTraits::white() || get_tree(v)==tColorTraits::black());
              return v;              
            }
          }
          
          /**
          * adds v as an active vertex
          */		
          inline void add_active_node(const tVertex& v){
            remove_active_node(v);
            m_active_nodes.push_back(v);
          }
          
          /**
          * removes a vertex from the queue of active nodes 
          */		
          inline void remove_active_node(const tVertex& v){
            tActiveNodesIterator ait=std::find(m_active_nodes.begin(),m_active_nodes.end(),v);
            if(ait!=m_active_nodes.end())
              m_active_nodes.erase(ait);
          }
          
          /**
          * returns the search tree of v; tColorValue::white() for source tree, black() for sink tree, gray() for no tree
          */		
          inline tColorValue get_tree(const tVertex& v) const {
            return m_tree_map[v];
          }
          
          /**
          * sets search tree of v; tColorValue::white() for source tree, black() for sink tree, gray() for no tree
          */		
          inline void set_tree(const tVertex& v, tColorValue t){
            m_tree_map[v] = t;
          }
          
          /**
           * returns edge to parent vertex of v;
           */		
          inline tOptionalEdge get_edge_to_parent(const tVertex& v) const {
            assert(get_tree(v) == tColorTraits::black() || get_tree(v) == tColorTraits::white());
            return m_pre_map[v];
          }
          
          /**
           * sets edge to parent vertex of v; 
          */		
          inline void set_edge_to_parent(const tVertex& v, const tOptionalEdge& f_edge_to_parent){
            assert(!f_edge_to_parent || m_res_cap_map[*f_edge_to_parent]>0);
            m_pre_map[v] = f_edge_to_parent;
          }
          
          /**
          * returns the bottleneck of a s->t path (end_of_path is last vertex in source-tree, begin_of_path is first vertex in sink-tree)
          */		
          inline tEdgeVal find_bottleneck(const tEdge& e) const{
            BOOST_USING_STD_MIN();
            tEdgeVal minimum_cap = m_res_cap_map[e];
            tVertex current_node = source(e,m_g);
            //first go back in the source tree
            while(current_node != m_source){
              tEdge pred = *get_edge_to_parent(current_node);
              minimum_cap = min BOOST_PREVENT_MACRO_SUBSTITUTION(minimum_cap,m_res_cap_map[pred]);
              current_node = source(pred,m_g);
            }
            //then go forward in the sink-tree
            current_node = target(e,m_g);
            while(current_node != m_sink){
              tEdge pred = *get_edge_to_parent(current_node);
              minimum_cap = min BOOST_PREVENT_MACRO_SUBSTITUTION(minimum_cap,m_res_cap_map[pred]);
              current_node = target(pred,m_g);
            }
            return minimum_cap;
          }
          
          /**
          * processes an edge from source to target, updates residual and checks if we generate orphans
          */		
          inline void process_edge(const tEdge& e, tEdgeVal bottleneck){
            m_res_cap_map[e] -= bottleneck;
            m_res_cap_map[m_rev_edge_map[e]]+=bottleneck;
            assert(m_res_cap_map[e] >= 0);
            if(m_res_cap_map[e] == 0){
              const tVertex& source_vertex=source(e,m_g);
              const tVertex& target_vertex=target(e,m_g);              
              tColorValue source_color = get_tree(source_vertex);
              tColorValue target_color = get_tree(target_vertex);
              if(source_color == target_color){
                if(source_color == tColorTraits::white()){
                  set_edge_to_parent(target_vertex,tOptionalEdge());
                  m_orphans.push(target_vertex);
                }
                else{
                  assert(source_color == tColorTraits::black()) ; //now both of them have to be in the sink tree
                  set_edge_to_parent(source_vertex,tOptionalEdge());
                  m_orphans.push(source_vertex);
                }
              }
            }
          }
          
          /**
          * returns true if we found a valid path back to the source TODO: use depth-first-search and store in nodes with timestamp
          */			
          inline bool has_sink_connect(const tVertex& v) const{
            tOptionalEdge e = get_edge_to_parent(v);
            while(e){
              tVertex target_vertex = target(*e,m_g);
              if(target_vertex == m_sink)
                return true;
              else
                e = get_edge_to_parent(target_vertex);
            }
            return false;
          }
          
          /**
          * returns true if we found a valid path to the sink TODO: use depth-first-search and store in nodes with timestamp
          */			
          inline bool has_source_connect(const tVertex& v) const{
            tOptionalEdge e = get_edge_to_parent(v);
            while(e){
              tVertex source_vertex = source(*e,m_g);
              if(source_vertex == m_source){
                return true;
              }
              else
                e = get_edge_to_parent(source_vertex);
            }
            return false;
          }          
          Graph& m_g;
          EdgeCapacityMap m_cap_map;
          ResidualCapacityEdgeMap m_res_cap_map;
          ReverseEdgeMap m_rev_edge_map;
          ColorMap m_tree_map; //maps each vertex into one of the two search tree or none (gray())
          tVertex m_source;
          tVertex m_sink;
          std::list<tVertex> m_active_nodes;
          tQueue m_orphans;
          std::vector< tOptionalEdge > m_pre_map; //stores paths found in the growth stage
          std::vector< long > m_time_map; //timestamp of each node, used for sink/source-path calculations
          tEdgeVal m_flow;
          long m_time;
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

