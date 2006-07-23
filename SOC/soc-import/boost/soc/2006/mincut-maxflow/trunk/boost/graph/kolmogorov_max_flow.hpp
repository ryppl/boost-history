//=======================================================================
// Copyright 2006 University of Mannheim.
// Author: Stephan Diederich
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================


//TODO: remove remove_active_node
//TODO: move invariants to test-class <- howto do? make everything protected,then derive? friend class?
//TODO: search for TODOs

#ifndef BOOST_KOLMOGOROV_MAX_FLOW_HPP
#define BOOST_KOLMOGOROV_MAX_FLOW_HPP

//debug defines

// #define KOLMOGOROV_CHECK_INVARIANTS 1
// 
// #define KOLMOGOROV_CHECK_INVARIANT_4 1
// #define KOLMOGOROV_CHECK_INVARIANT_5 1
// #define KOLMOGOROV_CHECK_INVARIANT_6 1
// #define KOLMOGOROV_CHECK_INVARIANT_7 1
// #define KOLMOGOROV_CHECK_INVARIANT_8 1


#include <boost/config.hpp>
#include <cassert>
#include <vector>
#include <list>
#include <utility>
#include <iosfwd>
#include <algorithm> // for std::min and std::max

#include <boost/pending/queue.hpp>
#include <boost/limits.hpp>
#include <boost/property_map.hpp>
#include <boost/none_t.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/named_function_params.hpp>


namespace boost {
  namespace detail {
    template <class Graph,
              class EdgeCapacityMap,
              class ResidualCapacityEdgeMap,
              class ReverseEdgeMap,
              class PredecessorMap,
              class ColorMap,
              class DistanceMap,
              class IndexMap>
    class kolmogorov{
      typedef typename property_traits< typename property_map<Graph, edge_capacity_t>::const_type>::value_type tEdgeVal;
      typedef graph_traits<Graph> tGraphTraits;
      typedef typename tGraphTraits::vertex_descriptor tVertex;
      typedef typename tGraphTraits::vertex_iterator tVertexIterator;
      typedef typename tGraphTraits::edge_descriptor tEdge;
      typedef typename tGraphTraits::edge_iterator tEdgeIterator;
      typedef typename tGraphTraits::out_edge_iterator tOutEdgeIterator;           
      typedef typename std::list<tVertex>::iterator tVertexListIterator;
      typedef boost::queue<tVertex> tQueue;                               //queue of vertices, use in adoption-stage
      typedef typename property_traits<ColorMap>::value_type tColorValue;
      typedef color_traits<tColorValue> tColorTraits;
      typedef typename property_traits<DistanceMap>::value_type tDistanceVal;
      
        public:
          kolmogorov(Graph& g,
                     EdgeCapacityMap cap,
                     ResidualCapacityEdgeMap res,    
                     ReverseEdgeMap rev,
                     PredecessorMap pre,
                     ColorMap color,
                     DistanceMap dist,
                     IndexMap idx,
                     typename graph_traits<Graph>::vertex_descriptor src,
                     typename graph_traits<Graph>::vertex_descriptor sink):
          m_g(g),
          m_index_map(idx),
          m_cap_map(cap),
          m_res_cap_map(res),
          m_rev_edge_map(rev),
          m_pre_map(pre),
          m_tree_map(color),
          m_dist_map(dist),
            m_source(src),
            m_sink(sink),
            m_active_nodes(),
            m_in_active_list_vec(num_vertices(g),false), 
            m_in_active_list_map(make_iterator_property_map(m_in_active_list_vec.begin(),m_index_map)),
            m_has_parent_vec(num_vertices(g), false),
            m_has_parent_map(make_iterator_property_map(m_has_parent_vec.begin(),m_index_map)),
            m_time_vec(num_vertices(g),0),
            m_time_map(make_iterator_property_map(m_time_vec.begin(),m_index_map)),
            m_flow(0),
            m_time(1){
              
            // initialize the color-map with gray-values
            tVertexIterator vi,v_end;
            for(tie(vi, v_end) = vertices(m_g); vi != v_end; ++vi){
              set_tree(*vi, tColorTraits::gray());
            }
            // Initialize flow to zero which means initializing
            // the residual capacity equal to the capacity.            
            tEdgeIterator ei,e_end;
            for(tie(ei, e_end) = edges(m_g); ei != e_end; ++ei) {
              m_res_cap_map[*ei] = m_cap_map[*ei];
            }
            //init the search trees with the two terminals
            set_tree(m_source,tColorTraits::white());
            set_tree(m_sink,tColorTraits::black());
            //populate the active_nodes queue with the two terminals
            m_time_map[m_source] = 1;
            m_time_map[m_sink] = 1;
          }
          
