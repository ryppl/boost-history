/* mutable_queue_ct.cpp source file
 *
 * Copyright Cromwell D. Enage 2004.  Use, modification, and distribution are
 * subject to the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

/*
 * Defines the Boost Unit Test Framework.
 */
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test_framework.hpp>

/*
 * Defines the standard compare functor class templates (e.g. std::less).
 */
#include <functional>

/*
 * Defines the std::string class.
 */
#include <string>

/*
 * Defines the std::vector class template.
 */
#include <vector>

/*
 * Defines the std::map class template.
 */
#include <map>

/*
 * Defines the boost::property_map, boost::associative_property_map, and
 * boost::iterator_property_map class templates and the boost::get and
 * boost::put function templates.
 */
#include <boost/property_map.hpp>

/*
 * Defines the boost::indirect_cmp functor class template.
 */
#include <boost/pending/indirect_cmp.hpp>

/*
 * Defines the boost::mutable_queue class template.
 */
#include <boost/pending/mutable_queue.hpp>

using boost::unit_test_framework::test_suite;
using namespace boost;

template <typename StringVector, typename IndexMap>
void init(StringVector& s, IndexMap index_map)
{
    s.push_back("Rocko Alboa");
    s.push_back("Sergeant Tim Allen");
    s.push_back("Reggie B.");
    s.push_back("Stanford Blum");
    s.push_back("Solomon Dominguez");
    s.push_back("Theon E. Mitty");
    s.push_back("Dick Wadsworth");

    for (int index = 0; index < s.size(); ++index)
    {
        boost::put(index_map, s[index], index);
    }
}

template <typename StringVector, typename PriorityMap>
void init_priorities(StringVector& s, PriorityMap priority_map)
{
    boost::put(priority_map, s[0], 65);
    boost::put(priority_map, s[1], 21);
    boost::put(priority_map, s[2], 32);
    boost::put(priority_map, s[3], 13);
    boost::put(priority_map, s[4], 24);
    boost::put(priority_map, s[5], 16);
    boost::put(priority_map, s[6], 68);
}

template <typename StringVector, typename PriorityQueue, typename PriorityMap>
void test(StringVector& s, PriorityQueue& Q, PriorityMap priority_map)
{
    BOOST_CHECK_MESSAGE(
        Q.size() == 7,
        "size() should return 7, not " << Q.size());
    BOOST_CHECK_MESSAGE(
        Q.top() == s[3],
        "top() should return Stanford Blum, not " << Q.top());
    Q.pop();
    boost::put(priority_map, s[0], 14);
    Q.update(s[0]);
    BOOST_CHECK_MESSAGE(
        Q.top() == s[0],
        "top() should return Rocko Alboa, not " << Q.top());
    Q.pop();
    boost::put(priority_map, s[5], 53);
    Q.update(s[5]);
    BOOST_CHECK_MESSAGE(
        Q.top() == s[1],
        "top() should return Sergeant Tim Allen, not " << Q.top());
    Q.pop();
    BOOST_CHECK_MESSAGE(
        Q.top() == s[4],
        "top() should return Solomon Dominguez, not " << Q.top());
    Q.pop();
    BOOST_CHECK_MESSAGE(
        Q.top() == s[2],
        "top() should return Reggie B., not " << Q.top());
    Q.pop();
    BOOST_CHECK_MESSAGE(
        Q.top() == s[5],
        "top() should return Theon E. Mitty, not " << Q.top());
    Q.pop();
    BOOST_CHECK_MESSAGE(
        Q.top() == s[6],
        "top() should return Dick Wadsworth, not " << Q.top());
    Q.pop();

    while (!Q.empty())
    {
        BOOST_ERROR("Q should be empty, not have top element " << Q.top());
        Q.pop();
    }
}

void test_case_1()
{
    typedef std::map<std::string,int>
            StringIntMap;
    typedef boost::associative_property_map<StringIntMap>
            IndexMap;
    typedef std::vector<std::string>
            StringVector;
    typedef std::vector<int>
            IntVector;
    typedef IntVector::iterator
            IntVectorIterator;
    typedef boost::iterator_property_map<IntVectorIterator,IndexMap,int,int&>
            PriorityMap;
    typedef std::less<int>
            IntCompare;
    typedef boost::indirect_cmp<PriorityMap,IntCompare>
            PriorityCompare;
    typedef boost::mutable_queue<
              std::string,StringVector,PriorityCompare,IndexMap>
            MutableQueue;

    StringVector s;
    StringIntMap id_map;
    IndexMap     index_map(id_map);

    init(s, index_map);

    IntVector   int_vec(s.size(), 0);
    PriorityMap priority_map =
        boost::make_iterator_property_map(int_vec.begin(), index_map, 0);

    init_priorities(s, priority_map);

    PriorityCompare priority_compare(priority_map, IntCompare());
    MutableQueue    Q1(s.size(), priority_compare, index_map);

    for (int index = 0; index < s.size(); ++index)
    {
        Q1.push(s[index]);
    }

    BOOST_CHECKPOINT("This mutable_queue was empty but had items pushed in.");
    test(s, Q1, priority_map);
}

void test_case_2()
{
    typedef std::map<std::string,int>
            StringIntMap;
    typedef boost::associative_property_map<StringIntMap>
            IndexMap;
    typedef std::vector<std::string>
            StringVector;
    typedef std::vector<int>
            IntVector;
    typedef IntVector::iterator
            IntVectorIterator;
    typedef boost::iterator_property_map<IntVectorIterator,IndexMap,int,int&>
            PriorityMap;
    typedef std::less<int>
            IntCompare;
    typedef boost::indirect_cmp<PriorityMap,IntCompare>
            PriorityCompare;
    typedef boost::mutable_queue<
              std::string,StringVector,PriorityCompare,IndexMap>
            MutableQueue;

    StringVector s;
    StringIntMap id_map;
    IndexMap     index_map(id_map);

    init(s, index_map);

    IntVector   int_vec(s.size(), 0);
    PriorityMap priority_map =
        boost::make_iterator_property_map(int_vec.begin(), index_map, 0);

    init_priorities(s, priority_map);

    PriorityCompare priority_compare(priority_map, IntCompare());
    MutableQueue    Q2(s.begin(), s.end(), priority_compare, index_map);

    BOOST_CHECKPOINT("This mutable_queue was initalized to store the items.");
    test(s, Q2, priority_map);
}

test_suite* init_unit_test_suite(int argc, char** argv)
{
    test_suite* test = BOOST_TEST_SUITE("mutable_queue constructor unit test");

    test->add(BOOST_TEST_CASE(&test_case_1));
    test->add(BOOST_TEST_CASE(&test_case_2));

    return test;
}

