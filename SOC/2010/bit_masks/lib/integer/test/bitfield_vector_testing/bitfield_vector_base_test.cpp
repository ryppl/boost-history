//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_vector.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>

using namespace boost;
template <class T> class allocator {
public:
typedef size_t    size_type;
typedef ptrdiff_t difference_type;
typedef T*        pointer;
typedef const T*  const_pointer;
typedef T&        reference;
typedef const T&  const_reference;
typedef T         value_type;
template <class U> struct rebind { typedef allocator<U>
other; };

allocator() throw();
allocator(const allocator&) throw();
template <class U> allocator(const allocator<U>&) throw();
~allocator() throw();

pointer address(reference x) const;
const_pointer address(const_reference x) const;

pointer allocate(size_type, void* hint = 0);
void deallocate(pointer p, size_type n);
size_type max_size() const throw();

void construct(pointer p, const T& val);
void destroy(pointer p);
};



typedef bits<short,2> T;
typedef std::allocator<int> alloc;
typedef std::allocator<unsigned char> rebound_alloc;
typedef detail::bitfield_vector_base<
    T,
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
                T
            >::type::value
        ));  
    }

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
        BOOST_TEST(temp_default_constructed._start == 0 );
        BOOST_TEST(temp_default_constructed._end == 0 );
        BOOST_TEST(temp_default_constructed._bits_in_use == 0 );

        alloc temp_alloc;
        vect_impl temp_allocator_constructed(temp_alloc);

        BOOST_TEST(temp_allocator_constructed._start == 0 );
        BOOST_TEST(temp_allocator_constructed._end == 0 );
        BOOST_TEST(temp_allocator_constructed._bits_in_use == 0 );        
    }

    // testing constructors for bitfield_vector_base
    {
        // Default Constructor.
        vector_base_type default_constructed;
        BOOST_TEST(default_constructed._impl._start == 0 );
        BOOST_TEST(default_constructed._impl._end == 0 );
        BOOST_TEST(default_constructed._impl._bits_in_use == 0 );

        // Allocator Constructor
        alloc temp_alloc;
        vector_base_type allocator_constructed(temp_alloc);
        BOOST_TEST(default_constructed._impl._start == 0 );
        BOOST_TEST(default_constructed._impl._end == 0 );
        BOOST_TEST(default_constructed._impl._bits_in_use == 0 );

        // N Constructor
        vector_base_type n_array_constructed(3);
        BOOST_TEST(n_array_constructed._impl._start != 0 );
        BOOST_TEST(n_array_constructed._impl._end != 0 );
        BOOST_TEST(n_array_constructed._impl._bits_in_use == 0 );

        // N + Allocator Constructor
        vector_base_type n_plus_allocator_array_constructed(3, temp_alloc);
        BOOST_TEST(n_plus_allocator_array_constructed._impl._start != 0 );
        BOOST_TEST(n_plus_allocator_array_constructed._impl._end != 0 );
        BOOST_TEST(n_plus_allocator_array_constructed._impl._bits_in_use == 0 );
    }
    {
    }
    return boost::report_errors();
}