          ~kolmogorov(){}
      
          tEdgeVal max_flow(){            
            iter=0;
            source_grow=0;
            sink_grow=0;
            
            //as we dont add nodes that have only a sink and no source connect below, we have to add the sink to the active nodes
            add_active_node(m_sink);            
            //in a first step, we augment all direct paths from source->NODE->sink
            //and additionally paths from source->sink
            //this improves especially graphcuts for segmentation, as most of the nodes have source/sink connects
            tOutEdgeIterator ei,e_end;
            for(tie(ei, e_end) = out_edges(m_source, m_g); ei != e_end; ++ei){
              tEdge from_source = *ei;
              tVertex current_node = target(from_source, m_g);
              if(current_node == m_sink){
                tEdgeVal cap = m_res_cap_map[from_source];
                m_res_cap_map[from_source] = 0;
                m_res_cap_map[ m_rev_edge_map[from_source] ] += cap;
                m_flow += cap;
                continue;
              }
              tEdge to_sink;
              bool is_there;
              tie(to_sink, is_there) = edge(current_node, m_sink, m_g);
              if(is_there){
                tEdgeVal cap_from_source = m_res_cap_map[from_source];
                tEdgeVal cap_to_sink = m_res_cap_map[to_sink];
                if( cap_from_source > cap_to_sink){
                  set_tree(current_node, tColorTraits::white());
                  add_active_node(current_node);
                  set_edge_to_parent(current_node, from_source);
                  m_dist_map[current_node] = 1;
                  //add stuff to flow and update residuals
                  m_res_cap_map[m_rev_edge_map[to_sink]] += cap_to_sink;
                  m_res_cap_map[from_source]-= cap_to_sink;
                  m_res_cap_map[m_rev_edge_map[from_source]] += cap_to_sink;
                  m_res_cap_map[to_sink] = 0;                  
                  m_flow += cap_to_sink;                  
                } else{
                  set_tree(current_node, tColorTraits::black());
                  add_active_node(current_node);                
                  set_edge_to_parent(current_node, to_sink);
                  m_dist_map[current_node] = 1;                
                  //add stuff to flow and update residuals
                  m_res_cap_map[to_sink]-= cap_from_source;
                  m_res_cap_map[m_rev_edge_map[to_sink]] += cap_from_source;
                  m_res_cap_map[m_rev_edge_map[from_source]] += cap_from_source;
                  m_res_cap_map[from_source] = 0;
                  m_flow += cap_from_source;
                }
              }
              else if (m_res_cap_map[from_source]){
                //there is no sink connect, so we cant augment this path
                //but to avoid adding m_source to the active nodes, we just activate this node and set the approciate things
                set_tree(current_node, tColorTraits::white());
                set_edge_to_parent(current_node, from_source);
                m_dist_map[current_node] = 1;                
                add_active_node(current_node);
              }
            }            
            
            
#ifdef KOLMOGOROV_CHECK_INVARIANTS            
            checkInvariants();           
#endif
            while(true){
              bool path_found;
              tEdge connecting_edge;
              tie(connecting_edge, path_found) = grow(); //find a path from source to sink
              if(!path_found){
                //we're finished, no more paths were found
                break; 
              }
              ++m_time;
#ifdef KOLMOGOROV_CHECK_INVARIANTS            
              checkInvariants();           
#endif
              augment(connecting_edge); //augment that path
#ifdef KOLMOGOROV_CHECK_INVARIANTS            
              checkInvariants();           
#endif
//              if(!(m_time%1000))
//                 std::cout << "max_flow " << m_flow << std::endl;
              adopt(); //rebuild search tree structure
#ifdef KOLMOGOROV_CHECK_INVARIANTS            
              checkInvariants();           
#endif
            }
            std::cout << "number of augmets " << iter << " source/sink growings(" << source_grow<<","<< sink_grow<<  ")" << std::endl;
            return m_flow;            
          }
          
