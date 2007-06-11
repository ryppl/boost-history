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
// Helper code for the "Single process tutorial" section in the
// documentation.
//

//[single_process_tutorial_all
#include <cstdlib>
#include <iostream>
#include <string>

//[single_process_tutorial_include_headers
#include <boost/process.hpp>
namespace bp = ::boost::process;
//]

int
main(int argc, char* argv[])
{

if (argc < 2) {
    std::cerr << "Please specify a directory." << std::endl;
    return EXIT_FAILURE;
}
std::string dir = argv[1];

//[single_process_tutorial_command_line
std::string exec = bp::find_executable_in_path("svn");
std::vector< std::string > args;
args.push_back("svn");
args.push_back("update");
//]

//[single_process_tutorial_context
bp::context ctx;
ctx.m_stdout_behavior = bp::capture_stream();
ctx.m_stderr_behavior = bp::redirect_stream_to_stdout();
ctx.m_work_directory = dir;
//]

//[single_process_tutorial_child_launch
bp::child c = bp::launch(exec, args, ctx);
//]

//[single_process_tutorial_get_output
bp::pistream& is = c.get_stdout();
std::string line;
while (std::getline(is, line))
    std::cout << "Got line from child: " << line << std::endl;
//]

//[single_process_tutorial_wait
const bp::status s = c.wait();
//]

//[single_process_tutorial_parse_status
if (s.exited() && s.exit_status() == EXIT_SUCCESS)
    std::cout << "Directory updated successfully." << std::endl;
else
    std::cout << "Update failed." << std::endl;
std::cout << std::endl;
//]

return EXIT_SUCCESS;

}
//]
