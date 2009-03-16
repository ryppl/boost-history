// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams

#ifndef BOOST_SYNCHRO_LOCKABLES_LOCK_HPP
#define BOOST_SYNCHRO_LOCKABLES_LOCK_HPP

#include <algorithm>
#include <iterator>

#include <boost/thread/locks.hpp>
#include <boost/synchro/lockables/detail/try_lock_internal.hpp>
#include <boost/synchro/lockables/detail/is_mutex_type_wrapper.hpp>
//#include <boost/synchro/lockables/detail/range_lock_guard.hpp>
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
        struct lock {
            typedef void result_type;
            template<typename Lockable>
            void operator()(Lockable& lockable) const {
                synchro::lockable::lock(lockable);
            }
        };
    }

    namespace detail
    {
        template<typename MutexType1,typename MutexType2>
        unsigned lock_helper(MutexType1& m1,MutexType2& m2) {
            unique_locker<MutexType1> l1(m1);
            if(!lockable::try_lock(m2))
            {
                return 1;
            }
            l1.release();
            return 0;
        }

        template<typename MutexType1,typename MutexType2,typename MutexType3>
        unsigned lock_helper(MutexType1& m1,MutexType2& m2,MutexType3& m3) {
            unique_locker<MutexType1> l1(m1);
            if(unsigned const failed_lock=try_lock_internal(m2,m3))
            {
                return failed_lock;
            }
            l1.release();
            return 0;
        }

        template<typename MutexType1,typename MutexType2,typename MutexType3,
                 typename MutexType4>
        unsigned lock_helper(MutexType1& m1,MutexType2& m2,MutexType3& m3,
                             MutexType4& m4) {
            unique_locker<MutexType1> l1(m1);
            if(unsigned const failed_lock=try_lock_internal(m2,m3,m4))
            {
                return failed_lock;
            }
            l1.release();
            return 0;
        }

        template<typename MutexType1,typename MutexType2,typename MutexType3,
                 typename MutexType4,typename MutexType5>
        unsigned lock_helper(MutexType1& m1,MutexType2& m2,MutexType3& m3,
                             MutexType4& m4,MutexType5& m5) {
            unique_locker<MutexType1> l1(m1);
            if(unsigned const failed_lock=try_lock_internal(m2,m3,m4,m5))
            {
                return failed_lock;
            }
            l1.release();
            return 0;
        }

        template<typename MutexType1,typename MutexType2>
        void lock_impl(MutexType1& m1,MutexType2& m2,is_mutex_type_wrapper<true>)
        {
            unsigned const lock_count=2;
            unsigned lock_first=0;
            while(true)
            {
                switch(lock_first)
                {
                case 0:
                    lock_first=detail::lock_helper(m1,m2);
                    if(!lock_first)
                        return;
                    break;
                case 1:
                    lock_first=detail::lock_helper(m2,m1);
                    if(!lock_first)
                        return;
                    lock_first=(lock_first+1)%lock_count;
                    break;
                }
            }
        }

        template<typename Iterator>
        void lock_impl(Iterator begin,Iterator end,is_mutex_type_wrapper<false>);
        
        template<typename Sequence>
        void lock_seq_impl(Sequence& seq);
    }

#if 0
    template <typename Sequence>
    typename boost::enable_if<fusion::traits::is_sequence<Sequence>,void>::type
    lock(Sequence& seq) {
        lock_seq_impl(seq);
    }
