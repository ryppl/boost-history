/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#include <boost/config/warning_disable.hpp>
#include <iostream>
#include <stdio.h>

#include <boost/icl/ptime.hpp>

#include <boost/icl/right_open_interval.hpp>
#include <boost/icl_xt/set.hpp>
#include <boost/icl/map.hpp>
#include <boost/icl/detail/interval_morphism.hpp>
#include <boost/validate/icl/functors.hpp>
#include <boost/validate/laws/induced_relation.hpp>
#include <boost/validate/laws/symmetric_difference.hpp>
#include <boost/validate/laws/pushouts.hpp>
#include <boost/validate/laws/set_laws.hpp>
#include <boost/validate/laws/minor_set_laws.hpp>
#include <boost/validate/laws/function_equality.hpp>
#include <boost/validate/laws/atomic_equivalence.hpp>
#include <boost/validate/laws/order.hpp>

//#include <boost/validate/laws/novial_tree.hpp>
#include <boost/validate/laws/inversion_laws.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/std/algorithm/copy.hpp>
#include <boost/validate/std/algorithm/find.hpp>
#include <boost/validate/gentor/gentorprofile.hpp>
#include <boost/validate/gentor/rangegentor.hpp>
#include <boost/icl/interval_set.hpp>
#include <boost/icl/interval_map.hpp>
#include <boost/icl/functors.hpp>
#include <boost/icl_xt/interval_bitset.hpp>

using namespace std;
using namespace Loki;
using namespace boost;
using namespace boost::icl;
using namespace boost::posix_time;


void test_LawValidater()
{

    //typedef UnaryAtomicEquivalence2
    //<
    //    icl::interval_map<int,int>, 
    //    //icl::list<int>,
    //    std::pair<int,int>,
    //    icl::std_find
    //> TestLawT;

    //typedef InducedRelation
    //<
    //    icl::interval_map<int,int>, 
    //    icl::map<int,int>, 
    //    segmental::atomizer, 
    //    element_equal
    //>
    //TestLawT;

    //typedef Antisymmetry<icl::map<int,int>, std::less_equal, std_equal> TestLawT;

    //typedef JointInverseExistence
    //    <interval_map<int,int,partial_enricher>, icl::inplace_plus, icl::inplace_minus> TestLawT;
    //LawValidater<TestLawT, RandomGentor> test_law;

    //typedef InplaceCommutativity<icl::set<double> > TestLawT;
    //LawValidater<TestLawT> test_law;

    //typedef FunctionEquality
    //<
    //    icl::list<std::pair<right_open_interval<double,std::less>,int> >, //SourceT, 
    //    split_interval_map<double,int,partial_absorber>,                 //TargetT, 
    //    base_insertion, 
    //    hint_insertion
    //>
    //TestLawT;

    //typedef Antisymmetry<icl::map<int,int>, icl::sub_super_set, icl::element_equal> TestLawT;
    //typedef InplaceAssociativity<icl::interval_map<int,icl::set<int> >, icl::inplace_caret, icl::element_equal> TestLawT;

    typedef InplaceFlip< icl::map<int,int,partial_enricher> > TestLawT;
    //typedef InplaceFlip< icl::map<int,int> > TestLawT;
    //typedef InplaceFlip< icl::map<int, icl::set<int> > > TestLawT;

    LawValidater<TestLawT> test_law;


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
    std::cout << "Stop. Time elapsed: " << stop - start << " for " << test_count << " tests\n";
}



int main()
{
    test_LawValidater();
    return 0;
}
