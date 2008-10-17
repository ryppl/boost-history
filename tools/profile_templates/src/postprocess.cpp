// postprocess.cpp
//
// Copyright (c) 2008
// Steven Watanabe
// 
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
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

boost::regex warning_message("(.*) : warning C4150: deletion of pointer to incomplete type 'template_profiler::incomplete'; no destructor called");
boost::regex call_graph_line("        (.*)\\((\\d+)\\) : see reference to .*");
boost::regex split_file_and_line("(.*)\\((\\d+)\\)");

#elif defined(__GNUC__)

boost::regex warning_message("(.*): warning: division by zero in .template_profiler::value / 0.");
boost::regex call_graph_line("(.*):(\\d+):   instantiated from .*");
boost::regex split_file_and_line("(.*):(\\d+)");

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
    std::ptrdiff_t max_match_length = 0;
    {
        std::ifstream input(input_file_name);
        while(std::getline(input, line)) {
            boost::smatch match;
            if(boost::regex_match(line, match, warning_message)) {
                max_match_length = (std::max)(max_match_length, match[1].length());
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
        std::map<const line_id*, node_info> graph;
        std::set<line_id> lines;
        typedef std::pair<std::string, int> raw_line;
        BOOST_FOREACH(const raw_line& l, messages) {
            boost::smatch match;
            boost::regex_match(l.first, match, split_file_and_line);
            lines.insert(line_id(match[1], boost::lexical_cast<int>(match[2].str())));
        }
        const line_id* current_instantiation = 0;
        std::ifstream input(input_file_name);
#if defined(_MSC_VER)
        // msvc puts the warning first and follows it with the backtrace.
        while(std::getline(input, line)) {
            boost::smatch match;
            if(boost::regex_match(line, match, warning_message)) {
                std::string file_and_line(match[1].str());
                boost::regex_match(file_and_line, match, split_file_and_line);
                std::string file = match[1];
                int line = boost::lexical_cast<int>(match[2].str());
                current_instantiation = &*lines.find(line_id(file, line));
                ++graph[current_instantiation].total_with_children;
                ++graph[current_instantiation].count;
            } else if(boost::regex_match(line, match, call_graph_line)) {
                std::string file = match[1];
                int line = boost::lexical_cast<int>(match[2].str());
                std::set<line_id>::const_iterator pos = lines.lower_bound(line_id(file, line));
                if(pos != lines.end()) {
                    if(*pos != line_id(file, line) && pos != lines.begin() && boost::prior(pos)->first == file) {
                        --pos;
                    }
                } else if(pos != lines.begin()) {
                    --pos;
                } else {
                    break;
                }
                const line_id* parent = &*pos;
                ++graph[current_instantiation].parents[parent];
                ++graph[parent].children[current_instantiation];
                ++graph[parent].total_with_children;
            }
        }
#elif defined(__GNUC__)
        // gcc puts the backtrace first, and then the warning.
        // so we have to buffer the backtrace until we reach the
        // warning.
        std::vector<std::string> pending_lines;
        while(std::getline(input, line)) {
            boost::smatch match;
            if(boost::regex_match(line, match, warning_message)) {
                std::string file_and_line(match[1].str());
                boost::regex_match(file_and_line, match, split_file_and_line);
                std::string file = match[1];
                int line = boost::lexical_cast<int>(match[2].str());
                current_instantiation = &*lines.find(line_id(file, line));
                ++graph[current_instantiation].total_with_children;
                ++graph[current_instantiation].count;
                BOOST_FOREACH(const std::string& line, pending_lines) {
                    boost::regex_match(line, match, call_graph_line);
                    std::string file = match[1];
                    int line = boost::lexical_cast<int>(match[2].str());
                    std::set<line_id>::const_iterator pos = lines.lower_bound(line_id(file, line));
                    if(pos != lines.end()) {
                        if(*pos != line_id(file, line) && pos != lines.begin() && boost::prior(pos)->first == file) {
                            --pos;
                        }
                    } else if(pos != lines.begin()) {
                        --pos;
                    } else {
                        break;
                    }
                    const line_id* parent = &*pos;
                    ++graph[current_instantiation].parents[parent];
                    ++graph[parent].children[current_instantiation];
                    ++graph[parent].total_with_children;
                }
                pending_lines.clear();
            } else if(boost::regex_match(line, match, call_graph_line)) {
                pending_lines.push_back(line);
            } else {
                pending_lines.clear();
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
