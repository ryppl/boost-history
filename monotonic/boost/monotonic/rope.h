// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/monotonic/allocator.h>
#include <boost/monotonic/storage_base.h>
#include <boost/utility/iter_range.h>
#include <boost/unordered_map.hpp>

namespace boost
{
	namespace monotonic
	{
		/// a set of sequences that are tied together to present a contiguous sequence
		///
		/// this is to provide a sequence type that is 'between' a list and a vector. it
		/// has slower access speed than a vector, but faster access speed than a list.
		///
		/// unlike a vector, a rope cannot be resize()d
		template <class T, size_t ChunkSize = 32>
		struct rope
		{
			typedef rope<T,ChunkSize> Rope;
			typedef allocator<T> Allocator;
			typedef vector<T> Vector;
			typedef list<Vector> Strands;
			typedef const_iter_range<Strands> ConstStrandsIterators;
			typedef iter_range<Strands> StrandsIterators;
			typedef const_iter_range<Vector> ConstVectorIterators;
			typedef iter_range<Vector> VectorIterators;

			template <class R, class S, class V, class Derived>
			struct iterator_base //: TODO: add iterator category
			{
				typedef R Rope;
				typedef S StrandIterators;
				typedef V VectorIterators;
				Rope *parent;
				StrandIterators strand;
				VectorIterators vec;

				iterator_base() { }
				iterator_base(Rope &P) : parent(&P) { }
				iterator_base(Rope &P, StrandIterators const &S)
					: parent(&P), strand(S) { }
				iterator_base(Rope &P, StrandIterators const &S, VectorIterators const &V) 
					: parent(&P), strand(S), vec(V) { }
				Derived &This()
				{
					return static_cast<Derived &>(*this);
				}
				Derived &operator++()
				{
					if (!++vec)
					{
						if (!++strand)
						{
							return This();
						}
						vec = *strand;
					}
					return This();
				}
				bool operator==(iterator_base const &B) const
				{
					return parent == B.parent && strand == B.strand && vec == B.vec;
				}
				bool operator!=(iterator_base const &B) const
				{
					return !(*this == B);
				}
			};
			struct iterator : iterator_base<Rope, StrandsIterators, VectorIterators, iterator>
			{
				typedef iterator_base<Rope, StrandsIterators, VectorIterators, iterator> Parent;
				iterator() { }
				iterator(Rope &P) 
					: Parent(P) { }
				iterator(Rope &P, StrandsIterators const &S)
					: Parent(P, S) { }
				iterator(Rope &P, StrandsIterators const &S, VectorIterators const &V) 
					: Parent(P, S, V) { }
				T const &operator*() const
				{
					return *Parent::vec;
				}
				T &operator*()
				{
					return *Parent::vec;
				}
			};
			struct const_iterator : iterator_base<Rope const, ConstStrandsIterators, ConstVectorIterators, const_iterator>
			{
				typedef iterator_base<Rope const, ConstStrandsIterators, ConstVectorIterators, const_iterator> Parent;
				const_iterator() { }
				const_iterator(Rope const &P) 
					: Parent(P) { }
				const_iterator(iterator const &X)
					: Parent(*X.parent, X.strand, X.vec) 
				{ }
				const_iterator(Rope const &P, ConstStrandsIterators const &S)
					: Parent(P, S) { }
				const_iterator(Rope const &P, ConstStrandsIterators const &S, ConstVectorIterators const &V) 
					: Parent(P, S, V) { }
				T const &operator*() const
				{
					return *Parent::vec;
				}
			};

		private:
			Strands strands;
			Allocator alloc;

		public:
			rope() { }
			rope(Allocator const &A) 
				: alloc(A), strands(A) { }
			rope(size_t N, T const &X, Allocator const &A)
				: alloc(A), strands(A)
			{
				//TODO
			}
			template <class II>
			rope(II F, II L, Allocator const &A)
				: alloc(A), strands(A)
			{
				//TODO
			}

			size_t size() const
			{
				size_t len = 0;
				BOOST_FOREACH(Vector const &vec, strands)
				{
					len += vec.size();
				}
				return len;
			}
			bool empty() const
			{
				return strands.empty() || size() == 0;
			}
			const_iterator begin() const
			{
				if (strands.empty())
					return const_iterator(*this);
				return const_iterator(*this, strands, strands.front());
			}
			const_iterator end() const
			{
				if (strands.empty())
					return const_iterator(*this);
				return const_iterator(*this, strands.end(), strands.back().end());
			}
			iterator begin()
			{
				if (strands.empty())
					return iterator(*this);
				return iterator(*this, strands, strands.front());
			}
			iterator end()
			{
				if (strands.empty())
					return iterator(*this);
				return iterator(*this, strands.end(), strands.back().end());
			}
			void push_back(T const &X)
			{
				bool require_new_vec = strands.empty();
				require_new_vec = require_new_vec || strands.back().size() == strands.back().capacity();
				if (require_new_vec)
				{
					strands.push_back(Vector(alloc));
					strands.back().reserve(ChunkSize);
				}
				strands.back().push_back(X);
			}

			T &at(size_t N)
			{
				size_t offset = 0;
				BOOST_FOREACH(Vector &vec, strands)
				{
					size_t local = N - offset;
					if (local < vec.size())
					{
						return vec.at(local);
					}
					offset += vec.size();
					if (offset > N)
						break;
				}
				throw std::out_of_range("rope");
			}
			T const &at(size_t N) const
			{
				size_t offset = 0;
				BOOST_FOREACH(Vector const &vec, strands)
				{
					size_t local = N - offset;
					if (local < vec.size())
					{
						return vec.at(local);
					}
					offset += vec.size();
					if (offset < N)
						break;
				}
				throw std::out_of_range("rope");
			}
			T &operator[](size_t N)
			{
				return at(N);
			}
			T const &operator[](size_t N) const
			{
				return at(N);
			}
		};
	}
}

//EOF
