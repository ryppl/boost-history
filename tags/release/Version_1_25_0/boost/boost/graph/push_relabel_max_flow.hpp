//=======================================================================
// Copyright 2000 University of Notre Dame.
// Authors: Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee
//
// This file is part of the Boost Graph Library
//
// You should have received a copy of the License Agreement for the
// Boost Graph Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================

#ifndef BOOST_PUSH_RELABEL_MAX_FLOW_HPP
#define BOOST_PUSH_RELABEL_MAX_FLOW_HPP

#include <boost/config.hpp>
#include <cassert>
#include <vector>
#include <list>
#include <iosfwd>

#include <boost/pending/queue.hpp>
#include <boost/limits.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/named_function_params.hpp>

namespace boost {

  namespace detail {
    
   // This implementation is based on Goldberg's 
   // "On Implementing Push-Relabel Method for the Maximum Flow Problem"
   // by B.V. Cherkassky and A.V. Goldberg, IPCO '95, pp. 157--171
   // and on the h_prf.c and hi_pr.c code written by the above authors.

   // This implements the highest-label version of the push-relabel method
   // with the global relabeling and gap relabeling heuristics.

   // The terms "rank", "distance", "height" are synonyms in
   // Goldberg's implementation, paper and in the CLR.  A "layer" is a
   // group of vertices with the same distance. The vertices in each
   // layer are categorized as active or inactive.  An active vertex
   // has positive excess flow and its distance is less than n (it is
   // not blocked).

    template <class Vertex>
    struct preflow_layer {
      std::list<Vertex> active_vertices;
      std::list<Vertex> inactive_vertices;
    };

    template <class Graph, 
              class EdgeCapacityMap,    // integer value type
              class ResidualCapacityEdgeMap,
              class ReverseEdgeMap,
              class VertexIndexMap,     // vertex_descriptor -> integer
              class FlowValue>
    class push_relabel
    {
    public:
      typedef graph_traits<Graph> Traits;
      typedef typename Traits::vertex_descriptor vertex_descriptor;
      typedef typename Traits::edge_descriptor edge_descriptor;
      typedef typename Traits::vertex_iterator vertex_iterator;
      typedef typename Traits::out_edge_iterator out_edge_iterator;
      typedef typename Traits::vertices_size_type vertices_size_type;
      typedef typename Traits::edges_size_type edges_size_type;

      typedef preflow_layer<vertex_descriptor> Layer;
      typedef std::vector< Layer > LayerArray;
      typedef typename LayerArray::iterator layer_iterator;
      typedef typename LayerArray::size_type distance_size_type;

      typedef color_traits<default_color_type> ColorTraits;

      //=======================================================================
      // Some helper predicates

      inline bool is_admissible(vertex_descriptor u, vertex_descriptor v) {
        return distance[u] == distance[v] + 1;
      }
      inline bool is_residual_edge(edge_descriptor a) {
        return 0 < residual_capacity[a];
      }
      inline bool is_saturated(edge_descriptor a) {
        return residual_capacity[a] == 0;
      }

      //=======================================================================
      // Layer List Management Functions

      typedef typename std::list<vertex_descriptor>::iterator list_iterator;

      void add_to_active_list(vertex_descriptor u, Layer& layer) {
        layer.active_vertices.push_front(u);
        max_active = std::max(distance[u], max_active);
        min_active = std::min(distance[u], min_active);
        layer_list_ptr[u] = layer.active_vertices.begin();
      }
      void remove_from_active_list(vertex_descriptor u) {
        layers[distance[u]].active_vertices.erase(layer_list_ptr[u]);    
      }

      void add_to_inactive_list(vertex_descriptor u, Layer& layer) {
        layer.inactive_vertices.push_front(u);
        layer_list_ptr[u] = layer.inactive_vertices.begin();
      }
      void remove_from_inactive_list(vertex_descriptor u) {
        layers[distance[u]].inactive_vertices.erase(layer_list_ptr[u]);    
      }

