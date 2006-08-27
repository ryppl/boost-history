//
// C++ Interface: graphkolmogorov
//
// Description: 
//
//
// Author: Stephan Diederich <S.Diederich@gmx.de>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GRAPHKOLMOGOROV_H
#define GRAPHKOLMOGOROV_H

#include <graphbase.h>
#include <iostream>
#include <fstream>
#include <external/kolmo_maxflow/adjacency_list/graph.h>
/**
	@author Stephan Diederich <S.Diederich@gmx.de>
*/
class GraphKolmogorov : public GraphBase
{
public:
	GraphKolmogorov(unsigned int f_numberOfVertices):GraphBase(){
		m_nodeMap=new Graph::node_id[f_numberOfVertices];
//       std::ofstream of("nodes.log");
      for(unsigned int i=0;i<f_numberOfVertices;++i){
			m_nodeMap[i]=m_graph.add_node();
//          of << i << std::hex << m_nodeMap[i] << std::dec <<std::endl;
      }
	};

	~GraphKolmogorov(){
		delete[] m_nodeMap;
	};
	
	virtual void add_edge(int source, int dest, tPrecision cap, tPrecision rev_cap){
// 		std::cout << source << " " << dest << " " << cap << " " <<rev_cap <<std::endl;
		m_graph.add_edge(m_nodeMap[source], m_nodeMap[dest], cap,rev_cap);
	}

	virtual void add_tedge(int node, tPrecision toSource, tPrecision toSink){
// 		std::cout << node << " " << toSource << " " << toSink<< " " <<std::endl;
		m_graph.set_tweights(m_nodeMap[node],toSource,toSink);
	}
			
	virtual tPrecision maxflow(){
		return m_graph.maxflow();
	}
	
	virtual eSegment what_segment(int f_node) const {
		switch(m_graph.what_segment(m_nodeMap[f_node])){
			case Graph::SOURCE:
				return GraphBase::SOURCE;
			case Graph::SINK:
				return GraphBase::SINK;
			default:
				assert(0);
				return GraphBase::SINK; //avoid warning;
		}
	}
	
private:
	Graph m_graph;
	Graph::node_id* m_nodeMap;
		
};
#endif
