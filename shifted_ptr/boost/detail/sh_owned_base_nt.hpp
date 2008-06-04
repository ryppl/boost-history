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
#include <boost/numeric/interval.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/remove_extent.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>
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

struct pool : boost::pool<>,
#ifndef BOOST_SH_DISABLE_THREADS
    thread_specific_ptr< std::list< numeric::interval<int> > >
#else
    std::auto_ptr< std::list< numeric::interval<int> > >
#endif
{
    typedef std::list< numeric::interval<int> > li;

    pool() : boost::pool<>(1)
    {
        reset(new std::list< numeric::interval<int> >());
    }
    
    owned_base * top(void * p)
    {
        li::reverse_iterator i;
        
        for (i = get()->rbegin(); i != get()->rend(); i ++)
            if (in((int)(p), * i))
            {
                get()->erase(i.base(), get()->end());
                break;
            }
            
        return (owned_base *)(i->lower());
    }
    
    void * allocate(std::size_t s)
    {
        void * p = ordered_malloc(s);
        
        get()->push_back(numeric::interval<int>((int) p, int((char *)(p) + s)));
        
        return p;
    }

    void deallocate(void * p)
    {
        li::reverse_iterator i;
        
        for (i = get()->rbegin(); i != get()->rend(); i ++)
            if (in((int)(p), * i))
            {
                get()->erase(i.base(), get()->end());
                break;
            }
            
        free(p, i->upper() - i->lower());
    }
};


/**
	Root class of all pointees.
*/

class owned_base : public sp_counted_base
{
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
};


pool owned_base::pool_;


/**
	Object wrapper.
*/

template <typename T>
    class owned : public owned_base
    {
        typedef T data_type;

        union
        {
            void * vp_;
            char p_[sizeof(data_type)];
        };

    public:
        class roofof;
        friend class roofof;

        data_type * element() 				{ return static_cast<data_type *>(static_cast<void *>(& p_[0])); }

        virtual ~owned()					{ dispose(); }
        virtual void dispose() 				{ dispose(element(), is_array<data_type>()); }

        virtual void * get_deleter( std::type_info const & ti ) { return 0; } // dummy

    public:
        class roofof
        {
            owned<data_type> * p_;

        public:
            roofof(data_type * p) : p_(sh::roofof((data_type owned<data_type>::*)(& owned<data_type>::p_), p)) {}
            
            operator owned<data_type> * () const { return p_; }
        };
        
        void * operator new (size_t s)
        {
            return pool_.allocate(s);
        }
        
        void operator delete (void * p)
        {
            pool_.deallocate(p);
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
    class owned<void> : public owned_base
    {
        typedef void data_type;

        union
        {
            void * vp_;
            char p_;
        };

        owned();

    public:
        class roofof;
        friend class roofof;

        data_type * element() 				{ return & p_; }

        virtual ~owned()					{}
        virtual void dispose() 				{}

        virtual void * get_deleter( std::type_info const & ti ) {}

    public:
        class roofof
        {
            owned<data_type> * p_;

        public:
            roofof(data_type * p) : p_(sh::roofof((char owned<data_type>::*)(& owned<data_type>::p_), static_cast<char *>(p))) {}
            
            operator owned<data_type> * () const { return p_; }
        };
    };


} // namespace sh

} // namespace detail

} // namespace boost


#endif  // #ifndef BOOST_DETAIL_SH_OWNED_BASE_NT_HPP_INCLUDED
