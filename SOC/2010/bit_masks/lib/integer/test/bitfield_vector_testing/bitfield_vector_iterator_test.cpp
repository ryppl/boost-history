//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_vector.hpp>
#include "test_utility.hpp"

using namespace boost::detail;

/** Test suite for all 4 iterator types of a bitfield_vector.
 *  iterator
 *  const_iterator
 *  reverse_iterator
 *  const_reverse_iterator
 */
int main() {
    // bf_vector_iterator
    {
        typedef bf_vector_iterator<unsigned int, 4>             iter_1;

        // default constructor
        iter_1 i1;
        BOOST_TEST(!i1);

        storage_t storage[20];
        std::memset(storage,0,20);

        // 2 parameter constructor
        iter_1 i2(storage,0);
        BOOST_TEST(i2);
        // copy constructor
        iter_1 i3(i2);
        BOOST_TEST(i3 == i2);
        ++i2;

        BOOST_TEST(i3 < i2);
        BOOST_TEST(i3 != i2);
        *i3 = 4;
        --i2;
        
        BOOST_TEST(*i2 == 4);
        i2++;
        BOOST_TEST(*i2 == 0);
        i2 -= 1;
        BOOST_TEST(i2 == i3);

        i2+=2;
        i2--;
        i3++;
        BOOST_TEST(i3 == i2);
        iter_1 i4(storage,0);
        iter_1 i5(storage,0);
        BOOST_TEST( i5 - i4 == 0);
        i5+=4;
        BOOST_TEST( i5 - i4 == 4);
    }
    {
        typedef const_bf_vector_iterator<unsigned int, 4>               iter_1;
        typedef bf_vector_iterator<unsigned int, 4>                     iter_2;
        iter_1 i1;
        BOOST_TEST(!i1);

        storage_t storage[20];
        std::memset(storage,0,20);

        // 2 parameter constructor
        iter_1 i2(storage,0);
        BOOST_TEST(i2);
        // copy constructor
        iter_1 i3(i2);
        BOOST_TEST(i3 == i2);
        ++i2;

        BOOST_TEST(i3 < i2);
        BOOST_TEST(i3 != i2);
        iter_2 non_const_iter(storage,0);
        *non_const_iter = 4;
        --i2;
        
        BOOST_TEST(*i2 == 4);
        i2++;
        BOOST_TEST(*i2 == 0);
        i2 -= 1;
        BOOST_TEST(i2 == i3);

        i2+=2;
        i2--;
        i3++;
        BOOST_TEST(i3 == i2);
        iter_2 i4(storage,0);
        iter_2 i5(storage,0);
        BOOST_TEST( i5 - i4 == 0);
        i5+=4;
        BOOST_TEST( i5 - i4 == 4);
    }
    {
        typedef bf_vector_reverse_iterator<unsigned int, 4>  iter_1;
        iter_1 i1;
        BOOST_TEST(!i1);

        storage_t storage[20];
        std::memset(storage,0,20);

        // 2 parameter constructor
        iter_1 i2(storage + 19, 0);
        BOOST_TEST(i2);
        // copy constructor
        iter_1 i3(i2);
        BOOST_TEST(i3 == i2);
        ++i2;

        BOOST_TEST(i3 < i2);
        BOOST_TEST(i3 != i2);

        *i2 = 4;
        --i2;
        
        BOOST_TEST(*i2 == 0);
        i2++;
        BOOST_TEST(*i2 == 4);

        i2 -= 1;
        BOOST_TEST(i2 == i3);

        i2+=2;
        i2--;
        i3++;
        BOOST_TEST(i3 == i2);
        iter_1 i4(storage,0);
        iter_1 i5(storage,0);
        BOOST_TEST( i5 - i4 == 0);
        i5+=4;
        BOOST_TEST( i5 - i4 == 4);
    }

    {
        typedef bf_vector_reverse_iterator<unsigned int, 4>  iter_2;
        typedef const_bf_vector_reverse_iterator<unsigned int, 4>  iter_1;
        iter_1 i1;
        BOOST_TEST(!i1);

        storage_t storage[20];
        std::memset(storage,0,20);

        // 2 parameter constructor
        iter_1 i2(storage + 19, 0);
        BOOST_TEST(i2);
        // copy constructor
        iter_1 i3(i2);
        BOOST_TEST(i3 == i2);
        ++i2;

        BOOST_TEST(i3 < i2);
        BOOST_TEST(i3 != i2);
        iter_2 non_const_iter(storage,0);
        *non_const_iter = 4;
        iter_1 const_iter(storage,0);
        BOOST_TEST(*const_iter == 4);

        --i2;
        
        BOOST_TEST(*i2 == 0);
        i2++;

        i2 -= 1;
        BOOST_TEST(i2 == i3);

        i2+=2;
        i2--;
        i3++;
        BOOST_TEST(i3 == i2);
        iter_1 i4(storage,0);
        iter_1 i5(storage,0);
        BOOST_TEST( i5 - i4 == 0);
        i5+=4;
        BOOST_TEST( i5 - i4 == 4);
    }
    return boost::report_errors();
}
