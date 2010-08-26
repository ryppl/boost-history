//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_vector.hpp>
#include "test_utility.hpp"

using namespace boost;

/** This is done so that I can inherit from bitfield_vector and have access
 *  to the functions and members which are protectedly inherited from
 *  the bitfield_vector_base class.
 *
 *  This is only done so that testing can be done easier.
 *
 */
typedef bitfield_vector<unsigned int, 3> bfv_1;

template <typename T, std::size_t Width, typename Alloc, typename Bfv>
struct test_for_bfv
    :Bfv
{
    typedef Bfv _base;

    /** Tests to make sure that I got all of the typedefs from within
     *  bitfield_vector correct and they are all correctly exposed.
     */
    static void test_typedefs() {
        // iterator types.
        BOOST_TEST((
            is_same<
                typename _base::iterator,
                detail::bf_vector_iterator<T,Width>
            >::value
        ));

        BOOST_TEST((
            is_same<
                typename _base::const_iterator,
                detail::const_bf_vector_iterator<T,Width>
            >::value
        ));

        BOOST_TEST((
            is_same<
                typename _base::reverse_iterator,
                detail::bf_vector_reverse_iterator<T,Width>
            >::value
        ));

        BOOST_TEST((
            is_same<
                typename _base::const_reverse_iterator,
                detail::const_bf_vector_reverse_iterator<T,Width>
            >::value
        ));

        // value_type
        BOOST_TEST((
            is_same<
                typename _base::value_type,
                T
            >::value
        ));

        // allocator type
        BOOST_TEST((
            is_same<
                typename _base::allocator_type,
                Alloc
            >::value
        ));

        // size_type
        BOOST_TEST(( 
            is_same<
                typename _base::size_type,
                std::size_t
            >::value
        ));

        // difference_type
        BOOST_TEST(( 
            is_same<
                typename _base::difference_type,
                std::ptrdiff_t
            >::value
        ));

        // pointer
        BOOST_TEST((
            is_same<
                typename _base::pointer,
                boost::detail::storage_ptr_t
            >::value
        ));

        // const_pointer
        typedef boost::detail::storage_t const* const_storage_ptr;
        BOOST_TEST((
            is_same<
                typename _base::const_pointer,
                const_storage_ptr
            >::value
        ));

        // reference
        BOOST_TEST((
            is_same<
                typename _base::reference,
                detail::proxy_reference_type<T,Width>
            >::value
        ));

        // const_reference
        BOOST_TEST((
            is_same<
                typename _base::const_reference,
                detail::const_proxy_reference_type<T,Width>
            >::value
        ));
    }
    
    

    // default constructor
    test_for_bfv()
        :_base()
    { }

    // default ctor test
    void test_default_ctor() {
        BOOST_TEST(this->m_impl.m_start == 0 );
        BOOST_TEST(this->m_impl.m_end == 0 );
        BOOST_TEST(this->m_impl.m_bits_in_use == 0 );
    }

    // ctor over alloc
    test_for_bfv(Alloc a)
        :_base(a)
    { }

    // ctor over alloc test
    void test_alloc_ctor() {
        BOOST_TEST(this->m_impl.m_start == 0 );
        BOOST_TEST(this->m_impl.m_end == 0 );
        BOOST_TEST(this->m_impl.m_bits_in_use == 0 );
    }

    // fill n with value ctor.
    test_for_bfv(typename _base::size_type n, typename _base::value_type val )
        :_base(n,val)
    { }

    void test_fill_with_n(typename _base::value_type val) {
        typename _base::iterator it(this->m_impl.m_start, 0);
        typename _base::iterator it_end(this->m_impl.m_start, 0);
        it_end += this->m_impl.m_bits_in_use / Width;
        while(it != it_end) {
            BOOST_TEST( *it == val);
            ++it;
        }
    }
};



typedef test_for_bfv<
    unsigned int,
    3,
    std::allocator<unsigned char>,
    bfv_1
>           test_type_1;

template<typename Tester>
void test_orchestrator() {

    Tester::test_typedefs();

    // default constructor
    {
        Tester t1;
        t1.test_default_ctor();
    }

    // constructor over allocator
    {
        typename Tester::allocator_type alloc_temp;
        Tester t1(alloc_temp);
        t1.test_alloc_ctor();
    }

    // testing fill with N constructor.
    {
        Tester t1(4, 2);
        t1.test_fill_with_n(2);
    }
    // regular begin and end.
    {
    }
    // const begin and end
    {
    }
    // reverse begin and end
    {
    }
    // const reverse begine and end
    {
    }

}

int main() {
    test_orchestrator<test_type_1>();
    return boost::report_errors();
}
