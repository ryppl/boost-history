// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#ifndef BOOST_POLICY_PTR_TEST_COUNTER_HPP_INCLUDED
#define BOOST_POLICY_PTR_TEST_COUNTER_HPP_INCLUDED 

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
struct semi_counted {
    semi_counted() { counter<Derived>::inc(); }
    semi_counted(const semi_counted&) { counter<Derived>::inc(); }
};

template<typename Derived>
struct counted {
    counted() { counter<Derived>::inc(); }
    counted(const counted&) { counter<Derived>::inc(); }
    ~counted() { counter<Derived>::dec(); }
};

#endif // #ifndef BOOST_POLICY_PTR_TEST_COUNTER_HPP_INCLUDED
