/*----------------------------------------------------------------------------+
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#include <iostream>
#include <stdio.h>

#include <boost/itl/ptime.hpp> //CL (4 a quick test only)

#include <boost/validate/loki_xt/Tuple.h>
#include <boost/itl/set.hpp>
#include <boost/itl/map.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/laws/monoid.hpp>
#include <boost/validate/gentor/gentorprofile.h>
#include <boost/validate/gentor/rangegentor.h>

//#include <boost/validate/typevalidater.h>
#include <boost/validate/realmvalidater.h>
#include <boost/itl/interval_set.hpp>
#include <boost/itl_xt/numbergentor.hpp>
#include <boost/itl_xt/setgentor.hpp>
#include <boost/itl/functors.hpp>

using namespace std;
using namespace Loki;
using namespace boost;
using namespace boost::itl;
using namespace boost::posix_time;


void test_Validater()
{
    //GentorProfileSgl::it()->set_range_int(-10, 10);
    //GentorProfileSgl::it()->set_range_nat(0, 64);
    //GentorProfileSgl::it()->set_range_double(-10.0, 10.0);
    //GentorProfileSgl::it()->set_range_ContainerSize(0, 20);
    //GentorProfileSgl::it()->set_range_interval_int(-20, 20);
    //GentorProfileSgl::it()->set_maxIntervalLength(5);
    //GentorProfileSgl::it()->set_range_element_ContainerSize(0,5);


    //typedef BinaryPushout<itl::split_interval_map<int,double>, itl::map<int,double>, Interval::Atomize, inplace_plus>
    //    Map_Atomize_Union_DiagramT;
    //LawValidater<Map_Atomize_Union_DiagramT, RandomGentor> map_atomize_plus_pushout;
    //map_atomize_plus_pushout.setTrialsCount(1000);
    //map_atomize_plus_pushout.run();

    //typedef BinaryPushout<itl::map<int,double>, itl::split_interval_map<int,double>, Interval::Cluster, inplace_et>
    //    Map_Cluster_Intersect_DiagramT;
    //LawValidater<Map_Cluster_Intersect_DiagramT, RandomGentor> map_cluster_star_pushout;
    //map_cluster_star_pushout.setTrialsCount(1000);
    //map_cluster_star_pushout.run();

	//typedef InplaceSymmetricDifference<interval_map<int, itl::set<int>, total_enricher >, std_equal> TestLawT;
	//LawValidater<TestLawT, RandomGentor> test_law;
	//test_law.setTrialsCount(1000);
	//test_law.run();

	//typedef InplaceDistributivity
	//	<split_interval_map<int, itl::set<int>, partial_absorber >, 
	//	inplace_et, inplace_plus, itl::std_equal>  TestLawT;
	//LawValidater<TestLawT, RandomGentor> test_law;
	//test_law.setTrialsCount(1000);

	typedef InplaceNaturalInversion 
		<split_interval_map<int, itl::set<int>, total_absorber >, 
		itl::inplace_plus, itl::protonic_equal>  TestLawT;
	LawValidater<TestLawT, RandomGentor> test_law;
	test_law.setTrialsCount(1000);

	//typedef InplaceAssociativity
	//   <interval_map<int, int, partial_absorber >, inplace_plus>  TestLawT;
	//LawValidater<TestLawT, RandomGentor> test_law;
	//test_law.setTrialsCount(1000);

	//typedef InplaceCommutativity
	//   <split_interval_map<int, int, total_absorber>, inplace_plus>  TestLawT;
	//LawValidater<TestLawT, RandomGentor> test_law;
	//test_law.setTrialsCount(1000);

	//typedef ProtonicEquality
	//   <split_interval_map<int, int, partial_enricher > >  TestLawT;
	//LawValidater<TestLawT, RandomGentor> test_law;
	//test_law.setTrialsCount(1000);

	std::cout << "Start\n";
	ptime start(microsec_clock::local_time());
	test_law.run();
	ptime stop(microsec_clock::local_time());
	std::cout << "Stop. Time elapsed: " << stop - start << endl;
}

void test_realmvalidater()
{
    RealmValidater validater;
    cout << 
    ">> ------------------------------------------------------ <<\n"
    ">> -------- Law based test automaton 'LaBatea' ---------- <<\n"
    ">> Output will be generated in a few seconds\n"
    ">> terminate by typing <CTRL>C\n"
    ">> ------------------------------------------------------ <<\n";
    validater.validate();
};

int main()
{
    //test_Validater();
    test_realmvalidater();
    return 0;
}
