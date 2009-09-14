/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::casual unit test
#include <string>
#include <boost/mpl/list.hpp>
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
#include <boost/validate/type/nat.hpp>


using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;


BOOST_AUTO_TEST_CASE(casual_test)
{
    typedef int T;
    typedef int U;
    typedef itl::map<int,int> ItlMapT;
    typedef itl::set<int>     ItlSetT;
    typedef interval_map<int,int,partial_enricher>  IntervalMapT;
    typedef split_interval_map<int,int>  SplitIntervalMapT;
    typedef interval_set<int> IntervalSetT;
    typedef split_interval_set<int> SplitIntervalSetT;

    const bool test = is_same<SplitIntervalSetT::key_type, SplitIntervalSetT::interval_type>::value;
    BOOST_CHECK_EQUAL(test, true);

    SplitIntervalSetT spliss;
    spliss.add(I_D(1,3)).add(I_D(3,4)).add(I_D(4,6)).add(I_D(6,8));
    spliss.contains(I_D(2,5));
    BOOST_CHECK_EQUAL(spliss.contains(I_D(2,7)), true);

    
    ItlMapT map_a(make_pair(1,1));
    ItlSetT set_a(1);
    map_a.add(make_pair(2,1));
    set_a.add(2);
    bool its  = Map::intersects(map_a, map_a);
    
    erase(map_a, set_a);
    BOOST_CHECK_EQUAL(erase(map_a, set_a), ItlMapT());
    BOOST_CHECK_EQUAL(erase(map_a, map_a), ItlMapT());

    itl::map<int,int,total_absorber> tomp_a, tomp_b, tomp_c;
    tomp_a.add(make_pair(1,1)).add(make_pair(2,1));

    BOOST_CHECK_EQUAL((tomp_a & tomp_b).empty(), false);
    BOOST_CHECK_EQUAL((tomp_b & tomp_c).empty(), true);

    interval_map<int, interval_set<int> > maose, maose2;
    interval_set<int> inse;
    inse.add(I_I(1,4));

    maose.add(make_pair(I_I(1,4), inse));
    maose2 = maose;
    maose2.add(make_pair(I_I(5,6), inse));


    BOOST_CHECK_EQUAL(maose2.contains(maose), true);

    itl::set<int> eleset_a, eleset_a2, eleset_b, eleset_c;
    //eleset_a.add(1).add(2).add(5).add(8).add(11);
    eleset_a.add(8).add(11);
    eleset_a2 = eleset_a;
    eleset_b.add(8).add(2).add(11);
    eleset_c.add(9);

    //BOOST_CHECK_EQUAL(Set::inclusion_compare(eleset_a, eleset_a2), inclusion::equal);
    //BOOST_CHECK_EQUAL(Set::inclusion_compare(eleset_a, eleset_b), inclusion::superset);
    //BOOST_CHECK_EQUAL(Set::inclusion_compare(eleset_b, eleset_a), inclusion::subset);

    inclusion_compare(eleset_a, eleset_c);
    BOOST_CHECK_EQUAL(inclusion_compare(eleset_a, eleset_c), inclusion::unrelated);
}
