//
// C++ Interface: graphboostedmundkarp
//
// Description: 
//
//
// Author: Stephan Diederich <S.Diederich@gmx.de>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef GRAPHBOOSTKOLMOGOROV_H
#define GRAPHBOOSTKOLMOGOROV_H

#include <graphbase.h>

#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <boost/graph/kolmogorov_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost_common.h>
/**
	@author Stephan Diederich <S.Diederich@gmx.de>
*/
class GraphBoostKolmogorov: public GraphBase
{
  typedef adjacency_list_traits < vecS, vecS, directedS > tListTraits;
  typedef adjacency_list< vecS, vecS, directedS, no_property,
    property < edge_capacity_t, GraphBase::tPrecision,
	 property < edge_residual_capacity_t, GraphBase::tPrecision,
    property < edge_reverse_t, tListTraits::edge_descriptor > > > > tMyListGraph;
   
   typedef std::vector<boost::default_color_type> tColorMap;
	
public:
	GraphBoostKolmogorov(unsigned int f_numberOfPixels)
      :m_graph(f_numberOfPixels+2),//add source and terminal as last 2 verts
   m_source_vertex(vertex(f_numberOfPixels,m_graph)),
   m_sink_vertex(vertex(f_numberOfPixels+1,m_graph)),
   m_segment(f_numberOfPixels+2){}

		~GraphBoostKolmogorov(){
		};

	
		virtual void add_edge(int source, int dest, tPrecision cap, tPrecision rev_cap){
        add_boost_edge(m_graph,source,dest,cap,rev_cap);        
// 			add_boost_edge(m_graph,source,dest,cap,0);
//          add_boost_edge(m_graph,dest,source,rev_cap,0);
		}
	 
		virtual void add_tedge(int node, tPrecision toSource, tPrecision toSink){	
        add_boost_tedge(m_graph, m_source_vertex,m_sink_vertex, node, toSource, toSink);
		}
			
		virtual tPrecision maxflow(){  
            GraphBase::tPrecision flow = kolmogorov_max_flow(m_graph,
              get(edge_capacity, m_graph), 
              get(edge_residual_capacity, m_graph), 
              get(edge_reverse, m_graph),
              &m_segment[0], 
              get(vertex_index, m_graph),
              m_source_vertex, 
              m_sink_vertex);
			return flow;
		}
		
		virtual eSegment what_segment(int f_node) const {
        if(m_segment[f_node] == boost::white_color)
          return GraphBase::SINK;
        return GraphBase::SOURCE;
      }
        
	private:
     typedef tMyListGraph tMyGraph;
      tMyGraph m_graph;
      graph_traits<tMyGraph>::vertex_descriptor m_source_vertex;
      graph_traits<tMyGraph>::vertex_descriptor m_sink_vertex;
		//a vector to hold the segment of each vertex
		tColorMap m_segment;		
};
#endif
