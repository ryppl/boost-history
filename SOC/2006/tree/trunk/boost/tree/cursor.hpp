//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file cursor.hpp
 * Cursor traits structs, traversal tags, ...
 */
 
#ifndef BOOST_TREE_CURSOR_HPP
#define BOOST_TREE_CURSOR_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_adaptor.hpp>

#include <stddef.h>
#include <iterator>

using std::input_iterator_tag;
using std::output_iterator_tag;
using std::forward_iterator_tag;
using std::bidirectional_iterator_tag;
using std::random_access_iterator_tag;

namespace boost {
namespace tree {


template <class Cursor>
struct cursor_value {
    typedef typename Cursor::value_type type;
};

template <class Cursor>
struct cursor_reference {
    typedef typename Cursor::reference type;
};

template <class Cursor>
struct cursor_const_reference {
    typedef typename Cursor::const_reference type;
};

template <class Cursor>
struct cursor_pointer {
    typedef typename Cursor::pointer type;
};

template <class Cursor>
struct cursor_difference {
    typedef typename Cursor::difference_type type;
};

template <class Cursor>
struct cursor_size {
    typedef typename Cursor::size_type type;
};

template <class Cursor>
struct cursor_category {
    typedef typename Cursor::cursor_category type;
};

template <class Cursor>
struct cursor_horizontal_traversal {
    typedef typename Cursor::horizontal_traversal type;
};

template <class Cursor>
struct cursor_vertical_traversal {
    typedef typename Cursor::vertical_traversal type;
};

template <class Cat, class T, class Dist = ptrdiff_t, class Size = size_t,
          class Ptr = T*, class Ref = T&>
struct cursor {
    typedef Cat        cursor_category;
    typedef T        value_type;
    typedef Dist    difference_type;
    typedef Size    size_type;
    typedef Ptr        pointer;
    typedef Ref        reference;    
};

// Deprecate?
struct cursor_tag {};

struct descending_tag {}; 
struct descending_cursor_tag
    : public cursor_tag, public descending_tag, 
      public input_iterator_tag, public output_iterator_tag {};
struct descending_forward_cursor_tag
    : public cursor_tag, public descending_tag, public forward_iterator_tag {};
struct descending_bidirectional_cursor_tag
    : public cursor_tag, public descending_tag, public bidirectional_iterator_tag {};
struct descending_random_access_cursor_tag
    : public cursor_tag, public descending_tag, public random_access_iterator_tag {};

struct ascending_tag {};
struct ascending_cursor_tag
    : public descending_cursor_tag, public ascending_tag {};
struct ascending_forward_cursor_tag
    : public descending_forward_cursor_tag, public ascending_tag {};
struct ascending_bidirectional_cursor_tag
    : public descending_bidirectional_cursor_tag, public ascending_tag {};
struct ascending_random_access_cursor_tag
    : public descending_random_access_cursor_tag, public ascending_tag {};

/*
template <class Hor, class Vert>
struct produce_cursor_category;

template <>
struct produce_cursor_category<> {
    typedef descending_cursor_tag type;
};
*/

//define freestanding begin, end, size, empty using cursor's member fns?

template <
    class OutputIterator
//  , class Value                         
//  , class HorizontalTraversalOrCategory 
//  , class VerticalTraversalOrCategory
//  , class Reference                     
//  , class Difference                
//  , class Size                          
>
class output_cursor_iterator_wrapper;

/**
 * @brief Output cursor wrapper around an output iterator.
 * 
 * This can be very useful e.g. to have cursor algorithms actually work on
 * iterators, thus permitting some kind of linearization of a given subtree.
 * (Modelled after std::insert_iterator and the like.)
 * 
 * For construction, the outputter_cursor_iterator_wrapper might come in useful
 * in saving keystrokes.
 */
// TODO: Complete this.
// Shouldn't we be using cursor_facade?
template <
    class OutputIterator
//  , class Value                         = use_default
//  , class HorizontalTraversalOrCategory = use_default
//  , class VerticalTraversalOrCategory   = bidirectional_traversal_tag
//  , class Reference                     = use_default
//  , class Difference                    = use_default
//  , class Size                          = use_default
>
class output_cursor_iterator_wrapper {
protected:
    OutputIterator* iter;
//private:
//    typedef iterator_adaptor<output_cursor_iterator_wrapper<OutputIterator> 
//                         , OutputIterator
//                         , Value
//                         , HorizontalTraversalOrCategory
//                         , Reference
//                         , Difference> ia_type;
public:
    /// Make the iterator type publicly accessible.
    typedef OutputIterator iterator;

