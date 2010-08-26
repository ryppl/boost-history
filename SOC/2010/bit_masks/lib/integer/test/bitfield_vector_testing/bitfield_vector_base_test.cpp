//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_vector.hpp>
#include "test_utility.hpp"
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>

using namespace boost;

void* ptr_allocation_requested;
bool allocate_called = false;
bool allocator_copy_called = false;
bool default_constructor_called = false;
bool copy_other_allocator_called = false;
std::size_t allocation_size = 0;
void* ptr_deallocation_address = 0;
std::size_t deallocation_size = 0;
bool destructor_called = false;
bool deallocate_called = false;

void reset_allocator_test_variables() {
    ptr_allocation_requested = (void*)0xdeadbeef;
    allocator_copy_called = false;
    default_constructor_called = false;
    copy_other_allocator_called = false;
    allocation_size = 0;
    ptr_deallocation_address = (void*)0;
    deallocation_size = 0;
    destructor_called = false;
    allocate_called = false;
    deallocate_called = false;
}

template <class T>
class allocator {
public:
    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef T         value_type;

    template <class U>
    struct rebind {
        typedef allocator<U> other;
    };

    allocator() throw() {
        default_constructor_called = true;
    }

    allocator(const allocator&) throw() {
        allocator_copy_called = true;
    }

    template <class U>
    allocator(const allocator<U>&) throw() {
        copy_other_allocator_called = true;
    }

    ~allocator() throw() {
        destructor_called = true;
    }

    pointer address(reference x) const;
    const_pointer address(const_reference x) const;

    pointer allocate(size_type n, void* = 0) {
        allocate_called = true;
        allocation_size = n;
        return reinterpret_cast<pointer>(ptr_allocation_requested);
    }

    void deallocate(pointer p, size_type n) {
        deallocation_size = n;
        deallocate_called = true;
        ptr_deallocation_address = reinterpret_cast<void*>(p);
    }

    size_type max_size() const throw();

    void construct(pointer p, const T& val);
    void destroy(pointer p);
};



// typedef bits<short,2> T;
typedef allocator<int> alloc;
typedef allocator<unsigned char> rebound_alloc;
typedef detail::bitfield_vector_base<
    short,
    alloc
>                           vector_base_type;

