// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_DETAIL_POINTER_HPP
#define BOOST_CLONEABLE_DETAIL_POINTER_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/base.hpp>

namespace boost
{
	namespace cloneable
	{
		namespace detail
		{
			/// a pointer store that can retrieve pointers from up and down 
			/// the inheritance tree.
			template <class U, class B>
			struct pointer
			{
				typedef U derived_type;
				typedef B base_type;
				typedef base<derived_type, base_type> cloneable_type;
				typedef typename cloneable_type::abstract_base_type abstract_base_type;

			private:
				cloneable_type *ptr;

			public:
				pointer(derived_type *p = 0) : ptr(dynamic_cast<cloneable_type *>(p))
				{
				}
				abstract_base_type *to_abstract() const
				{
					return ptr;
				}
				base_type *to_base() const
				{
					return ptr;
				}
				cloneable_type *to_cloneable() const
				{
					return ptr;
				}
				derived_type *to_derived() const
				{
					if (!ptr)
						return 0;
					return ptr->cloneable_type::self_ptr;
				}
			};

		} // namespace detail

	} // namespace cloneable

} // namespace boost

#endif // BOOST_CLONEABLE_DETAIL_POINTER_HPP

//EOF
