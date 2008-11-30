/* example to demonstrate compile-in-place for Boost.IOStreams

Compile with
  $CXX -I path/to/boost_X_YY_Z test-iostreams.cpp -lbz2 -lz

Notes:
- '-lbz2' and '-lz' of need to be replaced with the according libraries for
bzip2 and zlib. The above should work on Linux systems though.

- It would be helpful if we could exclude the two dependent libraries from
compilation somehow for people that don't want to use them.

$Id$
*/

#define BOOST_IOSTREAMS_COMPILE_IN_PLACE
#include <iostream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/line.hpp>
#include <boost/iostreams/compile_in_place.cpp>

namespace io = boost::iostreams;


class shell_comments_filter:
    public io::line_filter
{
public:
    shell_comments_filter()
    {}
private:
    typedef io::line_filter::string_type string_type;
    virtual string_type do_filter(string_type const& line)
    {
        if(line.empty())
            return line;
        if(line[0] == '#')
            return string_type();
        return line;
    }
};

int main()
{
    io::filtering_ostream out;
    out.push(shell_comments_filter());
    out.push(std::cout);
    // write to out using std::ostream interface

    out << std::cin.rdbuf();
}
