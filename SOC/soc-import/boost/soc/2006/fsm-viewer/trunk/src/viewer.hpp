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



#ifndef BOOST_STATECHART_VIEWER_VIEWER_HPP
#define BOOST_STATECHART_VIEWER_VIEWER_HPP

#include <irrXML.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <boost/config.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphviz.hpp>
#include "state_info.hpp"
#include "transition_info.hpp"

using namespace std;
using namespace boost;

namespace statechart_viewer
{


class viewer
{
	public:
		typedef graph_traits <GraphvizDigraph>::vertex_descriptor vertex_descriptor;

		int init_viewer (vector<string> &);
		void build_diagram (const string);

	private:
		
		void extract_file_ids (vector <string> &);
		void extract_states (vector <string> &, vector <string> &, map <string,string> &);
		void extract_transitions (map <string,string> &);
		
		void init_states (vector <string> &);
		void init_machines (vector <string> &);
		void init_transitions (map <string, string> &);

		bool is_source_file_id (string);
		bool is_state (string);
		bool is_machine (string);
		bool is_transition (string);

		void build_graph();

		state_info * get_state (string);
	
		enum headers
		{
			state_machine_hpp = 0,
			asynchronous_state_machine_hpp,
			simple_state_hpp,
			state_hpp,
			transition_hpp,
			list_hpp,
			item_hpp
		};
		
		static const int num_headers = 7;

		vector <string> header_ids;
		vector <string> source_ids;
		vector <string> machines;
		map <string, state_info *> states;
		vector<transition_info *> transitions;
		GraphvizDigraph G;
};

}//namespace
#endif //BOOST_STATECHART_VIEWER_VIEWER_HPP