      //=======================================================================
      // initialization
      push_relabel(Graph& g_, 
                   EdgeCapacityMap cap,
                   ResidualCapacityEdgeMap res,
                   ReverseEdgeMap rev,
                   vertex_descriptor src_, 
                   vertex_descriptor sink_,
                   VertexIndexMap idx)
        : g(g_), n(num_vertices(g_)), capacity(cap), src(src_), sink(sink_), 
          index(idx),
          excess_flow(num_vertices(g_)),
          layer_list_ptr(num_vertices(g_)),
          current(num_vertices(g_)),
          distance(num_vertices(g_)),
          color(num_vertices(g_)),
          reverse_edge(rev),
          residual_capacity(res),
          layers(num_vertices(g_)),
          push_count(0), update_count(0), relabel_count(0), 
          gap_count(0), gap_node_count(0),
          work_since_last_update(0)
      {
        vertex_iterator u_iter, u_end;
        // Don't count the reverse edges
        edges_size_type m = num_edges(g) / 2;
        nm = alpha() * n + m;

        // Initialize flow to zero which means initializing
        // the residual capacity to equal the capacity.
        out_edge_iterator ei, e_end;
        for (tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter)
          for (tie(ei, e_end) = out_edges(*u_iter, g); ei != e_end; ++ei) {
            residual_capacity[*ei] = capacity[*ei];
          }

        for (tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter) {
          vertex_descriptor u = *u_iter;
          excess_flow[u] = 0;
          current[u] = out_edges(u, g).first;
        }

        bool overflow_detected = false;
        FlowValue test_excess = 0;

        out_edge_iterator a_iter, a_end;
        for (tie(a_iter, a_end) = out_edges(src, g); a_iter != a_end; ++a_iter)
          if (target(*a_iter, g) != src)
            test_excess += residual_capacity[*a_iter];
        if (test_excess > std::numeric_limits<FlowValue>::max())
          overflow_detected = true;

        if (overflow_detected)
          excess_flow[src] = std::numeric_limits<FlowValue>::max();
        else {
          excess_flow[src] = 0;
          for (tie(a_iter, a_end) = out_edges(src, g); 
               a_iter != a_end; ++a_iter) {
            edge_descriptor a = *a_iter;
            if (target(a, g) != src) {
              ++push_count;
              FlowValue delta = residual_capacity[a];
              residual_capacity[a] -= delta;
              residual_capacity[reverse_edge[a]] += delta;
              excess_flow[target(a, g)] += delta;
            }
          }
        }
        max_distance = num_vertices(g) - 1;
        max_active = 0;
        min_active = n;

        for (tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter) {
          vertex_descriptor u = *u_iter;
          if (u == sink)
            distance[u] = 0;
          else if (u == src && !overflow_detected)
            distance[u] = n;
          else
            distance[u] = 1;

          if (excess_flow[u] > 0)
            add_to_active_list(u, layers[1]);
          else if (distance[u] < n)
            add_to_inactive_list(u, layers[1]);
        }       

      } // push_relabel constructor

      //=======================================================================
      // This is a breadth-first search over the residual graph
      // (well, actually the reverse of the residual graph).
      // Would be cool to have a graph view adaptor for hiding certain
      // edges, like the saturated (non-residual) edges in this case.
      // Goldberg's implementation abused "distance" for the coloring.
      void global_distance_update()
      {
        ++update_count;
        vertex_iterator u_iter, u_end;
        for (tie(u_iter,u_end) = vertices(g); u_iter != u_end; ++u_iter) {
          color[*u_iter] = ColorTraits::white();
          distance[*u_iter] = n;
        }
        color[sink] = ColorTraits::gray();
        distance[sink] = 0;
        
        for (distance_size_type l = 0; l != max_distance; ++l) {
          layers[l].active_vertices.clear();
          layers[l].inactive_vertices.clear();
        }
        
        max_distance = max_active = 0;
        min_active = n;

        Q.push(sink);
        while (! Q.empty()) {
          vertex_descriptor u = Q.top();
          Q.pop();
          distance_size_type d_v = distance[u] + 1;

          out_edge_iterator ai, a_end;
          for (tie(ai, a_end) = out_edges(u, g); ai != a_end; ++ai) {
            edge_descriptor a = *ai;
            vertex_descriptor v = target(a, g);
            if (color[v] == ColorTraits::white()
                && is_residual_edge(reverse_edge[a])) {
              distance[v] = d_v;
              color[v] = ColorTraits::gray();
              current[v] = out_edges(v, g).first;
              max_distance = std::max(d_v, max_distance);

              if (excess_flow[v] > 0)
                add_to_active_list(v, layers[d_v]);
              else
                add_to_inactive_list(v, layers[d_v]);

              Q.push(v);
            }
          }
        }
      } // global_distance_update()

