/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::casual unit test
#include <string>
#include <vector>
#include <boost/mpl/list.hpp>
#include <boost/itl/iterator.hpp>
#include "../unit_test_unwarned.hpp"
#include <boost/test/test_case_template.hpp>

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

#include <boost/type_traits/is_same.hpp>

#include <boost/itl/interval_set.hpp>
#include <boost/itl/separate_interval_set.hpp>
#include <boost/itl/split_interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/split_interval_map.hpp>
#include <boost/itl/detail/element_iterator.hpp>
#include <boost/itl_xt/detail/bit_element_iterator.hpp>
#include <boost/itl_xt/interval_bitset.hpp>
#include <boost/itl_xt/list.hpp>

#include <limits>
#include <bitset>


using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;

BOOST_AUTO_TEST_CASE(reverse_iter)
{
    interval_map<int,int> map_a;
    map_a += make_pair(interval<int>::rightopen(0,3),1);
    cout << map_a << endl;

    for(interval_map<int,int>::element_iterator elem = map_a.elements_begin();
        elem != map_a.elements_end(); elem++)
        cout << "(" << elem->first << "," << elem->second << ")";
    cout << "\n-------------------------------------\n";

    std::pair<const int, int> search_pair(2,1);

    interval_map<int,int>::element_const_iterator found 
        = std::find(map_a.elements_begin(), map_a.elements_end(), search_pair);
    cout << "(" << found->first << "," << found->second << ")\n";

    const_cast<int&>(found->second) = 2;
    cout << map_a << endl;
}

#include <limits>
#include <complex>

BOOST_AUTO_TEST_CASE(casual)
{
    cout << "--- limits ---\n";
    cout << "max<char>    = " << (std::numeric_limits<char>::max)() << endl;
    cout << "max<char>    = " << static_cast<int>((std::numeric_limits<char>::max)()) << endl;
    cout << "-----------------------------------\n";
    cout << "digits<char>    = " << std::numeric_limits<char>::digits << endl;
    cout << "digits<short>   = " << std::numeric_limits<short>::digits << endl;
    cout << "digits<float>   = " << std::numeric_limits<float>::digits << endl;
    cout << "digits<double>  = " << std::numeric_limits<double>::digits << endl;
    cout << "digits<complex<double>> = " << std::numeric_limits<std::complex<double> >::digits << endl;
    cout << "digits<string>  = " << std::numeric_limits<std::string>::digits << endl;


    BOOST_CHECK_EQUAL(0 != itl::infinity<int>::value(), true);
}

