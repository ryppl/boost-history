// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <boost/monotonic/allocator.h>
#include <boost/monotonic/storage_base.h>

namespace boost
{
	namespace monotonic
	{
		/// a std::vector<T> that uses a monotonic allocator
		template <class T>
		struct vector : std::vector<T, allocator<T> >
		{
			typedef allocator<T> Allocator;
			typedef std::vector<T,Allocator> Vector;

			vector() { }
			vector(Allocator const &A) 
				: Vector(A) { }
			vector(size_t N, T const &X, Allocator const &A)
				: Vector(N,X,A) { }
			template <class II>
			vector(II F, II L, Allocator const &A)
				: Vector(F,L,A) { }
		};
	}
}

//EOF
