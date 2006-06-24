#ifndef CONNECTED_COMPONENTS_HPP
#define CONNECTED_COMPONENTS_HPP
#include <boost/graph/depth_first_search.hpp>

/*
* This method computes the total number of connected components in a Graph g and assigns each vertex the number of that component
* Requirements: Graph G needs to be a vertex-list-graph, ComponentMap needs to be a writable property map
* as we're using dfs we need additionally a colormap
*
*
*/

namespace boost{
	namespace detail{
		template <typename ComponentMap> 
				struct comp_recorder:public dfs_visitor<>{
			typedef typename property_traits<ComponentMap>::value_type tCompType;
			
			comp_recorder(ComponentMap& m,tCompType& numberOfComponents):mr_map(m),mr_numberOfComponents(numberOfComponents){}
			
			template <typename Graph,typename  Vertex> void start_vertex(Vertex& v, Graph& g){
					++mr_numberOfComponents;
			}
			template <typename Graph,typename  Vertex> void discover_vertex(Vertex& v,Graph& g){
				put(mr_map,v,mr_numberOfComponents);
			}
			
			ComponentMap& mr_map;
			tCompType& mr_numberOfComponents;
		};
	}//namespace detail
	
	template <typename VertexListGraph, typename ComponentMap,typename P,typename T, typename R>
			int connected_components(const VertexListGraph& g,ComponentMap c,const bgl_named_params<P,T,R>& params){
				
				typedef typename graph_traits<VertexListGraph>::vertex_descriptor tVertex;
				typedef typename boost::graph_traits<VertexListGraph>::directed_category directed;
				BOOST_STATIC_ASSERT((boost::is_same<directed, undirected_tag>::value));	
				
				typedef typename property_traits<ComponentMap>::value_type tComponent;
				function_requires< WritablePropertyMapConcept<ComponentMap, tVertex> >();
				
				tComponent numberOfComponents(0);
				detail::comp_recorder<ComponentMap> recorder(c,numberOfComponents);
				depth_first_search(g,visitor(recorder));

				return numberOfComponents;
			} //connected_components

}//namespace boost

#endif