      //=======================================================================
      // This function is called "push" in Goldberg's h_prf implementation,
      // but it is called "discharge" in the paper and in hi_pr.c.
      void discharge(vertex_descriptor u)
      {
        assert(excess_flow[u] > 0);
        while (1) {
          out_edge_iterator ai, ai_end;
          for (ai = current[u], ai_end = out_edges(u, g).second;
               ai != ai_end; ++ai) {
            edge_descriptor a = *ai;
            if (is_residual_edge(a)) {
              vertex_descriptor v = target(a, g);
              if (is_admissible(u, v)) {
                ++push_count;
                if (v != sink && excess_flow[v] == 0) {
                  remove_from_inactive_list(v);
                  add_to_active_list(v, layers[distance[v]]);
                }
                push_flow(a);
                if (excess_flow[u] == 0)
                  break;
              } 
            } 
          } // for out_edges of i starting from current

          Layer& layer = layers[distance[u]];
          distance_size_type du = distance[u];

          if (ai == ai_end) {   // i must be relabeled
            relabel_distance(u);
            if (layer.active_vertices.empty()
                && layer.inactive_vertices.empty())
              gap(du);
            if (distance[u] == n)
              break;
          } else {              // i is no longer active
            current[u] = ai;
            add_to_inactive_list(u, layer);
            break;
          }
        } // while (1)
      } // discharge()

      //=======================================================================
      // This corresponds to the "push" update operation of the paper,
      // not the "push" function in Goldberg's h_prf.c implementation.
      // The idea is to push the excess flow from from vertex u to v.
      void push_flow(edge_descriptor u_v)
      {
        vertex_descriptor
          u = source(u_v, g),
          v = target(u_v, g);
        
        FlowValue flow_delta
          = std::min(excess_flow[u], residual_capacity[u_v]);

        residual_capacity[u_v] -= flow_delta;
        residual_capacity[reverse_edge[u_v]] += flow_delta;

        excess_flow[u] -= flow_delta;
        excess_flow[v] += flow_delta;
      } // push_flow()

      //=======================================================================
      // The main purpose of this routine is to set distance[v]
      // to the smallest value allowed by the valid labeling constraints,
      // which are:
      // distance[t] = 0
      // distance[u] <= distance[v] + 1   for every residual edge (u,v)
      //
      distance_size_type relabel_distance(vertex_descriptor u)
      {
        ++relabel_count;
        work_since_last_update += beta();

        distance_size_type min_distance = num_vertices(g);
        distance[u] = min_distance;

        // Examine the residual out-edges of vertex i, choosing the
        // edge whose target vertex has the minimal distance.
        out_edge_iterator ai, a_end, min_edge_iter;
        for (tie(ai, a_end) = out_edges(u, g); ai != a_end; ++ai) {
          ++work_since_last_update;
          edge_descriptor a = *ai;
          vertex_descriptor v = target(a, g);
          if (is_residual_edge(a) && distance[v] < min_distance) {
            min_distance = distance[v];
            min_edge_iter = ai;
          }
        }
        ++min_distance;
        if (min_distance < n) {
          distance[u] = min_distance;     // this is the main action
          current[u] = min_edge_iter;
          max_distance = std::max(min_distance, max_distance);
        }
        return min_distance;
      } // relabel_distance()

