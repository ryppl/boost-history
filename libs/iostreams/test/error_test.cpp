#include <boost/iostreams/categories.hpp>  // tags.
#include <boost/iostreams/detail/ios.hpp>  // openmode, seekdir, int types.
#include <boost/iostreams/detail/error.hpp>
#include <boost/iostreams/positioning.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

namespace bio = boost::iostreams;
using boost::unit_test::test_suite;

struct error_device
{
    typedef char   char_type;
    typedef bio::seekable_device_tag category;
    
    error_device(char const*) {}
    
    std::streamsize read(char_type* s, std::streamsize n);
    std::streamsize write(const char_type* s, std::streamsize n);
    std::streampos seek(bio::stream_offset off, BOOST_IOS::seekdir way);
    
};

std::streamsize error_device::read(char_type*, std::streamsize)
{
    throw bio::detail::bad_read();
}

std::streamsize error_device::write(const char_type*, std::streamsize)
{
    throw bio::detail::bad_write();
}

std::streampos error_device::seek(bio::stream_offset, BOOST_IOS::seekdir)
{
    throw bio::detail::bad_seek();
}

typedef bio::stream<error_device> test_stream;

void test_read()
{
    test_stream stream("foo");
    char data[10];
    try{
        stream.read(data, 10);
        BOOST_CHECK_MESSAGE(stream.fail(), "stream did not fail");
    }catch(...){
        BOOST_CHECK(false);
    }
}
void test_write()
{
    test_stream stream("foo");
    char data[10] = {0};
    try{
        stream.write(data, 10);
        BOOST_CHECK_MESSAGE(stream.fail(), "stream did not fail");
    }catch(...){
        BOOST_CHECK(false);
    }
}
void test_seekg()
{
    test_stream stream("foo");
    try{
        stream.seekg(10);
        BOOST_CHECK_MESSAGE(stream.fail(), "stream did not fail");
    }catch(...){
        BOOST_CHECK(false);
    }
}
void test_seekp()
{
    test_stream stream("foo");
    try{
        stream.seekp(10);
        BOOST_CHECK_MESSAGE(stream.fail(), "stream did not fail");
    }catch(...){
        BOOST_CHECK(false);
    }
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("error test");
    test->add(BOOST_TEST_CASE(&test_read));
    test->add(BOOST_TEST_CASE(&test_write));
    test->add(BOOST_TEST_CASE(&test_seekg));
    test->add(BOOST_TEST_CASE(&test_seekp));
    return test;
}