        private:
          /**
          * returns a pair of an edge and a boolean. if the bool is true, the edge is a connection of a found path from s->t , read "the link" and 
          *   source(returnVal, m_g) is the end of the path found in the source-tree
          *   target(returnVal, m_g) is the beginning of the path found in the sink-tree
          */
          std::pair<tEdge, bool> grow(){
            assert(m_orphans.empty());
            tVertex current_node;
            while((current_node = get_next_active_node()) != tGraphTraits::null_vertex()){ //if there is one
              assert(get_tree(current_node) != tColorTraits::gray()  && (has_parent(current_node) || current_node==m_source || current_node==m_sink)); 
              if(get_tree(current_node) == tColorTraits::white()){ 
                ++source_grow;
                //source tree growing
                tOutEdgeIterator ei,e_end;
                for(tie(ei, e_end) = out_edges(current_node, m_g); ei != e_end; ++ei){
                  tEdge out_edge = *ei;
                  if(m_res_cap_map[out_edge] > 0){ //check if we have capacity left on this edge
                    tVertex other_node = target(out_edge, m_g);
                    if(get_tree(other_node) == tColorTraits::gray()){ //it's a free node
                      set_tree(other_node, tColorTraits::white()); //aquire other node to our search tree
                      set_edge_to_parent(other_node, out_edge);   //set us as parent
                      m_dist_map[other_node] = m_dist_map[current_node] + 1;  //and update the distance-heuristic
                      m_time_map[other_node] = m_time_map[current_node];
                      add_active_node(other_node);                      
                    }
                    else if(get_tree(other_node) == tColorTraits::white()){
                      if(is_closer_to_terminal(current_node, other_node)){ //we do this to get shorter paths. check if we are nearer to the source as its parent is
                        set_edge_to_parent(other_node, out_edge);
                        m_dist_map[other_node] = m_dist_map[current_node] + 1;
                        m_time_map[other_node] = m_time_map[current_node];    
                      }
                    }     
                    else if(get_tree(other_node) == tColorTraits::black()){ 
                      //kewl, found a path from one to the other search tree, return the connecting edge in src->sink dir
                      return std::make_pair(out_edge, true);
                    }
                  }
                }//for all out-edges
              } //source-tree-growing
              else{
                tOutEdgeIterator ei,e_end;
                ++sink_grow;
                for(tie(ei, e_end) = out_edges(current_node, m_g); ei != e_end; ++ei){
                  tEdge in_edge = m_rev_edge_map[*ei]; //get the edge back to me
                  if(m_res_cap_map[in_edge] > 0){ //check if theres capacity left
                    tVertex other_node = source(in_edge, m_g);
                    if(get_tree(other_node) == tColorTraits::gray()){ //it's a free node
                      set_tree(other_node, tColorTraits::black());      //aquire that node to our search tree
                      set_edge_to_parent(other_node, in_edge);          //set us as parent
                      add_active_node(other_node);                      //activate that node
                      m_dist_map[other_node] = m_dist_map[current_node] + 1; //set its distance
                      m_time_map[other_node] = m_time_map[current_node];     //and time
                    }
                    else if(get_tree(other_node) == tColorTraits::black()){
                      if(is_closer_to_terminal(current_node, other_node)){ 
                        //we are closer to the sink than its parent is, so we "adopt" him
                        set_edge_to_parent(other_node, in_edge);
                        m_dist_map[other_node] = m_dist_map[current_node] + 1;
                        m_time_map[other_node] = m_time_map[current_node];    
                      }
                    }     
                    else if(get_tree(other_node) == tColorTraits::white()){ 
                      //kewl, found a path from one to the other search tree, return the connecting edge in src->sink dir
                      return std::make_pair(in_edge, true);
                    }
                  }
                }//for all out-edges                 
              }//sink-tree growing
              //all edges of that node are processed, and no more paths were found. so remove if from the front of the active queue
              finish_node(current_node);
            } //while active_nodes not empty
            return std::make_pair(tEdge(), false); //no active nodes anymore and no path found, we're done
          }

