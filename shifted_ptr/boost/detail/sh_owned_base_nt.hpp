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
#include <utility>

// Bypassing linkage by default
#define BOOST_SH_DISABLE_THREADS

#include <boost/thread.hpp>
#include <boost/thread/tss.hpp>
#include <boost/pool/pool.hpp>
#include <boost/pool/pool_alloc.hpp>
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
    Allocator wrapper tracking allocations.
*/

struct pool : boost::pool<>
{
    typedef std::list< std::pair<char *, char *>, fast_pool_allocator< std::pair<char *, char *> > > lpp;


    pool() : boost::pool<>(1)
    {
        alloc_.reset(new lpp());
        constr_.reset(new lpp());
    }

    /**
        @brief
        This function returns the most recent allocation block that contains p.

        @note
        Every block allocated after p is discarded.
    */

    owned_base * top(void * p)
    {
        char * const q = static_cast<char *>(p);

        lpp::reverse_iterator i;
        for (i = alloc_.get()->rbegin(); i != alloc_.get()->rend(); i ++)
            if (i->first <= q && q <= i->second)
                break;

//std::cout << __FUNCTION__ << ": " << &* i.base() << " - " << &* alloc_.get()->end() << std::endl;
        alloc_.get()->erase(i.base(), alloc_.get()->end());

        return (owned_base *)(i->first);
    }

    lpp & construct()
    {
        return * constr_.get();
    }

    void * allocate(std::size_t s)
    {
        char * p = static_cast<char *>(ordered_malloc(s));

        alloc_.get()->push_back(std::make_pair(p, p + s));

//std::cout << __FUNCTION__ << ": " << (void *) p << " - " << (void *) (p + s) << std::endl;

        return p;
    }

    void deallocate(void * p, std::size_t s)
    {
        char * const q = static_cast<char *>(p);

//std::cout << __FUNCTION__ << ": " << (void *) q << " - " << (void *) (q + s) << std::endl;

        lpp::reverse_iterator i;
        for (i = alloc_.get()->rbegin(); i != alloc_.get()->rend(); i ++)
            if (i->first <= q && q <= i->second)
                break;

        alloc_.get()->erase(i.base(), alloc_.get()->end());
        free(p, s);
    }

private:
#ifndef BOOST_SH_DISABLE_THREADS
    thread_specific_ptr<lpp> alloc_, constr_;
#else
    std::auto_ptr<lpp> alloc_, constr_;
#endif
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

protected:
    owned_base()
    {
        inits_.push_back(& init_tag_);
    }

public:
    intrusive_list::node set_tag_;
    intrusive_list::node init_tag_;

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
        text(BOOST_PP_REPEAT(n, ARGUMENT_DECL, 0)) : e_(BOOST_PP_REPEAT(n, PARAMETER_DECL, 0)) {}

/**
	Object wrapper.
*/

template <typename T>
    class shifted : public owned_base
    {
        typedef T data_type;

        data_type e_; // need alignas<long>

    public:
        class roofof;
        friend class roofof;

		shifted() : e_() {}

        BOOST_PP_REPEAT_FROM_TO(1, 10, CONSTRUCT_OWNED, shifted)

        data_type * element() 				{ return & e_; }

        virtual ~shifted()					{ dispose(); }
        virtual void dispose() 				{ /*dispose(element(), is_array<data_type>());*/ }

        virtual void * get_deleter( std::type_info const & ti ) { return 0; } // dummy

    public:
        class roofof
        {
            shifted<data_type> * p_;

        public:
            roofof(data_type * p) : p_(sh::roofof((data_type shifted<data_type>::*)(& shifted<data_type>::e_), p)) {}

            operator shifted<data_type> * () const { return p_; }
        };

        void * operator new (size_t s)
        {
//std::cout << std::hex << __FUNCTION__ << ": " << s << std::endl;

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

} // namespace sh

} // namespace detail

} // namespace boost


#endif  // #ifndef BOOST_DETAIL_SH_OWNED_BASE_NT_HPP_INCLUDED
