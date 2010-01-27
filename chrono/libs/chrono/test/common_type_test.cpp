//  common_type_test.cpp  ----------------------------------------------------//

//  Copyright 2010 Beman Dawes

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#define _CRT_SECURE_NO_WARNINGS  // disable VC++ foolishness

#include <boost/type_traits/common_type.hpp>
#include <iostream>

#include <boost/test/minimal.hpp>

struct C1 {};
    
struct C2 {};

struct C1C2 {
    C1C2() {}
    C1C2(C1 const&) {}
    C1C2(C2 const&) {}
    C1C2& operator=(C1C2 const&) {
        return *this;
    }
};

template <typename C, typename A>
void f2(typename boost::common_type<A, C>::type const& rhs) {}

template <typename C, typename A, typename B>
void f3(typename boost::common_type<C, A, B>::type const& rhs) {}

template <typename C, typename A>
void assignation_2() {
typedef typename boost::common_type<A, C>::type AC;
    A a;
    C c;
    AC ac;
    ac=a;
    ac=c;

    f2<C, A>(a);
    f2<C, A>(c);
    
}

template <typename C, typename A, typename B>
void assignation_3() {
#if 0
typedef typename boost::common_type<A, B, C>::type ABC; // DO not compile
#else
typedef typename boost::common_type<C, A, B>::type ABC;
#endif
    A a;
    B b;
    C c;
    ABC abc;
    
    abc=a;
    abc=b;
    abc=c;
    
    f3<C, A, B>(a);
    f3<C, A, B>(b);
    f3<C, A, B>(c);
}

int test_main( int argc, char * argv[] )
{
    //~ assignation_2<C1, C2>(); // fails
    assignation_2<C1C2, C1>();
    assignation_2<C1, C1C2>();
    assignation_2<C1C2, C2>();
    assignation_2<C2, C1C2>();
    assignation_3<C1, C1C2, C2>();
    assignation_3<C1C2, C1, C2>();
    assignation_3<C2, C1C2, C1>();
    assignation_3<C1C2, C2, C1>();
    //~ assignation_3<C1, C2, C1C2>(); // fails
    return 0;
}