          /**
          * augments path from s->t and updates residual graph
          * source(e,m_g) is the end of the path found in the source-tree
          * target(e,m_g) is the beginning of the path found in the sink-tree
          * this phase generates orphans on satured edges, if the attached verts are from different search-trees
          * orphans are ordered in distance to sink/source. first the farest from the source are front_inserted into the orphans list,
          * and after that the sink-tree-orphans are front_inserted. when going to adoption stage the orphans are popped_front, and so we process the nearest 
          * verts to the terminals first
          */
          void augment(tEdge e){
            assert(get_tree(target(e, m_g)) == tColorTraits::black());
            assert(get_tree(source(e, m_g)) == tColorTraits::white());
            assert(m_orphans.empty());
            
            ++iter;            
            const tEdgeVal bottleneck = find_bottleneck(e);
            //now we push the found flow through the path
            //for each edge we saturate we have to look for the verts that belong to that edge, one of them becomes an orphans
            //now process the connecting edge
            m_res_cap_map[e]-= bottleneck;
            assert(m_res_cap_map[e] >= 0);
            m_res_cap_map[ m_rev_edge_map[ e ] ] += bottleneck;
            
            //now we follow the path back to the source
            tVertex current_node = source(e, m_g);
            while(current_node != m_source){
              tEdge pred = get_edge_to_parent(current_node);
              m_res_cap_map[pred]-= bottleneck;
              assert(m_res_cap_map[pred] >= 0);
              m_res_cap_map[ m_rev_edge_map[pred] ] += bottleneck;
              if(m_res_cap_map[pred] == 0){
                set_no_parent(current_node);
                m_orphans.push_front(current_node);
              }
              current_node = source(pred, m_g);
            }
            //then go forward in the sink-tree
            current_node = target(e, m_g);
            while(current_node != m_sink){
              tEdge pred = get_edge_to_parent(current_node);
              m_res_cap_map[pred]-= bottleneck;
              assert(m_res_cap_map[pred] >= 0);
              m_res_cap_map[ m_rev_edge_map[pred] ] += bottleneck;
              if(m_res_cap_map[pred] == 0){
                set_no_parent(current_node);
                m_orphans.push_front(current_node);
              }
              current_node = target(pred, m_g);
            }                        
            //and add it to the max-flow
            m_flow += bottleneck;
          }
          
          /**
           * returns the bottleneck of a s->t path (end_of_path is last vertex in source-tree, begin_of_path is first vertex in sink-tree)
           */		
          inline tEdgeVal find_bottleneck(tEdge e){
            BOOST_USING_STD_MIN();
            tEdgeVal minimum_cap = m_res_cap_map[e];
            tVertex current_node = source(e, m_g);
            //first go back in the source tree
            while(current_node != m_source){
              tEdge pred = get_edge_to_parent(current_node);
              minimum_cap = min BOOST_PREVENT_MACRO_SUBSTITUTION(minimum_cap, m_res_cap_map[pred]);
              current_node = source(pred, m_g);
            }
            //then go forward in the sink-tree
            current_node = target(e, m_g);
            while(current_node != m_sink){
              tEdge pred = get_edge_to_parent(current_node);
              minimum_cap = min BOOST_PREVENT_MACRO_SUBSTITUTION(minimum_cap, m_res_cap_map[pred]);
              current_node = target(pred, m_g);
            }
            return minimum_cap;
          }

