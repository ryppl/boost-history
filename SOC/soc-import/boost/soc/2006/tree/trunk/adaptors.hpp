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

/* \class sibling_iter
 * Base template for sibling_iterator and const_sibling_iterator
 * with fallback declarations of increment() and decrement()
 */
// TODO: 
// what names should the different category/traversal types be given?
// forest_horizontal seems kinda ok, as it describes that
// we are checking for e.g. forward and backward iteration 
// "on the current horizontal level" -- we can use this from
// sibling_iterator, level_iterator, as well as from pre- and postorder
// iterators
// It's however a bit more difficult with other "directions",
// e.g. what is the best way to communicate if there is a parent() function?
// for a parent_iterator, and should we support e.g. a last_child()
// method from the template or just leave it to the specializations?
// Also note that I'm still a little unsure about how we should map
// forest-specific properties to binary ones in our node interface

// Can we remove second templ par safely, by replacing
// forest_horizontal_traversal in sibling_iter's template argument list
// with Binary::forest_horzontal_traversal?

template <typename Binary, typename forest_horizontal_traversal
	 = typename Binary::forest_horizontal_traversal, typename Enable = void>
class sibling_iter
  : public boost::iterator_adaptor<
        sibling_iter<Binary>
      , Binary*
      , boost::use_default
      , forest_horizontal_traversal
    >
{
 private:
    struct enabler {};
    
 public:
    sibling_iter()
      : sibling_iter::iterator_adaptor_(0) {}

    explicit sibling_iter(Binary* p)
      : sibling_iter::iterator_adaptor_(p) {}

    template <class OtherBinary>
    sibling_iter(
        sibling_iter<OtherBinary> const& other
# ifndef BOOST_NO_SFINAE
      , typename boost::enable_if<
            boost::is_convertible<OtherBinary*,Binary*>
          , enabler
        >::type = enabler()
# endif 
    )
      : sibling_iter::iterator_adaptor_(other.base()) {}

//Container part:...
//insert etc.

# if !BOOST_WORKAROUND(__GNUC__, == 2)
 private: // GCC2 can't grant friendship to template member functions    
    friend class boost::iterator_core_access;
# endif     
    void increment() { this->base_reference() = this->base()->next_sibling(); }
};

/** Bidirectional sibling_iter (partial) specialisation
 * TODO: look into that. can we have this more easily than via copy+paste?
 * derive specialisation from general template (CRTP...?)
 * disable decrement via enable_if? does that work? 
 */

template <typename Binary, typename forest_horizontal_traversal>
class sibling_iter<
	Binary, forest_horizontal_traversal, 
	typename boost::enable_if<typename boost::is_convertible<
		typename Binary::forest_horizontal_traversal, 
		typename boost::bidirectional_traversal_tag
	>::type>
>
  : public boost::iterator_adaptor<
        sibling_iter<Binary>
      , Binary*
      , boost::use_default
      , forest_horizontal_traversal
    >
{
 private:
    struct enabler {};
    
 public:
    sibling_iter()
      : sibling_iter::iterator_adaptor_(0) {}

    explicit sibling_iter(Binary* p)
      : sibling_iter::iterator_adaptor_(p) {}

    template <class OtherBinary>
    sibling_iter(
        sibling_iter<OtherBinary> const& other
# ifndef BOOST_NO_SFINAE
      , typename boost::enable_if<
            boost::is_convertible<OtherBinary*,Binary*>
          , enabler
        >::type = enabler()
# endif 
    )
      : sibling_iter::iterator_adaptor_(other.base()) {}

# if !BOOST_WORKAROUND(__GNUC__, == 2)
 private: // GCC2 can't grant friendship to template member functions    
    friend class boost::iterator_core_access;
# endif     
    void increment() { this->base_reference() = this->base()->next_sibling(); }
    void decrement() { this->base_reference() = this->base()->previous_sibling(); }
};


#endif // ADAPTORS_HH
