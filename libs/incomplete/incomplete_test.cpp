//-----------------------------------------------------------------------------
// boost libs/incomplete/incomplete_test.cpp source file
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

#include <iostream>

#include "stack_pimpl_test.h"
#include "heap_pimpl_test.h"

int main()
{
    std::cout << "boost::incomplete tests";

    std::cout << "\n\ntesting stack-based use of incomplete as pimpl:\n";
    stack_pimpl_test stack_test(42);
    std::cout << " - the meaning of life is " << stack_test.value();

    std::cout << "\n\ntesting heap-based use of incomplete as pimpl:\n";
    heap_pimpl_test heap_test(3141592);
    std::cout << " - some digits of pi are " << heap_test.value();
}
