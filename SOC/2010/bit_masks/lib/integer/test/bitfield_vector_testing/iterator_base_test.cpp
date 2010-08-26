//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_vector.hpp>
#include "test_utility.hpp"

using namespace boost::detail;

typedef bitfield_vector_iterator_base<unsigned int, 3>  test_type_1;
typedef bitfield_vector_iterator_base<int, 20>          test_type_2;
typedef bitfield_vector_iterator_base<int, 8>           test_type_3;

int main() {
    // iterator base
    
    // default constructor.
    {
        test_type_1 t1;
        test_type_2 t2;
        test_type_3 t3;

        BOOST_TEST( t1.m_ptr == 0);
        BOOST_TEST( t2.m_ptr == 0);
        BOOST_TEST( t3.m_ptr == 0);

        BOOST_TEST( t1.m_bit_offset == 0);
        BOOST_TEST( t2.m_bit_offset == 0);
        BOOST_TEST( t3.m_bit_offset == 0);
    }

    // testing 2 param constructor
    {
        storage_t storage[20];
        test_type_1 t1(storage,0);
        test_type_2 t2(storage,2);
        test_type_3 t3(storage,3);


        BOOST_TEST( t1.m_ptr == storage);
        BOOST_TEST( t2.m_ptr == storage);
        BOOST_TEST( t3.m_ptr == storage);

        BOOST_TEST( t1.m_bit_offset == 0);
        BOOST_TEST( t2.m_bit_offset == 2);
        BOOST_TEST( t3.m_bit_offset == 3);
    }

    // testing copy constructor.
    {
        storage_t storage[20];
        test_type_1 t1(storage,0);
        test_type_2 t2(storage,2);
        test_type_3 t3(storage,3);

        test_type_1 t4(t1);
        BOOST_TEST( t4.m_ptr == t1.m_ptr);
        BOOST_TEST( t4.m_bit_offset == t1.m_bit_offset);
    }
    
    // Testing constructor over a normal "non-const proxy_reference_type"
    {
        storage_t storage[20];
        test_type_1::proxy_ref_type r1(storage,0);
        test_type_1 t1(r1);
        BOOST_TEST( t1.m_ptr == storage);
        BOOST_TEST( t1.m_bit_offset == 0);

    }

    // testing next
    {
        // test type 1
        storage_t storage[20];
        storage_ptr_t ptr = storage;
        test_type_1 t1(storage, 0);
        t1.next();
        BOOST_TEST( t1.m_ptr == storage);
        BOOST_TEST( t1.m_bit_offset == 3);
        t1.next();
        BOOST_TEST( t1.m_ptr == storage);
        BOOST_TEST( t1.m_bit_offset == 6);
        ++ptr;
        t1.next();
        BOOST_TEST( t1.m_ptr == ptr);
        BOOST_TEST( t1.m_bit_offset == 1);

        // test type 2
        ptr = storage;
        test_type_2 t2(storage,0);
        t2.next();
        ptr += 2;
        BOOST_TEST( t2.m_ptr == ptr);
        BOOST_TEST( t2.m_bit_offset == 4);
        t2.next();
        ptr += 3;
        BOOST_TEST( t2.m_ptr == ptr);
        BOOST_TEST( t2.m_bit_offset == 0);

        // test type 3
        ptr = storage;
        test_type_3 t3(storage, 0);
        t3.next();
        ++ptr;
        BOOST_TEST( t3.m_ptr == ptr);
        BOOST_TEST( t3.m_bit_offset == 0);
    }

    // testing previous
    {
        storage_t storage[20];
        storage_ptr_t ptr = storage;
        test_type_1 t1(ptr, 0);

        --ptr;
        t1.previous();
        BOOST_TEST( t1.m_ptr == ptr);
        BOOST_TEST( t1.m_bit_offset == 5);

        t1.previous();
        BOOST_TEST( t1.m_ptr == ptr);
        BOOST_TEST( t1.m_bit_offset == 2);

        --ptr;
        t1.previous();
        BOOST_TEST( t1.m_ptr == ptr);
        BOOST_TEST( t1.m_bit_offset == 7);

        t1.previous();
        BOOST_TEST( t1.m_ptr == ptr);
        BOOST_TEST( t1.m_bit_offset == 4);

        
        ptr = storage;
        test_type_2 t2(storage,0);

        ptr -= 3;
        t2.previous();
        BOOST_TEST( t2.m_ptr == ptr);
        BOOST_TEST( t2.m_bit_offset == 4);

        ptr -= 2;
        t2.previous();
        BOOST_TEST( t2.m_ptr == ptr);
        BOOST_TEST( t2.m_bit_offset == 0);


        ptr = storage;
        test_type_3 t3(storage,0);
        --ptr;
        t3.previous();
        BOOST_TEST( t3.m_ptr == ptr);
        BOOST_TEST( t3.m_bit_offset == 0);
    }

    // testing distance
    {
        storage_t storage[20];
        storage_ptr_t ptr = storage;
        ptr = storage;

        test_type_1 t1(storage,0);
        test_type_1 t2(storage,0);
        t2.next();
        t2.next();
        t2.next();
        BOOST_TEST( t2.distance(t1) == 3 );


        test_type_1 t3(storage,0);
        test_type_1 t4(storage,0);
        t3.next();
        t3.next();
        t3.next();
        t3.next();
        BOOST_TEST(t3.distance(t4) == 4);

        test_type_3 t5(storage,0);
        test_type_3 t6(storage,0);
        t5.next();
        BOOST_TEST( t5.distance(t6) == 1);
    }

    // testing is_equal
    {
        storage_t storage[20];
        storage_ptr_t ptr = storage;
        ptr = storage;

        test_type_1 t1(storage,0);
        test_type_1 t2(storage,0);
        BOOST_TEST( t1.is_equal(t2) );
        BOOST_TEST( t2.is_equal(t1) );
        BOOST_TEST( t1.is_equal(t1) );
    }


    // testing assign
    {
        storage_t storage[20];
        // storage_ptr_t ptr = storage;
        // ptr = storage;

        test_type_1 t1(storage,0);
        test_type_1 t2(storage,0);
        t2.next();
        t2.next();
        BOOST_TEST( !t1.is_equal(t2) );
        t1.assign(t2);
        BOOST_TEST( t1.is_equal(t2) );
    }

    // testing const_deref
    {
        storage_t storage[20];
        storage_ptr_t ptr = storage;
        ptr = storage;
        test_type_1::proxy_ref_type r1(storage,0);
        r1 = 2;
        BOOST_TEST( *ptr != 0);
        test_type_1 t1(storage,0);
        BOOST_TEST(t1.const_deref() == 2);

    }

    // testing deref
    {
        storage_t storage[20];
        std::memset(storage,0,20);
        test_type_1 t1(storage,0);
        BOOST_TEST(t1.deref() == 0);
        t1.deref() = 2;
        BOOST_TEST( *storage != 0);
        BOOST_TEST(t1.deref() == 2);


    }

    // testing has_value
    {
        storage_t storage[20];
        test_type_1 t1;
        BOOST_TEST( !t1.has_value() );
        test_type_2 t2(storage,0);
        BOOST_TEST( t2.has_value() );
    }

    // testing advance function
    {
        storage_t storage[20];
        std::memset(storage,0,20);
        storage_ptr_t ptr = storage;
        test_type_1 t1(storage, 0);
        ++ptr;
        t1.advance(3);
        BOOST_TEST( t1.m_ptr == ptr);
        BOOST_TEST( t1.m_bit_offset == 1);
        t1.advance(-3);
        BOOST_TEST( t1.m_ptr == storage);
        BOOST_TEST( t1.m_bit_offset == 0);
    }

    // testing is_less
    {
        storage_t storage[20];
        std::memset(storage,0,20);
        test_type_1 t1(storage, 0);
        test_type_1 t2(t1);
        BOOST_TEST( !t1.is_less(t1) );
        BOOST_TEST( !t1.is_less(t2) );
        t1.next();
        BOOST_TEST( !t1.is_less(t2) );
        BOOST_TEST( t2.is_less(t1) );

    }

    // is_greater
    {
        storage_t storage[20];
        std::memset(storage,0,20);
        test_type_1 t1(storage, 0);
        test_type_1 t2(t1);
        BOOST_TEST( !t1.is_greater(t1) );
        BOOST_TEST( !t1.is_greater(t2) );
        t1.next();
        BOOST_TEST( t1.is_greater(t2) );
        BOOST_TEST( !t2.is_greater(t1) );

    }
    return boost::report_errors();
}
