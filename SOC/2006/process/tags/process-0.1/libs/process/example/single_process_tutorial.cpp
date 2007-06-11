//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// This code may be used under either of the following two licences:
//
//      Permission is hereby granted, free of charge, to any person
//      obtaining a copy of this software and associated documentation
//      files (the "Software"), to deal in the Software without
//      restriction, including without limitation the rights to use,
//      copy, modify, merge, publish, distribute, sublicense, and/or
//      sell copies of the Software, and to permit persons to whom the
//      Software is furnished to do so, subject to the following
//      conditions:
//
//      The above copyright notice and this permission notice shall be
//      included in all copies or substantial portions of the Software.
//
//      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//      OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//      HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//      WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//      FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//      OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at
//      http://www.boost.org/LICENSE_1_0.txt)
//

//
// Helper code for the "Single process tutorial" section in the
// documentation.
//

// quickbook:begin(code)
#include <cstdlib>
#include <iostream>
#include <string>

// quickbook:begin(include-headers)
#include <boost/process.hpp>
namespace bp = ::boost::process;
// quickbook:end(include-headers)

int
main(int argc, char* argv[])
{

if (argc < 2) {
    std::cerr << "Please specify a directory." << std::endl;
    return EXIT_FAILURE;
}
std::string dir = argv[1];

// quickbook:begin(command-line)
bp::command_line cl("svn");
cl.argument("update");
// quickbook:end(command-line)

// quickbook:begin(launcher)
bp::launcher l;
l.set_stdout_behavior(bp::redirect_stream);
l.set_merge_out_err(true);
l.set_work_directory(dir);
// quickbook:end(launcher)

// quickbook:begin(child-start)
bp::child c = l.start(cl);
// quickbook:end(child-start)

// quickbook:begin(get-output)
bp::pistream& is = c.get_stdout();
std::string line;
while (std::getline(is, line))
    std::cout << "Got line from child: " << line << std::endl;
// quickbook:end(get-output)

// quickbook:begin(wait)
bp::status s = c.wait();
// quickbook:end(wait)

// quickbook:begin(parse-status)
if (s.exited() && s.exit_status() == EXIT_SUCCESS)
    std::cout << "Directory updated successfully." << std::endl;
else
    std::cout << "Update failed." << std::endl;
std::cout << std::endl;
// quickbook:end(parse-status)

return EXIT_SUCCESS;

}
// quickbook:end(code)
