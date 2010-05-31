//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TRANSACT_TEST_STM_DEFAULT_HPP
#define BOOST_TRANSACT_TEST_STM_DEFAULT_HPP

#include <boost/transact/simple_transaction_manager.hpp>
#include <libs/transact/test/test_stm/test_stm.hpp>


#define BOOST_TRANSACT_CONFIGURATION \
    boost::transact::simple_transaction_manager<test_stm>

#include <libs/transact/test/test_stm/object.hpp>


#endif
