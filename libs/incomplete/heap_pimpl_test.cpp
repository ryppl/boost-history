//-----------------------------------------------------------------------------
// boost libs/incomplete/heap_pimpl_test.cpp header file
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

#include "heap_pimpl_test.h"

struct heap_pimpl_test::Impl
{
    int i;
};

heap_pimpl_test::heap_pimpl_test(int value)
{
    pimpl_.get().i = value;
}


heap_pimpl_test::~heap_pimpl_test()
{
}

void heap_pimpl_test::swap(heap_pimpl_test& operand)
{
    pimpl_.swap(operand.pimpl_);
}

int heap_pimpl_test::value() const
{
    return pimpl_.get().i;
}
