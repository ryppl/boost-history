//-----------------------------------------------------------------------------
// boost libs/incomplete/pimpl_test.h header file
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

#ifndef BOOST_LIBS_INCOMPLETE_PIMPL_TEST_H
#define BOOST_LIBS_INCOMPLETE_PIMPL_TEST_H

#include <cstddef> // for std::size_t
#include <boost/incomplete.hpp>

class pimpl_test
{
    struct Impl;
    boost::incomplete<Impl> pimpl_;

public:
    explicit pimpl_test(int value);
    ~pimpl_test();
    void swap(pimpl_test&);

    int value() const;
};

#endif // BOOST_LIBS_INCOMPLETE_PIMPL_TEST_H
