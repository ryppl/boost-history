// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_SET_H
#define BOOST_MONOTONIC_SET_H

#include <set>
#include <boost/monotonic/allocator.h>

namespace boost
{
	namespace monotonic
	{
		/// A std::set<T,P> that uses a monotonic allocator
		template <class T, class P = std::less<T> >
		struct set : std::set<T,P, allocator<T> >
		{
			typedef allocator<T> Allocator;
			typedef P Predicate;
			typedef std::set<T,P,allocator<T> > Set;

			set() { }
			set(Allocator const &A) 
				: Set(Predicate(), A) { }	
			set(Predicate Pr, Allocator const &A) 
				: Set(Pr, A) { }
			template <class II>
			set(II F, II L, Allocator const &A)
				: Set(F,L,Predicate(),A) { }
			template <class II>
			set(II F, II L, Predicate const &Pr, Allocator const &A)
				: Set(F,L,Pr,A) { }
		};
	}
}

#endif // BOOST_MONOTONIC_SET_H

//EOF
