/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#include <iostream>
#include <stdio.h>

#include <boost/itl/ptime.hpp>

#include <boost/itl/set.hpp>
#include <boost/itl/map.hpp>
#include <boost/itl/detail/interval_morphism.hpp>
#include <boost/validate/itl/functors.hpp>
#include <boost/validate/laws/induced_relation.hpp>
#include <boost/validate/laws/symmetric_difference.hpp>
#include <boost/validate/laws/pushouts.hpp>
#include <boost/validate/laws/set_laws.hpp>
#include <boost/validate/laws/minor_set_laws.hpp>
#include <boost/validate/laws/function_equality.hpp>
#include <boost/validate/laws/atomic_equivalence.hpp>

//#include <boost/validate/laws/novial_tree.hpp>
#include <boost/validate/laws/inversion_laws.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/std/algorithm/copy.hpp>
#include <boost/validate/std/algorithm/find.hpp>
#include <boost/validate/gentor/gentorprofile.hpp>
#include <boost/validate/gentor/rangegentor.hpp>
#include <boost/itl/interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/functors.hpp>
#include <boost/itl_xt/interval_bitset.hpp>

using namespace std;
using namespace Loki;
using namespace boost;
using namespace boost::itl;
using namespace boost::posix_time;


void test_LawValidater()
{

    typedef UnaryAtomicEquivalence2
    <
        itl::interval_map<int,int>, 
        //itl::list<int>,
        std::pair<int,int>,
        itl::std_find
    > TestLawT;


    LawValidater<TestLawT, RandomGentor> test_law;

    //-----------------------------------------------------------------------------
    int test_count = 10000;
    ptime start, stop;

    GentorProfileSgl::it()->set_std_profile(8,1);
    test_law.set_trials_count(test_count);

    TestLawT law;
    cout << law.typeString() << endl;
    std::cout << "Start\n";
    start = ptime(microsec_clock::local_time());
    test_law.run();
    stop = ptime(microsec_clock::local_time());
    std::cout << "Stop. Time elapsed: " << stop - start << endl;
}



int main()
{
    test_LawValidater();
    return 0;
}
