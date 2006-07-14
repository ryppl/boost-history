// Boost.bimap
//
// (C) Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/{{ library name }}  for the library home page.

// Boost.Test
#include <boost/test/minimal.hpp>

// std
#include <utility>
#include <cstddef>

// Boost.Static_assert
#include <boost/static_assert.hpp>

// Boost.Bimap
#include <boost/bimap/detail/test/check_metadata.hpp>
#include <boost/bimap/detail/test/check_size_of_pair.hpp>

#include <boost/bimap/container_adaptor/detail/container_adaptor.hpp>
#include <boost/bimap/container_adaptor/detail/weak_associative_container_adaptor.hpp>
#include <boost/bimap/container_adaptor/detail/associative_container_adaptor.hpp>
#include <boost/bimap/container_adaptor/detail/ordered_associative_container_adaptor.hpp>
#include <boost/bimap/container_adaptor/detail/unordered_associative_container_adaptor.hpp>

// Boost.Bimap.Container_adapator
#include <boost/bimap/container_adaptor/map_adaptor.hpp>
#include <boost/bimap/container_adaptor/support/iterator_facade_converters.hpp>

#include <boost/iterator/iterator_adaptor.hpp>

#include <map>
#include <iostream>

template< class K, class D >
class indirect_iterator
        : public boost::iterator_adaptor
        <
            indirect_iterator<K,D>,
            typename std::map<K,D>::iterator,
            std::pair<const K,D>
        >
{
    public:
    indirect_iterator() {}

    indirect_iterator(typename indirect_iterator::base_type i)
        : indirect_iterator::iterator_adaptor_(i) {}

    private:
    friend class boost::iterator_core_access;


};

template< class K, class D >
class const_indirect_iterator
        : public boost::iterator_adaptor
        <
            const_indirect_iterator<K,D>,
            typename std::map<K,D>::const_iterator,
            const std::pair<const K,D>
        >
{
    public:
    const_indirect_iterator() {}

    const_indirect_iterator(typename const_indirect_iterator::base_type i)
        : const_indirect_iterator::iterator_adaptor_(i) {}

    private:
    friend class boost::iterator_core_access;

};



template< class K, class D >
class reverse_indirect_iterator
        : public boost::iterator_adaptor
        <
            reverse_indirect_iterator<K,D>,
            typename std::map<K,D>::const_iterator,
            std::pair<const K,D>
        >
{
    public:
    reverse_indirect_iterator() {}

    reverse_indirect_iterator(typename reverse_indirect_iterator::base_type i)
        : reverse_indirect_iterator::iterator_adaptor_(i) {}

    private:
    friend class boost::iterator_core_access;

};


template< class K, class D >
class const_reverse_indirect_iterator
        : public boost::iterator_adaptor
        <
            const_reverse_indirect_iterator<K,D>,
            typename std::map<K,D>::const_iterator,
            const std::pair<const K,D>
        >
{
    public:
    const_reverse_indirect_iterator() {}

    const_reverse_indirect_iterator(typename const_reverse_indirect_iterator::base_type i)
        : const_reverse_indirect_iterator::iterator_adaptor_(i) {}

    private:
    friend class boost::iterator_core_access;

};


template< class K, class D >
class indirect_map
: public boost::bimap::container_adaptor::map_adaptor
<
    std::map<K,D>,
    indirect_iterator<K,D>,
    const_indirect_iterator<K,D>,
    reverse_indirect_iterator<K,D>,
    const_reverse_indirect_iterator<K,D>,
    boost::bimap::container_adaptor::support::iterator_facade_to_base
    <
        indirect_iterator<K,D>,
        const_indirect_iterator<K,D>
    >
>
{
    public:
    indirect_map(std::map<K,D> & m) : indirect_map::map_adaptor_(m) {}

};



BOOST_BIMAP_TEST_STATIC_FUNCTION( basic_static_test )
{

}


void basic_test()
{

    std::map<int,double> m;
    indirect_map<int,double> im(m);
    im.insert( std::pair<int,double>(10,2.5) );
    im.insert( im.begin(), std::pair<int,double>(12,4.5) );

}


int test_main( int, char* [] )
{
    BOOST_BIMAP_CALL_TEST_STATIC_FUNCTION( basic_static_test );

    basic_test();

    return 0;
}



