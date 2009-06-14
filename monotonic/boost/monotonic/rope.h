// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/monotonic/allocator.h>
#include <boost/monotonic/storage_base.h>

namespace boost
{
	namespace monotonic
	{
		/// a set of sequences that are tied together to present a contiguous sequence
		template <class T>
		struct rope
		{
			typedef allocator<T> Allocator;
			typedef vector<T> Vector;
			typedef list<Vector> Strands;

			struct const_iterator
			{
				rope<T> const *parent;
				Strands::const_iterator strand;
				Vector::const_iterator iter;

				const_iterator() : parent(0) { }
				const_iterator(rope<T> const *P) : parent(P) { }
				const_iterator(rope<T> const *P, Strands::const_iterator const &S, Vector::const_iterator const &V) 
					: parent(P), strand(S), iter(V) { }
				//const_iterator operator++(int)
			};

		private:
			Strands strands;
			Allocator alloc;

		public:
			rope() { }
			rope(Allocator const &A) 
				: alloc(A) { }
			rope(size_t N, T const &X, Allocator const &A)
				: alloc(A)
			{
			}
			template <class II>
			rope(II F, II L, Allocator const &A)
				: alloc(A)
			{
			}
		};
	}
}

//EOF
