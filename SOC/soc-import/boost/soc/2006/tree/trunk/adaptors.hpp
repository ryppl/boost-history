// Copyright 2006 Bernhard Reiter.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_HH
#define ADAPTORS_HH

#include <boost/iterator/iterator_adaptor.hpp>

# ifndef BOOST_NO_SFINAE
#  include <boost/type_traits/is_convertible.hpp>
#  include <boost/utility/enable_if.hpp>
# endif

/** \file adaptors.hh 
 * \brief Standard iterator implementations for a given Node type.
 * Standard iterator_adaptors around the (template parameter) Node type 
 * implementing fallback traversal mechanisms for various iterators,
 * depending on their CategoryOrTraversal properties.
 * The idea is specially that if there are ways to get the next or prior
 * element for a kind of simple traversal, and other traversal methods can be
 * composed of such simpler traversals in a straight-forward manner (though 
 * probably not most efficient), we should provide those traversal methods 
 * (as iterators), as they will probably needed by the tree user.
 * We leave optimisations, such as using the last child node's next_sibling
 * link for special purposes (e.g. point to the parent's next non-leaf sibling's
 * first_child for fast level-traversal), to specialisations.
 */

template <typename Node, typename Alloc>
class sibling_iter
{
};


template <typename Node, typename Alloc>
class sibling_container
{
 public:
 	typedef Alloc allocator_type;
 	sibling_container(Alloc const& alloc = Alloc())
 	  : m_alloc(alloc) {}
 	
 private:
 	allocator_type const& m_alloc;
 	//node with first and last child*
};

#endif // ADAPTORS_HH
