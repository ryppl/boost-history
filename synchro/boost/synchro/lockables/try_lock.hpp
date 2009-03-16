// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams

#ifndef BOOST_SYNCHRO_LOCKABLES_TRY_LOCK_HPP
#define BOOST_SYNCHRO_LOCKABLES_TRY_LOCK_HPP

#include <algorithm>
#include <iterator>

#include <boost/thread/locks.hpp>
#include <boost/synchro/lockables/detail/try_lock_internal.hpp>
#include <boost/synchro/lockables/detail/is_mutex_type_wrapper.hpp>
#include <boost/synchro/is_lockable.hpp>
#include <boost/synchro/lockers.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockables {

    namespace fct {
        struct try_lock {
            typedef bool result_type;
            template<typename Lockable>
            void operator()(Lockable& lockable) const {
                synchro::lockable::try_lock(lockable);
            }
        };
    }

    namespace result_of {
        namespace detail {
            template<typename Mutex,bool x=is_mutex_type<Mutex>::value>
            struct try_lock_impl {
                typedef int type;
            };
            
            template<typename Iterator>
            struct try_lock_impl<Iterator,false> {
                typedef Iterator type;
            };
        }
        template<typename T>
        struct try_lock {
            typedef typename result_of::detail::try_lock_impl<T, is_mutex_type<T>::value>::type type;
        };
    }    
    
    namespace detail
    {

        template<typename MutexType1,typename MutexType2>
        int try_lock_impl(MutexType1& m1,MutexType2& m2,is_mutex_type_wrapper<true>)
        {
            return ((int)detail::try_lock_internal(m1,m2))-1;
        }

        template<typename Iterator>
        Iterator try_lock_impl(Iterator begin,Iterator end,is_mutex_type_wrapper<false>);
    }
    
    template<typename MutexType1,typename MutexType2>
    typename result_of::try_lock<MutexType1>::type try_lock(MutexType1& m1,MutexType2& m2)
    {
        return detail::try_lock_impl(m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2>
    typename result_of::try_lock<MutexType1>::type try_lock(const MutexType1& m1,MutexType2& m2)
    {
        return detail::try_lock_impl(m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2>
    typename result_of::try_lock<MutexType1>::type try_lock(MutexType1& m1,const MutexType2& m2)
    {
        return detail::try_lock_impl(m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2>
    typename result_of::try_lock<MutexType1>::type try_lock(const MutexType1& m1,const MutexType2& m2)
    {
        return detail::try_lock_impl(m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3>
    int try_lock(MutexType1& m1,MutexType2& m2,MutexType3& m3)
    {
        return ((int)detail::try_lock_internal(m1,m2,m3))-1;
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3,typename MutexType4>
    int try_lock(MutexType1& m1,MutexType2& m2,MutexType3& m3,MutexType4& m4)
    {
        return ((int)detail::try_lock_internal(m1,m2,m3,m4))-1;
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3,typename MutexType4,typename MutexType5>
    int try_lock(MutexType1& m1,MutexType2& m2,MutexType3& m3,MutexType4& m4,MutexType5& m5)
    {
        return ((int)detail::try_lock_internal(m1,m2,m3,m4,m5))-1;
    }
    

    namespace detail {

        template<typename Iterator>
        Iterator try_lock_impl(Iterator begin,Iterator end,is_mutex_type_wrapper<false>)

        {
            if(begin==end)
            {
                return end;
            }
            typedef typename std::iterator_traits<Iterator>::value_type lock_type;
            unique_locker<lock_type> guard(*begin,try_to_lock);
            
            if(!guard.owns_lock())
            {
                return begin;
            }
            Iterator const failed=lockables::try_lock(++begin,end);
            if(failed==end)
            {
                guard.release();
            }
            
            return failed;
        }
    }
    
}}}

#include <boost/config/abi_suffix.hpp>

#endif
