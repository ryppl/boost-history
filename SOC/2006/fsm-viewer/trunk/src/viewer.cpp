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



#include "viewer.hpp"

using namespace std;
using namespace boost;
using namespace statechart_viewer;

int viewer::init_viewer (vector<string> & source_files) 
{
	string cmd = "gccxml " + source_files[0] + " -fxml=out.xml";
	if (system(cmd.c_str()))
		return 1;

	header_ids.resize(num_headers);
	extract_file_ids (source_files);

	vector <string> unparsed_states; 
	vector <string> unparsed_machines;
	map <string, string> unparsed_transitions;

	extract_states (unparsed_states, unparsed_machines, unparsed_transitions);
	init_states (unparsed_states);
	init_machines (unparsed_machines);
	extract_transitions(unparsed_transitions);
	init_transitions (unparsed_transitions);

return 0;
}

 void viewer::extract_file_ids(vector<string> & source_files)
{
	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader("out.xml");
	while(xml && xml->read()) 
	{
		if(xml->getNodeType() == irr::io::EXN_ELEMENT) {

			if (((string)xml->getNodeName()).find("File") == string::npos) 
				continue;

			string name = xml->getAttributeValue("name");
			string id = xml->getAttributeValue("id");

			if (name.find("/statechart/asynchronous_state_machine.hpp") != string::npos)
				header_ids[asynchronous_state_machine_hpp] = id;
			else if (name.find("/statechart/state_machine.hpp") != string::npos)
				header_ids[state_machine_hpp] = id;
			else if (name.find("/statechart/simple_state.hpp") != string::npos)
				header_ids[simple_state_hpp] = id;
			else if (name.find("/statechart/state.hpp") != string::npos)
				header_ids[state_hpp] = id;
			else if (name.find("/statechart/transition.hpp") != string::npos)
				header_ids[transition_hpp] = id;
			else if (name.find("mpl/aux_/preprocessed/gcc/list.hpp") != string::npos)
				header_ids[list_hpp] = id;
			else if (name.find("/mpl/list/aux_/item.hpp") != string::npos)
				header_ids[item_hpp] = id;
			else 
			{	
				for (int i = 0; i < source_files.size(); i++)
					if (name.find(source_files[i]) != string::npos) //potential problem with nested names
						source_ids.push_back(id);
						
			}
		}
	}
	delete xml;
}

bool viewer::is_source_file_id (string file_id)
{
	for (int i = 0; i < source_ids.size(); i++)
		if (source_ids[i] == file_id)
			return true;
return false;
}

bool viewer::is_state (string file_id)
{
	return (file_id == header_ids[simple_state_hpp] || file_id == header_ids[state_hpp]);
}

bool viewer::is_machine (string file_id)
{
 	return (file_id == header_ids[state_machine_hpp] || file_id == header_ids[asynchronous_state_machine_hpp]);	
}

bool viewer::is_transition (string file_id)
{
	return (file_id == header_ids[transition_hpp] || file_id == header_ids[list_hpp] || file_id == header_ids[item_hpp]);
}


void viewer::extract_states (vector <string> & unparsed_states, vector <string> & unparsed_machines, map <string,string> & unparsed_transitions)
{
	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader("out.xml");
	string bases = " ";
	map <string, string> members;
	while(xml && xml->read()) 
	{
		if(xml->getNodeType() == irr::io::EXN_ELEMENT) {

		if (((string)xml->getNodeName()).find("Struct") != string::npos) {	
		
			if (is_source_file_id ((string)xml->getAttributeValue("file"))) {
				
				if (xml->getAttributeValue("members") || xml->getAttributeValue("bases")) {
		
					members[xml->getAttributeValue("members")] = xml->getAttributeValue("name");
					bases += (string)xml->getAttributeValue("bases");
					
				}
			}
		}
	
		if (((string)xml->getNodeName()).find("Class") != string::npos) {
			
			string file_id = xml->getAttributeValue("file");
			string id = xml->getAttributeValue("id");
			string name = xml->getAttributeValue("name");

			if (is_state(file_id) && bases.find(id + " ") != string::npos) 
				unparsed_states.push_back(name);
			else if (is_machine(file_id) && bases.find(id + " ") != string::npos) 
				unparsed_machines.push_back(name);
			}
		}

		if (((string)xml->getNodeName()).find("Typedef") != string::npos) {
		
			if (((string)xml->getAttributeValue("name")) == "reactions") {
				
				string id = xml->getAttributeValue("id");
				map <string, string>::iterator it;			
				for (it = members.begin(); it != members.end(); ++it)
					if ( ((*it).first).find(id+" ") !=string::npos ){
						
						unparsed_transitions[xml->getAttributeValue("type")] = (*it).second;
						continue;
					}
				
			}
		}
	}
	
	delete xml;
}

state_info * viewer::get_state(string name)
{
	if (states.find(name) == states.end())			
		states[name] = new state_info(name);

return states[name];
}

void get_to_end_of_tag (string & raw, string::size_type & t)
{
	string::size_type s;
	int i = 1;
	t++;
	while(i)
	{
		s = raw.find_first_of("<",t);
		t = raw.find_first_of(">",t);
		if (s<t) 
		{
			i++;
			t = s + 1;
		}
		else
		{
			i--;
			t = t + 1;
		}
	}
}

