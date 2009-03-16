// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams

#ifndef BOOST_SYNCHRO_LOCKABLES_UNLOCK_HPP
#define BOOST_SYNCHRO_LOCKABLES_UNLOCK_HPP

#include <algorithm>
#include <iterator>

#include <boost/thread/locks.hpp>
#include <boost/synchro/lockables/detail/try_lock_internal.hpp>
#include <boost/synchro/lockables/detail/is_mutex_type_wrapper.hpp>
#include <boost/synchro/lockables/try_lock.hpp>
#include <boost/synchro/lockable/try_lock.hpp>
#include <boost/synchro/lockable/unlock.hpp>
#include <boost/synchro/is_lockable.hpp>
#include <boost/synchro/lockers.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/fusion/include/for_each.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockables {

    namespace fct {
        struct unlock {
            typedef void result_type;
            template<typename Lockable>
            void operator()(Lockable& lockable) const {
                synchro::lockable::unlock(lockable);
            }
        };
    }

    namespace detail
    {

        template<typename MutexType1,typename MutexType2>
        void unlock_impl(MutexType1& m1,MutexType2& m2,is_mutex_type_wrapper<true>)
        {
            lockable::unlock(m1);
            lockable::unlock(m2);
        }

        template<typename Iterator>
        void unlock_impl(Iterator begin,Iterator end,is_mutex_type_wrapper<false>);
    }

    template <typename Sequence>
    typename boost::enable_if<fusion::traits::is_sequence<Sequence>,void>::type
    unlock(Sequence& t) {
        fusion::for_each(t, fct::unlock());
    }

    template<typename MutexType1,typename MutexType2>
    void unlock(MutexType1& m1,MutexType2& m2) {
        detail::unlock_impl(m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2>
    void unlock(const MutexType1& m1,MutexType2& m2) {
        detail::unlock_impl(m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2>
    void unlock(MutexType1& m1,const MutexType2& m2) {
        detail::unlock_impl(m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2>
    void unlock(const MutexType1& m1,const MutexType2& m2) {
        detail::unlock_impl(m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3>
    void unlock(MutexType1& m1,MutexType2& m2,MutexType3& m3) {
            lockable::unlock(m1);
            lockable::unlock(m2);
            lockable::unlock(m3);
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3,
             typename MutexType4>
    void unlock(MutexType1& m1,MutexType2& m2,MutexType3& m3,
              MutexType4& m4) {
            lockable::unlock(m1);
            lockable::unlock(m2);
            lockable::unlock(m3);
            lockable::unlock(m4);
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3,
             typename MutexType4,typename MutexType5>
    void unlock(MutexType1& m1,MutexType2& m2,MutexType3& m3,
              MutexType4& m4,MutexType5& m5) {
            lockable::unlock(m1);
            lockable::unlock(m2);
            lockable::unlock(m3);
            lockable::unlock(m4);
            lockable::unlock(m5);
    }

    namespace detail
    {
        template<typename Iterator>
        void unlock_impl(Iterator begin,Iterator end,is_mutex_type_wrapper<false>) {
            typedef typename std::iterator_traits<Iterator>::value_type lock_type;
        
            for(Iterator it=begin; it!=end; ++it)
            {
                lockable::unlock(*it);
            }
        }        
    }
    
}}}

#include <boost/config/abi_suffix.hpp>

#endif
