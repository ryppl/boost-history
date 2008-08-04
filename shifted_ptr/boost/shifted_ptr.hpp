/**
	@file
	Boost shifted_ptr.hpp header file.

	@author
	Copyright (c) 2008 Phil Bouchard <phil@fornux.com>.

	@note
	Distributed under the Boost Software License, Version 1.0.

	See accompanying file LICENSE_1_0.txt or copy at
	http://www.boost.org/LICENSE_1_0.txt

	See http://www.boost.org/libs/smart_ptr/doc/index.html for documentation.


	Thanks to: Steven Watanabe <watanabesj@gmail.com>
*/


#ifndef BOOST_DETAIL_SH_RTCMM_H_INCLUDED
#define BOOST_DETAIL_SH_RTCMM_H_INCLUDED


#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4355 )

#include <new.h>
#endif

#include <boost/pool/pool_alloc.hpp>
#include <boost/type_traits/add_pointer.hpp>

#include <boost/detail/intrusive_list.hpp>
#include <boost/detail/intrusive_stack.hpp>
#include <boost/detail/sh_utility.h>
#include <boost/detail/shifted_ptr_base.hpp>


namespace boost
{

namespace detail
{

namespace sh
{


class owned_base;


/**
	Set header.
*/

class set
{
    long count_;
    mutable set * redir_;

    intrusive_list includes_;
    intrusive_list elements_;

    static fast_pool_allocator<set> pool_;

public:
    intrusive_list::node tag_;


    set() : count_(1), redir_(this)
    {
        includes_.push_back(& tag_);
    }

    bool release()
    {
        set * p = redir();

        if (-- p->count_ == 0)
        {
            for (intrusive_list::iterator<owned_base, & owned_base::set_tag_> i; i = p->elements_.begin(), i != p->elements_.end(); )
            {
                i->add_ref_copy();
                delete &* i;
            }
                
            for (intrusive_list::iterator<set, & set::tag_> i = p->includes_.begin(), j; j = i, ++ j, i != p->includes_.end(); i = j)
                if (&* i != this && &* i != p)
                    delete &* i;
                    
            if (p != this)
                delete p;

            return true;
        }

        return false;
    }

    set * redir() const
    {
        if (redir_ == this) return redir_;
        else return redir_ = redir_->redir();
    }

    void redir(set * p)
    {
        if (redir_ != p->redir())
        {
            redir_ = p->redir();
            redir_->includes_.merge(includes_);
            redir_->elements_.merge(elements_);
            redir_->count_ += count_;
        }
    }

    intrusive_list * elements() const
    {
        return & redir()->elements_;
    }
    
    void * operator new (size_t s)
    {
        return pool_.allocate(s);
    }
    
    void * operator new (size_t s, set * p)
    {
        return p;
    }

    void operator delete (void * p)
    {
        pool_.deallocate(static_cast<set *>(p), sizeof(set));
    }
};


fast_pool_allocator<set> set::pool_;


/**
	Deterministic memory manager of constant complexity.
*/

template <typename T>
    class shifted_ptr : public shifted_ptr_base<T>
    {
        //template <typename, template <typename> class> friend class shifted_ptr;

        typedef shifted_ptr_base<T> base;
        
        using base::share;


        union
        {
            set * ps_;
            intrusive_stack::node pn_;
        };

    public:
        typedef T element_type;


        shifted_ptr() : ps_(0)
        {
            if (! owned_base::pool_.is_from(this))
                ps_ = new set();
            else
                owned_base::pool_.top(this)->ptrs()->push(& pn_);
        }

        template <typename V>
            shifted_ptr(shifted<V> * p) : base(p)
            {
                if (! owned_base::pool_.is_from(this))
                {
                    ps_ = new set();

                    init(p);
                }
                else
                {
                    owned_base::pool_.top(this)->ptrs()->push(& pn_);
                    owned_base::pool_.top(this)->inits()->merge(* p->inits());
                }
            }

        template <typename V>
            shifted_ptr(shifted_ptr<V> const & p) : base(p)
            {
                if (! owned_base::pool_.is_from(this))
                    ps_ = new set();
                else
                    owned_base::pool_.top(this)->ptrs()->push(& pn_);

                ps_->redir(p.ps_);
            }

            shifted_ptr(shifted_ptr<T> const & p) : base(p)
            {
                if (! owned_base::pool_.is_from(this))
                    ps_ = new set();
                else
                    owned_base::pool_.top(this)->ptrs()->push(& pn_);

                ps_->redir(p.ps_);
            }

        template <typename V>
            shifted_ptr & operator = (shifted<V> * p)
            {
                if (ps_ && ! owned_base::pool_.is_from(ps_))
                {
                    if (! owned_base::pool_.is_from(this))
                        release();

                    init(p);
                }
                base::operator = (p);

                return * this;
            }

        template <typename V>
            shifted_ptr & operator = (shifted_ptr<V> const & p)
            {
                if (ps_ && ! owned_base::pool_.is_from(ps_))
                    if (ps_->redir() != p.ps_->redir())
                    {
                        release();
                        ps_->redir(p.ps_);
                    }
                base::operator = (p);

                return * this;
            }

            shifted_ptr & operator = (shifted_ptr<T> const & p)
            {
                return operator = <T>(p);
            }

        void reset()
        {
            release();
        }

        ~shifted_ptr()
        {
            release(true);
        }

    private:
        void release(bool d = false)
        {
            if (! owned_base::pool_.is_from(this))
            {
                if (ps_->release())
                {
                    base::po_ = 0;

                    if (! d)
                        new (ps_) set();
                    else
                        delete ps_;
                }
                else 
                {
                    base::reset();

                    if (! d)
                        ps_ = new set();
                }
            }
            else if (! d)
                base::reset();
        }

        void init(owned_base * p)
        {
            for (intrusive_list::iterator<owned_base, & owned_base::init_tag_> i = p->inits()->begin(), j; j = i, ++ j, i != p->inits()->end(); i = j)
            {
                ps_->elements()->push_back(i->set_tag());
                
                for (intrusive_stack::iterator<shifted_ptr, & shifted_ptr::pn_> m = i->ptrs()->begin(), n; n = m, ++ n, m != i->ptrs()->end(); m = n)
                {
                    m->ps_ = ps_;
                }
            }
        }
    };


} // namespace sh

} // namespace detail

using detail::sh::shifted_ptr;
using detail::sh::shifted;

} // namespace boost


#if defined(_MSC_VER)
#pragma warning( pop )
#endif


#endif // #ifndef BOOST_DETAIL_SH_RTCMM_H_INCLUDED
