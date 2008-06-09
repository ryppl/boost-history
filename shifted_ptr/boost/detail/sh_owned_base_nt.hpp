/**
	@file
	Boost detail/sh_owned_base_nt.hpp header file.

	@note
	Copyright (c) 2008 Phil Bouchard <phil@fornux.com>.

	Distributed under the Boost Software License, Version 1.0.

	See accompanying file LICENSE_1_0.txt or copy at
	http://www.boost.org/LICENSE_1_0.txt

	See http://www.boost.org/libs/smart_ptr/doc/index.html for documentation.
*/


#ifndef BOOST_DETAIL_SH_OWNED_BASE_NT_HPP_INCLUDED
#define BOOST_DETAIL_SH_OWNED_BASE_NT_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <stack>
#include <limits>

// Bypassing linkage by default
#define BOOST_SH_DISABLE_THREADS

#include <boost/thread.hpp>
#include <boost/thread/tss.hpp>
#include <boost/pool/pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/numeric/interval.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/remove_extent.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

#include <boost/detail/intrusive_list.hpp>
#include <boost/detail/intrusive_stack.hpp>
#include <boost/detail/sh_utility.h>
#include <boost/detail/sp_counted_base_nt.hpp>


namespace boost
{

namespace detail
{

namespace sh
{


class set;
class owned_base;


/**
    Syntax helper.
*/

typedef std::list< numeric::interval<unsigned>, fast_pool_allocator< numeric::interval<unsigned> > > pool_lii;


/**
    Allocator wrapper tracking allocations.
*/

struct pool : boost::pool<>,
#ifndef BOOST_SH_DISABLE_THREADS
    thread_specific_ptr<pool_lii>
#else
    std::auto_ptr<pool_lii>
#endif
{
    pool() : boost::pool<>(1)
    {
        reset(new pool_lii());
    }
    
    owned_base * top(void * p)
    {
        pool_lii::reverse_iterator i;
        
        for (i = get()->rbegin(); i != get()->rend(); i ++)
            if (in((unsigned)(p), * i))
                break;

        get()->erase(i.base(), get()->end());
        
        return (owned_base *)(i->lower());
    }
    
    void * allocate(std::size_t s)
    {
        void * p = ordered_malloc(s);
        
        get()->push_back(numeric::interval<unsigned>((unsigned) p, unsigned((char *)(p) + s)));
        
        return p;
    }

    void deallocate(void * p, std::size_t s)
    {
        pool_lii::reverse_iterator i;
        
        for (i = get()->rbegin(); i != get()->rend(); i ++)
            if (in((unsigned)(p), * i))
                break;
        
        get()->erase(i.base(), get()->end());
        free(p, s);
    }
};


template <typename T>
    class shifted_allocator;

/**
	Root class of all pointees.
*/

class owned_base : public sp_counted_base
{
    template <typename U> friend class shifted_allocator;

	intrusive_stack ptrs_;
	intrusive_list inits_;
	
public:
    intrusive_list::node set_tag_;
    intrusive_list::node init_tag_;

    owned_base()
    {
        inits_.push_back(& init_tag_); 
    }

    intrusive_stack * ptrs() 						{ return & ptrs_; }
    intrusive_list * inits()						{ return & inits_; }
    intrusive_list::node * set_tag() 				{ return & set_tag_; }
    intrusive_list::node * init_tag() 				{ return & init_tag_; }

    static pool pool_;
    
private:
    virtual void dispose() 				            {} // dummy
    virtual void * get_deleter( std::type_info const & ti ) {} // dummy
};


pool owned_base::pool_;


#define TEMPLATE_DECL(z, n, text) BOOST_PP_COMMA_IF(n) typename T ## n
#define ARGUMENT_DECL(z, n, text) BOOST_PP_COMMA_IF(n) T ## n const & t ## n
#define PARAMETER_DECL(z, n, text) BOOST_PP_COMMA_IF(n) t ## n

#define CONSTRUCT_OWNED(z, n, text)																			    \
	template <BOOST_PP_REPEAT(n, TEMPLATE_DECL, 0)>										                        \
		text(BOOST_PP_REPEAT(n, ARGUMENT_DECL, 0))                                                              \
        {                                                                                                       \
            new (element()) data_type(BOOST_PP_REPEAT(n, PARAMETER_DECL, 0));                                   \
        }																										

/**
	Object wrapper.
*/

template <typename T>
    class shifted : public owned_base
    {
        typedef T data_type;

