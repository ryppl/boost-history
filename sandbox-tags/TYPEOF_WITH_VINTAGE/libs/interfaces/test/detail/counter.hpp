// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_TEST_COUNTER_HPP_INCLUDED
#define BOOST_IDL_TEST_COUNTER_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

namespace boost { namespace interfaces { namespace test {

template<typename Counted>
class counter {
public:
    static int& value() { return value_; }

    static void inc() { ++value_; }

    static void dec() { --value_; }
private:
    static int value_;
};

template<typename Counted>
int counter<Counted>::value_;

template<typename Derived>
struct counted {
    counted() { counter<Derived>::inc(); }
    counted(const counted&) { counter<Derived>::inc(); }
    ~counted() { counter<Derived>::dec(); }
};

} } } // End namespace test, interfaces, boost.

#endif // #ifndef BOOST_IDL_TEST_COUNTER_HPP_INCLUDED
