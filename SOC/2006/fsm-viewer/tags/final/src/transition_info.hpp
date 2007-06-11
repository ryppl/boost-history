//  Copyright (c) 2006, Ioana Tibuleac
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)



#ifndef BOOST_STATECHART_VIEWER_TRANSITION_INFO_HPP
#define BOOST_STATECHART_VIEWER_TRANSITION_INFO_HPP

#include <vector>
#include <string>
#include <boost/graph/graphviz.hpp>
#include "state_info.hpp"

using namespace std;
using namespace boost;

namespace statechart_viewer
{
	
class transition_info
{
	public:
		transition_info(){}

		transition_info (state_info * p_initial, state_info * p_final, string p_event) : initial(p_initial), final(p_final), event(p_event) {}

		void build_edge(GraphvizDigraph & G)
		{
			graph_traits <GraphvizDigraph>::vertex_descriptor in, fin;
			boost::graph_traits<GraphvizDigraph>::edge_descriptor e;
			property_map<GraphvizDigraph, edge_attribute_t>::type edge_map = get(edge_attribute, G);	
			in = initial->get_vertex_descriptor();
			fin = final->get_vertex_descriptor();
			bool inserted;
			if (initial->get_name()==final->get_name() && initial->is_cluster())
			{
				property_map<GraphvizDigraph, vertex_attribute_t>::type attr_map = get(vertex_attribute, G);
				graph_traits <GraphvizDigraph>::vertex_descriptor u;
				u = add_vertex(G);
				attr_map[u]["shape"]="box";
				attr_map[u]["label"]="";
				tie(e,inserted) = add_edge(in,u,G);
				edge_map[e]["label"]=event;
				edge_map[e]["ltail"]="cluster_"+initial->get_name();
				tie(e,inserted) = add_edge(u,fin,G);
				edge_map[e]["lhead"]="cluster_"+final->get_name();
				return;
			}
			
			tie(e,inserted) = add_edge(in,fin,G);
			edge_map[e]["label"]=event;
			
			if (final->is_cluster())
				edge_map[e]["lhead"]="cluster_"+final->get_name();
			
			if (initial->is_cluster())
				edge_map[e]["ltail"]="cluster_"+initial->get_name();

		}

	private:
		state_info * initial;
		state_info * final;
		string event;
};


}//namespace

#endif //BOOST_STATECHART_VIEWER_TRANSITION_INFO_HPP
