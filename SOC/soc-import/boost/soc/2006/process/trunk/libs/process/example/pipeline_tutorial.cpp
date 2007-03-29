//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

//
// Helper code for the "Pipeline tutorial" section in the documentation.
//

//[pipeline_tutorial_all
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

//[pipeline_tutorial_include_headers
#include <boost/process.hpp>
namespace bp = ::boost::process;
//]

int
main(int argc, char* argv[])
{

if (argc < 2) {
    std::cerr << "Please specify a file name." << std::endl;
    return EXIT_FAILURE;
}

std::ifstream file(argv[1]);
if (!file) {
    std::cerr << "Cannot open file." << std::endl;
    return EXIT_FAILURE;
}

//[pipeline_tutorial_context
bp::context ctxin, ctxout;
ctxin.m_stdin_behavior = bp::capture_stream();
ctxout.m_stdout_behavior = bp::inherit_stream();
ctxout.m_stderr_behavior = bp::redirect_stream_to_stdout();
//]

//[pipeline_tutorial_command_lines
std::string exe1 = bp::find_executable_in_path("cut");
std::vector< std::string > args1;
args1.push_back("cut");
args1.push_back("-d ");
args1.push_back("-f2-5");

std::string exe2 = bp::find_executable_in_path("sed");
std::vector< std::string > args2;
args2.push_back("sed");
args2.push_back("s,^,line: >>>,");

std::string exe3 = bp::find_executable_in_path("sed");
std::vector< std::string > args3;
args3.push_back("sed");
args3.push_back("s,$,<<<,");
//]

//[pipeline_tutorial_addition
std::vector< bp::pipeline_entry > entries;
entries.push_back(bp::pipeline_entry(exe1, args1, ctxin));
entries.push_back(bp::pipeline_entry(exe2, args2, ctxout));
entries.push_back(bp::pipeline_entry(exe3, args3, ctxout));
//]

//[pipeline_tutorial_children
bp::children cs = bp::launch_pipeline(entries);
//]

//[pipeline_tutorial_feed_stdin
bp::postream& os = cs[0].get_stdin();
std::string line;
while (std::getline(file, line))
    os << line << std::endl;
os.close();
//]

//[pipeline_tutorial_wait
const bp::status s = bp::wait_children(cs);
//]

//[pipeline_tutorial_parse_status
return (s.exited() && s.exit_status() == EXIT_SUCCESS) ?
    EXIT_SUCCESS : EXIT_FAILURE;
//]

}
//]