    // FIXME: Very adhoc.
    typedef output_cursor_iterator_wrapper<OutputIterator> value_type;
    typedef std::size_t size_type;
    typedef output_cursor_iterator_wrapper<OutputIterator> const_cursor;
    typedef forward_traversal_tag horizontal_traversal;
    typedef bidirectional_traversal_tag vertical_traversal;
    typedef forward_traversal_tag iterator_category;
    typedef std::ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;
        
    /**
     * For construction, we obviously need an Output Iterator to work on (i.e., write to).
     */
    explicit output_cursor_iterator_wrapper(OutputIterator& i) : iter(&i) {}

    /** 
     * @param value A const& value of the value_type of container that iter is
     *              associated with.
     * @return      This cursor, for chained operations.
     * Assigning a value to this cursor will insert it before iter, the iterator it is
     * wrapped around.
     * 
     * Unfortunately, Output Iterators do not necessarily expose their
     * value_type (they might just give back void), so the following assignment operator
     * has to be a template.
     */
    // TODO: Consult C++0x if this has been changed
    template <class ValueType>
    output_cursor_iterator_wrapper& operator=(ValueType const& value)
    { 
        *((*iter)++) = value;
        return *this; 
    }

    /// Returns *this.
    output_cursor_iterator_wrapper& operator*() { return *this; }

    /// Returns *this, as this %cursor doesn't "move".
    output_cursor_iterator_wrapper& operator++() { return *this; }

    /// Returns *this, as this %cursor doesn't "move".
    output_cursor_iterator_wrapper operator++(int) { return *this; }

    /// Returns *this, as this %cursor doesn't "move".
    output_cursor_iterator_wrapper& operator--() { return *this; }

    /// Returns *this, as this %cursor doesn't "move".
    output_cursor_iterator_wrapper operator--(int) { return *this; }
    
    /// Returns *this, as this %cursor doesn't "move".
    output_cursor_iterator_wrapper& to_begin() { return *this; }
    output_cursor_iterator_wrapper& begin() { return *this; }

    /// Returns *this, as this %cursor doesn't "move".
    output_cursor_iterator_wrapper& to_end() { return *this; }
    output_cursor_iterator_wrapper& end() { return *this; }

    /// Returns *this, as this %cursor doesn't "move".
    output_cursor_iterator_wrapper& to_parent() { return *this; }
    output_cursor_iterator_wrapper& parent() { return *this; }
    
    /// Returns true, in case an algorithm has a loop only terminating at root.
    bool is_root() const { return true; }
    
    /// Returns true, in case an algorithm has a loop only terminating at a leaf.
    bool empty() const { return true; }
    
    std::size_t const index() const { return 0; }
};

template <class OutputIterator>
typename output_cursor_iterator_wrapper<OutputIterator>::size_type
index(output_cursor_iterator_wrapper<OutputIterator> const& cur)
{
    return cur.index();
}

/** 
 * @param o    An output iterator.
 * @result    An instance of output_cursor_iterator_wrapper working on o.
 * 
 * Use as shortcut for cumbersome typenames, just as with std::inserter and the like.
 */
template<class OutputIterator>
inline output_cursor_iterator_wrapper<OutputIterator>
outputter_cursor_iterator_wrapper(OutputIterator o)
{
    return output_cursor_iterator_wrapper<OutputIterator>(o);
}

} // namespace tree
} // namespace boost


#endif // BOOST_TREE_CURSOR_HPP
