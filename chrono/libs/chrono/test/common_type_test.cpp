//  common_type_test.cpp  ----------------------------------------------------//

//  Copyright 2010 Beman Dawes

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#define _CRT_SECURE_NO_WARNINGS  // disable VC++ foolishness

#include <boost/type_traits/common_type.hpp>
#include <iostream>

struct A {};
    
struct B {};

struct C {
    C() {}
    C(A const&) {}
    C(B const&) {}
    C& operator=(C const&) {
        return *this;
    }
};

typedef boost::common_type<A, C>::type AC;
typedef boost::common_type<B, C>::type BC;

#if 0
typedef boost::common_type<A, B, C>::type ABC; // DO not compile
#else
typedef boost::common_type<C, B, A>::type ABC;
#endif
void f(ABC const& rhs) {}
    
int main()
{
    A a;
    B b;
    C c;
    AC ac;
    ac=a;
    ac=c;
    
    ABC abc;
    c=b;
    c=a;
    abc=a;
    abc=b;
    abc=c;
    f(abc);
    f(a);
    f(b);
    f(c);
  return 0;
}
