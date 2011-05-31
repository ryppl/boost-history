
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include "string.hpp"

CONTRACT_CONSTRUCTOR_BODY(String, String)(const char* q) {
    init(q);
}

CONTRACT_CONSTRUCTOR_BODY(String, String)(const String& s) {
    init(s.p);
}

CONTRACT_DESTRUCTOR_BODY(String, String)(void) {
    delete[] p;
}

char& String::CONTRACT_MEMBER_BODY(operator([], at))(int i) {
    // check invariant and preconditions on entry
    return p[i]; // do work
    // check invariant on exit
}

int String::CONTRACT_MEMBER_BODY(size)(void) { return sz; }
    
void String::CONTRACT_MEMBER_BODY(init)(const char* q) {
    sz = strlen(q);
    p = new char[sz + 1];
    for (int i = 0; i < sz; ++i) p[i] = q[i];
    p[sz] = '\0';
}

