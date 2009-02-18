
// Copyright 2008 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// g++ 3.3 (and possibly other compilers) haven't got good enough support
// for SFINAE to detect movable types, but has enough to implement the
// functions. So they can be used by specializing boost::is_movable.
//
// Is this worth supporting?

#include <boost/move/move.hpp>
#include <boost/test/minimal.hpp>
#include "./vector.hpp"

struct movable {
    movable() {}
    movable(movable const&) {}

    movable(boost::move_from<movable> x)
    {
    }

    movable& operator=(boost::move_from<movable> x)
    {
        return *this;
    }
};

namespace boost
{
    template<>
    struct is_movable< ::movable > : public boost::mpl::true_ {};
    template<typename T>
    struct is_movable< ::test_vector<T> > : public boost::mpl::true_ {};
}

void move_test()
{
    BOOST_CHECK(boost::is_movable< ::movable >::value);

    test_vector<int> x;

    x.push_back(0);
    int* ptr = &x[0];

    test_vector<int> y(boost::move(x));
    BOOST_CHECK(ptr == &y[0]);
}

int test_main(int, char**) {
    move_test();
    
    return 0;
}
