//
// C++ Interface: graphboostpushrelabel
//
// Description: 
//
//
// Author: Stephan Diederich <S.Diederich@gmx.de>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GRAPHBOOSTPUSHRELABEL_H
#define GRAPHBOOSTPUSHRELABEL_H

#include <graphbase.h>

#include <boost/config.hpp>
#include <iostream>
#include <vector>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/breadth_first_search.hpp>

#include "boost_common.h"

/**
	@author Stephan Diederich <S.Diederich@gmx.de>
*/
using namespace boost; //arg, dont pollute global namespace
using namespace graph_boost_common;



class GraphBoostPushRelabel : public GraphBase
{
	
	typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
	typedef adjacency_list<listS, vecS, directedS, 
		property<vertex_index_t, long>,
		property<edge_capacity_t, GraphBase::tPrecision,
		property<edge_residual_capacity_t, GraphBase::tPrecision,
		property<edge_reverse_t, Traits::edge_descriptor> > >
			> tMyGraph;
	typedef std::vector<GraphBase::eSegment> tSegmentMap;
public:
	GraphBoostPushRelabel(unsigned int f_numberOfVertices)
	:m_graph(f_numberOfVertices+2),m_capacity(get(edge_capacity, m_graph)),m_reverse_edge(get(edge_reverse,m_graph)),
   m_source_vertex(vertex(f_numberOfVertices,m_graph)),m_sink_vertex(vertex(f_numberOfVertices+1,m_graph)),m_segment(f_numberOfVertices,GraphBase::SINK),m_segments_calculated(false){ //add source and terminal
	}

	~GraphBoostPushRelabel(){
	}
	
	 virtual void add_edge(int source, int dest, tPrecision cap, tPrecision rev_cap){
		 add_boost_edge(m_graph,source,dest,cap,0);
       add_boost_edge(m_graph,dest,source,rev_cap,0);
	 }

	 
	 virtual void add_tedge(int node, tPrecision toSource, tPrecision toSink){
		 add_boost_tedge(m_graph,m_source_vertex, m_sink_vertex, node,toSource,toSink);
	 }
			
	 virtual tPrecision maxflow(){  
		 GraphBase::tPrecision flow=push_relabel_max_flow(m_graph, m_source_vertex, m_sink_vertex);
		 return flow;
	 }
    
    void extractSegments() const {
      typedef property_map<tMyGraph,edge_residual_capacity_t>::const_type ResEdgeWeightMap;
      ResEdgeWeightMap map=get(edge_residual_capacity, m_graph);
      positive_edge_weight<ResEdgeWeightMap> filter(map);
      filtered_graph<tMyGraph, positive_edge_weight<ResEdgeWeightMap> > fg(m_graph, filter); 
      bfs_segment_visitor<GraphBase::eSegment*>vis(&m_segment[0], GraphBase::SOURCE);
      breadth_first_search(fg, m_source_vertex, visitor(vis));				
    }		 
    
    virtual eSegment what_segment(int f_node) const {
      if(!m_segments_calculated){
        extractSegments();
        m_segments_calculated=true;
      }
      return m_segment[f_node];
    }	
	private:
		tMyGraph m_graph;
		property_map<tMyGraph,edge_capacity_t>::type  m_capacity;
		property_map<tMyGraph,edge_reverse_t>::type  m_reverse_edge;
		graph_traits<tMyGraph>::vertex_descriptor m_source_vertex;
		graph_traits<tMyGraph>::vertex_descriptor m_sink_vertex;
		//a vector to hold the segment of each vertex
      mutable tSegmentMap m_segment;		
      mutable bool m_segments_calculated;
};

#endif
