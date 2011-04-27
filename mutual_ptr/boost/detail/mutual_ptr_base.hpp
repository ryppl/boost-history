/**
	@file
	Boost mutual_ptr_base.hpp header file.

	@note
	Copyright (c) 2003 - 2008 Phil Bouchard <phil@fornux.com>.
    Copyright (c) 2001 - 2007 Peter Dimov

	Distributed under the Boost Software License, Version 1.0.

	See accompanying file LICENSE_1_0.txt or copy at
	http://www.boost.org/LICENSE_1_0.txt

	See http://www.boost.org/libs/smart_ptr/doc/index.html for documentation.
*/


#ifndef BOOST_DETAIL_MUTUAL_PTR_BASE_HPP
#define BOOST_DETAIL_MUTUAL_PTR_BASE_HPP


#include <boost/detail/roofof.hpp>
#include <boost/detail/mutual_base.hpp>


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
	class mutual_ptr_comutualon
	{
		template <typename> friend class mutual_ptr_comutualon;

        // Borland 5.5.1 specific workaround
        typedef mutual_ptr_comutualon<T> this_type;

	protected:
		typedef T value_type;
		typedef mutual<value_type> element_type;

		value_type * po_;

	public:
		mutual_ptr_comutualon() : po_(0)
		{
		}

        ~mutual_ptr_comutualon()
		{
			if (po_)
			{
				header()->release();
			}
		}

		template <typename V>
			mutual_ptr_comutualon(detail::sh::mutual<V> * p) : po_(p->element())
			{
			}

		template <typename V>
			mutual_ptr_comutualon(mutual_ptr_comutualon<V> const & p) : po_(p.share())
			{
			}

			mutual_ptr_comutualon(mutual_ptr_comutualon<value_type> const & p) : po_(p.share())
			{
			}

		template <typename V>
			mutual_ptr_comutualon & operator = (detail::sh::mutual<V> * p)
			{
				reset(p->element());

				return * this;
			}

		template <typename V>
			mutual_ptr_comutualon & operator = (mutual_ptr_comutualon<V> const & p)
			{
				if (p.po_ != po_)
				{
					reset(p.share());
				}
				return * this;
			}

			mutual_ptr_comutualon & operator = (mutual_ptr_comutualon<value_type> const & p)
			{
				return operator = <value_type>(p);
			}

		value_type * get() const
		{
			return po_;
		}

		value_type * share() const
		{
			if (po_)
			{
				header()->add_ref_copy();
			}
			return po_;
		}

		void reset(value_type * p = 0)
		{
			if (po_)
			{
				header()->release();
			}
			po_ = p;
		}

#if ( defined(__SUNPRO_CC) && BOOST_WORKAROUND(__SUNPRO_CC, < 0x570) ) || defined(__CINT__)
        operator bool () const
        {
            return po_ != 0;
        }
#elif defined( _MANAGED )
        static void unspecified_bool( this_type*** )
        {
        }

        typedef void (*unspecified_bool_type)( this_type*** );

        operator unspecified_bool_type() const // never throws
        {
            return po_ == 0? 0: unspecified_bool;
        }
#elif \
        ( defined(__MWERKS__) && BOOST_WORKAROUND(__MWERKS__, < 0x3200) ) || \
        ( defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ < 304) ) || \
        ( defined(__SUNPRO_CC) && BOOST_WORKAROUND(__SUNPRO_CC, <= 0x590) )

        typedef value_type * (this_type::*unspecified_bool_type)() const;
        
        operator unspecified_bool_type() const // never throws
        {
            return po_ == 0? 0: &this_type::get;
        }
#else 
        typedef value_type * this_type::*unspecified_bool_type;

        operator unspecified_bool_type() const // never throws
        {
            return po_ == 0? 0: &this_type::po_;
        }
#endif

        // operator! is redundant, but some compilers need it

        bool operator! () const // never throws
        {
            return po_ == 0;
        }

        long use_count() const // never throws
        {
            return header()->use_count();
        }

	protected:
		detail::sh::mutual_base * header() const
		{
			detail::sh::mutual_base * p = (mutual<value_type> *) (typename mutual<value_type>::roofof) static_cast<value_type *>(rootof<is_polymorphic<value_type>::value>::get(po_));
			return p;
		}
	};


