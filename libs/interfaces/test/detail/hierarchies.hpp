// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_TEST_HIERARCHIES_HPP_INCLUDED
#define BOOST_IDL_TEST_HIERARCHIES_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces.hpp>
#include "./counter.hpp"
#include "./method_tags.hpp"

namespace boost { namespace interfaces { namespace test {

BOOST_IDL_BEGIN(ifirst)
    BOOST_IDL_FN2(one, char, int, float)
    BOOST_IDL_CONST_FN1(one, void, const char*)
    BOOST_IDL_CONST_FN0(two, void)
BOOST_IDL_END(ifirst)

BOOST_IDL_BEGIN(ifirst_copy)
    BOOST_IDL_FN2(one, char, int, float)
    BOOST_IDL_CONST_FN1(one, void, const char*)
    BOOST_IDL_CONST_FN0(two, void)
BOOST_IDL_END(ifirst_copy)

BOOST_IDL_BEGIN(isecond)
    BOOST_IDL_FN2(three, char, int, float)
    BOOST_IDL_CONST_FN1(three, void, const char*)
    BOOST_IDL_CONST_FN0(four, void)
BOOST_IDL_END(isecond)

BOOST_IDL_BEGIN(ithird)
    BOOST_IDL_FN2(five, char, int, float)
    BOOST_IDL_CONST_FN1(five, void, const char*)
    BOOST_IDL_CONST_FN0(six, void)
BOOST_IDL_END(ithird)

BOOST_IDL_BEGIN(ifourth)
    BOOST_IDL_FN2(seven, char, int, float)
    BOOST_IDL_CONST_FN1(seven, void, const char*)
    BOOST_IDL_CONST_FN0(eight, void)
BOOST_IDL_END(ifourth)

BOOST_IDL_BEGIN(iderived1)
    BOOST_IDL_EXTENDS(ifirst)
    BOOST_IDL_EXTENDS(isecond)
BOOST_IDL_END(iderived1)

BOOST_IDL_BEGIN(iderived2)
    BOOST_IDL_EXTENDS(ifirst)
    BOOST_IDL_EXTENDS(ifirst_copy)
BOOST_IDL_END(iderived2)

BOOST_IDL_BEGIN(iderived3)
    BOOST_IDL_EXTENDS(ithird)
    BOOST_IDL_EXTENDS(ifourth)
BOOST_IDL_END(iderived3)

BOOST_IDL_BEGIN(iderived4)
    BOOST_IDL_EXTENDS(iderived1)
    BOOST_IDL_EXTENDS(iderived3)
BOOST_IDL_END(iderived4)

template<int N>
struct cfirst : counted< cfirst<N> > {
    cfirst() { } // Needed for const interface test.
    char one(int i, float f)
    { 
        counter< method<cfirst<N>, func_one_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void one(const char* h) const 
    { 
        counter< method<cfirst<N>, func_one_tag, sig_b_tag> >::inc();
    }
    void two() const 
    { 
        counter< method<cfirst<N>, func_two_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct csecond : counted< csecond<N> > {
    csecond() { } // Needed for const interface test.
    char three(int i, float f)
    { 
        counter< method<csecond<N>, func_three_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void three(const char* h) const 
    { 
        counter< method<csecond<N>, func_three_tag, sig_b_tag> >::inc();
    }
    void four() const 
    { 
        counter< method<csecond<N>, func_four_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct cthird : counted< cthird<N> > {
    cthird() { } // Needed for const interface test.
    char five(int i, float f)
    { 
        counter< method<cthird<N>, func_five_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void five(const char* h) const 
    { 
        counter< method<cthird<N>, func_five_tag, sig_b_tag> >::inc();
    }
    void six() const 
    { 
        counter< method<cthird<N>, func_six_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct cfourth : counted< cfourth<N> > {
    cfourth() { } // Needed for const interface test.
    char seven(int i, float f)
    { 
        counter< method<cfourth<N>, func_seven_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void seven(const char* h) const 
    { 
        counter< method<cfourth<N>, func_seven_tag, sig_b_tag> >::inc();
    }
    void eight() const 
    { 
        counter< method<cfourth<N>, func_eight_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct cderived1 : counted< cderived1<N> > { 
    cderived1() { } // Needed for const interface test.
    char one(int i, float f)
    { 
        counter< method<cderived1<N>, func_one_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void one(const char* h) const 
    { 
        counter< method<cderived1<N>, func_one_tag, sig_b_tag> >::inc();
    }
    void two() const 
    { 
        counter< method<cderived1<N>, func_two_tag, sig_c_tag> >::inc();
    }
    char three(int i, float f)
    { 
        counter< method<cderived1<N>, func_three_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void three(const char* h) const 
    { 
        counter< method<cderived1<N>, func_three_tag, sig_b_tag> >::inc();
    }
    void four() const 
    { 
        counter< method<cderived1<N>, func_four_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct cderived2 : counted< cderived2<N> > { 
    cderived2() { } // Needed for const interface test.
    char one(int i, float f)
    { 
        counter< method<cderived2<N>, func_one_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void one(const char* h) const 
    { 
        counter< method<cderived2<N>, func_one_tag, sig_b_tag> >::inc();
    }
    void two() const 
    { 
        counter< method<cderived2<N>, func_two_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct cderived4 : counted< cderived4<N> > {
    cderived4() { } // Needed for const interface test.
    char one(int i, float f)
    { 
        counter< method<cderived4<N>, func_one_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void one(const char* h) const 
    { 
        counter< method<cderived4<N>, func_one_tag, sig_b_tag> >::inc();
    }
    void two() const 
    { 
        counter< method<cderived4<N>, func_two_tag, sig_c_tag> >::inc();
    }
    char three(int i, float f)
    { 
        counter< method<cderived4<N>, func_three_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void three(const char* h) const 
    { 
        counter< method<cderived4<N>, func_three_tag, sig_b_tag> >::inc();
    }
    void four() const 
    { 
        counter< method<cderived4<N>, func_four_tag, sig_c_tag> >::inc();
    }
    char five(int i, float f)
    { 
        counter< method<cderived4<N>, func_five_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void five(const char* h) const 
    { 
        counter< method<cderived4<N>, func_five_tag, sig_b_tag> >::inc();
    }
    void six() const 
    { 
        counter< method<cderived4<N>, func_six_tag, sig_c_tag> >::inc();
    }
    char seven(int i, float f)
    { 
        counter< method<cderived4<N>, func_seven_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void seven(const char* h) const 
    { 
        counter< method<cderived4<N>, func_seven_tag, sig_b_tag> >::inc();
    }
    void eight() const 
    { 
        counter< method<cderived4<N>, func_eight_tag, sig_c_tag> >::inc();
    }
};

} } } // End namespaces test, interfaces, boost.

#endif // #ifndef BOOST_IDL_TEST_HIERARCHIES_HPP_INCLUDED
