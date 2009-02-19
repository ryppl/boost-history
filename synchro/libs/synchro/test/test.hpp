/* Copyright 2008-2009 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_TEST_TEST_HPP_
#define BOOST_LUID_TEST_TEST_HPP_

#include <boost/test/unit_test.hpp>
#include <boost/luid/generator.hpp>
#include <set>

using namespace boost;
using namespace boost::mpl;
namespace bip =boost::interprocess;
namespace bdsl =boost::dsl;

namespace bluid = boost::luid;

template <typename G>
void test_inmediate(G& g) {
    typename G::numeric_type id1= g.make();
    g.release(id1);
    typename G::numeric_type id2= g.make();
    BOOST_CHECK(id1==id2);
    g.clear();
}

template <typename G>
void test_fifo(G& g) {
    typename G::numeric_type id1= g.make();
    typename G::numeric_type id2= g.make();
    typename G::numeric_type id3= g.make();
    g.release(id2);
    g.release(id3);
    g.release(id1);
    typename G::numeric_type id4= g.make();
    BOOST_CHECK(id4==id2);
    typename G::numeric_type id5= g.make();
    BOOST_CHECK(id5==id3);
    typename G::numeric_type id6= g.make();
    BOOST_CHECK(id6==id1);
    g.clear();
}

template <typename G>
void test_resize(G& g, unsigned int d, unsigned int u) {
        typename G::numeric_type i;
        for (i =g.lower_bound(); i<d; ++i) {
            BOOST_CHECK(g.make()!=g.error());
        }
        std::cout <<    "test_resize # " << d << std::endl;
        g.set_upper_bound(u);
        std::cout <<    "test_resize # " << g.upper_bound() << std::endl;
        for (;i<g.upper_bound(); ++i) {
            BOOST_CHECK(g.make()!=g.error());
        }
        std::cout <<    "test_resize # " << u << std::endl;
//        BOOST_CHECK(g.make()==g.error());
        g.clear();
}

template <typename G, typename D>
void test_delay(G& g, D d) {
//    std::cout <<    "T." <<++i<<  " test_delay " << std::endl;
    typename G::numeric_type id1= g.make();
    g.release(id1);
//    std::cout <<    "test_delay # " << g.count() << std::endl;
    typename G::numeric_type id2= g.make();
    BOOST_CHECK(id1!=id2);
//    thread::sleep(boost::posix_time::second_clock::local_time()+posix_time::time_duration(0,0,d,0));
    sleep(d);
    typename G::numeric_type id3= g.make();
    BOOST_CHECK(id1==id3);
//    std::cout <<    "test_delay # " << g.count() << std::endl;
    g.clear();
//    g.release(id2);
//    g.release(id3);
}

template <typename G>
void test_unique(G& g) {
//    std::cout <<    "T." <<++i<<  " test_unique " << std::endl;

//    typedef std::set<> used_type;
    typedef std::set<typename G::numeric_type> used_type;
    used_type used;
    typename G::numeric_type id;
    for (typename G::numeric_type i =g.lower_bound(); i<g.upper_bound(); ++i) {
        id=g.make();
        if(id==g.error()) {
            std::cout <<    "FAIL  " << __LINE__ << std::endl;
            BOOST_FAIL("unique");
        }
//        std::cout <<    "SUCCEED  " << id << std::endl;
        BOOST_CHECK(used.count(id)==0);
         used.insert(id);
    }
//    std::cout <<    "inserted test_unique " << std::endl;
//    for (typename used_type::iterator  it2=used.begin(), e=used.upper_bound()
//            ; it2!=e
//            ; ++it2) {
//        g.release(*it2);
//    }
//    std::cout <<    "END test_unique " << std::endl;
    g.clear();
}

template <typename G>
void test_overflow(G& g) {
//    std::cout <<    "T." <<++i<<  " test_overflow " << std::endl;
    typename G::numeric_type i;
    typename G::numeric_type id;
//    std::cout <<    "make test_overflow  " << int(g.upper_bound()) << std::endl;
    for (i =g.lower_bound(); i<g.upper_bound(); ++i) {
        id =g.make();
        if(id==g.error()) {
            std::cout <<    "FAIL  " << __LINE__ << std::endl;
            BOOST_FAIL("overflow");
        }
//        std::cout <<    "SUCCED  " << __LINE__ << std::endl;

    }
//    std::cout <<    "check test_overflow " << std::endl;
    for (i =0; i<10; ++i) {
        BOOST_CHECK(g.make()==g.error());
    }
//    std::cout <<    "end test_overflow " << std::endl;
    g.clear();
}

template <typename G>
void test_discard(G& g) {
//    std::cout <<    "T." <<++i<<  " test_overflow " << std::endl;
    typename G::numeric_type i;
    typename G::numeric_type id;
//    std::cout <<    "make test_overflow  " << int(g.upper_bound()) << std::endl;
    for (i =g.lower_bound(); i<g.upper_bound(); ++i) {
        id =g.make();
        if(id==g.error()) {
            std::cout <<    "FAIL  " << __LINE__ << std::endl;
            BOOST_FAIL("overflow");
        }
//        std::cout <<    "SUCCED  " << __LINE__ << std::endl;

    }
    g.release(id);

    typename G::numeric_type id2= g.make();
    BOOST_CHECK(id2==g.error());

    g.clear();
}


template <typename G>
void test_overflow_throw(G& g) {
//    std::cout <<    "T." <<++i<<  " test_overflow_throw " << std::endl;
    typename G::numeric_type i;
    for (i =g.lower_bound(); i<g.upper_bound(); ++i) {
        g.make();
    }
    for (i =0; i<10; ++i) {
        try {
            (void)g.make();
            BOOST_CHECK(false);
        } catch(...) {
        }
    }
}

template <typename G>
void test_check(G& g) {
//    std::cout <<    "T." <<++i<<  " test_check " << std::endl;
    typename G::numeric_type id1= g.make();
    g.release(id1);
//    std::cout <<    "test_check2 " << std::endl;

    try {
        g.release(id1);
        BOOST_CHECK(false);
    } catch(...) {
    }
//    std::cout <<    "test_check # " << g.count() << std::endl;

    g.clear();
}

#endif
