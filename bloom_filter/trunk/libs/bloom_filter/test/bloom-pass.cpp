//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Alejandro Cabrera 2011.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bloom_filter for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Boost Bloom Filter" 1
#include <iostream>

#include <boost/bloom_filter/bloom.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

using boost::bloom_filter::bloom_filter;
using boost::bloom_filter::boost_hash;

BOOST_AUTO_TEST_CASE(defaultConstructor) {
  typedef boost::mpl::vector<
    boost_hash<int, 13>,
    boost_hash<int, 17>,
    boost_hash<int, 19> > BoostHashFunctions;
  
  bloom_filter<int, 8> bloom1;
  bloom_filter<int, 8, BoostHashFunctions> bloom2;
}

BOOST_AUTO_TEST_CASE(rangeConstructor) {
  int elems[5] = {1,2,3,4,5};
  bloom_filter<int, 8> bloom(elems, elems+5);

  BOOST_CHECK_EQUAL(bloom.count(), 5ul);
}

#ifndef BOOST_NO_0X_HDR_INITIALIZER_LIST
BOOST_AUTO_TEST_CASE(initListConstructor) {
  bloom_filter<int, 8> bloom = {1,2,3,4,5};

  BOOST_CHECK_EQUAL(bloom.count(), 5ul);
}
#endif

BOOST_AUTO_TEST_CASE(copyConstructor) {
  int elems[5] = {1,2,3,4,5};
  bloom_filter<int, 8> bloom1(elems, elems+5);
  bloom_filter<int, 8> bloom2(bloom1);

  BOOST_CHECK_EQUAL(bloom1.count(), bloom2.count());
}

BOOST_AUTO_TEST_CASE(assignment)
{
  bloom_filter<int, 8> bloom1;
  bloom_filter<int, 8> bloom2;

  for (size_t i = 0; i < 200; ++i) {
    bloom1.insert(i);
    BOOST_CHECK_EQUAL(bloom1.probably_contains(i), true);
  }

  bloom2 = bloom1;

  for (size_t i = 0; i < 200; ++i) {
    BOOST_CHECK_EQUAL(bloom2.probably_contains(i), true);
  }
}

BOOST_AUTO_TEST_CASE(bit_capacity) {
  bloom_filter<size_t, 8> bloom_8;
  bloom_filter<size_t, 256> bloom_256;
  bloom_filter<size_t, 2048> bloom_2048;
  
  BOOST_CHECK_EQUAL(bloom_8.bit_capacity(), 8ul);
  BOOST_CHECK_EQUAL(bloom_256.bit_capacity(), 256ul);
  BOOST_CHECK_EQUAL(bloom_2048.bit_capacity(), 2048ul);
}

BOOST_AUTO_TEST_CASE(empty) {
  bloom_filter<size_t, 8> bloom;
  
  BOOST_CHECK_EQUAL(bloom.empty(), true);
  bloom.insert(1);
  BOOST_CHECK_EQUAL(bloom.empty(), false);
  bloom.clear();
  BOOST_CHECK_EQUAL(bloom.empty(), true);
}

BOOST_AUTO_TEST_CASE(numHashFunctions) {
  bloom_filter<size_t, 8> bloom_3;
  bloom_filter<size_t, 8, boost::mpl::vector<
    boost_hash<size_t, 1>,
    boost_hash<size_t, 2> > > bloom_2;
  bloom_filter<size_t, 8, boost::mpl::vector<
    boost_hash<size_t, 1>,
    boost_hash<size_t, 2>,
    boost_hash<size_t, 3>,
    boost_hash<size_t, 4>,
    boost_hash<size_t, 5>,
    boost_hash<size_t, 6>,
    boost_hash<size_t, 7> > > bloom_7;

  BOOST_CHECK_EQUAL(bloom_3.num_hash_functions(), 1ul);
  BOOST_CHECK_EQUAL(bloom_2.num_hash_functions(), 2ul);
  BOOST_CHECK_EQUAL(bloom_7.num_hash_functions(), 7ul);
}

BOOST_AUTO_TEST_CASE(falsePositiveRate) {
  bloom_filter<size_t, 64> bloom;

  BOOST_CHECK_EQUAL(bloom.false_positive_rate(), 0.0);

  bloom.insert(1);
  BOOST_CHECK_CLOSE(bloom.false_positive_rate(), 0.015504, .01);

  bloom.insert(2);
  BOOST_CHECK_CLOSE(bloom.false_positive_rate(), 0.030768, .01);

  bloom.insert(3);
  BOOST_CHECK_CLOSE(bloom.false_positive_rate(), 0.045794, .01);

  bloom.insert(4);
  BOOST_CHECK_CLOSE(bloom.false_positive_rate(), 0.060588, .01);

  bloom.insert(5);
  BOOST_CHECK_CLOSE(bloom.false_positive_rate(), 0.075151, .01);

  bloom.insert(6);
  BOOST_CHECK_CLOSE(bloom.false_positive_rate(), 0.089491, .01);

  for (size_t i = 7; i < 5000; ++i)
    bloom.insert(i);
  
  BOOST_CHECK_GE(bloom.false_positive_rate(), 0.6);
  BOOST_CHECK_LE(bloom.false_positive_rate(), 1.0);
}

BOOST_AUTO_TEST_CASE(probably_contains) {
  bloom_filter<size_t, 8> bloom;

  bloom.insert(1);
  BOOST_CHECK_EQUAL(bloom.probably_contains(1), true);
  BOOST_CHECK_LE(bloom.count(), 3ul);
  BOOST_CHECK_GE(bloom.count(), 1ul);
}

