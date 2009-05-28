// postprocess.cpp
//
// Copyright (c) 2008-2009
// Steven Watanabe
// 
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <exception>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <vector>

#ifdef _MSC_VER

#pragma warning(disable:4512)

boost::regex enter_message("(.*) : warning C4150: deletion of pointer to incomplete type 'template_profiler::incomplete_enter'; no destructor called");
boost::regex exit_message("(.*) : warning C4150: deletion of pointer to incomplete type 'template_profiler::incomplete_exit'; no destructor called");
boost::regex call_graph_line("        (.*)\\((\\d+)\\) : see reference to .*");
boost::regex split_file_and_line("(.*)\\((\\d+)\\)");

#elif defined(__GNUC__)

#if (__GNUC__ < 4) || (__GNUC_MINOR__ < 3)

boost::regex enter_message("(.*): warning: division by zero in .template_profiler::enter_value / 0.");
boost::regex exit_message("(.*): warning: division by zero in .template_profiler::exit_value / 0.");
boost::regex call_graph_line("(.*):(\\d+):   instantiated from .*");
boost::regex split_file_and_line("(.*):(\\d+)");

#else

boost::regex enter_message("(.*): warning: .int template_profiler::enter\\(int\\).*");
boost::regex exit_message("(.*): warning: .int template_profiler::exit\\(int\\).*");
boost::regex call_graph_line("(.*):(\\d+):   instantiated from .*");
boost::regex split_file_and_line("(.*):(\\d+)");

#endif

#else

#error Unknown Compiler

#endif

struct print {
    int* cummulative;
    int width;
    typedef void result_type;
    template<class T>
    void operator()(const T& t) {
        *cummulative += t.second;
        std::cout << std::setw(width) << t.first << std::setw(10) << t.second << std::setw(10) << *cummulative << std::endl;
    }
};

struct compare {
    template<class T>
    bool operator()(const T& lhs, const T& rhs) const {
        return(lhs.second > rhs.second);
    }
};

typedef std::pair<std::string, int> line_id;

struct node_info {
    node_info() : count(0), total_with_children(0) {}
    std::map<const line_id*, int> children;
    std::map<const line_id*, int> parents;
    int count;
    int total_with_children;
};

struct call_graph_less {
    template<class T>
    bool operator()(const T& lhs, const T& rhs) const {
        return(lhs.second.total_with_children > rhs.second.total_with_children);
    }
};

struct print_line_id {
    print_line_id(const line_id* x) : line(x) {}
    const line_id* line;
};

std::ostream& operator<<(std::ostream& os, const print_line_id& arg) {
    os << arg.line->first << '(' << arg.line->second << ')';
    return(os);
}

class instantiation_state {
public:
    instantiation_state(std::map<const line_id*, node_info>& g) : graph(g) {}
    void finish_instantiation() {
        std::cerr << "finish_instantiation, state.size(): " << state.size() << std::endl;
        if(state.empty() || !state.back()) {
            std::cerr << "attempt to finish unstarted instantiation: " << state.size() << std::endl;
            std::exit(1);
        }
        state.pop_back();
        pending_children.pop_back();
    }
    void add_instantiation(const line_id* new_line, std::size_t backtrace_size) {
        if(backtrace_size == 0) {
            std::cerr << "non-template" << std::endl;
            // top level non-template
            backtrace_size = state.size() + 1;
        } else {
            ++backtrace_size;
        }
        std::cerr << "backtrace size: " << backtrace_size << ", state.size(): " << state.size() << std::endl;
        if(backtrace_size < state.size()) {
            std::cerr << "Instantiation not exited." << std::endl;
            std::exit(1);
        }
        ++graph[new_line].count;
        std::cerr << "Adding lines" << std::endl;
        BOOST_FOREACH(const line_id* line, state) {
            if(line) {
                add_child(line, new_line);
            }
        }
        state.resize(backtrace_size);
        pending_children.resize(backtrace_size);
        state.back() = new_line;
        BOOST_FOREACH(const line_id* child, pending_children.back()) {
            add_child(new_line, child);
        }
        BOOST_FOREACH(std::vector<const line_id*>& v, pending_children) {
            v.push_back(new_line);
        }
    }
private:
    void add_child(const line_id* parent, const line_id* child) {
        std::cerr << "parent: " << parent << ", child: " << child << std::endl;
        if(parent != child) {
            ++graph[parent].children[child];
            ++graph[child].parents[child];
            ++graph[parent].total_with_children;
        }
    }
    std::vector<const line_id*> state;
    std::vector<std::vector<const line_id*> > pending_children;
    std::map<const line_id*, node_info>& graph;
};