          /**
          * rebuild search trees
          * empty the queue of orphans, and find new parents for them or just drop them from the search trees
          */
          void adopt(){
            while(!m_orphans.empty() || !m_child_orphans.empty()){
              tVertex current_node;
              if(m_child_orphans.empty()){
                //get the next orphan from the main-queue  and remove it
                current_node=m_orphans.front();
                m_orphans.pop_front();
              }
              else{
                current_node = m_child_orphans.front();
                m_child_orphans.pop();
              }
              if(get_tree(current_node) == tColorTraits::white()){ 
                //we're in the source-tree
                tDistanceVal min_distance = (std::numeric_limits< tDistanceVal >::max)();
                tEdge new_parent_edge;                                
                tOutEdgeIterator ei,e_end;
                for(tie(ei, e_end) = out_edges(current_node, m_g); ei != e_end; ++ei){
                  const tEdge in_edge = m_rev_edge_map[*ei];
                  assert(target(in_edge, m_g) == current_node); //we should be the target of this edge
                  tVertex other_node = source(in_edge, m_g);
                  if(m_res_cap_map[in_edge] > 0 && get_tree(other_node) == tColorTraits::white() && has_source_connect(other_node)){
                    if(m_dist_map[other_node] < min_distance){
                      min_distance = m_dist_map[other_node];
                      new_parent_edge = in_edge;
                      if(min_distance == 1)//it cant get shorter
                        break;
                    }
                  }
                }
                if(min_distance != (std::numeric_limits< tDistanceVal >::max)()){
                  set_edge_to_parent(current_node, new_parent_edge);
                  m_dist_map[current_node] = min_distance + 1;
                  m_time_map[current_node] = m_time;
                }
                else{             
                  for(tie(ei,e_end) = out_edges(current_node, m_g); ei != e_end; ++ei){
                    tEdge in_edge = m_rev_edge_map[*ei];
                    tVertex other_node = source(in_edge, m_g);
                    if(get_tree(other_node) == tColorTraits::white()){
                      if (m_res_cap_map[in_edge] > 0){
                        add_active_node(other_node);
                      }
                      if(has_parent(other_node) && source(get_edge_to_parent(other_node), m_g) == current_node){ 
                        //we are the parent of that node
                        //it has to find a new parent, too
                        set_no_parent(other_node);
                        m_child_orphans.push(other_node);
                      }
                    }                    
                  }
                  set_tree(current_node, tColorTraits::gray());
                  remove_active_node(current_node);                  
                }//no parent found
              }//source-tree-adoption
              else{
                //now we should be in the sink-tree, check that...
                assert(get_tree(current_node) == tColorTraits::black());
                tOutEdgeIterator ei,e_end;
                tEdge new_parent_edge;
                tDistanceVal min_distance = (std::numeric_limits< tDistanceVal >::max)();
                for(tie(ei,e_end) = out_edges(current_node,m_g);ei != e_end;++ei){
                  const tEdge out_edge = *ei;
                  const tVertex other_node = target(out_edge, m_g);
                  if(get_tree(other_node) == tColorTraits::black() && m_res_cap_map[out_edge] > 0 && 
                      has_sink_connect(other_node)){
                    if(m_dist_map[other_node] < min_distance){
                      min_distance = m_dist_map[other_node];
                      new_parent_edge = out_edge;
                      if(min_distance == 1) //it cant get shorter
                        break;                      
                    }
                  }
                }
                if(min_distance != (std::numeric_limits< tDistanceVal >::max)()){
                  set_edge_to_parent(current_node, new_parent_edge);
                  m_dist_map[current_node] = min_distance + 1;
                  m_time_map[current_node] = m_time;                          
                }
                else{
                  for(tie(ei,e_end) = out_edges(current_node, m_g); ei != e_end; ++ei){
                    const tEdge out_edge = *ei;
                    const tVertex other_node = target(out_edge, m_g);
                     if(get_tree(other_node) == tColorTraits::black()){ 
                      if(m_res_cap_map[out_edge] > 0){
                        add_active_node(other_node);
                      }
                      if(has_parent(other_node) && target(get_edge_to_parent(other_node), m_g) == current_node){
                        //we were its parent, so it has to find a new one, too
                        set_no_parent(other_node);
                        m_child_orphans.push(other_node);
                      }
                    }
                  }
                  set_tree(current_node, tColorTraits::gray());
                  remove_active_node(current_node);                  
                }//no parent found                
              }//sink-tree adoption
            }//while !orphans.empty()
          }//adopt
                    
