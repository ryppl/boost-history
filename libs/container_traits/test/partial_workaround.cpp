#include <boost/container_traits/detail/iterator.hpp>
#include <boost/container_traits/detail/const_iterator.hpp>
#include <boost/container_traits/detail/result_iterator.hpp>
#include <boost/container_traits/detail/size_type.hpp>
#include <boost/container_traits/detail/value_type.hpp>
#include <boost/container_traits/detail/difference_type.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>
#include <iostream>

using namespace boost;
using namespace std;

void check_array()
{
}



int test_main( int, char*[] )
{
    check_array();

    return 0;
}


