/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 2001-2003 Daniel Nuffer
    Copyright (c) 2003 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#include <boost/spirit/iterator/fixed_size_queue.hpp>
#include <boost/mpl/assert_is_same.hpp>
#include <boost/concept_check.hpp>
#include <cassert>
#include <iostream>

typedef boost::spirit::fixed_size_queue<int, 5> queue_t;
typedef queue_t::iterator iter_t;
typedef queue_t::const_iterator const_iter_t;
BOOST_CLASS_REQUIRE(const_iter_t, boost, RandomAccessIteratorConcept);

// Right now, the iterator is not a full compliant MutableRandomAccessIterator 
//  because operator[] does not return a reference. This seems a problem in
//  boost::iterator_adaptors. Anyway, this feature is not used in multi_pass
//  iterator, and this class is not really meant for public use yet.
BOOST_CLASS_REQUIRE(iter_t, boost, RandomAccessIteratorConcept);

int main(int, char**)
{

#if !defined(BOOST_ITERATOR_ADAPTORS_VERSION) || \
    BOOST_ITERATOR_ADAPTORS_VERSION < 0x0200
    
    // Iterators are random access.
    BOOST_MPL_ASSERT_IS_SAME(
        iter_t::iterator_category,
        std::random_access_iterator_tag);
    BOOST_MPL_ASSERT_IS_SAME(
        const_iter_t::iterator_category, 
        std::random_access_iterator_tag);

#endif // BOOST_ITERATOR_ADAPTORS_VERSION < 0x0200

    queue_t q;
    const queue_t& cq = q;

    q.push_back(1);
    q.push_back(2);
    q.push_back(3);
    q.push_back(4);
    assert(q.front() == 1);
    q.pop_front();
    assert(q.front() == 2);
    q.pop_front();
    assert(q.front() == 3);
    q.pop_front();
    assert(q.front() == 4);
    q.pop_front();
    q.push_back(5);
    q.push_back(6);
    q.push_back(7);
    q.push_back(8);
    assert(q.front() == 5);
    q.pop_front();
    assert(q.front() == 6);
    q.pop_front();
    assert(q.front() == 7);
    q.pop_front();
    assert(q.front() == 8);
    q.pop_front();

    q.push_front(5);
    q.push_front(4);
    q.push_front(3);
    q.push_front(2);
    q.push_front(1);

    // NOTE: Iterator tests are not exhaustive and they are not meant to be so.
    
    // Check iterator
    iter_t b = q.begin();
    assert(*b++ == 1);
    assert(*b++ == 2);
    assert(*b++ == 3);
    assert(*b++ == 4);
    assert(*b++ == 5);
    assert(b == q.end());
    assert(*--b == 5);
    assert(*--b == 4);
    assert(*--b == 3);
    assert(*--b == 2);
    assert(*--b == 1);
    assert(b == q.begin());

    // Check const_iterator
    const_iter_t c = cq.begin();
    assert(*c++ == 1);
    assert(*c++ == 2);
    assert(*c++ == 3);
    assert(*c++ == 4);
    assert(*c++ == 5);
    assert(c == cq.end());
    assert(*--c == 5);
    assert(*--c == 4);
    assert(*--c == 3);
    assert(*--c == 2);
    assert(*--c == 1);
    assert(c == cq.begin());

#if !defined(BOOST_ITERATOR_ADAPTORS_VERSION) || \
    BOOST_ITERATOR_ADAPTORS_VERSION < 0x0200 || \
    !BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1310))

// MSVC7.1 isn't able to compile this code for the new iterator adaptors

    // Check comparisons and interoperations (we are comparing
    //  const and non-const iterators)
    assert(c == b);
    assert(c+4 > b);
    assert(c < b+4);

#endif // BOOST_ITERATOR_ADAPTORS_VERSION < 0x0200 etc.

    // Check that you can actually modify the queue with an iterator
    *b = 123;
    assert(*c == 123);

    // Check random access
    assert(*((c+4)-4) == 123);
    assert(*((c-4)+4) == 123);

	std::cout << "Test completed succesfully" << std::endl;

    return 0;
}

