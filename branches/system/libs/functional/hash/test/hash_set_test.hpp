
// Copyright 2005-2007 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CONTAINER_TYPE)
#error "CONTAINER_TYPE not defined"
#else

#include <boost/preprocessor/cat.hpp>

#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable:4245) // signed/unsigned mismatch
#endif

namespace BOOST_PP_CAT(CONTAINER_TYPE, _tests)
{
    template <class T>
    void integer_tests(T* = 0)
    {
        const int number_of_containers = 12;
        T containers[number_of_containers];

        for(int i = 0; i < 5; ++i) {
            for(int j = 0; j < i; ++j)
                containers[i].insert(0);
        }

        containers[6].insert(1);
        containers[7].insert(1);
        containers[7].insert(1);
        containers[8].insert(-1);
        containers[9].insert(-1);
        containers[9].insert(-1);
        containers[10].insert(-1);
        containers[10].insert(1);
        containers[11].insert(1);
        containers[11].insert(2);
        containers[11].insert(3);
        containers[11].insert(4);
        containers[11].insert(5);

        HASH_NAMESPACE::hash<T> hasher;

        for(int i2 = 0; i2 < number_of_containers; ++i2) {
            BOOST_TEST(hasher(containers[i2]) == hasher(containers[i2]));

            BOOST_TEST(hasher(containers[i2]) ==
                    HASH_NAMESPACE::hash_value(containers[i2]));

            BOOST_TEST(hasher(containers[i2])
                    == HASH_NAMESPACE::hash_range(
                        containers[i2].begin(), containers[i2].end()));

            for(int j2 = i2 + 1; j2 < number_of_containers; ++j2) {
                BOOST_TEST(
                        (containers[i2] == containers[j2]) ==
                        (hasher(containers[i2]) == hasher(containers[j2]))
                        );
            }
        }
    }

    void BOOST_PP_CAT(CONTAINER_TYPE, _hash_integer_tests())
    {
        integer_tests((CONTAINER_TYPE<char>*) 0);
        integer_tests((CONTAINER_TYPE<int>*) 0);
        integer_tests((CONTAINER_TYPE<unsigned long>*) 0);
        integer_tests((CONTAINER_TYPE<double>*) 0);
    }
}

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#undef CONTAINER_TYPE
#endif
