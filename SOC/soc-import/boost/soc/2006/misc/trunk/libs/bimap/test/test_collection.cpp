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

// Boost.Bimap.Collection
#include <boost/bimap/collection/key_type_of_tag.hpp>
#include <boost/bimap/collection/support/is_key_type_of.hpp>
#include <boost/bimap/collection/support/compute_index_type.hpp>
#include <boost/bimap/collection/support/compute_view_type.hpp>
#include <boost/bimap/collection/set_of.hpp>
#include <boost/bimap/collection/multiset_of.hpp>
#include <boost/bimap/collection/unordered_set_of.hpp>
#include <boost/bimap/collection/unordered_multiset_of.hpp>
#include <boost/bimap/collection/list_of.hpp>
#include <boost/bimap/collection/vector_of.hpp>

void basic_test()
{

}

int test_main( int, char* [] )
{
    basic_test();
    return 0;
}