template <typename T>
	class mutual_ptr_base : public mutual_ptr_comutualon<T>
	{
        typedef mutual_ptr_comutualon<T> base;
        typedef typename base::value_type value_type;
		
	protected:
		using base::po_;

	public:
		mutual_ptr_base() : base()
		{
		}

		template <typename V>
			mutual_ptr_base(detail::sh::mutual<V> * p) : base(p)
			{
			}

		template <typename V>
			mutual_ptr_base(mutual_ptr_base<V> const & p) : base(p)
			{
			}

			mutual_ptr_base(mutual_ptr_base<value_type> const & p) : base(p)
			{
			}

		template <typename V>
			mutual_ptr_base & operator = (detail::sh::mutual<V> * p)
			{
				return static_cast<mutual_ptr_base &>(base::operator = (p));
			}

		template <typename V>
			mutual_ptr_base & operator = (mutual_ptr_base<V> const & p)
			{
				return static_cast<mutual_ptr_base &>(base::operator = (p));
			}

			mutual_ptr_base & operator = (mutual_ptr_base<value_type> const & p)
			{
				return static_cast<mutual_ptr_base &>(base::operator = (p));
			}

		value_type & operator * () const
		{
			return * po_;
		}

		value_type * operator -> () const
		{
			return po_;
		}
	};


#if !defined(_MSC_VER)
template <typename T, size_t N>
	class mutual_ptr_base<T [N]> : public mutual_ptr_comutualon<T [N]>
	{
        typedef mutual_ptr_comutualon<T [N]> base;
        typedef typename base::value_type value_type;

	protected:
		using base::po_;

	public:
		mutual_ptr_base() : base()
		{
		}

		template <typename V>
			mutual_ptr_base(detail::sh::mutual<V> * p) : base(p)
			{
			}

		template <typename V>
			mutual_ptr_base(mutual_ptr_base<V> const & p) : base(p)
			{
			}

			mutual_ptr_base(mutual_ptr_base<value_type> const & p) : base(p)
			{
			}

		template <typename V>
			mutual_ptr_base & operator = (detail::sh::mutual<V> * p)
			{
				return static_cast<mutual_ptr_base &>(base::operator = (p));
			}

		template <typename V>
			mutual_ptr_base & operator = (mutual_ptr_base<V> const & p)
			{
				return static_cast<mutual_ptr_base &>(base::operator = (p));
			}

			mutual_ptr_base & operator = (mutual_ptr_base<value_type> const & p)
			{
				return static_cast<mutual_ptr_base &>(base::operator = (p));
			}

		T & operator [] (std::size_t n)
		{
			return * (* po_ + n);
		}

		T const & operator [] (std::size_t n) const
		{
			return * (* po_ + n);
		}
	};
#endif


template <>
	class mutual_ptr_base<void> : public mutual_ptr_comutualon<void>
	{
        typedef mutual_ptr_comutualon<void> base;
        typedef base::value_type value_type;

	protected:
		using base::po_;

	public:
		mutual_ptr_base() : base()
		{
		}

		template <typename V>
			mutual_ptr_base(detail::sh::mutual<V> * p) : base(p)
			{
			}

		template <typename V>
			mutual_ptr_base(mutual_ptr_base<V> const & p) : base(p)
			{
			}

			mutual_ptr_base(mutual_ptr_base<value_type> const & p) : base(p)
			{
			}

		template <typename V>
			mutual_ptr_base & operator = (detail::sh::mutual<V> * p)
			{
				return static_cast<mutual_ptr_base &>(base::operator = (p));
			}

		template <typename V>
			mutual_ptr_base & operator = (mutual_ptr_base<V> const & p)
			{
				return static_cast<mutual_ptr_base &>(base::operator = (p));
			}

			mutual_ptr_base & operator = (mutual_ptr_base<value_type> const & p)
			{
				return static_cast<mutual_ptr_base &>(base::operator = (p));
			}
	};


} // namespace sh

} // namespace detail

} // namespace boost


#endif
