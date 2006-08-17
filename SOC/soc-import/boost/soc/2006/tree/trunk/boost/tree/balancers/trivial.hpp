// Copyright (c) 2006 Bernhard Reiter.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BALANCERS__TRIVIAL_HPP
#define BALANCERS__TRIVIAL_HPP

namespace boost {
namespace tree {

struct trivial_metadata {};

struct trivial_balance {

	trivial_balance() {}

	struct metadata_type {};
	metadata_type metadata;
	
	template <class Cursor>
	static void add(Cursor, Cursor)
	{ }
	  
	template <class Cursor>
	static void remove(Cursor)
	{ }
	
	template <class Cursor>
	static void read(Cursor)
{ }

};

} // namespace tree
} // namespace boost

#endif // BALANCERS__TRIVIAL_HPP