int main() {
    // Checking typedefs 
    {
        // checking the rebound_alloc_type.
        BOOST_TEST((
            is_same<
                vector_base_type::rebound_alloc_type,
                rebound_alloc
            >::type::value
        ));

        // checking allocator typedef
        BOOST_TEST((
            is_same<
                vector_base_type::allocator,
                alloc
            >::type::value
        ));

        // checking the value_type type def.
        BOOST_TEST((
            is_same<
                vector_base_type::value_type,
                short
            >::type::value
        ));  
    }
    reset_allocator_test_variables();
    // testing impl type
    {
        typedef vector_base_type::bitfield_vector_impl vect_impl;
        // making sure the impl type is convertible to the allocator type
        // this is specific to one of my implementation details.
        BOOST_TEST((
            is_convertible<
                vect_impl,
                rebound_alloc
            >::type::value
        ));

        // testing the default constructor for bitfield_vector_impl.
        vect_impl temp_default_constructed;
        BOOST_TEST(temp_default_constructed.m_start == 0 );
        BOOST_TEST(temp_default_constructed.m_end == 0 );
        BOOST_TEST(temp_default_constructed.m_bits_in_use == 0 );
    }
    BOOST_TEST( allocation_size == 0 );
    BOOST_TEST( default_constructor_called );
    BOOST_TEST( !allocator_copy_called );
    BOOST_TEST( !copy_other_allocator_called );
    BOOST_TEST( destructor_called );
    BOOST_TEST( ptr_deallocation_address == 0);
    BOOST_TEST( deallocation_size == 0);
    BOOST_TEST( !allocate_called );
    BOOST_TEST( !deallocate_called );
    reset_allocator_test_variables();
    {
        typedef vector_base_type::bitfield_vector_impl vect_impl;
        alloc temp_alloc;
        vect_impl temp_allocator_constructed(temp_alloc);

        BOOST_TEST(temp_allocator_constructed.m_start == 0 );
        BOOST_TEST(temp_allocator_constructed.m_end == 0 );
        BOOST_TEST(temp_allocator_constructed.m_bits_in_use == 0 );        
    }
    BOOST_TEST( allocation_size == 0 );
    BOOST_TEST( default_constructor_called );
    BOOST_TEST( allocator_copy_called );
    BOOST_TEST( copy_other_allocator_called );
    BOOST_TEST( destructor_called );
    BOOST_TEST( ptr_deallocation_address == 0);
    BOOST_TEST( deallocation_size == 0);
    BOOST_TEST( !allocate_called );
    BOOST_TEST( !deallocate_called );
    reset_allocator_test_variables();
    // testing constructors for bitfield_vector_base
    {
        // Default Constructor.
        vector_base_type default_constructed;
        BOOST_TEST(default_constructed.m_impl.m_start == 0 );
        BOOST_TEST(default_constructed.m_impl.m_end == 0 );
        BOOST_TEST(default_constructed.m_impl.m_bits_in_use == 0 );
    }
    BOOST_TEST( allocation_size == 0 );
    BOOST_TEST( default_constructor_called );
    BOOST_TEST( !allocator_copy_called );
    BOOST_TEST( !copy_other_allocator_called );
    BOOST_TEST( destructor_called );
    BOOST_TEST( ptr_deallocation_address == 0);
    BOOST_TEST( deallocation_size == 0);
    BOOST_TEST( !allocate_called );
    BOOST_TEST( !deallocate_called );
    reset_allocator_test_variables();
    {
        // Allocator Constructor
        alloc temp_alloc;
        vector_base_type allocator_constructed(temp_alloc);
        BOOST_TEST(allocator_constructed.m_impl.m_start == 0 );
        BOOST_TEST(allocator_constructed.m_impl.m_end == 0 );
        BOOST_TEST(allocator_constructed.m_impl.m_bits_in_use == 0 );
    }
    BOOST_TEST( allocation_size == 0 );
    BOOST_TEST( allocator_copy_called );
    BOOST_TEST( default_constructor_called );
    BOOST_TEST( copy_other_allocator_called );
    BOOST_TEST( destructor_called );
    BOOST_TEST( ptr_deallocation_address == 0);
    BOOST_TEST( deallocation_size == 0);
    BOOST_TEST( !allocate_called );
    BOOST_TEST( !deallocate_called );
    reset_allocator_test_variables();
    {
        // N Constructor
        vector_base_type n_array_constructed(3);
        BOOST_TEST(n_array_constructed.m_impl.m_start != 0 );
        BOOST_TEST(n_array_constructed.m_impl.m_end != 0 );
        BOOST_TEST(n_array_constructed.m_impl.m_bits_in_use == 0 );
    }
    BOOST_TEST( allocation_size == 3 );
    BOOST_TEST( !allocator_copy_called );
    BOOST_TEST( default_constructor_called );
    BOOST_TEST( !copy_other_allocator_called );
    BOOST_TEST( destructor_called );
    BOOST_TEST( ptr_deallocation_address == (void*)0xdeadbeef );
    BOOST_TEST( deallocation_size == 3 );
    BOOST_TEST( allocate_called );
    BOOST_TEST( deallocate_called );
    reset_allocator_test_variables();
    {
        alloc temp_alloc;
        // N + Allocator Constructor
        vector_base_type n_plus_allocator_array_constructed(3, temp_alloc);
        BOOST_TEST(n_plus_allocator_array_constructed.m_impl.m_start != 0 );
        BOOST_TEST(n_plus_allocator_array_constructed.m_impl.m_end != 0 );
        BOOST_TEST(n_plus_allocator_array_constructed.m_impl.m_bits_in_use == 0 );
    }
    BOOST_TEST( allocation_size == 3 );
    BOOST_TEST( allocator_copy_called );
    BOOST_TEST( default_constructor_called );
    BOOST_TEST( copy_other_allocator_called );
    BOOST_TEST( destructor_called );
    BOOST_TEST( ptr_deallocation_address == (void*)0xdeadbeef );
    BOOST_TEST( deallocation_size == 3 );
    BOOST_TEST( allocate_called );
    BOOST_TEST( deallocate_called );
    reset_allocator_test_variables();
    // testing for get alloc
    {
        // Allocator Constructor
        alloc temp_alloc;
        vector_base_type allocator_constructed(temp_alloc);
        reset_allocator_test_variables();
        alloc t = allocator_constructed.get_allocator();
        BOOST_TEST( allocation_size == 0 );
        BOOST_TEST( !allocator_copy_called );
        BOOST_TEST( !default_constructor_called );
        BOOST_TEST( copy_other_allocator_called );
        BOOST_TEST( !destructor_called );
        BOOST_TEST( ptr_deallocation_address == 0 );
        BOOST_TEST( deallocation_size == 0 );
        BOOST_TEST( !allocate_called );
        BOOST_TEST( !deallocate_called );
    }
    return boost::report_errors();
}


