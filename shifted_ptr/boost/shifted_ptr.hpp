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

#include <boost/type_traits/add_pointer.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

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
};




/**
	Deterministic memory manager of constant complexity.
*/

template <typename T, template <typename> class U = shifted_ptr_base>
    class shifted_ptr : public U<T>
    {
        //template <typename, template <typename> class> friend class shifted_ptr;

        using U<T>::share;


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
            shifted_ptr(owned<V> * p) : U<T>(p)
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
            shifted_ptr(shifted_ptr<V> const & p) : U<T>(p)
            {
                if (! owned_base::pool_.is_from(this))
                    ps_ = new set();
                else
                    owned_base::pool_.top(this)->ptrs()->push(& pn_);

                ps_->redir(p.ps_);
            }

            shifted_ptr(shifted_ptr<T> const & p) : U<T>(p)
            {
                if (! owned_base::pool_.is_from(this))
                    ps_ = new set();
                else
                    owned_base::pool_.top(this)->ptrs()->push(& pn_);

                ps_->redir(p.ps_);
            }

        template <typename V>
            shifted_ptr & operator = (owned<V> * p)
            {
                release();
                init(p);
                U<T>::operator = (p);

                return * this;
            }

        template <typename V>
            shifted_ptr & operator = (shifted_ptr<V> const & p)
            {
                if (p.po_ != U<T>::po_)
                {
                    if (ps_->redir() != p.ps_->redir())
                    {
                        release();
                        ps_->redir(p.ps_);
                    }
                    U<T>::operator = (p);
                }
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
                    U<T>::po_ = 0;

                    if (! d)
                        new (ps_) set();
                    else
                        delete ps_;
                }
                else 
                {
                    U<T>::reset();

                    if (! d)
                        ps_ = new set();
                }
            }
            else if (! d)
                U<T>::reset();
        }

        void init(owned_base * p)
        {
            for (intrusive_list::iterator<owned_base, & owned_base::init_tag_> i = p->inits()->begin(), j; j = i, ++ j, i != p->inits()->end(); i = j)
            {
                ps_->elements()->push_back(i->set_tag());
                
                for (intrusive_stack::iterator<shifted_ptr, & shifted_ptr::pn_> m = i->ptrs()->begin(), n; n = m, ++ n, m != i->ptrs()->end(); m = n)
                    m->ps_ = ps_;
            }
        }
    };


#define TEMPLATE_DECL(z, n, text) , typename T ## n
#define ARGUMENT_DECL(z, n, text) BOOST_PP_COMMA_IF(n) T ## n const & t ## n
#define PARAMETER_DECL(z, n, text) BOOST_PP_COMMA_IF(n) t ## n

#define MAKE_SHIFTED_PTR(z, n, text)																			\
	template <typename T BOOST_PP_REPEAT(n, TEMPLATE_DECL, 0)>													\
		inline owned<T> * new_sh(BOOST_PP_REPEAT(n, ARGUMENT_DECL, 0))											\
		{																										\
			typedef typename add_pointer<T>::type pointer_type;													\
			typedef typename remove_const<typename remove_volatile<T>::type>::type unqualified_type;			\
																												\
			owned<T> * p = new owned<T>();																		\
			pointer_type q = reinterpret_cast<pointer_type>(new (const_cast<unqualified_type *>(p->element())) T(BOOST_PP_REPEAT(n, PARAMETER_DECL, 0)));	\
																												\
			return (owned<T> *) (typename owned<T>::roofof) q;													\
		}

BOOST_PP_REPEAT(10, MAKE_SHIFTED_PTR, 0)


} // namespace sh

} // namespace detail

using detail::sh::shifted_ptr;
using detail::sh::new_sh;

} // namespace boost


#if defined(_MSC_VER)
#pragma warning( pop )
#endif


#endif // #ifndef BOOST_DETAIL_SH_RTCMM_H_INCLUDED
