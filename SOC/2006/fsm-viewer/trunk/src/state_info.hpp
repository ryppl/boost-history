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



#ifndef BOOST_STATECHART_VIEWER_STATE_INFO_HPP
#define BOOST_STATECHART_VIEWER_STATE_INFO_HPP

#include <vector>
#include <string>
#include <boost/graph/graphviz.hpp>

using namespace std;
using namespace boost;

namespace statechart_viewer
{

class state_info
{
	public:
		typedef graph_traits <GraphvizDigraph>::vertex_descriptor vertex_descriptor;
		typedef graph_traits<GraphvizDigraph>::edge_descriptor edge_descriptor;
		
		state_info (){}
		state_info (string p_name) : name(p_name),context(NULL),is_initial(false){}

		void set_context (state_info * p_context)
		{
			context = p_context;
		}

		const string get_name()
		{
			return name;
		}
		
 		void build_vertex (GraphvizDigraph * h)
		{
			property_map<GraphvizDigraph, vertex_attribute_t>::type vattr_map = get(vertex_attribute, h->root());
			property_map<GraphvizDigraph, edge_attribute_t>::type eattr_map = get(edge_attribute, h->root());
			
			if (is_cluster()) {

				GraphvizDigraph * g = &(h->create_subgraph());
				get_property(*g, graph_name) = "cluster_" + name; 
				get_property(*g, graph_graph_attribute)["label"] = name;
				vx_des = add_vertex(*g);
				vx_des = g->local_to_global(vx_des);
				vattr_map[vx_des]["shape"] = "point";
				vattr_map[vx_des]["style"] = "invis";
				set<state_info *>::iterator it;
				for (it = inner_states.begin(); it != inner_states.end(); ++it) {
					(*it)->build_vertex(g);}
				
			}
			else {
				vx_des = add_vertex(*h);
				vx_des = h->local_to_global(vx_des);
				vattr_map[vx_des]["label"] = name;
				vattr_map[vx_des]["shape"] = "box";
			}

			if (is_initial == true) {
				vertex_descriptor v = add_vertex(*h);
				v = h->local_to_global(v);
				vattr_map[v]["shape"] = "point";
				bool inserted;
				edge_descriptor e;
				tie(e,inserted) = add_edge(v, vx_des, h->root());
				eattr_map[e]["minlen"]="0.2";
				eattr_map[e]["arrowsize"]="0.5";
				if (is_cluster()) eattr_map[e]["lhead"] = "cluster_" + name;
			}
			
		}

		void add_inner (state_info * state)
		{
			inner_states.insert(state);
		}

		void set_orthogonal (const string & p_orthogonal)
		{
			orthogonal = p_orthogonal;
		}
		
		bool is_cluster()
		{
			return inner_states.size();
		}
	
		set<state_info*> get_inner_states()
		{
			return inner_states;
		}

		void set_vertex_descriptor (const vertex_descriptor & vdes)
		{
			vx_des=vdes;
		}

		vertex_descriptor get_vertex_descriptor()
		{		
			return vx_des;
		}

		bool get_is_initial()
		{
			return is_initial;
		}

		bool set_is_initial(bool v)
		{
			is_initial = v;
		}

	private:
		const string name;
		bool is_initial;
		state_info * context;
		string orthogonal;
		set<state_info*> inner_states;
		vertex_descriptor vx_des;
};
}//namespace


#endif //BOOST_STATECHART_VIEWER_STATE_INFO_HPP
