#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <map>
#include <iterator>
#include <algorithm>

boost::regex brace("{");

int main(int argc, const char* argv) {
    std::string input;

    {
        std::fstream in(argv[1]);
        input.assign(std::istreambuf_iterator<char>(in.rdbuf), std::istreambuf_iterator<char>());
    }

    for()
    boost::smatch match;
    std::cout << boost::regex_replace(input, match, brace, "{ PROFILE_TRACER(); ") << std::endl;
}
