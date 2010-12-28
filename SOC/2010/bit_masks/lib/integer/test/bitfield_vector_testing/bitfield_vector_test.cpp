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

    // fill constructor test.
    void test_fill_with_n(typename _base::value_type val) {
        typename _base::iterator it(this->m_impl.m_start, 0);
        typename _base::iterator it_end(this->m_impl.m_start, 0);
        it_end += this->m_impl.m_bits_in_use / Width;
        while(it != it_end) {
            BOOST_TEST( *it == val);
            ++it;
        }
    }

    // implementation function tests.
    // void allocate_and_fill(size_type n, value_type value)
    void test_allocate_and_fill(typename _base::size_type n,
        typename _base::value_type val)
    {
        this->allocate_and_fill(n,val);
        
        typename _base::size_type allocated_amount = ((n*Width)/CHAR_BIT)
            + ((n%CHAR_BIT)>0);
        BOOST_TEST(typename _base::size_type(this->m_impl.m_end - this->m_impl.m_start)
            == allocated_amount);
        BOOST_TEST(this->m_impl.m_bits_in_use == n*Width );
        typename _base::iterator it(this->m_impl.m_start, 0);
        typename _base::iterator it_end(this->m_impl.m_start, 0);
        while(it != it_end) {
            BOOST_TEST( *it == val);
            ++it;
        }
    }
    
    // testing the regular begin function.
    void test_begin_and_end(typename _base::difference_type n) {
        typename _base::iterator i1 = this->begin();
        typename _base::iterator i2 = this->end();
        BOOST_TEST( i1 != i2 );
        BOOST_TEST( i2 - i1 == n );
    }

    // testing the const begin function.
    void test_const_begin_and_end(typename _base::difference_type n) const {
        typename _base::const_iterator i1 = this->begin();
        typename _base::const_iterator i2 = this->end();
        BOOST_TEST( i1 != i2 );
        BOOST_TEST( i2 - i1 == n );
    }

    // reverse iterator begin and end
    void test_reverse_begin_and_end(typename _base::difference_type n) {
        typename _base::reverse_iterator i1 = this->rbegin();
        typename _base::reverse_iterator i2 = this->rend();
        std::cout << "i2 - i1: "  << i2 - i1 << std::endl;
        BOOST_TEST( i1 != i2 );
        BOOST_TEST( i2 - i1 == n );
    }

    // const reverse iterator begin and end
    void test_const_reverse_begin_and_end(typename _base::difference_type n) const {
        typename _base::const_reverse_iterator i1 = this->rbegin();
        typename _base::const_reverse_iterator i2 = this->rend();
        BOOST_TEST( i1 != i2 );
        BOOST_TEST( i2 - i1 == n );
    }

    // explicitly const begin and end
    void test_cbegin_and_cend(typename _base::difference_type n) {
        typename _base::const_iterator i1 = this->cbegin();
        typename _base::const_iterator i2 = this->cend();
        BOOST_TEST( i1 != i2 );
        BOOST_TEST( i2 - i1 == n );
    }

    // explictly const reverse begin and end
    void test_crbegin_and_crend(typename _base::difference_type n) {
        typename _base::const_reverse_iterator i1 = this->crbegin();
        typename _base::const_reverse_iterator i2 = this->crend();
        BOOST_TEST( i1 != i2 );
        BOOST_TEST( i2 - i1 == n );
    }

    // front and back
    void test_front_and_back(typename _base::value_type f,
        typename _base::value_type b)
    {
        BOOST_TEST(f == this->front());
        BOOST_TEST(b == this->back());
    }
    
    // const front and back
    void test_const_front_and_back(typename _base::value_type f,
        typename _base::value_type b) const
    {
        BOOST_TEST(f == this->front());
        BOOST_TEST(b == this->back());
    }

    // size
    void test_size(typename _base::size_type s) const {
        BOOST_TEST( s == this->size() );
    }

    // max size
    void test_max_size() {
        BOOST_TEST((~(typename _base::size_type(0))*CHAR_BIT)/Width
            ==this->max_size());
    }
    
    // capacity
    void test_capacity() {
        typename _base::size_type allcation_size = this->m_impl.m_end
            - this->m_impl.m_start;
        allcation_size *= CHAR_BIT;
        allcation_size /= Width;
        BOOST_TEST( allcation_size == this->capacity() );
    }

    // empty
    void test_empty(bool is_empty) {
        BOOST_TEST(is_empty == this->empty());
    }

    // clear
    void test_clear() {
        this->clear();
        BOOST_TEST( this->m_impl.m_bits_in_use == 0);
    }
    
    // resize
    void test_resize() {
        // BOOST_TEST( )
        // test case size > the the resizing value.
    }
    
    // push_back
    void test_push_back(T val) {
        BOOST_TEST(this->back() == val);
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

    // allocate_and_fill
    {
        Tester t1;
        t1.test_allocate_and_fill(4,3);
    }

    // regular begin and end.
    {
        Tester t1(5, 2);
        t1.test_begin_and_end(5);

    }

    // const begin and end
    {
        Tester t1(5, 2);
        t1.test_const_begin_and_end(5);
    }

    // reverse begin and end
    {
        Tester t1(4, 2);
        t1.test_reverse_begin_and_end(4);
    }

    // const reverse begine and end
    {
        Tester t1(2, 2);
        t1.test_const_reverse_begin_and_end(2);
    }

    // explicitly const begin and end
    {
        Tester t1(2, 2);
        t1.test_cbegin_and_cend(2);
    }
    
    // explicity const reverse begin and end
    {
        Tester t1(2, 2);
        t1.test_crbegin_and_crend(2);
    }
    
    // non const front and back.
    {
        Tester t1(2, 2);
        t1.test_front_and_back(2,2);
    }

    // const front and back
    {
        Tester t1(2, 2);
        *--t1.end() = 3;
        t1.test_const_front_and_back(2,3);
    }

    // size
    {
        Tester t1(1, 2);
        Tester t2(5, 2);
        Tester t3(8, 2);

        t1.test_size(1);
        t2.test_size(5);
        t3.test_size(8);
    }

    // max size
    {
        Tester t1;
        t1.test_max_size();
    }
    
    // capacity
    {
        Tester t1;
        t1.test_capacity();
        Tester t2(8, 2);
        t2.test_capacity();
    }
    
    // empty
    {
        Tester t1;
        t1.test_empty(true);
        Tester t2(8, 2);
        t2.test_empty(false);
    }

    // clear test.
    {
        Tester t2(8, 2);
        t2.test_clear();
    }
    
    // resize test
    {
        Tester t1(8,2);
        t1.test_resize();
    }

    // push_back test
    {
        Tester t1(8,2);
        t1.push_back(3);
        t1.test_push_back(3);
    }
}

int main() {
    test_orchestrator<test_type_1>();
    return boost::report_errors();
}
