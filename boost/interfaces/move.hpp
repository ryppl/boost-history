// (C) Copyright Daniel Wallin 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Contains the definitions of the class template move_source and the function
// template move, which together make move pointers moveable.

#ifndef BOOST_IDL_MOVE_HPP_INCLUDED
#define BOOST_IDL_MOVE_HPP_INCLUDED

namespace boost { namespace interfaces {

template<typename Ptr>
class move_source {
public:
    move_source(Ptr& ptr) : ptr_(ptr) {}
    Ptr& ptr() const { return ptr_; }
private:
    Ptr& ptr_;
    move_source(const Ptr&);
};

template<typename T>
move_source<T> move(T& x) { return move_source<T>(x); }

} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_MOVE_HPP_INCLUDED
