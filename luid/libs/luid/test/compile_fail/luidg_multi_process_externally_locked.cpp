/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#include <boost/luid/generator.hpp>
using namespace boost;
namespace bip =boost::interprocess;
namespace bdsl =boost::dsl;

//____________________________________________________________________________//

int test { 
    bip::interprocess_mutex            mutex;
    bip::scoped_lock<bip::interprocess_mutex>    lock(mutex);
    luid::make_luidg<
        bluid::dsl::multi_process<
            bluid::dsl::externally_locked<> 
        > 
    >::type luidg( _high=10, _ext_mtx=mutex );

    luidg.clear();
    luidg.release(luidg.make());
    luidg.set_upper_bound(luidg.upper_bound());
    luidg.set_upper_bound(luidg.lower_bound());
    luidg.set_upper_bound(luidg.count());
    luidg.set_upper_bound(luidg.error());
}

//____________________________________________________________________________//

