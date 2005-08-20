/* priority_queue.cpp source file
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
 * Defines the std::sort function template.
 */
#include <algorithm>

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

/*
 * Defines the boost::fibonacci_heap class template.
 */
#include <boost/pending/fibonacci_heap.hpp>

/*
 * Defines the boost::mt19937 random priority generator class.
 */
#include <boost/random/mersenne_twister.hpp>

/*
 * Defines the boost::uniform_int class template, to be used as a random-number
 * distribution.
 */
#include <boost/random/uniform_int.hpp>

/*
 * Defines the boost::variate_generator class template, to be used as the
 * random index generator.
 */
#include <boost/random/variate_generator.hpp>

using boost::unit_test_framework::test_suite;
using namespace boost;

template <typename StringVector, typename IndexMap>
void init(StringVector& s, IndexMap index_map)
{
    s.push_back("0");
    s.push_back("1");
    s.push_back("2");
    s.push_back("3");
    s.push_back("4");
    s.push_back("5");
    s.push_back("6");
    s.push_back("7");
    s.push_back("8");
    s.push_back("9");
    s.push_back("-");
    s.push_back("=");
    s.push_back("!");
    s.push_back("@");
    s.push_back("#");
    s.push_back("$");
    s.push_back("%");
    s.push_back("^");
    s.push_back("&");
    s.push_back("*");
    s.push_back("(");
    s.push_back(")");
    s.push_back("_");
    s.push_back("+");
    s.push_back("A");
    s.push_back("B");
    s.push_back("C");
    s.push_back("D");
    s.push_back("E");
    s.push_back("F");
    s.push_back("G");
    s.push_back("H");
    s.push_back("I");
    s.push_back("J");
    s.push_back("K");
    s.push_back("L");
    s.push_back("M");
    s.push_back("N");
    s.push_back("O");
    s.push_back("P");
    s.push_back("Q");
    s.push_back("R");
    s.push_back("S");
    s.push_back("T");
    s.push_back("U");
    s.push_back("V");
    s.push_back("W");
    s.push_back("X");
    s.push_back("Y");
    s.push_back("Z");
    s.push_back("Aa");
    s.push_back("Bb");
    s.push_back("Cc");
    s.push_back("Dd");
    s.push_back("Ee");
    s.push_back("Ff");
    s.push_back("Gg");
    s.push_back("Hh");
    s.push_back("Ii");
    s.push_back("Jj");
    s.push_back("Kk");
    s.push_back("Ll");
    s.push_back("Mm");
    s.push_back("Nn");
    s.push_back("Oo");
    s.push_back("Pp");
    s.push_back("Qq");
    s.push_back("Rr");
    s.push_back("Ss");
    s.push_back("Tt");
    s.push_back("Uu");
    s.push_back("Vv");
    s.push_back("Ww");
    s.push_back("Xx");
    s.push_back("Yy");
    s.push_back("Zz");
    s.push_back("a");
    s.push_back("b");
    s.push_back("c");
    s.push_back("d");
    s.push_back("e");
    s.push_back("f");
    s.push_back("g");
    s.push_back("h");
    s.push_back("i");
    s.push_back("j");
    s.push_back("k");
    s.push_back("l");
    s.push_back("m");
    s.push_back("n");
    s.push_back("o");
    s.push_back("p");
    s.push_back("q");
    s.push_back("r");
    s.push_back("s");
    s.push_back("t");
    s.push_back("u");
    s.push_back("v");
    s.push_back("w");
    s.push_back("x");
    s.push_back("y");
    s.push_back("z");
    s.push_back("aA");
    s.push_back("bB");
    s.push_back("cC");
    s.push_back("dD");
    s.push_back("eE");
    s.push_back("fF");
    s.push_back("gG");
    s.push_back("hH");
    s.push_back("iI");
    s.push_back("jJ");
    s.push_back("kK");
    s.push_back("lL");
    s.push_back("mM");
    s.push_back("nN");
    s.push_back("oO");
    s.push_back("pP");
    s.push_back("qQ");
    s.push_back("rR");
    s.push_back("sS");
    s.push_back("tT");
    s.push_back("uU");
    s.push_back("vV");
    s.push_back("wW");
    s.push_back("xX");
    s.push_back("yY");
    s.push_back("zZ");

    for (unsigned int index = 0; index < s.size(); ++index)
    {
        put(index_map, s[index], index);
    }
}

template <typename StringVector, typename PriorityQueue, typename Compare,
          typename PriorityMap, typename RandomNumberGenerator,
          typename RandomPriorityGenerator>
