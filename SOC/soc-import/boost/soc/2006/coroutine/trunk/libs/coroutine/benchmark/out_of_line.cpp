//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/*
 * Out of line definitions for sink and ol_foo.
 * Prototypes in call_overhead.cpp.
 * This functions are on a diferent translation unit to prevent
 * compilers that do unit at a time optimizations to eliminate or inline 
 * them. This won't fool compilers that do whole program optimization though.
 */
void sink(int&) {}

void ol_foo(int *x) {
  *x ^=0xAAAA;
}