          /**
          * return next active vertex if there is one, otherwise a null_vertex
          */	
          inline tVertex get_next_active_node(){
            while(true){
              if(m_active_nodes.empty())
                return tGraphTraits::null_vertex();
              tVertex v = m_active_nodes.front();

              if(!has_parent(v) && v != m_source && v != m_sink){ //if it has no parent, this node cant be active(if its not source or sink)
                m_active_nodes.pop_front();
                m_in_active_list_map[v] = false;
              }
              else{
                assert(get_tree(v) == tColorTraits::white() || get_tree(v) == tColorTraits::black());
                return v;
              }
            }
          }
          
          /**
          * adds v as an active vertex, but only if its not in the list already
          */		
          inline void add_active_node(tVertex v){
            assert(get_tree(v) != tColorTraits::gray());
            if(m_in_active_list_map[v]){
              return;
            }
            else{
              m_in_active_list_map[v] = true;
              m_active_nodes.push_back(v);  
            }
          }
          
          /**
           * finish_node removes a node from the front of the active queue (its called in grow phase, if no more paths can be found using this node)
           */		                    
          inline void finish_node(tVertex v){
            assert(m_active_nodes.front() == v);
            m_active_nodes.pop_front();
            m_in_active_list_map[v] = false;
          }
          
          /**
          * removes a vertex from the queue of active nodes (actually this does nothing, 
          * but checks if this node has no parent edge, as this is the criteria for beeing no more active) 
          */		
          inline void remove_active_node(tVertex v){
#ifdef CHECK_INVARIANT_4
            invariant_four(v);
#endif            
            assert(!has_parent(v));
          }
          
          /**
          * returns the search tree of v; tColorValue::white() for source tree, black() for sink tree, gray() for no tree
          */		
          inline tColorValue get_tree(tVertex v) const {
            return m_tree_map[v];
          }
          
          /**
          * sets search tree of v; tColorValue::white() for source tree, black() for sink tree, gray() for no tree
          */		
          inline void set_tree(tVertex v, tColorValue t){
#ifdef KOLMOGOROV_CHECK_INVARIANT_5
            invariant_five(v);
#endif //KOLMOGOROV_CHECK_INVARIANT_5
            m_tree_map[v] = t;
          }
          
          /**
           * returns edge to parent vertex of v;
           */		
          inline tEdge get_edge_to_parent(tVertex v) const{
            return m_pre_map[v];
          }
          
          /**
           * returns true if the edge stored in m_pre_map[v] is a valid entry
           */		          
          inline bool has_parent(tVertex v) const{
            return m_has_parent_map[v];  
          }          
          
          /**
           * sets edge to parent vertex of v; 
          */		
          inline void set_edge_to_parent(tVertex v, tEdge f_edge_to_parent){
            assert(m_res_cap_map[f_edge_to_parent] > 0);
            m_pre_map[v] = f_edge_to_parent;
            m_has_parent_map[v] = true;
          }
          /**
           * removes the edge to parent of v (this is done by invalidating the entry an additional map)
           */		          
          inline void set_no_parent(tVertex v){
            m_has_parent_map[v] = false;
          }
          
          /**
           * checks if vertex v has a connect to the sink-vertex (@var m_sink)
           * @param v the vertex which is checked
           * @return true if a path to the sink was found, false if not
           */                  
          inline bool has_sink_connect(tVertex v){
            tDistanceVal current_distance = 0;
            tVertex current_vertex = v;
            while(true){
              if(m_time_map[current_vertex] == m_time){
                //we found a node which was already checked this round. use it for distance calculations
                current_distance += m_dist_map[current_vertex];
                break;
              }
              if(current_vertex == m_sink){
                m_time_map[m_sink] = m_time;
                break; 
              }
              if(has_parent(current_vertex)){
                //it has a parent, so get it
                const tEdge e = get_edge_to_parent(current_vertex);
                current_vertex = target(e, m_g);
                ++current_distance;
              }
              else{
                //no path found
                return false;
              }
            }
            current_vertex=v;
            while(m_time_map[current_vertex] != m_time){
              m_dist_map[current_vertex] = current_distance -- ;
              m_time_map[current_vertex] = m_time;
              current_vertex = target(get_edge_to_parent(current_vertex), m_g);
            }
            return true;
          }
          
