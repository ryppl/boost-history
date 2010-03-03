/*=============================================================================
    Copyright (c) 2010 Daniel James

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

// This header defines a class which can will manage quickbook rules for a
// grammar class so that it doesn't have to declare every rule it'll use.
//
// TODO: Noncopyable, but some sort of reference counting scheme would work.

#if !defined(BOOST_SPIRIT_QUICKBOOK_RULE_STORE_HPP)
#define BOOST_SPIRIT_QUICKBOOK_RULE_STORE_HPP

#include <deque>
#include <boost/assert.hpp>

namespace quickbook
{
    namespace detail
    {
        template <typename T>
        void delete_impl(void* ptr) {
            delete static_cast<T*>(ptr);
        }
        
        struct scoped_void
        {
            void* ptr_;
            void (*del_)(void* x);
            
            scoped_void() : ptr_(0), del_(0) {}
            scoped_void(scoped_void const& src) : ptr_(0), del_(0) {
                BOOST_ASSERT(!src.ptr_);
            }
            ~scoped_void() {
                if(ptr_) del_(ptr_);
            }
            
            template <typename T>
            T* instantiate() {
                T* obj;
            
                del_ = &delete_impl<T>;
                ptr_ = obj = new T();
                return obj;
            }
        private:
            scoped_void& operator=(scoped_void const&);
        };
    }
    
    struct rule_store
    {
        struct instantiate
        {
            rule_store& s;
            instantiate(rule_store& s) : s(s) {}
            
            template <typename T>
            operator T&() {
                s.store_.push_back(detail::scoped_void());
                return *s.store_.back().instantiate<T>();
            }
        };

        rule_store() {}

        instantiate create() {
            return instantiate(*this);
        }

        std::deque<detail::scoped_void> store_;
    private:
        rule_store& operator=(rule_store const&);
        rule_store(rule_store const&);
    };
}

#endif