#endif    
    template<typename MutexType1,typename MutexType2>
    void lock(MutexType1& m1,MutexType2& m2) {
        detail::lock_impl(m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2>
    void lock(const MutexType1& m1,MutexType2& m2) {
        detail::lock_impl(m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2>
    void lock(MutexType1& m1,const MutexType2& m2) {
        detail::lock_impl(m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2>
    void lock(const MutexType1& m1,const MutexType2& m2) {
        detail::lock_impl(m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3>
    void lock(MutexType1& m1,MutexType2& m2,MutexType3& m3) {
        unsigned const lock_count=3;
        unsigned lock_first=0;
        while(true)
        {
            switch(lock_first)
            {
            case 0:
                lock_first=detail::lock_helper(m1,m2,m3);
                if(!lock_first)
                    return;
                break;
            case 1:
                lock_first=detail::lock_helper(m2,m3,m1);
                if(!lock_first)
                    return;
                lock_first=(lock_first+1)%lock_count;
                break;
            case 2:
                lock_first=detail::lock_helper(m3,m1,m2);
                if(!lock_first)
                    return;
                lock_first=(lock_first+2)%lock_count;
                break;
            }
        }
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3,
             typename MutexType4>
    void lock(MutexType1& m1,MutexType2& m2,MutexType3& m3,
              MutexType4& m4) {
        unsigned const lock_count=4;
        unsigned lock_first=0;
        while(true)
        {
            switch(lock_first)
            {
            case 0:
                lock_first=detail::lock_helper(m1,m2,m3,m4);
                if(!lock_first)
                    return;
                break;
            case 1:
                lock_first=detail::lock_helper(m2,m3,m4,m1);
                if(!lock_first)
                    return;
                lock_first=(lock_first+1)%lock_count;
                break;
            case 2:
                lock_first=detail::lock_helper(m3,m4,m1,m2);
                if(!lock_first)
                    return;
                lock_first=(lock_first+2)%lock_count;
                break;
            case 3:
                lock_first=detail::lock_helper(m4,m1,m2,m3);
                if(!lock_first)
                    return;
                lock_first=(lock_first+3)%lock_count;
                break;
            }
        }
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3,
             typename MutexType4,typename MutexType5>
    void lock(MutexType1& m1,MutexType2& m2,MutexType3& m3,
              MutexType4& m4,MutexType5& m5) {
        unsigned const lock_count=5;
        unsigned lock_first=0;
        while(true)
        {
            switch(lock_first)
            {
            case 0:
                lock_first=detail::lock_helper(m1,m2,m3,m4,m5);
                if(!lock_first)
                    return;
                break;
            case 1:
                lock_first=detail::lock_helper(m2,m3,m4,m5,m1);
                if(!lock_first)
                    return;
                lock_first=(lock_first+1)%lock_count;
                break;
            case 2:
                lock_first=detail::lock_helper(m3,m4,m5,m1,m2);
                if(!lock_first)
                    return;
                lock_first=(lock_first+2)%lock_count;
                break;
            case 3:
                lock_first=detail::lock_helper(m4,m5,m1,m2,m3);
                if(!lock_first)
                    return;
                lock_first=(lock_first+3)%lock_count;
                break;
            case 4:
                lock_first=detail::lock_helper(m5,m1,m2,m3,m4);
                if(!lock_first)
                    return;
                lock_first=(lock_first+4)%lock_count;
                break;
            }
        }
    }

    namespace detail {
        template<typename Iterator>
        struct range_lock_guard
        {
            Iterator begin;
            Iterator end;
            
            range_lock_guard(Iterator begin_,Iterator end_):
                begin(begin_),end(end_)
            {
                lockables::lock(begin,end);
            }
            
            void release()
            {
                begin=end;
            }

            ~range_lock_guard()
            {
                for(;begin!=end;++begin)
                {
                    lockable::unlock(*begin);
                }
            }
        };
    }
    

    namespace detail
    {
        template<typename Iterator>
        void lock_impl(Iterator begin,Iterator end,is_mutex_type_wrapper<false>) {
            typedef typename std::iterator_traits<Iterator>::value_type lock_type;
        
            if(begin==end)
            {
                return;
            }
            bool start_with_begin=true;
            Iterator second=begin;
            ++second;
            Iterator next=second;
        
            for(;;)
            {
                unique_locker<lock_type> begin_lock(*begin,defer_lock);
                if(start_with_begin)
                {
                    begin_lock.lock();
                    Iterator const failed_lock=lockables::try_lock(next,end);
                    if(failed_lock==end)
                    {
                        begin_lock.release();
                        return;
                    }
                    start_with_begin=false;
                    next=failed_lock;
                }
                else
                {
                    range_lock_guard<Iterator> guard(next,end);
                    if(begin_lock.try_lock())
                    {
                        Iterator const failed_lock=lockables::try_lock(second,next);
                        if(failed_lock==next)
                        {
                            begin_lock.release();
                            guard.release();
                            return;
                        }
                        start_with_begin=false;
                        next=failed_lock;
                    }
                    else
                    {
                        start_with_begin=true;
                        next=second;
                    }
                }
            }
        }
        #if 0
        template<typename Sequence>
        void lock_seq_impl(Sequence& seq) {
            typedef typename std::iterator_traits<Iterator>::value_type lock_type;
        
            if(begin==end)
            {
                return;
            }
            bool start_with_begin=true;
            Iterator second=begin;
            ++second;
            Iterator next=second;
        
            for(;;)
            {
                unique_locker<lock_type> begin_lock(*begin,defer_lock);
                if(start_with_begin)
                {
                    begin_lock.lock();
                    Iterator const failed_lock=lockables::try_lock(next,end);
                    if(failed_lock==end)
                    {
                        begin_lock.release();
                        return;
                    }
                    start_with_begin=false;
                    next=failed_lock;
                }
                else
                {
                    range_lock_guard<Iterator> guard(next,end);
                    if(begin_lock.try_lock())
                    {
                        Iterator const failed_lock=lockables::try_lock(second,next);
                        if(failed_lock==next)
                        {
                            begin_lock.release();
                            guard.release();
                            return;
                        }
                        start_with_begin=false;
                        next=failed_lock;
                    }
                    else
                    {
                        start_with_begin=true;
                        next=second;
                    }
                }
            }
        }
        #endif
    }
    
}}}

#include <boost/config/abi_suffix.hpp>

#endif
