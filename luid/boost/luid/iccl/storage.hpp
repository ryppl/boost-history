/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_STORAGE_HPP_
#define BOOST_LUID_STORAGE_HPP_

#include "boost/pointee.hpp"
#include <boost/detail/allocator_utilities.hpp>
#include "boost/interprocess/managed_shared_memory.hpp"
#include "boost/interprocess/allocators/allocator.hpp"
#include "boost/mpl/void.hpp"
#include "boost/parameter.hpp"
#include <boost/utility/base_from_member.hpp>

BOOST_PARAMETER_NAME(shm_size)    // Note: no semicolon
BOOST_PARAMETER_NAME(shm_name)    // Note: no semicolon

namespace boost
{
namespace luid
{
namespace iccl
{

template <typename Inner>
struct shm_storage {
    typedef Inner inner_type;
    typedef typename inner_type::config config;
    
    typedef typename inner_type::node_type node_type;
    typedef typename inner_type::header_type header_type;
    struct type :  inner_type::type
    {
        typedef typename inner_type::type super_type;
        typedef typename config::allocator_type allocator_type;
        typedef typename config::header_type header_type;
//TODO        typedef typename allocator_type::segment_manager segment_manager_type;
        typedef interprocess::managed_shared_memory segment_manager_type;
        
        // forwards construction
        template <typename ArgsParam>              
        type(const ArgsParam& args) 
        : super_type(args)
        , shm_size_(args[_shm_size|1000])
        , shm_name_(args[_shm_name|"aa"])
        , segment_(interprocess::open_or_create, shm_name_, shm_size_)
        , allocator_(segment_.get_segment_manager())
        , header_(segment_.construct<header_type>("header")(args, allocator_))
        //TODO , header_(segment_.construct<Header>(interprocess::anonymous_instance)(args))
        {}
    
        ~type() {
            // if the lifetime is process
            segment_.destroy<header_type>("header");
            //TODO         segment_.destroy<header_type>(header_);
            interprocess::shared_memory_object::remove(shm_name_);
        }
        header_type& header() {
            return *header_;
        }
        const header_type& header() const {
            return *header_;
        }
    
        // allocator functions
        allocator_type& allocator() {
            return allocator_;
        }

        
//    private:
        const unsigned int shm_size_;
        const char *shm_name_;
        
        segment_manager_type segment_;
        allocator_type allocator_; 
        header_type* header_;
    };
};
template <typename Inner>
struct raw_storage {
    typedef Inner inner_type;
    typedef typename inner_type::config config;

    typedef typename inner_type::node_type node_type;
    typedef typename inner_type::header_type header_type;
    struct type : inner_type::type
    {
        typedef typename inner_type::type super_type;
        typedef typename config::allocator_type allocator_type;
        typedef typename config::header_type header_type;

        // forwards construction
        template <typename ArgsParam>              
        type(const ArgsParam& args) 
        : super_type(args)
        , allocator_()
        , header_(new header_type(args, allocator_))
        {}

        ~type() {
            delete header_;
        }

        header_type& header() {
            return *header_;
        }
        const header_type& header() const {
            return *header_;
        }
    
        // allocator functions
        allocator_type& allocator() {
            return allocator_;
        }

//    private:
        allocator_type allocator_; 
        header_type* header_;
    };
};
template <typename Inner>
struct storage {
    typedef Inner inner_type;
    typedef typename inner_type::config config;
    typedef typename inner_type::node_type node_type;
    typedef typename inner_type::header_type header_type;
    struct type : inner_type::type
    {
        typedef typename inner_type::type super_type;
        typedef typename config::allocator_type allocator_type;
        typedef typename config::node_type node_type;
        typedef typename config::node_pointer_type node_pointer_type;
        typedef typename config::node_type node_type;
        typedef typename config::allocator_type allocator_type;
        typedef typename config::header_type header_type; 
        
    
        // constructor
        template <typename ArgsParam>
        type(const ArgsParam& args)
        : super_type(args) {}
    
        template <typename TPTR>
        TPTR allocate() {
            typedef typename boost::pointee<TPTR>::type T;
            typedef typename boost::detail::allocator::rebind_to<
                        allocator_type,T>::type allocator_type;
            allocator_type allo(super_type::allocator());
            return allo.allocate(1);
        }
    
        template <typename TPTR>
        void deallocate(TPTR t) {
            typedef typename boost::pointee<TPTR>::type T;
            typedef typename boost::detail::allocator::rebind_to<
                allocator_type,    T>::type allocator_type;
            typedef typename allocator_type::pointer node_pointer;
            allocator_type allo(super_type::allocator());
            allo.deallocate(static_cast<node_pointer>(t), 1);
        }

        void deallocate_node(node_pointer_type n) {
              deallocate<node_pointer_type>(n);
        }
        
        node_pointer_type allocate_node() {
            return allocate<node_pointer_type>();
        }

    };
};



} // iccl
} // luid
} // boost
#endif // BOOST_LUID_STORAGE_HPP_