void test_priority_queue(StringVector& s, PriorityQueue& Q, Compare& compare,
                         PriorityMap priority_map, RandomNumberGenerator& rng,
                         RandomPriorityGenerator& rpg)
{
    StringVector control;

    for (unsigned int index = 0; index < s.size(); ++index)
    {
        control.push_back(s[index]);
        put(priority_map, s[index], rpg());
        Q.push(s[index]);
    }

    variate_generator<RandomNumberGenerator&,uniform_int<unsigned int> >
        rig(rng, uniform_int<unsigned int>(0, s.size() - 1));

    for (unsigned int n = 0; n < s.size() >> 1; ++n)
    {
        unsigned int index = rig();

        put(priority_map, s[index], rpg());
        Q.update(s[index]);
    }

    std::sort(control.begin(), control.end(), compare);

    for (unsigned int index = 0; index < control.size(); ++index)
    {
        if (Q.empty())
        {
            BOOST_ERROR("Q should not be empty.  " <<
                        "top() should be " << control[index]);
        }
        else
        {
            BOOST_CHECK_MESSAGE(
                Q.top() == control[index],
                "Q.top()=<" << Q.top() << "," << get(priority_map, Q.top())
                            << ">, control[" << index << "]=<" << control[index]
                            << "," << get(priority_map, control[index]) << ">");
            Q.pop();
        }
    }
}

void mutable_queue_test_case()
{
    typedef std::map<std::string,int>
            StringIntMap;
    typedef associative_property_map<StringIntMap>
            IndexMap;
    typedef std::vector<std::string>
            StringVector;
    typedef std::vector<int>
            IntVector;
    typedef IntVector::iterator
            IntVectorIterator;
    typedef iterator_property_map<IntVectorIterator,IndexMap,int,int&>
            PriorityMap;
    typedef std::less<int>
            IntCompare;
    typedef indirect_cmp<PriorityMap,IntCompare>
            PriorityCompare;
    typedef mutable_queue<std::string,StringVector,PriorityCompare,IndexMap>
            MutableQueue;
    typedef uniform_int<int>
            PriorityDistribution;
    typedef variate_generator<mt19937&,PriorityDistribution>
            RPG;

    StringVector s;
    StringIntMap id_map;
    IndexMap     index_map(id_map);

    init(s, index_map);

    IntVector       int_vec(s.size(), 0);
    PriorityMap     priority_map =
        make_iterator_property_map(int_vec.begin(), index_map, 0);
    mt19937         rng;
    RPG             rpg(rng, PriorityDistribution(-32767, 32768));
    PriorityCompare priority_compare(priority_map, IntCompare());
    MutableQueue    Q(s.size(), priority_compare, index_map);

    BOOST_MESSAGE("mutable_queue");
    test_priority_queue(s, Q, priority_compare, priority_map, rng, rpg);
}

void fibonacci_heap_test_case()
{
    typedef std::map<std::string,int>
            StringIntMap;
    typedef associative_property_map<StringIntMap>
            IndexMap;
    typedef std::vector<std::string>
            StringVector;
    typedef std::vector<int>
            IntVector;
    typedef IntVector::iterator
            IntVectorIterator;
    typedef iterator_property_map<IntVectorIterator,IndexMap,int,int&>
            PriorityMap;
    typedef std::less<int>
            IntCompare;
    typedef indirect_cmp<PriorityMap,IntCompare>
            PriorityCompare;
    typedef fibonacci_heap<std::string,PriorityCompare,IndexMap>
            FibonacciHeap;
    typedef uniform_int<int>
            PriorityDistribution;
    typedef variate_generator<mt19937&,PriorityDistribution>
            RPG;

    StringVector s;
    StringIntMap id_map;
    IndexMap     index_map(id_map);

    init(s, index_map);

    IntVector       int_vec(s.size(), 0);
    PriorityMap     priority_map =
        make_iterator_property_map(int_vec.begin(), index_map, 0);
    mt19937         rng;
    RPG             rpg(rng, PriorityDistribution(-32767, 32768));
    PriorityCompare priority_compare(priority_map, IntCompare());
    FibonacciHeap   Q(s.size(), priority_compare, index_map);

    BOOST_MESSAGE("fibonacci_heap");
    test_priority_queue(s, Q, priority_compare, priority_map, rng, rpg);
}

test_suite* init_unit_test_suite(int argc, char** argv)
{
    test_suite* test = BOOST_TEST_SUITE("priority queue unit test");

    test->add(BOOST_TEST_CASE(&mutable_queue_test_case));
    test->add(BOOST_TEST_CASE(&fibonacci_heap_test_case));

    return test;
}