          /**
           * checks if vertex v has a connect to the source-vertex (@var m_source)
           * @param v the vertex which is checked
           * @return true if a path to the source was found, false if not
           */                  
          inline bool has_source_connect(tVertex v){
            tDistanceVal current_distance = 0;
            tVertex current_vertex = v;
            while(true){
              if(m_time_map[current_vertex] == m_time){
                //we found a node which was already checked this round. use it for distance calculations
                current_distance += m_dist_map[current_vertex];
                break;
              }
              if(current_vertex == m_source){
                m_time_map[m_source] = m_time;
                break;
              }
              
              if(has_parent(current_vertex)){
                const tEdge e = get_edge_to_parent(current_vertex);
                //it has a parent, so get it
                current_vertex = source(e, m_g);
                ++current_distance;
              }
              else{
                //no path found
                return false;
              }
            }
            current_vertex=v;
            while(m_time_map[current_vertex] != m_time){
                m_dist_map[current_vertex] = current_distance -- ;
                m_time_map[current_vertex] = m_time;
                current_vertex = source(get_edge_to_parent(current_vertex), m_g);
            }
            return true;
          }
          
          /**
          * returns true, if p is closer to a terminal than q
          */
          inline bool is_closer_to_terminal(tVertex p, tVertex q){
            //checks the timestamps first, to build no cycles, and after that the real distance
            return (m_time_map[q] <= m_time_map[p] && m_dist_map[q] > m_dist_map[p]+1);
          }
          
          void checkInvariants(){
            tVertexIterator vit,v_end;
            for(tie(vit, v_end) = vertices(m_g); vit != v_end; ++vit){
#ifdef KOLMOGOROV_CHECK_INVARIANT_4 
               invariant_four(*vit);
#endif
#ifdef KOLMOGOROV_CHECK_INVARIANT_5
              invariant_five(*vit);
#endif
#ifdef KOLMOGOROV_CHECK_INVARIANT_6
              invariant_six(*vit);
#endif
#ifdef KOLMOGOROV_CHECK_INVARIANT_7
              invariant_seven(*vit);
#endif
#ifdef KOLMOGOROV_CHECK_INVARIANT_8 
              invariant_eight(*vit);             
#endif
            }
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
              for(tie(ei, e_end) = out_edges(v,m_g);ei != e_end; ++ei){
                const tVertex& other_node = target(*ei, m_g);
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
                current_node = (color==tColorTraits::white())? source(e, m_g) : target(e, m_g);
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
                tVertex current_node = (color==tColorTraits::white()) ? source(*e, m_g):target(*e, m_g);
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
                tVertex current_node = (color == tColorTraits::white())? source(*e, m_g):target(*e, m_g);
                if(m_time_map[current_node] == time)
                  assert(m_dist_map[current_node] < distance);
                e = get_edge_to_parent(current_node);   
              }              
              return;
            }
          }
          
          ////////
          // member vars
          ////////          
          Graph& m_g;
          IndexMap m_index_map;
          EdgeCapacityMap m_cap_map;
          ResidualCapacityEdgeMap m_res_cap_map;
          ReverseEdgeMap m_rev_edge_map;       
          PredecessorMap m_pre_map; //stores paths found in the growth stage
          ColorMap m_tree_map; //maps each vertex into one of the two search tree or none (gray())
          DistanceMap m_dist_map; //stores distance to source/sink nodes
          tVertex m_source;
          tVertex m_sink;
          std::list<tVertex> m_active_nodes;
          
          std::vector< bool > m_in_active_list_vec;
          iterator_property_map<std::vector< bool>::iterator, IndexMap> m_in_active_list_map;
          
          std::list<tVertex> m_orphans;
          tQueue m_child_orphans; // we use a second queuqe for child orphans, as they are FIFO processed
          
          std::vector< bool > m_has_parent_vec;
          iterator_property_map<std::vector< bool>::iterator, IndexMap> m_has_parent_map;
          
