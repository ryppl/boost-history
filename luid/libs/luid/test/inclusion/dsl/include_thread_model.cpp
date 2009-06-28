/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */
 
#include <boost/dsl/thread_model.hpp>

#include <boost/static_assert.hpp>

namespace bldsl = ::boost::dsl;

// ------------------------------------------------------------------------



BOOST_STATIC_ASSERT((
        bldsl::is_externally_locked<bldsl::externally_locked<> >::value
));
BOOST_STATIC_ASSERT((
        bldsl::is_internally_locked<bldsl::internally_locked<> >::value
));
BOOST_STATIC_ASSERT((
        bldsl::is_no_locking<bldsl::no_locking<> >::value
));
BOOST_STATIC_ASSERT((
        bldsl::is_mono_threaded<bldsl::mono_threaded<> >::value
));
BOOST_STATIC_ASSERT((
        bldsl::is_multi_threaded<bldsl::multi_threaded<> >::value
));

BOOST_STATIC_ASSERT((
        bldsl::is_multi_process<bldsl::multi_process<> >::value
));



void
test_dsl_thread_model(void)
{
    typedef bldsl::externally_locked<>::locking_traits l1;
    typedef bldsl::internally_locked<>::locking_traits l2;
    typedef bldsl::no_locking<>::locking_traits l3;

    typedef bldsl::mono_threaded<>::locking_strategy l4;
    typedef bldsl::multi_threaded<>::locking_strategy l5;
    typedef bldsl::multi_process<>::locking_strategy l6;
}
