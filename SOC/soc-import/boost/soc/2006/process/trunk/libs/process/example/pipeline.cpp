//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

//
// This example program demonstrates the construction and management of
// a pipeline.  The program performs a conversion on the given file and
// shows the results on stdout.
//

// quickbook:begin(code)
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <boost/process.hpp>

namespace bp = ::boost::process;

// ------------------------------------------------------------------------

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

    //
    // Constructs three command lines, one per each process in the
    // pipeline.
    //
    bp::command_line cl1 = bp::command_line::shell("cut -d ' ' -f 2-5");
    bp::command_line cl2 = bp::command_line::shell("sed 's,^,line: >>>,'");
    bp::command_line cl3 = bp::command_line::shell("sed 's,$,<<<,'");

    //
    // Constructs the pipeline.  It is configured to receive the input
    // we will be feeding it and to show the output on the parent's
    // stdout.
    bp::pipeline p;
    p.set_stdin_behavior(bp::redirect_stream);
    p.set_stdout_behavior(bp::inherit_stream);

    //
    // Adds the command lines to the pipeline, in the right order.
    //
    p.add(cl1).add(cl2).add(cl3);

    //
    // Starts the pipeline.
    //
    bp::children cs = p.start();

    //
    // Feeds the input file to the pipeline.
    //
    bp::postream& os = cs.get_stdin();
    std::string line;
    while (std::getline(file, line))
        os << line << std::endl;
    os.close();

    //
    // Waits until the pipeline terminates and returns an appropriate
    // error code to the caller.
    //
    bp::status s = cs.wait();
    return (s.exited() && s.exit_status() == EXIT_SUCCESS) ?
        EXIT_SUCCESS : EXIT_FAILURE;
}
// quickbook:end(code)
