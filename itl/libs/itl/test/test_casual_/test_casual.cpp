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
#include <boost/test/unit_test.hpp>
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



BOOST_AUTO_TEST_CASE(casual_test)
{
    typedef interval_bitset<nat, bits8> InterBits8T;
    InterBits8T lbs;

    lbs.add(1).add(4).add(7).add(8);
    
    InterBits8T::element_iterator it = lbs.elements_begin();
    cout << "{";
    while(it != lbs.elements_end())
    {
        int ele = *it;
        cout << *it << " ";
        if(ele == 8)
        {
            int dbg = 1;
        }
        ++it;
    }
    cout << "}";
}


/*
BOOST_AUTO_TEST_CASE(biter64_test)
{
    typedef nat64 worT ;
    const bit_range_type wdth = 64;

    worT val = 1;
    short pos = -1;
    short nxt, prv;

    cout << "------- bitter 64 ---------\n";

    val = 0;
    val |= 1ull << 63ull;
    val |= 1ull << 62ull;
    val |= 1ull << 55ull;
    val |= 1ull << 44ull;
    val |= 1ull << 33ull;
    val |= 1ull << 32ull;
    val |= 1ull << 31ull;
    val |= 1ull << 30ull;
    val |= 1ull << 22ull;
    val |= 1ull << 11ull;
    val |= 1ull <<  3ull;
    val |= 1ull <<  2ull;
    val |= 1ull <<  1ull;
    val |= 1ull <<  0ull;
    pos = -1;

    while(pos < wdth)
    {
        nxt = biterate::proceed<worT,wdth>::next(val,pos);
        cout << "next(" << val <<","<< pos << ")" << "=" << nxt << endl;
        pos = nxt;
    }
    cout << "- - - - - - - - - -\n";
    while(pos > -1)
    {
        if(pos == 31)
        {
            int dbg = 42;
        }
        prv = biterate::proceed<worT,wdth>::prev(val,pos);
        cout << "prev(" << val <<","<< pos << ")" << "=" << prv << endl;
        pos = prv;
    }
}

BOOST_AUTO_TEST_CASE(biter64_up_test)
{
    typedef nat64 worT ;
    const bit_range_type wdth = 64;

    worT val = 1;
    short pos = -1;
    short nxt, prv;

    cout << "------- bitter 64 up ---------\n";

    val = 0;
    val |= 1ull << 63ull;
    val |= 1ull << 62ull;
    val |= 1ull << 55ull;
    val |= 1ull << 44ull;
    val |= 1ull << 33ull;
    val |= 1ull << 32ull;
    pos = -1;

    while(pos < wdth)
    {
        nxt = biterate::proceed<worT,wdth>::next(val,pos);
        cout << "next(" << val <<","<< pos << ")" << "=" << nxt << endl;
        pos = nxt;
    }
    cout << "- - - - - - - - - -\n";
    while(pos > -1)
    {
        if(pos == 31)
        {
            int dbg = 42;
        }
        prv = biterate::proceed<worT,wdth>::prev(val,pos);
        cout << "prev(" << val <<","<< pos << ")" << "=" << prv << endl;
        pos = prv;
    }
}

BOOST_AUTO_TEST_CASE(biter64_low_test)
{
    typedef nat64 worT ;
    const bit_range_type wdth = 64;

    worT val = 1;
    short pos = -1;
    short nxt, prv;

    cout << "------- bitter 64 low ---------\n";

    val = 0;
    val |= 1ull << 31ull;
    val |= 1ull << 30ull;
    val |= 1ull << 22ull;
    val |= 1ull << 11ull;
    val |= 1ull <<  3ull;
    val |= 1ull <<  2ull;
    val |= 1ull <<  1ull;
    val |= 1ull <<  0ull;
    pos = -1;

    while(pos < wdth)
    {
        nxt = biterate::proceed<worT,wdth>::next(val,pos);
        cout << "next(" << val <<","<< pos << ")" << "=" << nxt << endl;
        pos = nxt;
    }
    cout << "- - - - - - - - - -\n";
    while(pos > -1)
    {
        if(pos == 64)
        {
            int dbg = 42;
        }
        prv = biterate::proceed<worT,wdth>::prev(val,pos);
        cout << "prev(" << val <<","<< pos << ")" << "=" << prv << endl;
        pos = prv;
    }
}

BOOST_AUTO_TEST_CASE(biter32_test)
{
    typedef nat32 worT ;
    const bit_range_type wdth = 32;

    worT val = 1;
    short pos = -1;
    short nxt, prv;

    cout << "------- bitter 32 ---------\n";

    val = 0;
    val |= 1ul << 31ul;
    val |= 1ul << 30ul;
    val |= 1ul << 22ul;
    val |= 1ul << 11ul;
    val |= 1ul <<  3ul;
    val |= 1ul <<  2ul;
    val |= 1ul <<  1ul;
    val |= 1ul <<  0ul;
    pos = -1;

    while(pos < wdth)
    {
        if(pos == wdth)
        {
            int dbg = 42;
        }
        nxt = biterate::proceed<worT,wdth>::next(val,pos);
        cout << "next(" << val <<","<< pos << ")" << "=" << nxt << endl;
        pos = nxt;
    }

    cout << "- - - - - - - - - -\n";
    while(pos > -1)
    {
        if(pos == 31)
        {
            int dbg = 42;
        }
        prv = biterate::proceed<worT,wdth>::prev(val,pos);
        cout << "prev(" << val <<","<< pos << ")" << "=" << prv << endl;
        pos = prv;
    }
}


BOOST_AUTO_TEST_CASE(biter16_test)
{
    typedef nat16 worT ;
    const bit_range_type wdth = 16;

    worT val = 1;
    short pos = -1;
    short nxt;

    cout << "------- bitter 16 ---------\n";

    val = 0;
    val |= 1 << 15;
    val |= 1 << 14;
    val |= 1 << 12;
    val |= 1 << 11;
    val |= 1 <<  3;
    val |= 1 <<  2;
    val |= 1 <<  1;
    val |= 1 <<  0;
    pos = -1;

    while(pos < wdth)
    {
        nxt = biterate::proceed<worT,wdth>::next(val,pos);
        cout << "next(" << val <<","<< pos << ")" << "=" << nxt << endl;
        pos = nxt;
    }
}

BOOST_AUTO_TEST_CASE(biter8_test)
{
    typedef nat8 worT ;
    const bit_range_type wdth = 8;

    worT val = 1;
    short pos = -1;
    short nxt;

    cout << "------- bitter 8 ---------\n";

    val = 0;
    val |= 1 <<  7;
    val |= 1 <<  4;
    val |= 1 <<  2;
    val |= 1 <<  1;
    val |= 1 <<  0;
    pos = -1;

    while(pos < wdth)
    {
        nxt = biterate::proceed<worT,wdth>::next(val,pos);
        cout << "next(" << static_cast<short>(val) <<","<< pos << ")" << "=" << nxt << endl;
        pos = nxt;
    }
}
*/
