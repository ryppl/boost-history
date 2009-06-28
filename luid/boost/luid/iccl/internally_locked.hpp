/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_INTERNALLY_LOCKED_HPP_
#define BOOST_LUID_INTERNALLY_LOCKED_HPP_

#include "boost/thread/mutex.hpp"

namespace boost {
namespace luid {
namespace iccl {

template <typename Inner>
struct internally_locked
{
    typedef Inner inner_type;
    typedef typename inner_type::config config;
    typedef typename inner_type::node_type node_type;

    struct header_type :  inner_type::header_type
    {
        typedef typename inner_type::header_type super_type;
        typedef typename config::locking_traits locking_traits;
        typedef typename locking_traits::mutex_type mutex_type;
        typedef typename config::allocator_type allocator_type;

        template <typename ArgsParam>
        header_type(const ArgsParam& args, allocator_type& allo) 
            : super_type(args, allo)
            , mutex_() 
            {}
   
        mutex_type& mutex() { return mutex_;}
        const mutex_type& mutex() const { return mutex_;}
        mutable mutex_type mutex_;
    };

    struct type : inner_type::type
    {
        typedef typename inner_type::type super_type;
        typedef typename config::numeric_type numeric_type;
        typedef typename config::locking_traits 
                    locking_traits;
        typedef typename locking_traits::mutex_type mutex_type;
        typedef typename locking_traits::scoped_lock scoped_lock;
    
        mutex_type& mutex() { return super_type::header().mutex();}
        const mutex_type& mutex() const { return super_type::header().mutex();}
        
        template <typename ArgsParam>              
        type(const ArgsParam& args) 
        : super_type(args) {}
    
        numeric_type make() {
            scoped_lock g(mutex());
            return super_type::make();
        }
        void release(numeric_type n) {
            scoped_lock g(mutex());
            super_type::release(n);
        }
        numeric_type set_upper_bound(numeric_type upper) {
            scoped_lock g(mutex());
            return super_type::set_upper_bound(upper);
        }
        
        numeric_type count() const {
            scoped_lock g(mutex());
            return super_type::count();
        }
        
        void clear() {
            scoped_lock g(mutex());
            super_type::clear();
        }
    };
};

} } }
#endif // BOOST_LUID_INTERNALLY_LOCKED_HPP_
