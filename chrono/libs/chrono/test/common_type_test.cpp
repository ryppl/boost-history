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

    
struct C3 : C2 {};
struct C1C2 {
    C1C2() {}
    C1C2(C1 const&) {}
    C1C2(C2 const&) {}
    C1C2& operator=(C1C2 const&) {
        return *this;
    }
};

template <typename C, typename A>
void f2(typename boost::common_type<A, C>::type const& ) {}

template <typename C, typename A, typename B>
void f3(typename boost::common_type<C, A, B>::type const& ) {}

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
typedef typename boost::common_type<C, A, B>::type ABC;
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

C1C2 c1c2;
C1 c1;

int f(C1C2 ) { return 1;}
int f(C1 ) { return 2;}
template <typename OSTREAM>
OSTREAM& operator<<(OSTREAM& os, C1 const&) {return os;}

C1C2& declval_C1C2() {return c1c2;}
C1& declval_C1(){return c1;}
bool declval_bool(){return true;}

int test_main( int /*argc*/, char * /*argv*/[] )
{
    assignation_2<C1C2, C1>();
    typedef boost::common_type<C1C2&, C1&>::type T1; // fails if BOOST_COMMON_TYPE_DONT_USE_TYPEOF
    //~ std::cout << f(declval_bool()?declval_C1C2():declval_C1()) << std::endl;
    typedef boost::common_type<C3*, C2*>::type T2;
    typedef boost::common_type<int*, int const*>::type T3;
    typedef boost::common_type<int volatile*, int const*>::type T4; // fails if BOOST_COMMON_TYPE_DONT_USE_TYPEOF
    typedef boost::common_type<int*, int volatile*>::type T5;
    //~ typedef boost::common_type<short volatile*, int const*>::type T5; // fails

    assignation_2<C1, C1C2>();
    assignation_2<C1C2, C2>();
    assignation_2<C2, C1C2>();
    assignation_3<C1, C1C2, C2>();
    assignation_3<C1C2, C1, C2>();
    assignation_3<C2, C1C2, C1>();
    assignation_3<C1C2, C2, C1>();
    //~ assignation_3<C1, C2, C1C2>(); // fails because the common type is the third
    return 0;
}
