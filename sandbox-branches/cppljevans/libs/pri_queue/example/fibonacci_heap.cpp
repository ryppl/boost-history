/* fibonacci_heap.cpp source file
 *
 * Copyright Cromwell D. Enage 2004
 *
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without fee provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * Cromwell D. Enage makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 */

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
 * Defines the std::ios class and std::cout, its global output instance.
 */
#include <iostream>

/*
 * Defines the boost::indirect_cmp functor class template.
 */
#include <boost/pending/indirect_cmp.hpp>

/*
 * Defines the boost::fibonacci_heap class template.
 */
#include <boost/pending/fibonacci_heap.hpp>

/*
 * Defines the boost::associative_property_map and boost::iterator_property_map
 * class templates and the boost::get and boost::put function templates.
 */
#include <boost/property_map.hpp>

int main()
{
    typedef std::map<std::string, int>
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
    typedef boost::fibonacci_heap<std::string,PriorityCompare,IndexMap>
            FibonacciHeap;

    StringVector s;

    s.push_back("Rocko Alboa");
    s.push_back("Sergeant Tim Allen");
    s.push_back("Reggie B.");
    s.push_back("Stanford Blum");
    s.push_back("Solomon Dominguez");
    s.push_back("Theon E. Mitty");
    s.push_back("Dick Wadsworth");

    StringIntMap id_map;
    IndexMap     index_map(id_map);

    for (int index = 0; index < s.size(); ++index)
    {
        boost::put(index_map, s[index], index);
    }

    IntVector   int_vec(s.size(), 0);
    PriorityMap priority_map =
        boost::make_iterator_property_map(int_vec.begin(), index_map, 0);

    boost::put(priority_map, s[0], 65);
    boost::put(priority_map, s[1], 21);
    boost::put(priority_map, s[2], 32);
    boost::put(priority_map, s[3], 13);
    boost::put(priority_map, s[4], 24);
    boost::put(priority_map, s[5], 16);
    boost::put(priority_map, s[6], 68);

    PriorityCompare priority_compare(priority_map, IntCompare());
    FibonacciHeap   Q(s.size(), priority_compare, index_map);

    for (int index = 0; index < s.size(); ++index)
    {
        Q.push(s[index]);
        std::cout << "Q = ";
        Q.print(std::cout);
        std::cout << std::endl;
    }

    /*
     * Invalidates Q.
     */
    boost::put(priority_map, s[0], 14);

    /*
     * Revalidate Q.
     */
    std::cout << "Updating." << std::endl;
    Q.update(s[0]);

    std::cout << "Q = ";
    Q.print(std::cout);
    std::cout << std::endl;

    Q.pop();

    /*
     * Invalidates Q.
     */
    boost::put(priority_map, s[5], 53);

    /*
     * Revalidate Q.
     */
    std::cout << "Updating." << std::endl;
    Q.update(s[5]);

    std::cout << "Q = ";
    Q.print(std::cout);
    std::cout << std::endl;

    std::cout << "Popping:" << std::endl;
    while (!Q.empty())
    {
        Q.pop();
        std::cout << "Q = ";
        Q.print(std::cout);
        std::cout << std::endl;
    }

    return 0;
}