          std::vector< long > m_time_vec; //timestamp of each node, used for sink/source-path calculations
          iterator_property_map<std::vector< long >::iterator, IndexMap> m_time_map;
          tEdgeVal m_flow;
          long m_time;
          long iter;
          long source_grow;
          long sink_grow;
    };
  } //namespace detail
  
  
  /**
   * non-named-parameter version, give everything 
   */			
  template <class Graph, class CapacityEdgeMap, class ResidualCapacityEdgeMap, class ReverseEdgeMap, 
    class PredecessorMap, class ColorMap, class DistanceMap, class IndexMap>
      typename property_traits< typename property_map<Graph, edge_capacity_t>::const_type>::value_type  
    kolmogorov_max_flow
      (Graph& g, 
       CapacityEdgeMap cap,
       ResidualCapacityEdgeMap res_cap,
       ReverseEdgeMap rev_map,
       PredecessorMap pre_map,
       ColorMap color,
       DistanceMap dist,
       IndexMap idx,
       typename graph_traits<Graph>::vertex_descriptor src,
       typename graph_traits<Graph>::vertex_descriptor sink
       )
      {
        typedef typename graph_traits<Graph>::vertex_descriptor tVertex;
        typedef typename graph_traits<Graph>::edge_descriptor tEdge;
        //as this method is the last one before we instantiate the solver, we do the concept checks here
        function_requires< VertexListGraphConcept<Graph> >(); //to have vertices(),num_vertices(),
        function_requires< EdgeListGraphConcept<Graph> >(); //to have edges()
        function_requires< IncidenceGraphConcept<Graph> >(); //to have source(), target() and out_edges()
        function_requires< ReadablePropertyMapConcept<CapacityEdgeMap,tEdge> >(); //read flow-values from edges
        function_requires< Mutable_LvaluePropertyMapConcept<ResidualCapacityEdgeMap,tEdge> >(); //write flow-values to residuals
        function_requires< ReadablePropertyMapConcept<ReverseEdgeMap,tEdge> >(); //read out reverse edges
        function_requires< Mutable_LvaluePropertyMapConcept<PredecessorMap,tVertex> >(); //store predecessor there
        function_requires< Mutable_LvaluePropertyMapConcept<ColorMap,tVertex> >(); //write corresponding tree        
        function_requires< Mutable_LvaluePropertyMapConcept<DistanceMap,tVertex> >(); //write distance to source/sink
        function_requires< ReadablePropertyMapConcept<IndexMap,tVertex> >(); //get index 0...|V|-1 
        
        detail::kolmogorov<Graph,CapacityEdgeMap,ResidualCapacityEdgeMap,ReverseEdgeMap,PredecessorMap, ColorMap,DistanceMap,IndexMap> 
            algo(g, cap, res_cap, rev_map, pre_map, color, dist, idx, src, sink);
         return algo.max_flow();
      }
  
   /**
    * named-parameter version, some given
    */			            
   template <class Graph, class P, class T, class R>
       typename property_traits< typename property_map<Graph, edge_capacity_t>::const_type>::value_type
     kolmogorov_max_flow
      (Graph& g, 
           typename graph_traits<Graph>::vertex_descriptor src,
           typename graph_traits<Graph>::vertex_descriptor sink,
           const bgl_named_params<P, T, R>& params)
          {
            return kolmogorov_max_flow(g, 
                                       choose_const_pmap(get_param(params, edge_capacity), g, edge_capacity),
                                       choose_pmap(get_param(params, edge_residual_capacity), g, edge_residual_capacity),
                                       choose_const_pmap(get_param(params, edge_reverse), g, edge_reverse),
                                       choose_pmap(get_param(params, vertex_predecessor), g, vertex_predecessor),
                                       choose_pmap(get_param(params, vertex_color), g, vertex_color),
                                       choose_pmap(get_param(params, vertex_distance), g, vertex_distance),
                                       choose_const_pmap(get_param(params, vertex_index), g, vertex_index),
                                       src, sink);
          }
  /**
   * named-parameter version, none given
   */			      
      template <class Graph >
          typename property_traits< typename property_map<Graph, edge_capacity_t>::const_type>::value_type
          kolmogorov_max_flow
          (Graph& g, 
           typename graph_traits<Graph>::vertex_descriptor src,
           typename graph_traits<Graph>::vertex_descriptor sink )
          {
            bgl_named_params<int, buffer_param_t> params(0); // bogus empty param
            return kolmogorov_max_flow(g, src, sink, params);
          }
} // namespace boost

#endif // BOOST_KOLMOGOROV_MAX_FLOW_HPP

