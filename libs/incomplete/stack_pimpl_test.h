//-----------------------------------------------------------------------------
// libs stack_pimpl/x.h header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_LIBS_INCOMPLETE_STACK_PIMPL_TEST_H
#define BOOST_LIBS_INCOMPLETE_STACK_PIMPL_TEST_H

#include <cstddef> // for std::size_t
#include <boost/incomplete.hpp>

class stack_pimpl_test
{
    static const std::size_t sizeofimpl = sizeof(int);

    struct Impl;
    boost::incomplete<Impl, sizeofimpl> pimpl_;

public:
    explicit stack_pimpl_test(int value);
    ~stack_pimpl_test();
    void swap(stack_pimpl_test&);

    int value() const;
};

#endif // BOOST_LIBS_INCOMPLETE_STACK_PIMPL_TEST_H