BOOST_AUTO_TEST_CASE(doesNotContain) {
  bloom_filter<size_t, 8> bloom;

  BOOST_CHECK_EQUAL(bloom.probably_contains(1), false);
}

BOOST_AUTO_TEST_CASE(insertNoFalseNegatives) {
  bloom_filter<size_t, 2048> bloom;

  for (size_t i = 0; i < 100; ++i) {
    bloom.insert(i);
    BOOST_CHECK_EQUAL(bloom.probably_contains(i), true);
  }
}

BOOST_AUTO_TEST_CASE(rangeInsert) {
  int elems[5] = {1,2,3,4,5};
  bloom_filter<size_t, 8> bloom;

  bloom.insert(elems, elems+5);
  BOOST_CHECK_EQUAL(bloom.count(), 5ul);
}

BOOST_AUTO_TEST_CASE(clear) {
  bloom_filter<size_t, 8> bloom;

  for (size_t i = 0; i < 1000; ++i)
    bloom.insert(i);

  bloom.clear();
  BOOST_CHECK_EQUAL(bloom.probably_contains(1), false);
  BOOST_CHECK_EQUAL(bloom.count(), 0ul);
}

BOOST_AUTO_TEST_CASE(memberSwap) {
  size_t elems[5] = {1,2,3,4,5};
  bloom_filter<size_t, 8> bloom1(elems, elems+2);
  bloom_filter<size_t, 8> bloom2(elems+2, elems+5);

  bloom1.swap(bloom2);

  BOOST_CHECK_EQUAL(bloom1.count(), 3ul);
  BOOST_CHECK_EQUAL(bloom2.count(), 2ul);
}

BOOST_AUTO_TEST_CASE(testUnion) {
  bloom_filter<size_t, 32> bloom_1;
  bloom_filter<size_t, 32> bloom_2;
  bloom_filter<size_t, 32> bloom_union;

  for (size_t i = 0; i < 100; ++i)
    bloom_1.insert(i);

  for (size_t i = 100; i < 200; ++i)
    bloom_2.insert(i);

  bloom_union = bloom_1 | bloom_2;

  for (size_t i = 0; i < 200; ++i)
    BOOST_CHECK_EQUAL(bloom_union.probably_contains(i), true);
  BOOST_CHECK_GE(bloom_union.count(), bloom_1.count());
  BOOST_CHECK_GE(bloom_union.count(), bloom_2.count());
}

BOOST_AUTO_TEST_CASE(testUnionAssign) {
  bloom_filter<size_t, 32> bloom_1;
  bloom_filter<size_t, 32> bloom_union;

  for (size_t i = 0; i < 100; ++i) 
    bloom_1.insert(i);

  bloom_union |= bloom_1;

  for (size_t i = 0; i < 100; ++i)
    BOOST_CHECK_EQUAL(bloom_union.probably_contains(i), true);
  BOOST_CHECK_EQUAL(bloom_union.count(), bloom_1.count());
}

BOOST_AUTO_TEST_CASE(testIntersect) {
  bloom_filter<size_t, 32> bloom_1;
  bloom_filter<size_t, 32> bloom_2;
  bloom_filter<size_t, 32> bloom_intersect;

  // overlap at 100
  for (size_t i = 0; i < 101; ++i) 
    bloom_1.insert(i);
  
  for (size_t i = 100; i < 200; ++i) 
    bloom_2.insert(i);

  bloom_intersect = bloom_1 & bloom_2;

  BOOST_CHECK_LE(bloom_intersect.count(), bloom_1.count());
  BOOST_CHECK_LE(bloom_intersect.count(), bloom_2.count());
  BOOST_CHECK_EQUAL(bloom_intersect.probably_contains(100), true);
}

BOOST_AUTO_TEST_CASE(testIntersectAssign) {
  bloom_filter<size_t, 32> bloom_1;
  bloom_filter<size_t, 32> bloom_intersect;

  for (size_t i = 0; i < 100; ++i) 
    bloom_1.insert(i);
  
  bloom_intersect &= bloom_1;

  for (size_t i = 0; i < 100; ++i)
    BOOST_CHECK_EQUAL(bloom_intersect.probably_contains(i), false);
}

BOOST_AUTO_TEST_CASE(globalSwap) {
  size_t elems[5] = {1,2,3,4,5};
  bloom_filter<size_t, 8> bloom1(elems, elems+2);
  bloom_filter<size_t, 8> bloom2(elems+2, elems+5);

  swap(bloom1, bloom2);

  BOOST_CHECK_EQUAL(bloom1.count(), 3ul);
  BOOST_CHECK_EQUAL(bloom2.count(), 2ul);
}

/*
BOOST_AUTO_TEST_CASE(collisionBenchmark) {
  typedef boost::mpl::vector<
    OHash <size_t, 2>,
    OHash<size_t, 3>,
    OHash<size_t, 5>,
    OHash<size_t, 7>,
    OHash<size_t, 11>,
    OHash<size_t, 13>,
    OHash<size_t, 17>,
    OHash<size_t, 19>> EightHashFunctions_O;

  static const size_t INSERT_VAL = 100;
  static const size_t SEARCH_SPACE = 10000000;
  static const size_t FILTER_SIZE = 64; 
  size_t collisions = 0;
  bloom_filter<size_t, FILTER_SIZE, EightHashFunctions_O> bloom;

  std::cout << "bloom size " << bloom.size() << std::endl;
  bloom.insert(INSERT_VAL);
  for (size_t i = 0; i < SEARCH_SPACE; ++i) {
    if (bloom.probably_contains(i) && i != INSERT_VAL) ++collisions;
  }

  std::cout << collisions << " collisions" << std::endl;
  bloom.clear();
}
*/
