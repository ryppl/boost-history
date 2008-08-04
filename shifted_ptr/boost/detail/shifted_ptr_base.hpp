/**
	@file
	Boost shifted_ptr_base.hpp header file.

	@note
	Copyright (c) 2003 - 2008 Phil Bouchard <phil@fornux.com>.

	Distributed under the Boost Software License, Version 1.0.

	See accompanying file LICENSE_1_0.txt or copy at
	http://www.boost.org/LICENSE_1_0.txt

	See http://www.boost.org/libs/smart_ptr/doc/index.html for documentation.
*/


#ifndef BOOST_SHIFTED_PTR_HPP
#define BOOST_SHIFTED_PTR_HPP


#include <boost/detail/sh_utility.h>
#include <boost/detail/sh_owned_base.hpp>


namespace boost
{

namespace detail
{

namespace sh
{


/**
	Smart pointer optimized for speed and memory usage.
	
	This class represents a basic smart pointer interface.
*/

template <typename T>
	class shifted_ptr_base
	{
		typedef T element_type;
		template <typename> friend class shifted_ptr_base;

	protected:
		element_type * po_;

	public:
		shifted_ptr_base() : po_(0)
		{
		}

		template <typename V>
			shifted_ptr_base(detail::sh::shifted<V> * p) : po_(p->element())
			{
			}

		template <typename V>
			shifted_ptr_base(shifted_ptr_base<V> const & p) : po_(p.share())
			{
			}

			shifted_ptr_base(shifted_ptr_base<element_type> const & p) : po_(p.share())
			{
			}

		template <typename V>
			shifted_ptr_base & operator = (detail::sh::shifted<V> * p)
			{
				reset(p->element());

				return * this;
			}

		template <typename V>
			shifted_ptr_base & operator = (shifted_ptr_base<V> const & p)
			{
				if (p.po_ != po_)
				{
					reset(p.share());
				}
				return * this;
			}

			shifted_ptr_base & operator = (shifted_ptr_base<element_type> const & p)
			{
				return operator = <element_type>(p);
			}

		element_type & operator * () const
		{
			return * po_;
		}

		element_type * operator -> () const
		{
			return po_;
		}

		element_type * get() const
		{
			return po_;
		}

		element_type * share() const
		{
			if (po_)
			{
				header()->add_ref_copy();
			}
			return po_;
		}

		void reset(element_type * p = 0)
		{
			if (po_)
			{
				header()->release();
			}
			po_ = p;
		}

		~shifted_ptr_base()
		{
			if (po_)
			{
				header()->release();
			}
		}


	protected:
		detail::sh::owned_base * header() const
		{
			detail::sh::owned_base * p = (shifted<element_type> *) (typename shifted<element_type>::roofof) static_cast<element_type *>(rootof<is_polymorphic<element_type>::value>::get(po_));
			return p;
		}
	};


#if !defined(_MSC_VER)
template <typename T, size_t N>
	class shifted_ptr_base<T [N]>
	{
		typedef T element_type[N];
		template <typename> friend class shifted_ptr_base;

	protected:
		element_type * po_;

	public:
		shifted_ptr_base() : po_(0)
		{
		}

		template <typename V>
			shifted_ptr_base(detail::sh::shifted<V> * p) : po_(p->element())
			{
			}

		template <typename V>
			shifted_ptr_base(shifted_ptr_base<V> const & p) : po_(p.share())
			{
			}

			shifted_ptr_base(shifted_ptr_base<element_type> const & p) : po_(p.share())
			{
			}

		template <typename V>
			shifted_ptr_base & operator = (detail::sh::shifted<V> * p)
			{
				reset(p->element());

				return * this;
			}

		template <typename V>
			shifted_ptr_base & operator = (shifted_ptr_base<V> const & p)
			{
				if (p.po_ != po_)
				{
					reset(p.share());
				}
				return * this;
			}

			shifted_ptr_base & operator = (shifted_ptr_base<element_type> const & p)
			{
				return operator = <element_type>(p);
			}

		T & operator [] (std::size_t n)
		{
			return ** (po_ + n);
		}

		T const & operator [] (std::size_t n) const
		{
			return ** (po_ + n);
		}

		element_type * get() const
		{
			return po_;
		}

		element_type * share() const
		{
			if (po_)
			{
				header()->add_ref_copy();
			}
			return po_;
		}

		void reset(element_type * p = 0)
		{
			if (po_)
			{
				header()->release();
			}
			po_ = p;
		}

		~shifted_ptr_base()
		{
			if (po_)
			{
				header()->release();
			}
		}


	protected:
		detail::sh::owned_base * header() const
		{
			return (shifted<element_type> *) (typename shifted<element_type>::roofof) static_cast<element_type *>(rootof<is_polymorphic<element_type>::value>::get(po_));
		}
	};
#endif


template <>
	class shifted_ptr_base<void>
	{
		typedef void element_type;
		template <typename> friend class shifted_ptr_base;

	protected:
		element_type * po_;

	public:
		shifted_ptr_base() : po_(0)
		{
		}

		template <typename V>
			shifted_ptr_base(detail::sh::shifted<V> * p) : po_(p->element())
			{
			}

		template <typename V>
			shifted_ptr_base(shifted_ptr_base<V> const & p) : po_(p.share())
			{
			}

			shifted_ptr_base(shifted_ptr_base<element_type> const & p) : po_(p.share())
			{
			}

		template <typename V>
			shifted_ptr_base & operator = (detail::sh::shifted<V> * p)
			{
				reset(p->element());

				return * this;
			}

		template <typename V>
			shifted_ptr_base & operator = (shifted_ptr_base<V> const & p)
			{
				if (p.po_ != po_)
				{
					reset(p.share());
				}
				return * this;
			}

			shifted_ptr_base & operator = (shifted_ptr_base<element_type> const & p)
			{
				return operator = <element_type>(p);
			}

		element_type * get() const
		{
			return po_;
		}

		element_type * share() const
		{
			if (po_)
			{
				header()->add_ref_copy();
			}
			return po_;
		}

		void reset(element_type * p = 0)
		{
			if (po_)
			{
				header()->release();
			}
			po_ = p;
		}

		~shifted_ptr_base()
		{
			if (po_)
			{
				header()->release();
			}
		}


	protected:
		detail::sh::owned_base * header() const
		{
			return (shifted<element_type> *) (shifted<element_type>::roofof) static_cast<element_type *>(rootof<is_polymorphic<element_type>::value>::get(po_));
		}
	};


} // namespace sh

} // namespace detail

} // namespace boost


#endif
