#include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
#include <sstream>
#include <boost/test/unit_test.hpp>


void indent_scoped_ostreambuf_test(void)
{
  std::ostringstream mout;
  mout<<"line1\n";
 
  {
    boost::iostreams::indent_scoped_ostreambuf<char> indent_outbuf(mout,4);
    mout<<indent_buf_in;
    mout<<"line1.1"<<std::endl;
    { 
    #if 1
      //This shows that repeating the indent_scoped_ostreambuf construction
      //doesn't affect output.
      boost::iostreams::indent_scoped_ostreambuf<char> indent_outbuf(mout,4);
    #endif
      mout<<indent_buf_in;
      mout<<"line1.1.1"<<std::endl;
      mout<<"line1.1.2"<<std::endl;
      mout<<indent_buf_out;
      unsigned u=22;
      mout<<"line1.2:unsigned="<<u<<std::endl;
    }
    float f=3.1416;
    mout<<"line1.3:float="<<f<<std::endl;
    mout<<indent_buf_out;
    mout<<indent_buf_out;
    mout<<"line2\n";
    mout<<indent_buf_in;
    mout<<"line3\n";
  }
  mout<<"line4\n";
  std::string eout="\
line1\n\
    line1.1\n\
        line1.1.1\n\
        line1.1.2\n\
    line1.2:unsigned=22\n\
    line1.3:float=3.1416\n\
line2\n\
    line3\n\
line4\n\
";
        BOOST_CHECK_MESSAGE(
            std::string(mout.str()) == eout,
            "failed mout==eout"
        );
}

using boost::unit_test::test_suite;

test_suite* init_unit_test_suite(int, char* [])
{
    test_suite* test = BOOST_TEST_SUITE("indent_scoped_ostreambuf test");
    test->add(BOOST_TEST_CASE(&indent_scoped_ostreambuf_test));
    return test;
}
