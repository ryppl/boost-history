//
// C++ Interface: graphbase
//
// Description: 
//
//
// Author: Stephan Diederich <S.Diederich@gmx.de>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GRAPHBASE_H
#define GRAPHBASE_H

/**
	@author Stephan Diederich <S.Diederich@gmx.de>
*/
class GraphBase{
public:
	typedef int tPrecision;
	
	enum eSegment{
		SOURCE,
		SINK
	};
	typedef eSegment tSegment;
	GraphBase(){};
	virtual ~GraphBase(){};
	
	virtual void add_edge(int sourceNode, int destNode, tPrecision cap, tPrecision rev_cap)=0;

	
	virtual void add_tedge(int node, tPrecision toSource, tPrecision toSink)=0;
			
	virtual tPrecision maxflow()=0;
	
	virtual eSegment what_segment(int f_node) const =0;
};

#endif