      //=======================================================================
      // cleanup beyond the gap
      void gap(distance_size_type empty_distance)
      {
        ++gap_count;

        distance_size_type r; // distance of layer before the current layer
        r = empty_distance - 1;

        // Set the distance for the vertices beyond the gap to "infinity".
        for (layer_iterator l = layers.begin() + empty_distance + 1;
             l < layers.begin() + max_distance; ++l) {
          list_iterator i;
          for (i = l->inactive_vertices.begin(); 
               i != l->inactive_vertices.end(); ++i) {
            distance[*i] = n;
            ++gap_node_count;
          }
          l->inactive_vertices.clear();
        }
        max_distance = r;
        max_active = r;
      }

      //=======================================================================
      // This is the core part of the algorithm, "phase one".
      FlowValue maximum_preflow()
      {
        work_since_last_update = 0;

        while (max_active >= min_active) { // "main" loop

          Layer& layer = layers[max_active];
          list_iterator u_iter = layer.active_vertices.begin();

          if (u_iter == layer.active_vertices.end())
            --max_active;
          else {
            vertex_descriptor u = *u_iter;
            remove_from_active_list(u);
            
            discharge(u);

            if (work_since_last_update * global_update_frequency() > nm) {
              global_distance_update();
              work_since_last_update = 0;
            }
          }
        } // while (max_active >= min_active)

        return excess_flow[sink];
      } // maximum_preflow()

      //=======================================================================
      // remove excess flow, the "second phase"
      // This does a DFS on the reverse flow graph of nodes with excess flow.
      // If a cycle is found, cancel it.
      // Return the nodes with excess flow in topological order.
      //
      // Unlike the prefl_to_flow() implementation, we use
      //   "color" instead of "distance" for the DFS labels
      //   "parent" instead of nl_prev for the DFS tree
      //   "topo_next" instead of nl_next for the topological ordering
      void convert_preflow_to_flow()
      {
        vertex_iterator u_iter, u_end;
        out_edge_iterator ai, a_end;

        vertex_descriptor r, restart, u;

        std::vector<vertex_descriptor> parent(n);
        std::vector<vertex_descriptor> topo_next(n);

        vertex_descriptor tos(parent[0]), 
          bos(parent[0]); // bogus initialization, just to avoid warning
        bool bos_null = true;

        // handle self-loops
        for (tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter)
          for (tie(ai, a_end) = out_edges(*u_iter, g); ai != a_end; ++ai)
            if (target(*ai, g) == *u_iter)
              residual_capacity[*ai] = capacity[*ai];

        // initialize
        for (tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter) {
          u = *u_iter;
          color[u] = ColorTraits::white();
          parent[u] = u;
          current[u] = out_edges(u, g).first;
        }
        // eliminate flow cycles and topologically order the vertices
        for (tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter) {
          u = *u_iter;
          if (color[u] == ColorTraits::white() 
              && excess_flow[u] > 0
              && u != src && u != sink ) {
            r = u;
            color[r] = ColorTraits::gray();
            while (1) {
              for (; current[u] != out_edges(u, g).second; ++current[u]) {
                edge_descriptor a = *current[u];
                if (capacity[a] == 0 && is_residual_edge(a)) {
                  vertex_descriptor v = target(a, g);
                  if (color[v] == ColorTraits::white()) {
                    color[v] = ColorTraits::gray();
                    parent[v] = u;
                    u = v;
                    break;
                  } else if (color[v] == ColorTraits::gray()) {
                    // find minimum flow on the cycle
                    FlowValue delta = residual_capacity[a];
                    while (1) {
                      delta = std::min(delta, residual_capacity[*current[v]]);
                      if (v == u)
                        break;
                      else
                        v = target(*current[v], g);
                    }
                    // remove delta flow units
                    v = u;
                    while (1) {
                      a = *current[v];
                      residual_capacity[a] -= delta;
                      residual_capacity[reverse_edge[a]] += delta;
                      v = target(a, g);
                      if (v == u)
                        break;
                    }

                    // back-out of DFS to the first saturated edge
                    restart = u;
                    for (v = target(*current[u], g); v != u; v = target(a, g)){
                      a = *current[v];
                      if (color[v] == ColorTraits::white() 
                          || is_saturated(a)) {
                        color[target(*current[v], g)] = ColorTraits::white();
                        if (color[v] != ColorTraits::white())
                          restart = v;
                      }
                    }
                    if (restart != u) {
                      u = restart;
                      ++current[u];
                      break;
                    }
                  } // else if (color[v] == ColorTraits::gray())
                } // if (capacity[a] == 0 ...
              } // for out_edges(u, g)  (though "u" changes during loop)
              
              if (current[u] == out_edges(u, g).second) {
                // scan of i is complete
                color[u] = ColorTraits::black();
                if (u != src) {
                  if (bos_null) {
                    bos = u;
                    bos_null = false;
                    tos = u;
                  } else {
                    topo_next[u] = tos;
                    tos = u;
                  }
                }
                if (u != r) {
                  u = parent[u];
                  ++current[u];
                } else
                  break;
              }
            } // while (1)
          } // if (color[u] == white && excess_flow[u] > 0 & ...)
        } // for all vertices in g

        // return excess flows
        // note that the sink is not on the stack
        if (! bos_null) {
          for (u = tos; u != bos; u = topo_next[u]) {
            ai = out_edges(u, g).first;
            while (excess_flow[u] > 0 && ai != out_edges(u, g).second) {
              if (capacity[*ai] == 0 && is_residual_edge(*ai))
                push_flow(*ai);
              ++ai;
            }
          }
          // do the bottom
          u = bos;
          ai = out_edges(u, g).first;
          while (excess_flow[u] > 0) {
            if (capacity[*ai] == 0 && is_residual_edge(*ai))
              push_flow(*ai);
            ++ai;
          }
        }
        
      } // convert_preflow_to_flow()

