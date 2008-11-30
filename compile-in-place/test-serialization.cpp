/* example to demonstrate compile-in-place for Boost.Serialization

Compile with
  $CXX -I path/to/boost_X_YY_Z test-serialization.cpp

Notes:

$Id$
*/

#define BOOST_SERIALIZATION_COMPILE_IN_PLACE
#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

int main()
{
    // data to store
    std::string str = "Hello World!";
    int answer = 42;
    // stream for storage
    std::stringstream s;
    {
        // write data
        boost::archive::text_oarchive oa(s);
        oa << str << answer;
    }
    {
        // read back data
        boost::archive::text_iarchive ia(s);
        ia >> str >> answer;
    }
    // verify results
    assert(str=="Hello World!");
    assert(answer=42);

    return 0;
}

#include <boost/serialization/compile_in_place.cpp>
