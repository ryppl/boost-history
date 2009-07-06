// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_STACK_HPP
#define BOOST_MONOTONIC_STACK_HPP

#include <boost/monotonic/detail/prefix.hpp>
#include <boost/monotonic/storage.hpp>
#include <boost/monotonic/containers/vector.hpp>

// warning C4345: behavior change: an object of POD type constructed with an initializer of the form () will be default-initialized
#pragma warning(disable:4345)

namespace boost
{
	namespace monotonic
	{
		template <size_t InlineSize = DefaultSizes::InlineSize>
		struct stack
		{
			typedef stack<InlineSize> this_type;

		private:
			fixed_storage<InlineSize> store;

			struct element_base
			{
				element_base *previous;
				size_t cursor;
				virtual ~element_base() { }
			};
			template <class T>
			struct element : element_base
			{
				typedef T type;

				type *val;
				char value[sizeof(T)];
				element()
				{
					val = reinterpret_cast<T *>(value);
				}
				~element()
				{
					val->~T();
				}
			};

			element_base *previous;

		public:
			stack() : previous(0)
			{
			}

			template <class T>
			T &push()
			{
				size_t cursor = store.get_cursor();
				element<T> &elem = store.create<element<T> >();
				elem.previous = previous;
				elem.cursor = cursor;
				previous = &elem;
				new (elem.val) T();
				return *elem.val;
			}

			template <class T, class A0>
			T &push(A0 a0)
			{
				size_t cursor = store.get_cursor();
				element<T> &elem = store.create<element<T> >();
				elem.previous = previous;
				elem.cursor = cursor;
				previous = &elem;
				new (elem.val) T(a0);
				return *elem.val;
			}
			void pop()
			{
				BOOST_ASSERT(previous);
				element_base *elem = previous;
				previous = elem->previous;
				size_t cursor = elem->cursor;
				elem->~element_base();
				store.set_cursor(cursor);
			}

			size_t top() const
			{
				return store.get_cursor();
			}
		};
	}
}

#include <boost/monotonic/detail/postfix.hpp>

#endif // BOOST_MONOTONIC_STACK_HPP

//EOF

