// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_TEST_SEQUENCER_HPP_INCLUDED
#define BOOST_IDL_TEST_SEQUENCER_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/assert.hpp>

namespace boost { namespace interfaces { namespace test {

template<typename Dummy = int>
struct sequencer_base {
    static int value_;
};

template<typename Dummy>
int sequencer_base<Dummy>::value_;

template<typename Sequenced>
class sequencer {
public:
    static int value() 
    {  
        int result = value_;
        value_ = -1;
        return result; 
    }
    static void mark() 
    {
        BOOST_ASSERT(value_ == -1);
        value_ = sequencer_base<>::value_++;
    }
    static bool marked() { return value_ != -1; }
private:
    static int value_;
};

template<typename Sequenced>
int sequencer<Sequenced>::value_ = -1;

} } } // End namespace test, interfaces, boost.

#endif // #ifndef BOOST_IDL_TEST_SEQUENCER_HPP_INCLUDED