string get_special_token (string & raw, string::size_type & t)
{
	string::size_type u, s = ++t;
	t = raw.find_first_of(",",s);
	u = raw.find_first_of("<",s);
	if (u < t) {
		t = u;
		get_to_end_of_tag(raw, t);
	}
		
return raw.substr(s,t-s);
}

void viewer::init_states (vector<string> & unparsed_states)
{
	string::size_type s, t, u;
	for (int i = 0; i < unparsed_states.size(); i++) 
	{
		string raw = unparsed_states[i];
	
		string name;
		t = raw.find_first_of("<");
		name = get_special_token(raw,t);
		state_info * s1 = get_state (name);
	
		string context;
		s = ++t;
		t = raw.find_first_of(",",s);
		u = raw.find_first_of("<",s);
		if (u < t) {
			
			t = raw.find_first_of(",",++u);
			context = raw.substr(u,t-u);
			get_to_end_of_tag(raw,u);
			t = raw.find_first_of(">",u);
			t++;
		}
		else context = raw.substr(s,t-s); 

		state_info * s2 = get_state (context);
		s1->set_context(s2);
		s2->add_inner(s1);	
	
		string initial;		
		state_info * s3;
		s = ++t;
		t = raw.find("boost::mpl::list",s);
		if (t == string::npos) {
			
			t = raw.find_first_of(",",s);
			initial = raw.substr(s,t-s);
			s3 = get_state (initial);
			s3->set_is_initial(true);
			s1->add_inner(s3);
		}
		else {
			s = raw.find_first_of("<",t);
			t = raw.find_first_of(",",s);
			while(true)
			{
				initial = raw.substr(s+1,t-s-1);
				if (initial.find("mpl_::na") != string::npos)
					break;
				s3 = get_state (initial);
				s3->set_is_initial(true);
				s1->add_inner(s3);
				s = t+1;
				t = raw.find_first_of(",",s);
			}
		}
	}
}

void viewer::init_machines (vector<string> & unparsed_machines)
{
	string::size_type s, t, u;
	for (int i = 0; i < unparsed_machines.size(); i++) 
	{
		string raw = unparsed_machines[i];
		s = raw.find_first_of("<");
		t = raw.find_first_of(",",s++);
		machines.push_back(raw.substr(s,t-s));
		states[get_special_token(raw,t)]->set_is_initial(true);
	}
}

void viewer::extract_transitions (map <string,string> & unparsed_transitions)
{
	irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader("out.xml");
	string bases = " ";
	map <string, string> members;
	while(xml && xml->read()) 
	{
		if(xml->getNodeType() == irr::io::EXN_ELEMENT) {
			
			if (xml->getAttributeValue("id")) {

				string id = xml->getAttributeValue("id");
				
				if (unparsed_transitions.find(id) != unparsed_transitions.end()) {
					unparsed_transitions[xml->getAttributeValue("name")] = unparsed_transitions[id];
					unparsed_transitions.erase(id);
				}
			}	
		}
		
	}
	
	delete xml;
}

void viewer::init_transitions (map <string, string> & unparsed_transitions)
{
	string::size_type r,s,t,u,v;
	map<string, string>::iterator it;
	for (it = unparsed_transitions.begin(); it != unparsed_transitions.end(); ++it)
	{
		string raw = (*it).first;
		r = raw.find("transition");
		while(true)
		{
			if (r==string::npos)
				break;

			r = raw.find_first_of("<",r);
			string event = get_special_token(raw,r);
			string final (get_special_token(raw,r));
			final = final.erase (0, final.find_first_not_of (" "));	
			transitions.push_back(new transition_info(get_state((*it).second),get_state(final),event));
			r = raw.find("transition",r);
		}
	}
}

void viewer::build_graph()
{  
	get_property(G, graph_graph_attribute)["compound"] = "true";
	for (int i = 0; i < machines.size(); i++)
	{
		GraphvizDigraph * H = &(G.create_subgraph());
		get_property(*H, graph_name) = "cluster_" + machines[i];
		get_property(*H, graph_graph_attribute)["label"] = machines[i];
		get_property(*H, graph_graph_attribute)["compound"] = "true";  
		states[machines[i]]->build_vertex(H	);
	}
	for (int i = 0; i < transitions.size(); i++)
		transitions[i]->build_edge(G);
}

void viewer::build_diagram (const string output)
{
	build_graph();
	std::ofstream myfile;
	myfile.open ("out.dot");
	write_graphviz(myfile,G);
	string cmd = "dot out.dot -o" + output + " -Tpng";
	system(cmd.c_str());
}

int main (int argc, char *argv[])
{
	if (argc < 3)
	{
		cout<<"syntax: "<<argv[0]<<" source_files output\n";
		return 1;
	}

	vector<string> source_files;
	for (int i = 1; i < argc - 1; i++)
		source_files.push_back(argv[i]);
	viewer * v = new viewer();
	if (v->init_viewer(source_files))
	{
		cout<<"error during init\n";
		return 1;
	}
	v->build_diagram(argv[argc-1]);
}

