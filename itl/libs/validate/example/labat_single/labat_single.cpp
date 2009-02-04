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
#include <boost/validate/lawvalidater.h>
#include <boost/validate/laws/monoid.h>
#include <boost/validate/laws/element_order.hpp>
#include <boost/validate/laws/subset_order.hpp>
#include <boost/validate/gentor/gentorprofile.h>
#include <boost/validate/gentor/rangegentor.h>

#include <boost/validate/single_law_validater.hpp>
#include <boost/itl/interval_set.hpp>
//#include <boost/itl_xt/numbergentor.hpp>
//#include <boost/itl_xt/setgentor.hpp>
#include <boost/itl/functors.hpp>

using namespace std;
using namespace Loki;
using namespace boost;
using namespace boost::itl;
using namespace boost::posix_time;


void test_LawValidater()
{
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

	//typedef InplaceSymmetricDifference<split_interval_map<int, int, total_absorber >, std_equal> TestLawT;
	//LawValidater<TestLawT, RandomGentor> test_law;
	//test_law.setTrialsCount(1000);
	//test_law.run();

	//typedef InplaceDistributivity
	//	<split_interval_map<int, itl::set<int>, partial_absorber >, 
	//	inplace_et, inplace_plus, itl::std_equal>  TestLawT;
	//LawValidater<TestLawT, RandomGentor> test_law;
	//test_law.setTrialsCount(1000);

	//typedef InplaceNaturalInversion 
	//	<split_interval_map<int, itl::set<int>, total_absorber >, 
	//	itl::inplace_plus, itl::protonic_equal>  TestLawT;
	//LawValidater<TestLawT, RandomGentor> test_law;
	//test_law.setTrialsCount(1000);

	//typedef InplaceAssociativity
	//   <split_interval_map<int, int, total_absorber >, inplace_et>  TestLawT;
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

	//typedef InplaceInverseExistence
	//   <split_interval_map<int, int, total_absorber>, inplace_plus>  TestLawT;
	//LawValidater<TestLawT, RandomGentor> test_law;
	//test_law.setTrialsCount(1000);

	//typedef InplaceFlip
	//	<split_interval_map<int, int, total_enricher > >  TestLawT;
	//LawValidater<TestLawT, RandomGentor> test_law;
	//test_law.setTrialsCount(10000);

	ptime start, stop;

	//-----------------------------------------------------------------------------
	/*
	typedef ElementEqualDefined
		<split_interval_map<int,int> >  TestLawT;
	LawValidater<TestLawT, RandomGentor> test_law;
	test_law.setTrialsCount(10000);

	std::cout << "1Start\n";
	start = ptime(microsec_clock::local_time());
	test_law.run();
	stop = ptime(microsec_clock::local_time());
	std::cout << "Stop. Time elapsed: " << stop - start << endl;

	typedef ElementLessDefined
		<split_interval_map<int,int> >  TestLaw2T;
	LawValidater<TestLaw2T, RandomGentor> test_law2;
	test_law2.setTrialsCount(10000);

	std::cout << "2Start\n";
	start = ptime(microsec_clock::local_time());
	test_law2.run();
	stop = ptime(microsec_clock::local_time());
	std::cout << "Stop. Time elapsed: " << stop - start << endl;

	typedef ElementGreaterDefined
		<split_interval_map<int,int> >  TestLaw3T;
	LawValidater<TestLaw3T, RandomGentor> test_law3;
	test_law3.setTrialsCount(10000);

	std::cout << "3Start\n";
	start = ptime(microsec_clock::local_time());
	test_law3.run();
	stop = ptime(microsec_clock::local_time());
	std::cout << "Stop. Time elapsed: " << stop - start << endl;
	*/

	//-----------------------------------------------------------------------------
	int test_count = 10000;

	typedef InclusionEqualDefined
		<split_interval_map<int,int> >  TestLaw4T;
	LawValidater<TestLaw4T, RandomGentor> test_law4;
	test_law4.setTrialsCount(test_count);

	std::cout << "4Start\n";
	start = ptime(microsec_clock::local_time());
	test_law4.run();
	stop = ptime(microsec_clock::local_time());
	std::cout << "Stop. Time elapsed: " << stop - start << endl;

	typedef SubsetDefined
		<split_interval_map<int,int> >  TestLaw5T;
	LawValidater<TestLaw5T, RandomGentor> test_law5;
	test_law5.setTrialsCount(test_count);

	std::cout << "5Start\n";
	start = ptime(microsec_clock::local_time());
	test_law5.run();
	stop = ptime(microsec_clock::local_time());
	std::cout << "Stop. Time elapsed: " << stop - start << endl;

	typedef SupersetDefined
		<split_interval_map<int,int> >  TestLaw6T;
	LawValidater<TestLaw6T, RandomGentor> test_law6;
	test_law6.setTrialsCount(test_count);

	std::cout << "6Start\n";
	start = ptime(microsec_clock::local_time());
	test_law6.run();
	stop = ptime(microsec_clock::local_time());
	std::cout << "Stop. Time elapsed: " << stop - start << endl;

}



int main()
{
	test_LawValidater();
	return 0;
}