int main(int argc, char** argv) {
    const char* input_file_name = 0;
    bool use_call_graph = false;
    for(int i = 1; i < argc; ++i) {
        if(std::strcmp(argv[i], "--call-graph") == 0) {
            use_call_graph = true;
        } else {
            input_file_name = argv[i];
        }
    }
    if(input_file_name == 0) {
        std::cerr << "Usage: " << argv[0] << " <input file>\n";
        return(EXIT_FAILURE);
    }


    std::map<std::string, int> messages;
    std::string line;
    int total_matches = 0;
    int max_match_length = 0;
    {
        std::ifstream input(input_file_name);
        while(std::getline(input, line)) {
            boost::smatch match;
            if(boost::regex_match(line, match, enter_message)) {
                max_match_length = boost::numeric_cast<int>((std::max)(max_match_length, match[1].length()));
                ++messages[match[1]];
                ++total_matches;
            }
        }
    }
    std::vector<std::pair<std::string, int> > copy(messages.begin(), messages.end());
    std::sort(copy.begin(), copy.end(), compare());
    std::cout << "Total instantiations: " << total_matches << std::endl;
    int cummulative = 0;
    std::cout << std::setw(max_match_length) << "Location" << std::setw(10) << "count" << std::setw(10) << "cum." << std::endl;
    std::cout << std::setfill('-') << std::setw(max_match_length + 20) << '-' << std::setfill(' ') << std::endl;
    print p = { &cummulative, max_match_length };
    std::for_each(copy.begin(), copy.end(), p);

    if(use_call_graph) {
        std::size_t backtrace_depth = 0;
        std::map<const line_id*, node_info> graph;
        instantiation_state state(graph);
        std::set<line_id> lines;
        typedef std::pair<std::string, int> raw_line;
        BOOST_FOREACH(const raw_line& l, messages) {
            boost::smatch match;
            boost::regex_match(l.first, match, split_file_and_line);
            lines.insert(line_id(match[1], boost::lexical_cast<int>(match[2].str())));
        }
        const line_id* current_instantiation = 0;
        std::ifstream input(input_file_name);
        int line_number = 0;
#if defined(_MSC_VER)
        // msvc puts the warning first and follows it with the backtrace.
        while(std::getline(input, line)) {
            std::cerr << line_number++ << std::endl;
            boost::smatch match;
            if(boost::regex_match(line, match, enter_message)) {
                // commit the current instantiation
                if(current_instantiation) {
                    state.add_instantiation(current_instantiation, backtrace_depth);
                }
                // start a new instantiation
                std::string file_and_line(match[1].str());
                boost::regex_match(file_and_line, match, split_file_and_line);
                std::string file = match[1];
                int line = boost::lexical_cast<int>(match[2].str());
                current_instantiation = &*lines.find(line_id(file, line));
            } else if(boost::regex_match(line, match, call_graph_line)) {
                ++backtrace_depth;
            } else if(boost::regex_match(line, match, exit_message)) {
                // commit the current instantiation
                if(current_instantiation) {
                    state.add_instantiation(current_instantiation, backtrace_depth);
                }

                state.finish_instantiation();
                if(backtrace_depth) {
                    --backtrace_depth;
                }
                current_instantiation = 0;
            }
        }
        // commit the current instantiation
        if(current_instantiation) {
            state.add_instantiation(current_instantiation, backtrace_depth);
        }
#elif defined(__GNUC__)
        // gcc puts the backtrace first, and then the warning.
        while(std::getline(input, line)) {
            boost::smatch match;
            if(boost::regex_match(line, match, enter_message)) {
                std::string file_and_line(match[1].str());
                boost::regex_match(file_and_line, match, split_file_and_line);
                std::string file = match[1];
                int line = boost::lexical_cast<int>(match[2].str());
                current_instantiation = &*lines.find(line_id(file, line));
                ++backtrace_depth;
                std::cerr << backtrace_depth << std::endl;
                state.add_instantiation(current_instantiation, backtrace_depth);
                backtrace_depth = 0;
            } else if(boost::regex_match(line, match, call_graph_line)) {
                ++backtrace_depth;
            } else if(boost::regex_match(line, match, exit_message)) {
                state.finish_instantiation();
                backtrace_depth = 0;
            }
        }
#else
    #error Unsupported compiler
#endif
        typedef std::pair<const line_id*, node_info> call_graph_node_t;
        std::vector<call_graph_node_t> call_graph;
        std::copy(graph.begin(), graph.end(), std::back_inserter(call_graph));
        std::sort(call_graph.begin(), call_graph.end(), call_graph_less());
        std::cout << "\nCall Graph\n\n";
        BOOST_FOREACH(const call_graph_node_t& node, call_graph) {
            std::cout << print_line_id(node.first) << " (" << node.second.count << ")\n";
            typedef std::map<const line_id*, int>::const_reference node_t;
            std::cout << "  Parents:\n";
            BOOST_FOREACH(node_t n, node.second.parents) {
                std::cout << "    " << print_line_id(n.first) << " (" << n.second << ")\n";
            }
            std::cout << "  Children:\n";
            BOOST_FOREACH(node_t n, node.second.children) {
                std::cout << "    " << print_line_id(n.first) << " (" << n.second << "/" << graph[n.first].count << ")\n";
            }
        }
    }
}
