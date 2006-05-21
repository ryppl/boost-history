// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_TEST_ADVICE_HPP_INCLUDED
#define BOOST_IDL_TEST_ADVICE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces.hpp>
#include <boost/interfaces/advice.hpp>
#include <boost/interfaces/detail/empty_base.hpp>
#include "./counter.hpp"
#include "./method_tags.hpp"
#include "./sequencer.hpp"

#include <iostream>

namespace boost { namespace interfaces { namespace test {

BOOST_IDL_BEGIN(ipointcut)
    BOOST_IDL_FN1(one, void, (int, param))
    BOOST_IDL_FN1(two, int, (int, param))
BOOST_IDL_END(ipointcut)

struct cpointcut {
    void one(int n) 
    { 
        sequencer< method<cpointcut, func_one_tag> >::mark();
    }
    int two(int n) 
    { 
        sequencer< method<cpointcut, func_two_tag> >::mark();
        return n; 
    }
};

template<typename Id, typename Base = detail::empty_base >
struct advice1 : basic_advice<Base> {
    template<typename Tuple>
    void before(Tuple&, const char*, int, const char*[]) const
    { 
        sequencer< method<Id, before_tag> >::mark();
    }

    template<typename Tuple>
    void after(Tuple&, const char*, int, const char*[]) const
    { 
        sequencer< method<Id, after_tag> >::mark();
    }

    template<typename Tuple>
    void finally(Tuple&, const char*, int, const char*[]) const
    { 
        sequencer< method<Id, finally_tag> >::mark();
    }

    template<typename Tuple>
    void error( std::exception&, Tuple&, 
                const char*, int, const char*[] ) const
    { 
        sequencer< method<Id, error_tag> >::mark();
    }

    template<typename Tuple>
    bool cancel(Tuple&, const char*, int, const char*[]) const
    { 
        sequencer< method<Id, cancel_tag> >::mark();
        return false;
    }

    template<typename Result, typename Tuple>
    Result transform( const Result& result, Tuple&, 
                      const char*, int, const char*[] ) const
    { 
        sequencer< method<Id, transform_tag> >::mark();
        return result;
    }
};

// cancel() returns true.
template<typename Id, typename Base = detail::empty_base >
struct advice2 : basic_advice<Base> {
    template<typename Tuple>
    void before(Tuple&, const char*, int, const char*[]) const
    { 
        sequencer< method<Id, before_tag> >::mark();
    }

    template<typename Tuple>
    void after(Tuple&, const char*, int, const char*[]) const
    { 
        sequencer< method<Id, after_tag> >::mark();
    }

    template<typename Tuple>
    void finally(Tuple&, const char*, int, const char*[]) const
    { 
        sequencer< method<Id, finally_tag> >::mark();
    }

    template<typename Tuple>
    void error( std::exception&, Tuple&, 
                const char*, int, const char*[] ) const
    { 
        sequencer< method<Id, error_tag> >::mark();
    }

    template<typename Tuple>
    bool cancel(Tuple&, const char*, int, const char*[]) const
    { 
        sequencer< method<Id, cancel_tag> >::mark();
        return true;
    }

    template<typename Result, typename Tuple>
    Result transform( const Result& result, Tuple&, 
                      const char*, int, const char*[] ) const
    { 
        sequencer< method<Id, transform_tag> >::mark();
        return result;
    }
};

// transform() returns result + 1.
template<typename Id, typename Base = detail::empty_base >
struct advice3 : basic_advice<Base> {
    template<typename Tuple>
    void before(Tuple&, const char*, int, const char*[]) const
    { 
        sequencer< method<Id, before_tag> >::mark();
    }

    template<typename Tuple>
    void after(Tuple&, const char*, int, const char*[] ) const
    { 
        sequencer< method<Id, after_tag> >::mark();
    }

    template<typename Tuple>
    void finally(Tuple&, const char*, int, const char*[]) const
    { 
        sequencer< method<Id, finally_tag> >::mark();
    }

    template<typename Tuple>
    void error( std::exception&, Tuple&, 
                const char*, int, const char*[] )const
    { 
        sequencer< method<Id, error_tag> >::mark();
    }

    template<typename Tuple>
    bool cancel(Tuple&, const char*, int, const char*[]) const
    { 
        sequencer< method<Id, cancel_tag> >::mark();
        return false;
    }

    template<typename Result, typename Tuple>
    Result transform( const Result& result, Tuple&, 
                      const char*, int, const char*[] ) const
    { 
        sequencer< method<Id, transform_tag> >::mark();
        return result + 1;
    }
};

} } } // End namespaces test, interfaces, boost.

#endif // #ifndef BOOST_IDL_TEST_ADVICE_HPP_INCLUDED
