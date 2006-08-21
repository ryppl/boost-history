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
// Helper code for the "Pipeline tutorial" section in the documentation.
//

// quickbook:begin(code)
#include <cstdlib>
#include <fstream>
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
    std::cerr << "Please specify a file name." << std::endl;
    return EXIT_FAILURE;
}

std::ifstream file(argv[1]);
if (!file) {
    std::cerr << "Cannot open file." << std::endl;
    return EXIT_FAILURE;
}

// quickbook:begin(pipeline)
bp::pipeline p;
p.set_stdin_behavior(bp::redirect_stream);
p.set_stdout_behavior(bp::inherit_stream);
// quickbook:end(pipeline)

// quickbook:begin(command-lines)
bp::command_line cl1 = bp::command_line::shell("cut -d ' ' -f 2-5");
bp::command_line cl2 = bp::command_line::shell("sed 's,^,line: >>>,'");
bp::command_line cl3 = bp::command_line::shell("sed 's,$,<<<,'");
// quickbook:end(command-lines)

// quickbook:begin(addition)
p.add(cl1).add(cl2).add(cl3);
// quickbook:end(addition)

// quickbook:begin(children)
bp::children cs = p.start();
// quickbook:end(children)

// quickbook:begin(feed-stdin)
bp::postream& os = cs.get_stdin();
std::string line;
while (std::getline(file, line))
    os << line << std::endl;
os.close();
// quickbook:end(feed-stdin)

// quickbook:begin(wait)
bp::status s = cs.wait();
// quickbook:end(wait)

// quickbook:begin(parse-status)
return (s.exited() && s.exit_status() == EXIT_SUCCESS) ?
    EXIT_SUCCESS : EXIT_FAILURE;
// quickbook:end(parse-status)

}
// quickbook:end(code)
