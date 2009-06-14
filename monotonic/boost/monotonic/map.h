// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <map>
#include <boost/monotonic/allocator.h>

namespace boost
{
	namespace monotonic
	{
		/// A std::map<K,T,P> that uses a monotonic allocator
		template <class K, class T, class P = std::less<K> >
		struct map : std::map<K,T,P, allocator<K> >
		{
			typedef allocator<K> Allocator;
			typedef std::map<K,T,P,Allocator > Map;
			typedef P Predicate;

			map() { }
			map(Allocator const &A)
				: Map(Predicate(), A) { }
			map(Predicate Pr, Allocator const &A) 
				: Map(Pr, A) { }
			template <class II>
			map(II F, II L, Allocator const &A, Predicate const &Pr = Predicate())
				: Map(F,L,Pr,A) { }
			template <class II>
			map(II F, II L, Predicate const &Pr, Allocator const &A)
				: Map(F,L,Pr,A) { }
		};
	}
}

//EOF
