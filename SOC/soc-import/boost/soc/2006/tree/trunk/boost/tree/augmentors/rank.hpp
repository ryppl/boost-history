// Copyright (c) 2006 Bernhard Reiter.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_AUGMENTORS_RANK_HPP
#define BOOST_TREE_AUGMENTORS_RANK_HPP


//how do we know what size_type to use?
//1. from base_type
//2. from "instantiate node type with trivial augmentor" - size_type. cool.

template <typename SizeType = std::size_t>
class rank_search
{
 public:
	typedef SizeType size_type;
	//get_rank - just show rank info	
private:
	size_type rank;
};

//cursor part: what is my rank?
//tree part: go to rank...
#endif // BOOST_TREE_AUGMENTORS_RANK_HPP