      //=======================================================================
      inline bool is_flow()
      {
        vertex_iterator u_iter, u_end;
        out_edge_iterator ai, a_end;

        // check edge flow values
        for (tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter) {
          for (tie(ai, a_end) = out_edges(*u_iter, g); ai != a_end; ++ai) {
            edge_descriptor a = *ai;
            if (capacity[a] > 0)
              if ((residual_capacity[a] + residual_capacity[reverse_edge[a]]
                   != capacity[a])
                  || (residual_capacity[a] < 0)
                  || (residual_capacity[reverse_edge[a]] < 0))
              return false;
          }
        }
        
        // check conservation
        FlowValue sum;  
        for (tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter) {
          vertex_descriptor u = *u_iter;
          if (u != src && u != sink) {
            if (excess_flow[u] != 0)
              return false;
            sum = 0;
            for (tie(ai, a_end) = out_edges(u, g); ai != a_end; ++ai) 
              if (capacity[*ai] > 0)
                sum -= capacity[*ai] - residual_capacity[*ai];
              else
                sum += residual_capacity[*ai];

            if (excess_flow[u] != sum)
              return false;
          }
        }

        return true;
      } // is_flow()

      bool is_optimal() {
        // check if mincut is saturated...
        global_distance_update();
        return distance[src] >= n;
      }

      void print_statistics(std::ostream& os) const {
        os << "pushes:     " << push_count << std::endl
           << "relabels:   " << relabel_count << std::endl
           << "updates:    " << update_count << std::endl
           << "gaps:       " << gap_count << std::endl
           << "gap nodes:  " << gap_node_count << std::endl
           << std::endl;
      }

