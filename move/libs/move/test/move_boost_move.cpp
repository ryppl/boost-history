// Copyright David Abrahams 2004.
// Copyright Daniel James 2008.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define DEBUG_MOVE
#include "x.hpp"
#include "y.hpp"

#include <boost/static_assert.hpp>
#include "./generic_tests.hpp"

int main() {
    test::generic_tests<X>::copy_and_move_tests();
    test::generic_tests<Y>::move_tests();
}
