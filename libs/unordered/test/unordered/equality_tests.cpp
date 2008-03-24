
// Copyright 2008 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include "../helpers/test.hpp"
#include "../objects/test.hpp"
#include "../helpers/random_values.hpp"
#include <list>

namespace equality_tests {
    test::seed_t seed(78634);

    template<class T>
    struct container_holder {
        test::random_values<T> values;
        T container;

        container_holder()
            : values(0), container()
        {}

        container_holder(int count, test::random_generator const& generator)
            : values(count, generator),
            container(values.begin(), values.end())
        {
        }
    };

    template <class T>
    void equality_tests1(T*, test::random_generator generator
            = test::default_generator)
    {
        const std::size_t num_containers = 100;

        std::list<container_holder<T> > containers;
        container_holder<T> empty;
        containers.push_back(container_holder<T>());
        BOOST_TEST(empty.values == empty.values);
        BOOST_TEST(empty.values == containers.back().values);
        BOOST_TEST(empty.container == empty.container);
        BOOST_TEST(empty.container == containers.back().container);

        container_holder<T> one(1, generator);
        containers.push_back(one);
        
        BOOST_TEST(empty.values != one.values);
        BOOST_TEST(one.values == one.values);
        BOOST_TEST(one.values == containers.back().values);
        BOOST_TEST(empty.container != one.container);
        BOOST_TEST(one.container == one.container);
        BOOST_TEST(one.container == containers.back().container);

        container_holder<T> hundred(100, generator);
        container_holder<T> hundred2(100, generator);
        BOOST_TEST(hundred.values != hundred2.values);

        containers.push_back(hundred);
        containers.push_back(hundred2);
        
        BOOST_TEST(empty.values != hundred.values);
        BOOST_TEST(one.values != hundred.values);
        BOOST_TEST(hundred.values == hundred.values);
        BOOST_TEST(hundred2.values != hundred.values);
        BOOST_TEST(hundred.values == hundred.values);
        BOOST_TEST(hundred2.values == containers.back().values);

        BOOST_TEST(empty.container != hundred.container);
        BOOST_TEST(one.container != hundred.container);
        BOOST_TEST(hundred.container == hundred.container);
        BOOST_TEST(hundred2.container != hundred.container);
        BOOST_TEST(hundred.container == hundred.container);
        BOOST_TEST(hundred2.container == containers.back().container);

        for(std::size_t i = containers.size(); i < num_containers; ++i) {
            using namespace std;
            containers.push_back(
                container_holder<T>(rand() % 150, generator));
        }

        std::size_t count1, count2;
        typename std::list<container_holder<T> >::const_iterator it1, it2;
        for(it1 = containers.begin(), count1 = 0; it1 != containers.end(); ++it1, ++count1) {
            for(it2 = it1, count2 = count1; it2 != containers.end(); ++it2, ++count2) {
                if(it1 == it2) {
                    if(it1->container != it2->container ||
                        !(it1->container == it2->container))
                    {
                        std::cerr<<"Container "<<count1<<":\n";
                        BOOST_ERROR("Not equal to itself!");
                    }
                }
                else if(it1->values == it2->values) {
                    if(it1->container != it2->container ||
                        !(it1->container == it2->container))
                    {
                        std::cerr<<"Containers "<<count1<<","<<count2<<":\n";
                        BOOST_ERROR("Should be equal");
                    }
                }
                else {
                    if(it1->container == it2->container ||
                        !(it1->container != it2->container))
                    {
                        std::cerr<<"Containers "<<count1<<","<<count2<<":\n";
                        BOOST_ERROR("Should not be equal");
                    }
                }
            }
        }
    }

    boost::unordered_set<test::object, test::hash, test::equal_to, test::allocator<test::object> >* test_set;
    boost::unordered_multiset<test::object, test::hash, test::equal_to, test::allocator<test::object> >* test_multiset;
    boost::unordered_map<test::object, test::object, test::hash, test::equal_to, test::allocator<test::object> >* test_map;
    boost::unordered_multimap<test::object, test::object, test::hash, test::equal_to, test::allocator<test::object> >* test_multimap;

    using test::default_generator;
    using test::generate_collisions;

    UNORDERED_TEST(equality_tests1,
        ((test_set)(test_multiset)(test_map)(test_multimap))
        ((default_generator)(generate_collisions))
    )
}

RUN_TESTS()
