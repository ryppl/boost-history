//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_CONDITION_BACKDOOR__HPP
#define BOOST_SYNCHRO_CONDITION_BACKDOOR__HPP

#include <boost/synchro/lockable_concepts.hpp>
#include <boost/thread/condition.hpp>
#include <boost/synchro/thread/mutex.hpp>
#include <boost/synchro/condition_safe.hpp>

namespace boost { namespace synchro {
 
template <
    class Condition
>
struct condition_backdoor {
	condition_backdoor(condition_safe<Condition>&cnd): that_(cnd) {}
    template <typename Locker, typename Predicate>
    void wait_when(Locker& lock, Predicate pred){
    	that_.wait_when(lock, pred);
    }
    template <typename Locker>
    void wait(Locker& lock) {
    	that_.wait(lock);
    }
    template <typename Locker>
    bool wait_until(Locker& lock, boost::system_time  const&  abs_time) {
    	return that_.wait_until(lock, abs_time);
    }

    template<typename Locker, typename duration_type>
    bool wait_for(Locker& lock, duration_type const& rel_time) {
    	return that_.wait_for(lock, rel_time);
    }

    template<typename Locker, typename predicate_type>
    bool wait_when_until(Locker& lock, predicate_type pred, boost::system_time const& abs_time) {
    	return that_.timed_wait(lock, pred, abs_time);
    }
    template <typename Locker>
    void notify_one(Locker& lock) {
    	that_.notify_one(lock);
    }
    template <typename Locker>
    void notify_all(Locker& lock) {
    	that_.notify_all(lock);
    }
private:
	condition_safe<Condition>& that_;
};

}
}
#endif
