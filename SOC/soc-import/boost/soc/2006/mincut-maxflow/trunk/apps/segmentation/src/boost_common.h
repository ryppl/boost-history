#ifndef SEGBOOST_COMMON_H
#define SEGBOOST_COMMON_H

#include <cassert>
#include <boost/config.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/breadth_first_search.hpp>

namespace graph_boost_common{
using namespace boost;
//functor for creation of the filtered graph
template <typename EdgeWeightMap>
struct positive_edge_weight {
	positive_edge_weight(){}
	positive_edge_weight(EdgeWeightMap weight) : m_weight(weight) { }
	template <typename Edge>
			bool operator()(const Edge& e) const {
		return 0 < get(m_weight, e);
			}
			EdgeWeightMap m_weight;
};
//functor for sorting out vertices which belong to the source
//segment map has to be initialized with type we dont add here
template < typename SegmentMap > 
		class bfs_segment_visitor:public default_bfs_visitor {
	typedef typename property_traits < SegmentMap >::value_type T;
	
			public:
				bfs_segment_visitor(SegmentMap smap, T  t):m_segmentMap(smap), m_segment(t) { }
	
				template < typename Vertex, typename Graph >
						void discover_vertex(Vertex u, const Graph & g) const	{
							put(m_segmentMap, u, m_segment);
						}
						SegmentMap m_segmentMap;
						T  m_segment;
		};

template <typename TGraph,typename tPrecision> 
void add_boost_edge(TGraph& g, int source, int dest, tPrecision cap, tPrecision rev_cap){
	typename graph_traits<TGraph>::edge_descriptor e1,e2;
	typename property_map<TGraph,edge_capacity_t>::type  capMap= get(edge_capacity,g);
	typename property_map<TGraph,edge_reverse_t>::type revedgeMap = get(edge_reverse,g);
	
	bool succ1,succ2;			 
	tie(e1,succ1)=boost::add_edge( vertex(source,g),vertex(dest,g),g);
	tie(e2,succ2)=boost::add_edge( vertex(dest,g),vertex(source,g),g);
	assert(succ1 && succ2);
	capMap[e1] = cap;
	capMap[e2] = 0;
	revedgeMap[e1] = e2;
	revedgeMap[e2] = e1;
}

template <typename TGraph,typename TVertex,typename tPrecision> 
void add_boost_tedge(TGraph& g, const TVertex& source_vertex,const TVertex& sink_vertex, int node, tPrecision toSource, tPrecision toSink){
	typename graph_traits<TGraph>::edge_descriptor e1,e2;
	typename property_map<TGraph,edge_capacity_t>::type  capMap= get(edge_capacity,g);
	typename property_map<TGraph,edge_reverse_t>::type revedgeMap = get(edge_reverse,g);
	
	bool succ1,succ2;			 
	tie(e1,succ1)=boost::add_edge( source_vertex,vertex(node,g),g);
	tie(e2,succ2)=boost::add_edge( vertex(node,g),source_vertex,g);
	assert(succ1 && succ2);
	capMap[e1] = toSource;
	capMap[e2] = 0;
	revedgeMap[e1] = e2;
	revedgeMap[e2] = e1;	
		 
	tie(e1,succ2)=boost::add_edge( vertex(node,g),sink_vertex,g);
	tie(e2,succ1)=boost::add_edge( sink_vertex,vertex(node,g),g);
	assert(succ1 && succ2);
	capMap[e1] = toSink;
	capMap[e2] = 0;
	revedgeMap[e1] = e2;
	revedgeMap[e2] = e1;	
}

}//namespace graph_boost_common
#endif //SEGBOOST_COMMON_H
