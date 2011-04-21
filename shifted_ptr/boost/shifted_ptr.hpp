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

#include <iostream>
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
	
	Proxy object used to count the number of pointers from the stack are referencing pointee objects belonging to the same @c set .
*/

class set
{
    long count_;									/**< Count of the number of pointers from the stack referencing the same @c set .*/
    mutable set * redir_;							/**< Redirection in the case of an union multiple @c set s.*/

    intrusive_list includes_;						/**< List of all @c set s of an union. */
    intrusive_list elements_;						/**< List of all pointee objects belonging to a @c set . */

    static fast_pool_allocator<set> pool_;			/**< Pool where all @c set s are allocated. */

public:
    intrusive_list::node tag_;						/**< Tag used to enlist to @c set::includes_ . */


	/**
		Initialization of a single @c set .
	*/
	
    set() : count_(1), redir_(this)
    {
        includes_.push_back(& tag_);
    }
	
	
	/**
		Release of a @c set with possible destruction of all its elements and other @c set s unified to it.
		
		@return		True if the @c set was released.
	*/
	
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
                
            for (intrusive_list::iterator<set, & set::tag_> i = p->includes_.begin(), j; j = i, i != p->includes_.end(); i = j)
			{ 
				++ j;
                if (&* i != this && &* i != p)
                    delete &* i;
			}
                    
            if (p != this)
                delete p;

            return true;
        }

        return false;
    }

	
	/**
		Recursive search for the @c set header of an union.
		
		@return		@c set responsible for managing the counter of an union.
	*/
	
    set * redir() const
    {
        if (redir_ == this) return redir_;
        else return redir_ = redir_->redir();
    }
	
	
	/**
		Unification with a new @c set .
		
		@param	p	New @c set to unify with.
	*/

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

	
	/**
		Finds the elements constituting one or many @c set s unified.
		
		@return		List of all elements.
	*/
	
    intrusive_list * elements() const
    {
        return & redir()->elements_;
    }
    
	
	/**
		Allocates a new @c set using the fast pool allocator.
		
		@param	s	Size of the @c set .
		@return		Pointer of the new memory block.
	*/
	
    void * operator new (size_t s)
    {
        return pool_.allocate(s);
    }
    
	
	/**
		Placement new.
		
		@param	s	Size of the @c set .
		@param	p	Address to construct the @c set on.
		@return		Address to construct the @c set on.
	*/
	
    void * operator new (size_t s, set * p)
    {
        return p;
    }

	
	/**
		Deallocates a @c set from the fast pool allocator.
		
		@param	p	Address of the @c set to deallocate.
	*/
	
    void operator delete (void * p)
    {
        pool_.deallocate(static_cast<set *>(p), sizeof(set));
    }
};


fast_pool_allocator<set> set::pool_;


/**
	Deterministic memory manager of constant complexity.
	
	Complete memory management utility on top of standard reference counting.
*/

template <typename T>
    class shifted_ptr : public shifted_ptr_base<T>
    {
        template <typename> friend class shifted_ptr;

        typedef shifted_ptr_base<T> base;
        
        using base::share;
		using base::po_;

        union
        {
            set * ps_;								/**< Pointer to the @c set node @c shifted_ptr<> belongs to. */
            intrusive_stack::node pn_;				/**< Tag used for enlisting a pointer on the heap to later share the @c set it belongs to. */
        };

    public:
        typedef T                       value_type;
        typedef shifted<value_type>     element_type;


		/**
			Initialization of a pointer living on the stack or proper enlistment if living on the heap.
		*/
		
        shifted_ptr() : ps_(0)
        {
            if (! owned_base::pool_.is_from(this))
                ps_ = new set();
            else
                owned_base::pool_.top(this)->ptrs_.push(& pn_);
        }

		
		/**
			Initialization of a pointer living on the stack or proper enlistment if living on the heap.
			
			@param	p	New pointee object to manage.
		*/
		
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
                    owned_base::pool_.top(this)->ptrs_.push(& pn_);
                    owned_base::pool_.top(this)->inits_.merge(p->inits_);
                }
            }

		
		/**
			Initialization of a pointer living on the stack or proper enlistment if living on the heap.
			
			@param	p	New pointer to manage.
		*/

        template <typename V>
            shifted_ptr(shifted_ptr<V> const & p) : base(p)
            {
                if (! owned_base::pool_.is_from(this))
                    ps_ = new set();
                else
                    owned_base::pool_.top(this)->ptrs_.push(& pn_);

                ps_->redir(p.ps_);
            }

		
		/**
			Initialization of a pointer living on the stack or proper enlistment if living on the heap.
			
			@param	p	New pointer to manage.
		*/

			shifted_ptr(shifted_ptr<T> const & p) : base(p)
            {
                if (! owned_base::pool_.is_from(this))
                    ps_ = new set();
                else
                    owned_base::pool_.top(this)->ptrs_.push(& pn_);
				
                ps_->redir(p.ps_);
            }


		/**
			Assignment & union of 2 @c set s if the pointee resides a different @c set.
			
			@param	p	New pointee object to manage.
		*/
		
        template <typename V>
            shifted_ptr & operator = (shifted<V> * p)
            {
                release(false);

                init(p);

                base::operator = (p);

                return * this;
            }


		/**
			Assignment & union of 2 @c set s if the pointee resides a different @c set.
			
			@param	p	New pointer to manage.
		*/
			
        template <typename V>
            shifted_ptr & operator = (shifted_ptr<V> const & p)
            {
                if (ps_->redir() != p.ps_->redir())
                {
                    release(false);
					
                    ps_->redir(p.ps_);
                }
                base::operator = (p);

                return * this;
            }


		/**
			Assignment & union of 2 @c set s if the pointee resides a different @c set.
			
			@param	p	New pointer to manage.
		*/

            shifted_ptr & operator = (shifted_ptr<T> const & p)
            {
                return operator = <T>(p);
            }

        void reset()
        {
            release(false);
        }

        ~shifted_ptr()
        {
            release(true);
        }

    private:
		/**
			Release of the pointee object with or without destroying the entire @c set it belongs to.
			
			@param	d	Destroy (true) or reuse (false) the @c set it is releasing.
		*/
		
        void release(bool d)
        {
            if (! owned_base::pool_.is_from(this))
            {
                if (ps_->release())
                {
                    base::po_ = 0;

                    if (! d)
					{
						ps_->set::~set();
                        new (ps_) set();
					}
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

		
		/**
			Enlist & initialize pointee objects belonging to the same @c set .  This initialization occurs when a pointee object is affected to the first pointer living on the stack it encounters.
			
			@param	p	Pointee object to initialize.
		*/
		
        void init(owned_base * p)
        {
            if (p->init_)
                return;
        
			// iterate memory blocks
            for (intrusive_list::iterator<owned_base, & owned_base::init_tag_> i = p->inits_.begin(); i != p->inits_.end(); ++ i)
            {
                i->init_ = true;
                ps_->elements()->push_back(& i->set_tag_);

				// iterate shifted_ptr elements
                for (intrusive_stack::iterator<shifted_ptr, & shifted_ptr::pn_> j = i->ptrs_.begin(), k; k = j, j != i->ptrs_.end(); j = k)
				{
					++ k;
                    j->ps_ = ps_;
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
