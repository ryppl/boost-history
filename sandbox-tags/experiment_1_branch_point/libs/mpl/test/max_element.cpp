#include "boost/mpl/list_c.hpp"
#include "boost/mpl/max_element.hpp"
#include "boost/mpl/comparison/less.hpp"

#include "boost/test/minimal.hpp"
#include "boost/cstdlib.hpp"

namespace mpl = boost::mpl;

int test_main( int, char *[] )
{
    using namespace mpl::placeholder;

    typedef mpl::list_c<int, 3, 4, 2, 0, -5, 8, -1, 7>::type numbers;

    typedef mpl::max_element< numbers, mpl::less<_,_> >::type max_el;
    typedef max_el::type max_number;

    BOOST_TEST(max_number() == 8);

    return boost::exit_success;
}
