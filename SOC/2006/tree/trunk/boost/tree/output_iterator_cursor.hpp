//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file output_iterator_cursor.hpp
 * Cursor wrapper around a given output iterator.
 */
 
#ifndef BOOST_TREE_OUTPUT_ITERATOR_CURSOR_HPP
#define BOOST_TREE_OUTPUT_ITERATOR_CURSOR_HPP

#include <boost/tree/cursor.hpp>

namespace boost {
namespace tree {

template <
    class OutputIterator
//  , class Value                         
//  , class HorizontalTraversalOrCategory 
//  , class VerticalTraversal
//  , class Reference                     
//  , class Difference                
//  , class Size                          
>
class output_iterator_cursor;

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
//  , class VerticalTraversal   = bidirectional_traversal_tag
//  , class Reference                     = use_default
//  , class Difference                    = use_default
//  , class Size                          = use_default
>
class output_iterator_cursor {
protected:
    OutputIterator* iter;
//private:
//    typedef iterator_adaptor<output_iterator_cursor<OutputIterator> 
//                         , OutputIterator
//                         , Value
//                         , HorizontalTraversalOrCategory
//                         , Reference
//                         , Difference> ia_type;
public:
    /// Make the iterator type publicly accessible.
    typedef OutputIterator iterator;

    // FIXME: Very adhoc.
    typedef output_iterator_cursor<OutputIterator> value_type;
    typedef std::size_t size_type;
    typedef output_iterator_cursor<OutputIterator> const_cursor;
    typedef boost::forward_traversal_tag horizontal_traversal;
    typedef boost::tree::ascending_vertical_traversal_tag vertical_traversal;
    typedef boost::forward_traversal_tag iterator_category;
    typedef std::ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;
        
    /**
     * For construction, we obviously need an Output Iterator to work on (i.e., write to).
     */
    explicit output_iterator_cursor(OutputIterator& i) : iter(&i) {}

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
    output_iterator_cursor& operator=(ValueType const& value)
    { 
        *((*iter)++) = value;
        return *this; 
    }

    /// Returns *this.
    output_iterator_cursor& operator*() { return *this; }

    /// Returns *this, as this %cursor doesn't "move".
    output_iterator_cursor& operator++() { return *this; }

    /// Returns *this, as this %cursor doesn't "move".
    output_iterator_cursor operator++(int) { return *this; }

    /// Returns *this, as this %cursor doesn't "move".
    output_iterator_cursor& operator--() { return *this; }

    /// Returns *this, as this %cursor doesn't "move".
    output_iterator_cursor operator--(int) { return *this; }
    
    /// Returns *this, as this %cursor doesn't "move".
    output_iterator_cursor& to_begin() { return *this; }
    output_iterator_cursor& begin() { return *this; }

    /// Returns *this, as this %cursor doesn't "move".
    output_iterator_cursor& to_end() { return *this; }
    output_iterator_cursor& end() { return *this; }

    /// Returns *this, as this %cursor doesn't "move".
    output_iterator_cursor& to_parent() { return *this; }
    output_iterator_cursor& parent() { return *this; }
    
    /// Returns true, in case an algorithm has a loop only terminating at root.
    bool is_root() const { return true; }
    
    /// Returns true, in case an algorithm has a loop only terminating at a leaf.
    bool empty() const { return true; }
    
    std::size_t const index() const { return 0; }
};

template <class OutputIterator>
typename output_iterator_cursor<OutputIterator>::size_type
index(output_iterator_cursor<OutputIterator> const& cur)
{
    return cur.index();
}

/** 
 * @param o    An output iterator.
 * @result    An instance of output_iterator_cursor working on o.
 * 
 * Use as shortcut for cumbersome typenames, just as with std::inserter and the like.
 */
template<class OutputIterator>
inline output_iterator_cursor<OutputIterator>
outputter_cursor_iterator_wrapper(OutputIterator o)
{
    return output_iterator_cursor<OutputIterator>(o);
}

} // namespace tree
} // namespace boost


#endif // BOOST_TREE_OUTPUT_ITERATOR_CURSOR_HPP
