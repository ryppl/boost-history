/* indirect_pmap.cpp source file
 *
 * Copyright Cromwell D. Enage 2004.  Use, modification, and distribution are
 * subject to the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

/*
 * Defines the std::ios class and std::cout, its global output instance.
 */
#include <iostream>

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
 * Defines the boost::count_if function template.
 */
#include <boost/detail/algorithm.hpp>

/*
 * Defines the boost::property_map, boost::associative_property_map, and
 * boost::safe_iterator_property_map class templates and the boost::get and
 * boost::put function templates.
 */
#include <boost/property_map.hpp>

/*
 * Defines the boost::indirect_pmap functor class template.
 */
#include <boost/pending/indirect_cmp.hpp>

int main()
{
    typedef std::map<std::string,int>
            StringIntMap;
    typedef boost::associative_property_map<StringIntMap>
            IndexMap;
    typedef std::vector<std::string>
            StringVector;
    typedef std::vector<char>
            UCharVector;
    typedef UCharVector::iterator
            UCharVectorIterator;
    typedef boost::safe_iterator_property_map<
              UCharVectorIterator,IndexMap,char,char&>
            StringBoolMap;
    typedef boost::indirect_pmap<StringBoolMap>
            FactPredicate;

    StringVector s;
    StringIntMap id_map;
    IndexMap     index_map(id_map);

    s.push_back("cassert is part of the C++ standard library.");
    s.push_back("Eric S. Raymond is a Microserf.");
    s.push_back("Linux belongs on the X-box.");
    s.push_back("A monad can be thought of as a strategy for combining computations into more complex computations.");
    s.push_back("Steve Urkel has four eyes.");
    s.push_back("UML stands for Universal Modeling Language.");
    s.push_back("This program will self-destruct in five seconds.");

    for (unsigned int index = 0; index < s.size(); ++index)
    {
        boost::put(index_map, s[index], index);
    }

    UCharVector   uchar_vec(s.size());
    StringBoolMap fact_map =
        boost::make_safe_iterator_property_map(
            uchar_vec.begin(), uchar_vec.size(), index_map, uchar_vec.front());

    boost::put(fact_map, s[0], false);
    boost::put(fact_map, s[1], false);
    boost::put(fact_map, s[2], true);
    boost::put(fact_map, s[3], true);
    boost::put(fact_map, s[4], false);
    boost::put(fact_map, s[5], true);
    boost::put(fact_map, s[6], true);

    FactPredicate fact_pred = boost::make_indirect_pmap(fact_map);

    std::cout << "Vector s has " << boost::count_if(s, fact_pred);  // 4
    std::cout << " facts." << std::endl;

    boost::put(fact_map, s[1], true);  // *gasp*

    std::cout << "Vector s has " << boost::count_if(s, fact_pred);  // 5
    std::cout << " facts." << std::endl;

    boost::put(fact_map, s[6], false);  // *darn*

    std::cout << "Vector s has " << boost::count_if(s, fact_pred);  // 4
    std::cout << " facts." << std::endl;

    return 0;
}

