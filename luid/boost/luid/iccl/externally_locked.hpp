/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUIDG_ICCL_EXTERNALLY_LOCKED_HPP_
#define BOOST_LUIDG_ICCL_EXTERNALLY_LOCKED_HPP_

#include "boost/mpl/push_front.hpp"
#include "boost/parameter.hpp"
#include "boost/thread/mutex.hpp"
// declare the tag for the mtx named parameter
BOOST_PARAMETER_NAME(ext_mtx)    // Note: no semicolon

namespace boost
{
    template <typename Lock>
    class strict_lock {
        public:
        bool same_lock(Lock& mtx) { return true;}
        
    };
namespace luid
{
 
struct bad_lock : std::exception {
    virtual ~bad_lock() throw() {}
    virtual const char* what() const throw() {
      return "bad_lock";
    }
}; 
    
namespace iccl
{

template <typename Inner>
struct externally_locked
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
            , mutex_(args[_ext_mtx]) 
            {}
   
        mutex_type& mutex() { return mutex_;}
        const mutex_type& mutex() const { return mutex_;}
        mutex_type& mutex_;

    };

    struct type :  protected inner_type::type
    {
        typedef typename inner_type::type super_type;
        typedef typename super_type::type facade_type;
        typedef typename config::numeric_type numeric_type;
        typedef typename config::locking_traits locking_traits;
        typedef typename locking_traits::mutex_type mutex_type;
        typedef typename locking_traits::scoped_lock scoped_lock;
    
        mutex_type& mutex() { return super_type::header().mutex();}
        const mutex_type& mutex() const { return super_type::header().mutex();}
        
        template <typename ArgsParam>              
        type(const ArgsParam& args) 
        : super_type(args) {}
    
        facade_type& get(scoped_lock& locker) { 
            if (locker &&  (locker.mutex()==&mutex())) {
                return super_type::facade();
            } else {
                throw bad_lock();
            }
        }
        const facade_type& get(scoped_lock& locker) const { 
            if (locker &&  (locker.mutex()==&mutex())) {
                return super_type::facade();
            } else {
                throw bad_lock();
            }
        }
    };
};

}
}
}
#endif // BOOST_LUIDG_ICCL_EXTERNALLY_LOCKED_HPP_
