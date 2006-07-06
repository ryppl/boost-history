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
#include <boost/graph/graph_utility.hpp>
#include <boost_common.h>
/**
	@author Stephan Diederich <S.Diederich@gmx.de>
*/
class GraphBoostKolmogorov: public GraphBase
{
	typedef adjacency_list_traits < vecS, vecS, directedS > Traits;
	typedef adjacency_list < listS, vecS, directedS,
	property < vertex_index_t, long>,
	property < edge_capacity_t, GraphBase::tPrecision,
	property < edge_residual_capacity_t, GraphBase::tPrecision,
	property < edge_reverse_t, Traits::edge_descriptor > > > > tMyGraph;

   typedef std::vector<boost::default_color_type> tColorMap;
	
public:
	GraphBoostKolmogorov(unsigned int f_numberOfVertices)
		:m_graph(f_numberOfVertices+2),m_capacity(get(edge_capacity, m_graph)),m_reverse_edge(get(edge_reverse,m_graph)),
   m_source_vertex(vertex(f_numberOfVertices,m_graph)),m_sink_vertex(vertex(f_numberOfVertices+1,m_graph)),m_segment(f_numberOfVertices){ //add source and terminal as last 2 verts
		}

		~GraphBoostKolmogorov(){
		};

	
		virtual void add_edge(int source, int dest, tPrecision cap, tPrecision rev_cap){
			add_boost_edge(m_graph,source,dest,cap,rev_cap);
		}
	 
		virtual void add_tedge(int node, tPrecision toSource, tPrecision toSink){	
        add_boost_tedge(m_graph, m_source_vertex,m_sink_vertex, node, toSource, toSink);
		}
			
		virtual tPrecision maxflow(){  
        GraphBase::tPrecision flow=kolmogorov_max_flow(m_graph,get(edge_capacity,m_graph),get(edge_residual_capacity,m_graph),get(edge_reverse,m_graph), &m_segment[0],m_source_vertex, m_sink_vertex);
			return flow;
		}
		
		virtual eSegment what_segment(int f_node) const {
        if(m_segment[f_node]==boost::white_color)
          return GraphBase::SOURCE;
        else if(m_segment[f_node]==boost::black_color)
          return GraphBase::SINK;
    
        std::cerr << "node: " << f_node <<  " unknown segment: " << m_segment[f_node] <<std::endl;
//         exit(0);
      }
        
	private:
		tMyGraph m_graph;
		property_map<tMyGraph,edge_capacity_t>::type  m_capacity;
		property_map<tMyGraph,edge_reverse_t>::type  m_reverse_edge;
		graph_traits<tMyGraph>::vertex_descriptor m_source_vertex;
		graph_traits<tMyGraph>::vertex_descriptor m_sink_vertex;
		//a vector to hold the segment of each vertex
		tColorMap m_segment;		
};
#endif
