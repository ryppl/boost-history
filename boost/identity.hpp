//  Boost identity library

//  (C) Copyright Herve Bronnimann 2002.  Permission to copy, use,
//  modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies.  This software is provided
//  "as is" without express or implied warranty, and with no claim as to
//  its suitability for any purpose. 

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_IDENTITY_HPP
#define BOOST_IDENTITY_HPP

namespace boost {

    template <class T>
    struct identity
    {
        typedef T argument_type; // directly to avoid including <functional>
        typedef T result_type;   // directly to avoid including <functional>
        typedef T type;          // somewhat redundant with unary_function,
                                 // but apparently to satisfy mpl's interface
 
        template <class U> struct rebind { typedef identity<U> other; };
 
        // allow identity to return an lvalue if lvalue is passed as argument
        T const& operator()(T const& x) const { return x; }
        T&       operator()(T& x)       const { return x; }
    };

}  // namespace boost

#endif  // BOOST_IDENTITY_HPP
