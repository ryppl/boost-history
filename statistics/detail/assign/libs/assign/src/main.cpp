#include <iostream>
#define BOOST_ASSIGN_CHECK_EQUAL(a,b) BOOST_ASSERT(a==b)
//#include <boost/assign/auto_size/check/ref_list_of.hpp>
//#include <boost/assign/auto_size/check/ref_csv.hpp>
#undef BOOST_ASSIGN_CHECK_EQUAL
//#include <libs/assign/example/chain.h>
#include <libs/assign/example/range.h>

int main (int argc, char * const argv[]) {

     //using namespace boost::assign::detail::auto_size;

     //check_ref_list_of_int();
     //std::cout << "check_ref_list_of : ok" << std::endl;
     //check_ref_csv_int();
     //std::cout << "check_csv : ok" << std::endl;

//     example_chain(std::cout);
     example_range(std::cout);

    return 0;

}
