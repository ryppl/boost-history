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

typedef std::list< numeric::interval<int>, fast_pool_allocator< numeric::interval<int> > > pool_lii;


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
	static const size_t chunk_size_ = sizeof(int);

    pool() : boost::pool<>(chunk_size_)
    {
        reset(new pool_lii());
    }
    
    owned_base * top(void * p)
    {
        pool_lii::reverse_iterator i;
        
        for (i = get()->rbegin(); i != get()->rend(); i ++)
            if (in((int)(p), * i))
                break;

        get()->erase(i.base(), get()->end());
        
        return (owned_base *)(i->lower());
    }
    
    void * allocate(std::size_t s)
    {
        void * p = ordered_malloc(s / chunk_size_ + 1);
        
        get()->push_back(numeric::interval<int>((int) p, int((char *)(p) + s)));
        
        return p;
    }

    void deallocate(void * p, std::size_t s)
    {
        pool_lii::reverse_iterator i;
        
        for (i = get()->rbegin(); i != get()->rend(); i ++)
            if (in((int)(p), * i))
                break;
        
        get()->erase(i.base(), get()->end());
        free(p, s);
    }
};


/**
	Root class of all pointees.
*/

class owned_base : public sp_counted_base
{
    bool init_;
	intrusive_stack ptrs_;
	intrusive_list inits_;

protected:
    virtual void dispose() 				                    {} // dummy
    virtual void * get_deleter( std::type_info const & ti ) { return 0; } // dummy

public:
    intrusive_list::node set_tag_;
    intrusive_list::node init_tag_;

    owned_base() : init_(false)
    {
        inits_.push_back(& init_tag_); 
    }

    bool init()                                     { return init_; }
    void init(bool i)                               { init_ = i; }
    intrusive_stack * ptrs() 						{ return & ptrs_; }
    intrusive_list * inits()						{ return & inits_; }
    intrusive_list::node * set_tag() 				{ return & set_tag_; }
    intrusive_list::node * init_tag() 				{ return & init_tag_; }

    static pool pool_;
};


pool owned_base::pool_;


#define TEMPLATE_DECL(z, n, text) BOOST_PP_COMMA_IF(n) typename T ## n
#define ARGUMENT_DECL(z, n, text) BOOST_PP_COMMA_IF(n) T ## n const & t ## n
#define PARAMETER_DECL(z, n, text) BOOST_PP_COMMA_IF(n) t ## n

#define CONSTRUCT_OWNED(z, n, text)																			    \
	template <BOOST_PP_REPEAT(n, TEMPLATE_DECL, 0)>										                        \
		text(BOOST_PP_REPEAT(n, ARGUMENT_DECL, 0)) : elem_(BOOST_PP_REPEAT(n, PARAMETER_DECL, 0)) {}																										

/**
	Object wrapper.
*/

template <typename T>
    class shifted : public owned_base
    {
        typedef T data_type;

        T elem_; // need alignas<long>
        
    public:
        class roofof;
        friend class roofof;

		shifted() : elem_() 
        {
        }

        BOOST_PP_REPEAT_FROM_TO(1, 10, CONSTRUCT_OWNED, shifted)


        data_type * element() 				{ return & elem_; }
        operator data_type & ()             { return * element(); }
        operator data_type const & () const { return * element(); }

        virtual ~shifted()					
        { 
            dispose(); 
        }

    public:
        class roofof
        {
            shifted<data_type> * p_;

        public:
            roofof(data_type * p) : p_(sh::roofof((data_type shifted<data_type>::*)(& shifted<data_type>::elem_), p)) {}
            
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
    };


template <>
    class shifted<void> : public owned_base
    {
        typedef void data_type;

        long elem_;

        shifted();

    public:
        class roofof;
        friend class roofof;

        data_type * element() 				{ return & elem_; }

        virtual ~shifted()					{}
        virtual void dispose() 				{}

        virtual void * get_deleter( std::type_info const & ti ) {}

    public:
        class roofof
        {
            shifted<data_type> * p_;

        public:
            roofof(data_type * p) : p_(sh::roofof((long shifted<data_type>::*)(& shifted<data_type>::elem_), static_cast<long *>(p))) {}
            
            operator shifted<data_type> * () const { return p_; }
        };
    };


} // namespace sh

} // namespace detail

} // namespace boost


#endif  // #ifndef BOOST_DETAIL_SH_OWNED_BASE_NT_HPP_INCLUDED
