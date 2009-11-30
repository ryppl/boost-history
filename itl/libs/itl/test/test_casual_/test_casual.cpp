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


BOOST_AUTO_TEST_CASE(reverse_iter)
{
    typedef interval_bitset<int, bits8> InterBitsT;
    InterBitsT lbs;

    lbs.add(0).add(3).add(5);

	InterBitsT::element_reverse_iterator rit_   = lbs.elements_rend();
	InterBitsT::element_reverse_iterator revbeg = lbs.elements_rbegin();

	cout << "{";
	while(!(rit_ == revbeg))
	{
		--rit_;
		cout << *rit_ << " ";
	}
	cout << "}";
}


/*
BOOST_AUTO_TEST_CASE(bitter16_forward)
{
    typedef interval_bitset<int, bits16> InterBitsT;
    InterBitsT lbs;

    lbs
	   .add(-50).add(-62).add(-63).add(-64).add(-65)
	   .add(-17).add(-20).add(-31).add(-32).add(-33)
	   .add(-1).add(-2).add(-4).add(-7).add(-8).add(-9).add(-15).add(-16)
	   .add(0)
	   .add(1).add(2).add(4).add(7).add(8).add(9).add(15).add(16)
	   .add(17).add(20).add(31).add(32).add(33)
	   .add(50).add(62).add(63).add(64).add(65)
	   ;
    
	lbs.show_segments();
	cout << lbs << endl;

    InterBitsT::element_iterator it = lbs.elements_begin();
	int ele;
    cout << "{";
    while(it != lbs.elements_end())
    {
        ele = *it;
        cout << *it << " ";
        if(ele == 65)
        {
            int dbg = 0;
        }
        ++it;
    }
    cout << "}";
}


BOOST_AUTO_TEST_CASE(itv_set_reverted_backwards)
{
    typedef interval_set<int> InterSetT;
	InterSetT itv_set;

	itv_set;
	InterSetT::element_reverse_iterator rit;
	rit = itv_set.elements_rend();
	while(rit != itv_set.elements_rbegin())
	{
		cout << *rit << endl;
		--rit;
	}

	itv_set.add(0).add(1).add(3);

	rit = itv_set.elements_rend();

	if(rit != itv_set.elements_rbegin())
		--rit;

	cout << "{";
	while(rit != itv_set.elements_rbegin())
	{
		cout << *rit << " ";
		--rit;
	}

	BOOST_ASSERT(rit == itv_set.elements_rbegin());
	cout << *rit << "}\n";
}


BOOST_AUTO_TEST_CASE(itv_set_backwards)
{
    typedef interval_set<int> InterSetT;
	InterSetT itv_set;

	itv_set;
	InterSetT::element_iterator rit;
	rit = itv_set.elements_end();
	while(rit != itv_set.elements_begin())
	{
		cout << *rit << endl;
		--rit;
	}

	itv_set.add(0).add(1).add(3);

	rit = itv_set.elements_end();

	if(rit != itv_set.elements_begin())
		--rit;

	cout << "{";
	while(rit != itv_set.elements_begin())
	{
		cout << *rit << " ";
		--rit;
	}

	BOOST_ASSERT(rit == itv_set.elements_begin());
	cout << *rit << "}\n";
}



BOOST_AUTO_TEST_CASE(bitter64_reverted_backwards)
{
    typedef interval_bitset<int, bits64> InterBitsT;
    InterBitsT lbs;

    lbs
	   .add(-50).add(-62).add(-63).add(-64).add(-65)
	   .add(-17).add(-20).add(-31).add(-32).add(-33)
	   .add(-1).add(-2).add(-4).add(-7).add(-8).add(-9).add(-15).add(-16)
	   .add(0)
	   .add(1).add(2).add(4).add(7).add(8).add(9).add(15).add(16)
	   .add(17).add(20).add(31).add(32).add(33)
	   .add(50).add(62).add(63).add(64).add(65)
	   ;
    
	lbs.show_segments();
	cout << lbs << endl;

    InterBitsT::element_reverse_iterator it = lbs.elements_rend();
	InterBitsT::element_reverse_iterator rbeg_eq_at_64 = lbs.elements_rbegin();
	int ele;
	--it;
    cout << "{";
    while(it != lbs.elements_rbegin())
    {
        ele = *it;
        cout << *it << " ";
        if(ele == 64)
        {
            int dbg = 0;
        }
        --it;
    }
    ele = *it;
    cout << *it << " ";
    cout << "}";
}


BOOST_AUTO_TEST_CASE(bitter64_reverted)
{
    typedef interval_bitset<int, bits64> InterBitsT;
    InterBitsT lbs;

    lbs
	   .add(-50).add(-62).add(-63).add(-64).add(-65)
	   .add(-17).add(-20).add(-31).add(-32).add(-33)
	   .add(-1).add(-2).add(-4).add(-7).add(-8).add(-9).add(-15).add(-16)
	   .add(0)
	   .add(1).add(2).add(4).add(7).add(8).add(9).add(15).add(16)
	   .add(17).add(20).add(31).add(32).add(33)
	   .add(50).add(62).add(63).add(64).add(65)
	   ;
    
	lbs.show_segments();
	cout << lbs << endl;

    InterBitsT::element_reverse_iterator it = lbs.elements_rbegin();
	int ele;
    cout << "{";
    while(it != lbs.elements_rend())
    {
        ele = *it;
        cout << *it << " ";
        if(ele == -65)
        {
            int dbg = 0;
        }
        ++it;
    }
    cout << "}";
}
*/

/*---------------- bggy case
BOOST_AUTO_TEST_CASE(bitter64_backwards)
{
    typedef interval_bitset<int, bits64> InterBitsT;
    InterBitsT lbs;

    lbs
	   .add(-50).add(-62).add(-63).add(-64).add(-65)
	   .add(-17).add(-20).add(-31).add(-32).add(-33)
	   .add(-1).add(-2).add(-4).add(-7).add(-8).add(-9).add(-15).add(-16)
	   .add(0)
	   .add(1).add(2).add(4).add(7).add(8).add(9).add(15).add(16)
	   .add(17).add(20).add(31).add(32).add(33)
	   .add(50).add(62).add(63).add(64).add(65)
	   ;
    
	lbs.show_segments();
	cout << lbs << endl;

    InterBitsT::element_iterator it = lbs.elements_end();
	int ele;
	--it;
    cout << "{";
    while(it != lbs.elements_begin())
    {
        ele = *it;
        cout << *it << " ";
        if(ele == -65)
        {
            int dbg = 0;
        }
        --it;
    }
    ele = *it;
    cout << *it << " ";
    cout << "}";
}
*/

/*
BOOST_AUTO_TEST_CASE(bitter64_forward)
{
    typedef interval_bitset<int, bits64> InterBitsT;
    InterBitsT lbs;

    lbs
	   .add(-50).add(-62).add(-63).add(-64).add(-65)
	   .add(-17).add(-20).add(-31).add(-32).add(-33)
	   .add(-1).add(-2).add(-4).add(-7).add(-8).add(-9).add(-15).add(-16)
	   .add(0)
	   .add(1).add(2).add(4).add(7).add(8).add(9).add(15).add(16)
	   .add(17).add(20).add(31).add(32).add(33)
	   .add(50).add(62).add(63).add(64).add(65)
	   ;
    
	lbs.show_segments();
	cout << lbs << endl;

    InterBitsT::element_iterator it = lbs.elements_begin();
	int ele;
    cout << "{";
    while(it != lbs.elements_end())
    {
        ele = *it;
        cout << *it << " ";
        if(ele == 65)
        {
            int dbg = 0;
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