      void print_flow_values(std::ostream& os) const {
        os << "flow values" << std::endl;
        vertex_iterator u_iter, u_end;
        out_edge_iterator ei, e_end;
        for (tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter)
          for (tie(ei, e_end) = out_edges(*u_iter, g); ei != e_end; ++ei)
            if (capacity[*ei] > 0)
              os << *u_iter << " " << target(*ei, g) << " " 
                 << (capacity[*ei] - residual_capacity[*ei]) << std::endl;
        os << std::endl;
      }

      //=======================================================================

      Graph& g;
      vertices_size_type n;
      vertices_size_type nm;
      EdgeCapacityMap capacity;
      vertex_descriptor src;
      vertex_descriptor sink;
      VertexIndexMap index;

      // will need to use random_access_property_map with these
      std::vector< FlowValue > excess_flow;
      std::vector< list_iterator > layer_list_ptr;
      std::vector< out_edge_iterator > current;
      std::vector< distance_size_type > distance;
      std::vector< default_color_type > color;

      // Edge Property Maps that must be interior to the graph
      ReverseEdgeMap reverse_edge;
      ResidualCapacityEdgeMap residual_capacity;

      LayerArray layers;
      distance_size_type max_distance;  // maximal distance
      distance_size_type max_active;    // maximal distance with active node
      distance_size_type min_active;    // minimal distance with active node
      boost::queue<vertex_descriptor> Q;

      // Statistics counters
      long push_count;
      long update_count;
      long relabel_count;
      long gap_count;
      long gap_node_count;

      inline double global_update_frequency() { return 0.5; }
      inline vertices_size_type alpha() { return 6; }
      inline long beta() { return 12; }

      long work_since_last_update;
    };

    template <class Graph, 
	      class CapacityEdgeMap, class ResidualCapacityEdgeMap,
	      class ReverseEdgeMap, class VertexIndexMap>
    typename property_traits<CapacityEdgeMap>::value_type
    push_relabel_max_flow_impl
      (Graph& g, 
       typename graph_traits<Graph>::vertex_descriptor src,
       typename graph_traits<Graph>::vertex_descriptor sink,
       CapacityEdgeMap cap, ResidualCapacityEdgeMap res,
       ReverseEdgeMap rev, VertexIndexMap index_map)
    {
      typedef typename property_traits<CapacityEdgeMap>::value_type FlowValue;

      detail::push_relabel<Graph, CapacityEdgeMap, ResidualCapacityEdgeMap, 
	ReverseEdgeMap, VertexIndexMap, FlowValue>
      algo(g, cap, res, rev, src, sink, index_map);

      FlowValue flow = algo.maximum_preflow();

      algo.convert_preflow_to_flow();

      assert(algo.is_flow());
      assert(algo.is_optimal());

      return flow;
    } // push_relabel_max_flow_impl()

  } // namespace detail
  

  template <class Graph, class P, class T, class R>
  typename detail::edge_capacity_value<Graph, P, T, R>::type
  push_relabel_max_flow
    (Graph& g, 
     typename graph_traits<Graph>::vertex_descriptor src,
     typename graph_traits<Graph>::vertex_descriptor sink,
     const bgl_named_params<P, T, R>& params)
  {
    return detail::push_relabel_max_flow_impl
      (g, src, sink,
       choose_const_pmap(get_param(params, edge_capacity), g, edge_capacity),
       choose_pmap(get_param(params, edge_residual_capacity), 
		   g, edge_residual_capacity),
       choose_const_pmap(get_param(params, edge_reverse), g, edge_reverse),
       choose_const_pmap(get_param(params, vertex_index), g, vertex_index)
       );
  }

  template <class Graph>
  typename property_traits<
    typename property_map<Graph, edge_capacity_t>::const_type
  >::value_type
  push_relabel_max_flow
    (Graph& g, 
     typename graph_traits<Graph>::vertex_descriptor src,
     typename graph_traits<Graph>::vertex_descriptor sink)
  {
    bgl_named_params<int,int> params(0);
    return push_relabel_max_flow(g, src, sink, params);
  }

} // namespace boost

#endif // BOOST_PUSH_RELABEL_MAX_FLOW_HPP

