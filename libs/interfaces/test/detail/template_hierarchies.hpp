// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_TEST_TEMPLATE_HIERARCHIES_HPP_INCLUDED
#define BOOST_IDL_TEST_TEMPLATE_HIERARCHIES_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces.hpp>
#include "./counter.hpp"
#include "./method_tags.hpp"
#include "./protect.hpp"

#define BOOST_IDL_NO_ADVICE

namespace boost { namespace interfaces { namespace test {

template<typename T, typename U>
BOOST_IDL_BEGIN_TEMPLATE(itemplate_first, 2)
    BOOST_IDL_FN1(one, void, T)
    BOOST_IDL_FN1(one, void, U)
    BOOST_IDL_FN2(two, void, T, U)
BOOST_IDL_END_TEMPLATE(itemplate_first, 2)

template<typename T, typename U>
BOOST_IDL_BEGIN_TEMPLATE(itemplate_first_copy, 2)
    BOOST_IDL_FN1(one, void, T)
    BOOST_IDL_FN1(one, void, U)
    BOOST_IDL_FN2(two, void, T, U)
BOOST_IDL_END_TEMPLATE(itemplate_first_copy, 2)

template<typename T, typename U>
BOOST_IDL_BEGIN_TEMPLATE(itemplate_second, 2)
    BOOST_IDL_FN1(three, void, T)
    BOOST_IDL_FN1(three, void, U)
    BOOST_IDL_FN2(four, void, T, U)
BOOST_IDL_END_TEMPLATE(itemplate_second, 2)

template<typename T, typename U>
BOOST_IDL_BEGIN_TEMPLATE(itemplate_third, 2)
    BOOST_IDL_FN1(five, void, T)
    BOOST_IDL_FN1(five, void, U)
    BOOST_IDL_FN2(six, void, T, U)
BOOST_IDL_END_TEMPLATE(itemplate_third, 2)

template<typename T, typename U>
BOOST_IDL_BEGIN_TEMPLATE(itemplate_fourth, 2)
    BOOST_IDL_FN1(seven, void, T)
    BOOST_IDL_FN1(seven, void, U)
    BOOST_IDL_FN2(eight, void, T, U)
BOOST_IDL_END_TEMPLATE(itemplate_fourth, 2)

template<typename T, typename U>
BOOST_IDL_BEGIN_TEMPLATE(itemplate_derived1, 2)
    BOOST_IDL_EXTENDS(PROTECT((itemplate_first<T, U>)))
    BOOST_IDL_EXTENDS(PROTECT((itemplate_second<T, U>)))
BOOST_IDL_END_TEMPLATE(itemplate_derived1, 2)

template<typename T, typename U>
BOOST_IDL_BEGIN_TEMPLATE(itemplate_derived2, 2)
    BOOST_IDL_EXTENDS(PROTECT((itemplate_first<T, U>)))
    BOOST_IDL_EXTENDS(PROTECT((itemplate_first_copy<T, U>)))
BOOST_IDL_END_TEMPLATE(itemplate_derived2, 2)

template<typename T, typename U>
BOOST_IDL_BEGIN_TEMPLATE(itemplate_derived5, 2)
    BOOST_IDL_EXTENDS(PROTECT((itemplate_third<T, U>)))
    BOOST_IDL_EXTENDS(PROTECT((itemplate_fourth<T, U>)))
BOOST_IDL_END_TEMPLATE(itemplate_derived5, 2)

template<typename T, typename U>
BOOST_IDL_BEGIN_TEMPLATE(itemplate_derived4, 2)
    BOOST_IDL_EXTENDS(PROTECT((itemplate_derived1<T, U>)))
    BOOST_IDL_EXTENDS(PROTECT((itemplate_derived5<T, U>)))
BOOST_IDL_END_TEMPLATE(itemplate_derived4, 2)

template<int N>
struct ctemplate_first {
    void one(int i)
    { 
        counter< method<ctemplate_first<N>, func_one_tag, sig_a_tag> >::inc();
    }
    void one(const char*) 
    { 
        counter< method<ctemplate_first<N>, func_one_tag, sig_b_tag> >::inc();
    }
    void two(int, const char*) 
    { 
        counter< method<ctemplate_first<N>, func_two_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct ctemplate_second {
    void three(int)
    { 
        counter< method<ctemplate_second<N>, func_three_tag, sig_a_tag> >::inc(); 
    }
    void three(const char*) 
    { 
        counter< method<ctemplate_second<N>, func_three_tag, sig_b_tag> >::inc();
    }
    void four(int, const char*) 
    { 
        counter< method<ctemplate_second<N>, func_four_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct ctemplate_third {
    void five(int)
    { 
        counter< method<ctemplate_third<N>, func_five_tag, sig_a_tag> >::inc(); 
    }
    void five(const char*) 
    { 
        counter< method<ctemplate_third<N>, func_five_tag, sig_b_tag> >::inc();
    }
    void six(int, const char*) 
    { 
        counter< method<ctemplate_third<N>, func_six_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct ctemplate_derived1 { 
    void one(int i)
    { 
        counter< method<ctemplate_derived1<N>, func_one_tag, sig_a_tag> >::inc(); 
    }
    void one(const char*) 
    { 
        counter< method<ctemplate_derived1<N>, func_one_tag, sig_b_tag> >::inc();
    }
    void two(int, const char*) 
    { 
        counter< method<ctemplate_derived1<N>, func_two_tag, sig_c_tag> >::inc();
    }
    void three(int)
    { 
        counter< method<ctemplate_derived1<N>, func_three_tag, sig_a_tag> >::inc(); 
    }
    void three(const char*) 
    { 
        counter< method<ctemplate_derived1<N>, func_three_tag, sig_b_tag> >::inc();
    }
    void four(int, const char*) 
    { 
        counter< method<ctemplate_derived1<N>, func_four_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct ctemplate_derived2 { 
    void one(int i)
    { 
        counter< method<ctemplate_derived2<N>, func_one_tag, sig_a_tag> >::inc(); 
    }
    void one(const char*) 
    { 
        counter< method<ctemplate_derived2<N>, func_one_tag, sig_b_tag> >::inc();
    }
    void two(int, const char*) 
    { 
        counter< method<ctemplate_derived2<N>, func_two_tag, sig_c_tag> >::inc();
    }
};

template<int N>
struct ctemplate_derived4 {
    void one(int i)
    { 
        counter< method<ctemplate_derived4<N>, func_one_tag, sig_a_tag> >::inc(); 
    }
    void one(const char*) 
    { 
        counter< method<ctemplate_derived4<N>, func_one_tag, sig_b_tag> >::inc();
    }
    void two(int, const char*) 
    { 
        counter< method<ctemplate_derived4<N>, func_two_tag, sig_c_tag> >::inc();
    }
    void three(int)
    { 
        counter< method<ctemplate_derived4<N>, func_three_tag, sig_a_tag> >::inc(); 
    }
    void three(const char*) 
    { 
        counter< method<ctemplate_derived4<N>, func_three_tag, sig_b_tag> >::inc();
    }
    void four(int, const char*) 
    { 
        counter< method<ctemplate_derived4<N>, func_four_tag, sig_c_tag> >::inc();
    }
    void five(int)
    { 
        counter< method<ctemplate_derived4<N>, func_five_tag, sig_a_tag> >::inc(); 
    }
    void five(const char*) 
    { 
        counter< method<ctemplate_derived4<N>, func_five_tag, sig_b_tag> >::inc();
    }
    void six(int, const char*) 
    { 
        counter< method<ctemplate_derived4<N>, func_six_tag, sig_c_tag> >::inc();
    }
    void seven(int)
    { 
        counter< method<ctemplate_derived4<N>, func_seven_tag, sig_a_tag> >::inc(); 
    }
    void seven(const char*) 
    { 
        counter< method<ctemplate_derived4<N>, func_seven_tag, sig_b_tag> >::inc();
    }
    void eight(int, const char*) 
    { 
        counter< method<ctemplate_derived4<N>, func_eight_tag, sig_c_tag> >::inc();
    }
};

} } } // End namespaces test, interfaces, boost.

#endif // #ifndef BOOST_IDL_TEST_TEMPLATE_HIERARCHIES_HPP_INCLUDED