        union
        {
            long a_;
            char p_[sizeof(data_type)];
        };
        
    public:
        class roofof;
        friend class roofof;

		shifted() 
        {
            new (element()) data_type();
        }
        
        BOOST_PP_REPEAT_FROM_TO(1, 10, CONSTRUCT_OWNED, shifted)


        data_type * element() 				{ return static_cast<data_type *>(static_cast<void *>(& p_[0])); }

        virtual ~shifted()					{ dispose(); }
        virtual void dispose() 				{ dispose(element(), is_array<data_type>()); }

        virtual void * get_deleter( std::type_info const & ti ) { return 0; } // dummy

    public:
        class roofof
        {
            shifted<data_type> * p_;

        public:
            roofof(data_type * p) : p_(sh::roofof((data_type shifted<data_type>::*)(& shifted<data_type>::p_), p)) {}
            
            operator shifted<data_type> * () const { return p_; }
        };
        
        void * operator new (size_t s)
        {
            return pool_.allocate(s);
        }
        
        void operator delete (void * p)
        {
            pool_.deallocate(p, sizeof(shifted));
        }

    private:
        template <typename U>
            static void dispose_array(U * p, const false_type &)
            {
                typedef typename remove_extent<U>::type element_type;
                
                for (element_type * i = * p; i != * p + sizeof(data_type) / sizeof(element_type); i ++)
                    dispose(i, is_array<element_type>());
            }

        template <typename U>
            static void dispose_array(U * p, const true_type &)
            {
            }

        template <typename U>
            static void dispose(U * p, const false_type &)
            {
                p->~U();
            }
        
        template <typename U>
            static void dispose(U * p, const true_type &)
            {
                dispose_array(p, has_trivial_destructor<data_type>());
            }
    };


template <>
    class shifted<void> : public owned_base
    {
        typedef void data_type;

        long p_;

        shifted();

    public:
        class roofof;
        friend class roofof;

        data_type * element() 				{ return & p_; }

        virtual ~shifted()					{}
        virtual void dispose() 				{}

        virtual void * get_deleter( std::type_info const & ti ) {}

    public:
        class roofof
        {
            shifted<data_type> * p_;

        public:
            roofof(data_type * p) : p_(sh::roofof((long shifted<data_type>::*)(& shifted<data_type>::p_), static_cast<long *>(p))) {}
            
            operator shifted<data_type> * () const { return p_; }
        };
    };


/**
    STL compliant allocator.
*/

//! FIXME
template <typename T>
    class shifted_allocator
    {
    public:
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
        typedef T *         pointer;
        typedef const T *   const_pointer;
        typedef T &         reference;
        typedef const T &   const_reference;
        typedef T           value_type;

        template <typename U>
            struct rebind
            { 
                typedef shifted_allocator<U> other; 
            };

        shifted_allocator() throw()                                 {}
        shifted_allocator(const shifted_allocator &) throw()        {}
        template <typename U>
            shifted_allocator(const shifted_allocator<U> &) throw() {}

        ~shifted_allocator() throw()                                {}
        pointer address(reference x) const                          { return & x; }
        const_pointer address(const_reference x) const              { return & x; }

        pointer allocate(size_type s, const void * = 0)
        { 
            void * q = shifted<T>::operator new(s * sizeof(T));
            
            // only T's constructor will be called so take care of the rest
            return static_cast<shifted<T> *>(new (q) owned_base)->element(); 
        }

        void deallocate(pointer p, size_type)
        { 
            owned_base * q = shifted<T>::roofof(p);
            
            // T's destructor already called so let's handle ~owned_base()
            q->owned_base::~owned_base();
            
            shifted<T>::operator delete(q); 
        }

        //! FIXME
        size_type max_size() const throw() 
        { 
            return size_t(-1) / sizeof(T); 
        }

        void construct(pointer p, const T & x) 
        { 
            ::new (p) T(x); 
        }

        void destroy(pointer p) 
        { 
            p->~T(); 
        }
    };

template <typename T>
    inline bool operator == (const shifted_allocator<T> &, const shifted_allocator<T> &)
    { 
        return true; 
    }

template <typename T>
    inline bool operator != (const shifted_allocator<T> &, const shifted_allocator<T> &)
    { 
        return false; 
    }


} // namespace sh

} // namespace detail

} // namespace boost


#endif  // #ifndef BOOST_DETAIL_SH_OWNED_BASE_NT_HPP_INCLUDED
