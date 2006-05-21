/* priority_queue.cpp source file
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
 * Defines the boost::mutable_queue class template.
 */
#include <boost/pending/mutable_queue.hpp>

/*
 * Defines the boost::fibonacci_heap class template.
 */
#include <boost/pending/fibonacci_heap.hpp>

/*
 * Defines the boost::associative_property_map and boost::iterator_property_map
 * class templates and the boost::get and boost::put function templates.
 */
#include <boost/property_map.hpp>

template <typename String, typename IndexMap, typename PriorityMap>
void cout_stats(String owner, IndexMap index_map, PriorityMap priority_map)
{
    std::cout << boost::get(index_map, owner) << ". " << owner;
    std::cout << "'s priority is " << boost::get(priority_map, owner);
    std::cout << "." << std::endl;
}

template <typename StringVector, typename IndexMap, typename PriorityMap>
void init_priorities(StringVector& s, IndexMap index_map,
                     PriorityMap priority_map)
{
    std::cout << "Mapping priorities:" << std::endl;
    boost::put(priority_map, s[0], 65);
    boost::put(priority_map, s[1], 21);
    boost::put(priority_map, s[2], 32);
    boost::put(priority_map, s[3], 13);
    boost::put(priority_map, s[4], 24);
    boost::put(priority_map, s[5], 16);
    boost::put(priority_map, s[6], 68);

    for (int index = 0; index < s.size(); ++index)
    {
        cout_stats(s[index], index_map, priority_map);
    }

    std::cout << std::endl;
}

template <typename StringVector, typename PriorityQueue, typename IndexMap,
          typename PriorityMap>
void cout_example(StringVector& s, PriorityQueue& Q, IndexMap index_map,
                  PriorityMap priority_map)
{
    //3. Stanford Blum's priority is 13.
    std::cout << "Popping:" << std::endl;
    cout_stats(Q.top(), index_map, priority_map);
    Q.pop();

    /*
     * Invalidates Q.
     */
    boost::put(priority_map, s[0], 14);

    /*
     * Revalidate Q.
     */
    std::cout << "Updating." << std::endl;
    Q.update(s[0]);

    //0. Rocko Alboa's priority is 14.
    cout_stats(Q.top(), index_map, priority_map);
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

    //1. Sergeant Tim Allen's priority is 21.
    //4. Solomon Dominguez's priority is 24.
    //2. Reggie B.'s priority is 32.
    //5. Theon E. Mitty's priority is 53.
    //6. Dick Wadsworth's priority is 68.
    std::cout << "Popping:" << std::endl;
    while (!Q.empty())
    {
        cout_stats(Q.top(), index_map, priority_map);
        Q.pop();
    }
}

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
    typedef boost::mutable_queue<
              std::string,StringVector,PriorityCompare,IndexMap>
            MutableQueue;
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

    IntVector       int_vec(s.size(), 0);
    PriorityMap     priority_map =
        boost::make_iterator_property_map(int_vec.begin(), index_map, 0);

    init_priorities(s, index_map, priority_map);

    PriorityCompare priority_compare(priority_map, IntCompare());
    MutableQueue    Q1(s.size(), priority_compare, index_map);

    for (int index = 0; index < s.size(); ++index)
    {
        Q1.push(s[index]);
    }

    std::cout << "This mutable_queue was empty but had items pushed in.";
    std::cout << std::endl;
    cout_example(s, Q1, index_map, priority_map);
    std::cout << std::endl;

    init_priorities(s, index_map, priority_map);

    MutableQueue Q2(s.begin(), s.end(), priority_compare, index_map);

    std::cout << "This mutable_queue was initalized to store the items.";
    std::cout << std::endl;
    cout_example(s, Q2, index_map, priority_map);
    std::cout << std::endl;

    init_priorities(s, index_map, priority_map);

    FibonacciHeap Q3(s.size(), priority_compare, index_map);

    for (int index = 0; index < s.size(); ++index)
    {
        Q3.push(s[index]);
    }

    std::cout << "This fibonacci_heap was empty but had items pushed in.";
    std::cout << std::endl;
    cout_example(s, Q3, index_map, priority_map);

    return 0;
}

