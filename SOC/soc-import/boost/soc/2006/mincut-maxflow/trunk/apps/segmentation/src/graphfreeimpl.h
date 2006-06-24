//
// C++ Interface: graphfreeimpl
//
// Description: 
//
//
// Author: Stephan Diederich <S.Diederich@gmx.de>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GRAPHFREEIMPL_H
#define GRAPHFREEIMPL_H

#include <external/freeimpl/freegraph.h>

/**
	@author Stephan Diederich <S.Diederich@gmx.de>
*/
class GraphFreeImpl : public GraphBase
{
public:
	GraphFreeImpl(unsigned int f_numberOfVertices):m_graph(f_numberOfVertices){	
	};

	~GraphFreeImpl(){
	};
	
	virtual void add_edge(int source, int dest, tPrecision cap, tPrecision rev_cap){
		m_graph.add_edge(source, dest, cap,rev_cap);
	}

	virtual void add_tedge(int source, tPrecision toSource, tPrecision toSink){
		m_graph.add_weight(source, FreeGraph::SOURCE ,toSource);
		m_graph.add_weight(source, FreeGraph::SINK ,toSink);
		
	}
			
	virtual tPrecision maxflow(){
		return static_cast<tPrecision>(m_graph.maxflow());
	}
	
	virtual eSegment what_segment(int f_node) const {
		switch(m_graph.what_segment(f_node)){
			case FreeGraph::SOURCE:
				return GraphBase::SOURCE;
			case FreeGraph::SINK:
				return GraphBase::SINK;
			default:
				assert(0);
				return GraphBase::SINK; //avoid warning;
		}
	}
	
	private:
		FreeGraph m_graph;
};

#endif
