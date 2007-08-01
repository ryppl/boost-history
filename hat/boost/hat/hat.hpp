//  Boost hat/hat.hpp header file  -------------------------------------------//

// Copyright 2007 Daryle Walker.
// Distributed under the Boost Software License, Version 1.0. (See the
// accompanying file LICENSE_1_0.txt or a copy at
// <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/hat/> for the library's home page.

/** \file
    \brief  A container class template with random selection of its contents

    Contains the declaration (and definition) of the \c boost::hats::hat
    container class template with related operator and function templates.
 */

#ifndef BOOST_HAT_HAT_HPP
#define BOOST_HAT_HAT_HPP

#include <algorithm>  // for std::swap, equal, lexicographical_compare
#include <iterator>   // for iterator categories and traits
#include <utility>    // for std::pair, make_pair

#include <boost/assert.hpp>                                  // for BOOST_ASSERT
#include <boost/concept_check.hpp>            // for the concept-checking system
#include <boost/multi_index_container.hpp>         // for the multi-index system
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/operators.hpp>                         // for operator templates
#include <boost/random/uniform_int.hpp>          // for the random-number system
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/static_assert.hpp>                    // for BOOST_STATIC_ASSERT
#include <boost/tuple/tuple.hpp>                        // for boost::make_tuple
#include <boost/type_traits/is_same.hpp>                   // for boost::is_same


namespace boost
{
namespace hats
{


//  Forward declarations  ----------------------------------------------------//

// The main type
template < typename Key, class RandomNumberGenerator = mt19937,
 class Compare = ::std::less<Key>, class Allocator = ::std::allocator<Key> >
    class hat;

// Operators for the main type
template < typename Key1, typename Key2, class Rng1, class Rng2, class Cmp1,
 class Cmp2, class Al1, class Al2 >
    bool  operator ==( hat<Key1, Rng1, Cmp1, Al1> const &l, hat<Key2, Rng2,
     Cmp2, Al2> const &r );
template < typename Key1, typename Key2, class Rng1, class Rng2, class Cmp1,
 class Cmp2, class Al1, class Al2 >
    bool  operator !=( hat<Key1, Rng1, Cmp1, Al1> const &l, hat<Key2, Rng2,
     Cmp2, Al2> const &r );

template < typename Key1, typename Key2, class Rng1, class Rng2, class Cmp1,
 class Cmp2, class Al1, class Al2 >
    bool  operator  <( hat<Key1, Rng1, Cmp1, Al1> const &l, hat<Key2, Rng2,
     Cmp2, Al2> const &r );
template < typename Key1, typename Key2, class Rng1, class Rng2, class Cmp1,
 class Cmp2, class Al1, class Al2 >
    bool  operator  >( hat<Key1, Rng1, Cmp1, Al1> const &l, hat<Key2, Rng2,
     Cmp2, Al2> const &r );
template < typename Key1, typename Key2, class Rng1, class Rng2, class Cmp1,
 class Cmp2, class Al1, class Al2 >
    bool  operator <=( hat<Key1, Rng1, Cmp1, Al1> const &l, hat<Key2, Rng2,
     Cmp2, Al2> const &r );
template < typename Key1, typename Key2, class Rng1, class Rng2, class Cmp1,
 class Cmp2, class Al1, class Al2 >
    bool  operator >=( hat<Key1, Rng1, Cmp1, Al1> const &l, hat<Key2, Rng2,
     Cmp2, Al2> const &r );

// Specialized algorithms
template < typename Key, class Rng, class Cmp, class Al >
    void  swap( hat<Key, Rng, Cmp, Al> &a, hat<Key, Rng, Cmp, Al> &b );


//  Hat random-access container class template declaration  ------------------//

/** \brief  A set-like container that provides random selection of its contents,
            similar to pulling names from a hat.

    \author  Daryle Walker (Boost implementation)
    \author  Jonathan Wooldridge [a.k.a. AngleWyrm] (original concept)

    Selection of a contained value can be repeated, like rolling dice, or
    removed once chosen, like drawing cards.  This variant of the hat container
    maintains equal choosing-weight for all stored values.  It meets the
    requirements of a <a href
    ="http://www.sgi.com/tech/stl/UniqueSortedAssociativeContainer.html">unique
    sorted associative container</a>.

    \pre  <code>typename <var>Allocator</var>&nbsp;::&nbsp;value_type</code> is
          the same as <code><var>Key</var></code>.

    \param Key                    The type for the contained objects.  It must
                                  be <a href
                  ="http://www.sgi.com/tech/stl/Assignable.html">assignable</a>.
    \param RandomNumberGenerator  The type of the uniform random number
                                  generation engine.  It should have an
                                  interface compatible with the engines used in
                                  the Boost.Random library, including being <a
              href="http://www.sgi.com/tech/stl/Assignable.html">assignable</a>.
                                  (This container will supply the corresponding
                                  distribution and front-end objects.)  If not
                                  given, \c boost::mt19937 is the default.
    \param Compare                The type used for comparison objects.  It must
                                  provide a <a href
                   ="http://www.sgi.com/tech/stl/StrictWeakOrdering.html">strict
                                  weak ordering</a>.  If not given,
                                  <code>std::less&lt;<var>Key</var>&gt;</code>
                                  is the default;
    \param Allocator              The base type used for objects needed to
                                  allocate and deallocate any memory required.
                                  It must meet the C++ (2003) standard for such
                                  allocator types described in section 20.1.5
                                  [lib.allocator.requirements].  If not given,
                               <code>std::allocator&lt;<var>Key</var>&gt;</code>
                                  is the default;

    \todo  Can the container's size get big enough, or the RNG engine's
           randomness small enough, that gaps will exist between chosen indices?
           If so, how can we tell?  Is there some general parameter that can be
           read from the engine so we'll know when this problem happens?
    \todo  Figure out the complexity of the various (member) functions
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
class hat
{
    // Pre-conditions
    BOOST_CLASS_REQUIRE( RandomNumberGenerator, boost, SGIAssignableConcept );
    BOOST_STATIC_ASSERT((is_same<Key, typename Allocator::value_type>::value));

    // Helper types
    typedef multi_index_container<
        Key,
        multi_index::indexed_by<
            multi_index::ordered_unique<
                multi_index::identity<Key>,
                Compare
            >,
            multi_index::random_access<>
        >,
        Allocator
    >  container;

    typedef typename container::template nth_index<0>::type     set_view;
    typedef typename container::template nth_index<1>::type  vector_view;

    typedef typename set_view::const_iterator     s_iterator;
    typedef typename vector_view::const_iterator  v_iterator;

    typedef uniform_int<typename container::size_type>           dist_type;
    typedef variate_generator<RandomNumberGenerator, dist_type>  generator;

public:
    // Template parameters
    /** \brief  Type of element keys

        Represents the type as a key for elements.  For set types, the elements
        themselves are used as keys.
     */
    typedef Key                          key_type;
    /** \brief  Type of the comparator

        Represents the class with the criterion for the container's comparisons
        between keys.
     */
    typedef Compare                      key_compare;
    /** \brief  Type of the allocator

        Represents the class used for making memory (de)allocations.
     */
    typedef Allocator              allocator_type;
    /** \brief  Type of the random number generation engine

        Represents the class used for choosing randomly-selected numbers.
     */
    typedef RandomNumberGenerator  generator_type;

    // Other types
    /** \brief  Type of elements

        Represents the type of the container's elements.  For set types, the
        element type is also the key type.
     */
    typedef typename set_view::value_type        value_type;
    /** \brief  Type of the whole-element comparator

        Represents the class with the criterion for the container's comparisons
        between elements.  Since the keys are reused as values for set types,
        this type should the same as the key-comparison type.  However, due to
        the implementation, it's only compatible with the key-comparison type.
     */
    typedef typename set_view::value_compare  value_compare;

    /** \brief  Type of element references

        Represents the type of references to the container's elements.  For set
        types, the elements cannot be directly mutable, so this type is fairly
        useless.
     */
    typedef typename allocator_type::reference              reference;
    /** \brief  Type of element references with immutable access

        Represents the type of references to the container's elements with
        immutable access.
     */
    typedef typename allocator_type::const_reference  const_reference;

    /** \brief  Type of a pointer to the element type

        Represents the type of pointers to a container's element.  For set
        types, the elements cannot be directly mutable, so this type is fairly
        useless.
     */
    typedef typename allocator_type::pointer              pointer;
    /** \brief  Type of a pointer to the element type via immutable access

        Represents the type of pointers to a container's element with
        immutable access.
     */
    typedef typename allocator_type::const_pointer  const_pointer;

    /** \brief  Type of size values

        Represents the type used for sizing parameters and returns.  It should
        be an unsigned integer.
     */
    typedef typename container::size_type              size_type;
    /** \brief  Type of difference values

        Represents the type used to measure the span between elements.  It
        should be a signed integer.
     */
    typedef typename container::difference_type  difference_type;

    /** \brief  Type of choosing-weight values

        Represents the type used to express relative choosing-weights for
        elements.  It should be an unsigned integer.  (Weights must be
        expressible as positive integers, with the value 0 reserved for element
        values that are currently unregistered.)
     */
    typedef size_type  weight_type;

    /** \brief  Type of iterators into the container's elements with immutable
                access

        Represents the type for iterating traversal among the container's
        elements with immutable access.

        \note  The Boost.Operators library is used to synthesize the following
               operators: <code>!=</code>, <code>++</code> (post), and
               <code>--</code> (post).
     */
    class const_iterator
        : equality_comparable1< const_iterator, unit_steppable<const_iterator> >
    {
    public:
        // Types
        /** \brief  Iteration category of this type

            Indicates which style of iteration this type represents.
         */
        typedef ::std::bidirectional_iterator_tag  iterator_category;
        /** \brief  Type of iterated elements

            Represents the type of the elements that this type "points to."
         */
        typedef typename hat::value_type                  value_type;
        /** \brief  Type of difference values

            Represents the type used to measure the distance between iterators.
         */
        typedef typename hat::difference_type        difference_type;
        /** \brief  Type of a pointer to the iterated element's type

            Represents the type of pointers to an iterator's element.  For set
            iterator types, the elements are treated as immutable.
         */
        typedef typename hat::const_pointer                  pointer;
        /** \brief  Type of iterated-element references

            Represents the type of references to an iterator's element.  For set
            iterator types, the elements are treated as immutable.
         */
        typedef typename hat::const_reference              reference;

        // Operators (use automatic copy-assignment operator)
        /** \brief  Equals

            Compares two iterators for equivalence.  Such objects are equal if
            they refer to either the same element object or the past-the-end
            mark for the same container.

            \param l  The left-side operand to be compared.
            \param r  The right-side operand to be compared.

            \retval true   \p l and \p r are equivalent.
            \retval false  \p l and \p r are not equivalent.

            \note  The class declaration uses the Boost.Operators library to
                   synthesize <code>operator !=</code> from this operator.
         */
        friend
        bool  operator ==( const_iterator const &l, const_iterator const &r )
        { return l.inner_ == r.inner_; }

        /** \brief  Indirection

            Dereferences the iterator to provide "direct" access to the target
            element.

            \pre  \c *this cannot be targeting a past-the-end mark.

            \return  A reference to this object's target.
         */
        reference  operator  *() const  { return *this->inner_; }
        /** \brief  Class member access (via pointer)

            Dereferences the iterator to provide access to the target element in
            a pointer-like style.

            \pre  \c *this cannot be targeting a past-the-end mark.

            \return  A pointer to this object's target.

            \note  The class declaration does \em not use the Boost.Operators
                   library to synthesize this operator from the unary
                   <code>operator *</code> since the conversion (as of this
                   writing) is fixed to always use the unary <code>operator
                   &amp;</code>, which may not be how customized reference and
                   pointer types relate.
         */
        pointer    operator ->() const  { return this->inner_.operator ->(); }

        /** \brief  Prefix-increment

            Changes the iterator's target to the element following the current
            element in iteration order.

            \pre  \c *this cannot be targeting a past-the-end mark.

            \return  A reference to <code>*this</code> object.

            \post  <code>1 == std::distance( <var>old_this</var>, *this )</code>

            \note  The class declaration uses the Boost.Operators library to
                   synthesize the postfix-increment <code>operator ++</code>
                   from this one.
         */
        const_iterator &  operator ++()  { ++this->inner_; return *this; }
        /** \brief  Prefix-decrement

            Changes the iterator's target to the element trailing the current
            element in iteration order.

            \pre  \c *this cannot be targeting a beginning element.

            \return  A reference to <code>*this</code> object.

            \post  <code>1 == std::distance( *this, <var>old_this</var> )</code>

            \note  The class declaration uses the Boost.Operators library to
                   synthesize the postfix-decrement <code>operator --</code>
                   from this one.
         */
        const_iterator &  operator --()  { --this->inner_; return *this; }

    private:
        friend class hat;

        // Lifetime management (use automatic [public] dtr and copy ctr)
        explicit  const_iterator( typename hat::s_iterator const &tracker )
            : inner_( tracker )  {}

        // Member data
        typename hat::s_iterator  inner_;

    };  // boost::hats::hat::const_iterator
    /** \brief  Type of iterators into the container's elements

        Represents the type for iterating traversal among the container's
        elements.  For set types, the elements cannot be directly mutable, so
        this type is the same as the \c hat::const_iterator type.
     */
    typedef const_iterator  iterator;

    /** \brief  Type of iterators into the container's elements in reverse order
                with immutable access

        Represents the type for iterating traversal among the container's
        elements in reverse order with immutable access.
     */
    typedef ::std::reverse_iterator<const_iterator>  const_reverse_iterator;
    /** \brief  Type of iterators into the container's elements in reverse order

        Represents the type for iterating traversal among the container's
        elements in reverse order.  For set types, the elements cannot be
        directly mutable, so this type is the same as the
        \c #const_reverse_iterator type.
     */
    typedef const_reverse_iterator                         reverse_iterator;

    // Lifetime management (use automatic destructor and copy constructor)
    //! Default and auxillary object constructor
    explicit  hat( RandomNumberGenerator const &rng = RandomNumberGenerator(),
     Compare const &cmp = Compare(), Allocator const &alloc = Allocator() );

    //! Range (possibly with auxillary objects) constructor
    template < typename InputIterator >
    hat( InputIterator first, InputIterator last,
     RandomNumberGenerator const &rng = RandomNumberGenerator(),
     Compare const &cmp = Compare(), Allocator const &alloc = Allocator() );

    /*! \name Non-Element Accessors */ //@{
    // Auxillary objects
    /** \brief  Returns a copy of the memory allocator used

        \return  A copy of this object's allocator
     */
    allocator_type  get_allocator() const
        { return this->elements_.get_allocator(); }

    /** \brief  Returns the comparison criterion

        \return  A copy of the comparator for \c #key_type
     */
      key_compare    key_comp() const
        { return this->element_set().key_comp(); }
    /** \brief  Returns the comparison criterion used for values

        The return type bases on something internal to the implementation.  It
        has the same behavior as \c #key_compare only for the <code>operator
        ()</code> that works with \c #key_type values, but there is no other
        part of the interface (other <code>operator ()</code>, other operators,
        type-aliases, etc.) carried over.

        \return  A copy of the comparator for \c #value_type
     */
    value_compare  value_comp() const
        { return this->element_set().value_comp(); }

    /** \brief  Refers to the random number generator engine

        This mutable method of access enables any post-construction adjustments
        of the random number engine.  (The engine is the only support-part of a
        hat that can be changed after construction, and it's the only one that
        should since the others control the set-container invariants.)  An
        example for mutable access would be to change the engine's seed, to
        ensure that it's independent from the engine from which it was copied
        (or default-constructed).

        \return  A reference to this object's random number generator engine
     */
    generator_type &        rng_engine()
        { return this->generator_.engine(); }
    /** \brief  Refers to the random number generator engine, with immutable
                access

        \overload

        Inspection of the RNG engine is by reference since <code>sizeof(
        generator_type )</code> may be large.

        \return  A reference to this object's random number generator engine
     */
    generator_type const &  rng_engine() const
        { return this->generator_.engine(); }//@}

    /*! \name Iterators */ //@{
    // Iterators
    /** \brief  Returns an iterator to the beginning element, with immutable
                access

        Iteration is done in ascending order of keys based on the comparator.

        \return  A forward-iterator to the beginning element (It equals
                 <code>this-&gt;end()</code> if this hat is empty.  Otherwise,
                 there is no other element in this hat that compares "less" than
                 this iterator's referent.)
     */
    const_iterator  begin() const
        { return const_iterator(this->element_set().begin()); }
    /** \brief  Returns an iterator to the past-the-end mark, with immutable
                access

        Iteration is done in ascending order of keys based on the comparator.

        \return  A forward-iterator to the past-the-end mark (It equals
                 <code>this-&gt;begin()</code> if this hat is empty.)
     */
    const_iterator  end() const
        { return const_iterator(this->element_set().end()); }

    /** \brief  Returns an iterator to the beginning element

        \overload

        Iteration is done in ascending order of keys based on the comparator.
        (Overload provided to complete the iterator interface.)

        \return  A forward-iterator to the beginning element (It equals
                 <code>this-&gt;end()</code> if this hat is empty.  Otherwise,
                 there is no other element in this hat that compares "less" than
                 this iterator's referent.)
     */
    iterator  begin()  { return const_cast<hat const *>(this)->begin(); }
    /** \brief  Returns an iterator to the past-the-end mark

        \overload

        Iteration is done in ascending order of keys based on the comparator.
        (Overload provided to complete the iterator interface.)

        \return  A forward-iterator to the past-the-end mark (It equals
                 <code>this-&gt;begin()</code> if this hat is empty.)
     */
    iterator  end()    { return const_cast<hat const *>(this)->end(); }

    /** \brief  Returns a reversed-iterator to the end element, with immutable
                access

        Iteration is done in descending order of keys based on the comparator.

        \return  A reverse-iterator to the end element (It equals
                 <code>this-&gt;rend()</code> if this hat is empty.  Otherwise,
                 there is no other element in this hat that compares "greater"
                 than this iterator's referent.)
     */
    const_reverse_iterator  rbegin() const
        { return const_reverse_iterator(this->end()); }
    /** \brief  Returns a reversed-iterator to the past-the-beginning mark, with
                immutable access

        Iteration is done in descending order of keys based on the comparator.

        \return  A reverse-iterator to the past-the-beginning mark (It equals
                 <code>this-&gt;rbegin()</code> if this hat is empty.)
     */
    const_reverse_iterator  rend() const
        { return const_reverse_iterator(this->begin()); }

    /** \brief  Returns a reversed-iterator to the end element

        \overload

        Iteration is done in descending order of keys based on the comparator.
        (Overload provided to complete the iterator interface.)

        \return  A reverse-iterator to the end element (It equals
                 <code>this-&gt;rend()</code> if this hat is empty.  Otherwise,
                 there is no other element in this hat that compares "greater"
                 than this iterator's referent.)
     */
    reverse_iterator  rbegin()
        { return const_cast<hat const &>(*this).rbegin(); }
    /** \brief  Returns a reversed-iterator to the past-the-beginning mark

        \overload

        Iteration is done in descending order of keys based on the comparator.
        (Overload provided to complete the iterator interface.)

        \return  A forward-iterator to the past-the-beginning mark (It equals
                 <code>this-&gt;rbegin()</code> if this hat is empty.)
     */
    reverse_iterator  rend()
        { return const_cast<hat const &>(*this).rend(); }//@}

    /*! \name Size & Capacity Operations */ //@{
    // Capacity
    /** \brief  Returns the number of elements contained

        \return  The number of distinct (as determined by the comparator) values
                 <var>v</var> such that <code>this-&gt;count( <var>v</var> ) ==
                 1</code>
     */
    size_type  size() const  { return this->element_set().size(); }
    /** \brief  Checks if any elements are contained

        This may be faster than using <code>this-&gt;size()</code> and an
        equality-comparison with zero.

        \retval true   <code>this-&gt;size() == 0</code>
        \retval false  <code>this-&gt;size() &gt; 0</code>
     */
    bool      empty() const  { return this->element_set().empty(); }

    /** \brief  Returns the maximum number of elements that can be contained

        \return  The highest value <code>this-&gt;size()</code> (and
                 <code>this-&gt;capacity()</code>) could have
     */
    size_type  max_size() const
    {
        return ( this->element_set().max_size() <
         this->element_vector().max_size() ) ? this->element_set().max_size() :
         this->element_vector().max_size();
    }

    /** \brief  Returns the maximum number of elements before reallocation

        Any insertions such that this object's element-length doesn't exceed
        this return value will occur with complexity <b>O</b>( log
        <code>this-&gt;size()</code> ) directly, instead of <em>amortized</em>.

        \return  The highest value <code>this-&gt;size()</code> could have
                 before reallocations need to be done
     */
    size_type  capacity() const
        { return this->element_vector().capacity(); }
    /** \brief  Reserves space for a given (lower-bound) number of elements

        There is no effect if \p n is not greater than the current
        <code>this-&gt;capacity()</code> value.  The complexity is <b>O</b>(
        <code>this-&gt;size()</code> ) if the capacity needed to increase,
        <b>O</b>( 1 ) otherwise.

        \param n  The desired minimum capacity

        \post  <code>this-&gt;capacity() >= <var>n</var></code>
     */
    void       reserve( size_type n )
    {
        this->element_vector().reserve(n);
        BOOST_ASSERT( this->test_invariant() );
    }//@}

    /*! \name Container Operations */ //@{
    // Containment
    //! Inserts a copy of the given value (if not already present)
    ::std::pair<iterator, bool>   insert( value_type const &x );
    //! Secures a copy of the given value for containment, with insertion hint
    iterator   insert( iterator position, value_type const &x );
    //! Inserts a copy of all the values taken from the given iterator range
    template < typename InputIterator >
        void  insert( InputIterator first, InputIterator last );

    //! Removes the given element
    void       erase( iterator position );
    //! Removes all elements that correspond with the given key
    size_type  erase( key_type const &x );
    //! Removes all elements from the given iterator range
    void       erase( iterator first, iterator last );
    //! Removes all elements from the container
    void       clear();

    //! Exchanges state with the given object
    void  swap( hat &other );//@}

    /*! \name Set Operations */ //@{
    // Set operations
    /** \brief  Returns the position of the given key

        The complexity is <b>O</b>( log <code>this-&gt;size()</code> ).

        \param x  The value to search for

        \return  An iterator to the element that ranks equivalent to \p x
                 (relative to this object's comparator),
                 <code>this-&gt;end()</code> if no such element exists
     */
    iterator    find( key_type const &x ) const
        { return const_iterator(this->element_set().find( x )); }
    /** \brief  Returns the number of elements that correspond with the given
                key

        The complexity is <b>O</b>( log <code>this-&gt;size()</code> ).

        \param x  The value to search for

        \return  The number of times the value of \p x appears as an element;
                 for equal-opportunity hat types, this is always either 0 or 1
     */
    size_type  count( key_type const &x ) const
        { return this->element_set().count(x); }

    /** \brief  Returns the position of first element not ranked below the given
                key

        The complexity is <b>O</b>( log <code>this-&gt;size()</code> ).

        \param x  The value to search for

        \return  An iterator to the first element that does not rank lower than
                 \p x (relative to this object's comparator),
                 <code>this-&gt;end()</code> if no such element exists
     */
    iterator                         lower_bound( key_type const &x ) const
        { return const_iterator(this->element_set().lower_bound( x )); }
    /** \brief  Returns the position of the first element ranked above the given
                key

        The complexity is <b>O</b>( log <code>this-&gt;size()</code> ).

        \param x  The value to search for

        \return  An iterator to the first element that ranks higher than \p x
                 (relative to this object's comparator),
                 <code>this-&gt;end()</code> if no such element exists
     */
    iterator                         upper_bound( key_type const &x ) const
        { return const_iterator(this->element_set().upper_bound( x )); }
    /** \brief  Returns a key's lower- and upper-bound iterators in one object

        The complexity is <b>O</b>( log <code>this-&gt;size()</code> ).

        \param x  The value to search for

        \return  <code>std::make_pair( this-&gt;lower_bound(<var>x</var>),
                 this-&gt;upper_bound(<var>x</var>) )</code>

        \see  #lower_bound #upper_bound
     */
    ::std::pair<iterator, iterator>  equal_range( key_type const &x ) const
    {
        return ::std::make_pair(this->lower_bound( x ), this->upper_bound( x ));
    }//@}

    /*! \name Hat Operations: Selection */ //@{
    // Hat operations, selection
    //! Returns an iterator to a randomly-chosen element
    const_iterator   pick();
    //! Returns the value of a randomly-chosen element
    const_reference  sample();

    //! Copies out a randomly-chosen element before removal
    bool  draw( reference result );
    //! Removes a randomly-chosen element
    bool  discard();//@}

    /*! \name Hat Operations: Weight-Handling */ //@{
    // Hat operations, weight
    /** \brief  Returns the maximum allowed combined element weight

        \return  The highest value <code>this-&gt;total_weight()</code> could
                 have
     */
    weight_type  max_total_weight() const  { return this->max_size(); }
    /** \brief  Returns the maximum allowed weight per element

        The maximum single-element weight is without regard to the total sum of
        existing elements.  (In other words, the return value assumes that this
        object is empty.)  A given entry <var>x</var> can't have its weight
        changed beyond <b>Min</b>( <code>this-&gt;max_weight()</code>,
        <code>this-&gt;max_total_weight() - this-&gt;total_weight() +
        this-&gt;get_weight(<var>x</var> )</code>).  For equal-opportunity hat
        types, the return value is always 1.

        \return  The highest value <code>this-&gt;total_weight()</code> could
                 have
     */
    weight_type  max_weight() const  { return 1u; }

    /** \brief  Returns the weight of a given key

        \param x  The value to check the weight for

        \return  The number of chances, out of
                 <code>this-&gt;total_weight()</code>, that \p x tends to be
                 chosen after an arbitrarily large number of trials (assuming a
                 uniformly random selection)
     */
    weight_type  get_weight( key_type const &x ) const
        { return this->count(x); }
    //! Changes the weight of a given key
    bool         set_weight( key_type const &x, weight_type weight );
    /** \brief  Returns the combined weight of the contained elements

        \return  <b>Sum</b>{ <code>this-&gt;begin()</code> &lt;= <var>i</var>
                 &lt; <code>this-&gt;end()</code> }(
                 <code>this-&gt;get_weight(*<var>i</var>)</code> ); for
                 equal-opportunity hat types, the return value always equals
                 <code>this-&gt;size()</code>
     */
    weight_type  total_weight() const  { return this->size(); }//@}

private:
    // State maintainence
    bool  test_invariant() const
    {
        size_type const  es = this->elements_.size();
        size_type const  gm = this->generator_.max();

        // RNG max. value is #elements - 1; use special value for empty hat
        return ( es && (gm == es - 1u) ) || !gm;
    }

    void  set_rng_bounds()
    {
        size_type const  es = this->elements_.size();

        this->generator_.distribution() = dist_type( 0u, (es ? es - 1u : 0u) );
    }

    // Container access
    set_view const &     element_set() const
        { return this->elements_.get<0>(); }
    vector_view const &  element_vector() const
        { return this->elements_.get<1>(); }

    set_view &     element_set()     { return this->elements_.get<0>(); }
    vector_view &  element_vector()  { return this->elements_.get<1>(); }

    // Core hat selection routine
    v_iterator  choose()
        { return this->element_vector().begin() + this->generator_(); }
        // Non-const since the RNG changes as it's used

    // Member data
    container  elements_;
    generator  generator_;

};  // boost::hats::hat


} // namespace hats
} // namespace boost


//! \cond
//  Implementation details  --------------------------------------------------//

namespace boost
{
namespace hats
{
namespace detail
{

// Nothing here right now.

}  // namespace detail
}  // namespace hats
}  // namespace boost
//! \endcond


//  Hat random-access container constructor definitions  ---------------------//

/** Constructs an empty \c hat object with given random number generation
    engine, comparator, and allocator.  It also serves as a default constructor.

    \param rng    The object to be used as the engine for random-number
                  generation.  If not given, a default-constructed
                  \c #generator_type object is used.
    \param cmp    The object to be used for comparing key values.  If not given,
                  a default-constructed \c #key_compare object is used.
    \param alloc  The object to be used for any memory allocations or
                  deallocations needed.  If not given, a default-constructed
                  \c #allocator_type object is used.

    \post  <code>this-&gt;size() == 0</code>
    \post  <code><var>rng</var> == this-&gt;rng_engine()</code>
    \post  <code><var>cmp</var> == this-&gt;key_comp()</code>
    \post  <code><var>allocator</var> == this-&gt;get_allocator()</code>
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
inline
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::hat
(
    RandomNumberGenerator const &  rng,   // = RandomNumberGenerator()
    Compare const &                cmp,   // = Compare()
    Allocator const &              alloc  // = Allocator()
)
    : elements_( make_tuple(make_tuple( multi_index::identity<Key>(), cmp ),
       make_tuple()), alloc )
    , generator_( rng, dist_type(0u, 0u) )
{
    BOOST_ASSERT( this->test_invariant() );
}

/** Constructs a \c hat object with given random number generation engine,
    comparator, and allocator.  Furthermore, the object will be initially filled
    via insertions of values copied over from a given iterator range.

    \pre  \p InputIterator models the input-iterator concept (at least).
    \pre  [<var>first</var>, <var>last</var>) is a valid iterator range, and the
          iterated elements can be converted to \c #value_type objects.

    \param first  An iterator targeting the beginning of the list of new
                  entries.  If the list is empty, then this iterator must equal
                  \p last instead.
    \param last   An iterator targeting the past-the-end mark of the list of new
                  entries.  It must be reachable from \p first through a
                  nonnegative number of increments.
    \param rng    The object to be used as the engine for random-number
                  generation.  If not given, a default-constructed
                  \c #generator_type object is used.
    \param cmp    The object to be used for comparing key values.  If not given,
                  a default-constructed \c #key_compare object is used.
    \param alloc  The object to be used for any memory allocations or
                  deallocations needed.  If not given, a default-constructed
                  \c #allocator_type object is used.

    \post  <code>this-&gt;size() &lt;= std::distance(<var>first</var>,
           <var>last</var>)</code>
    \post  For a non-empty [<var>first</var>, <var>last</var>) range and all
           iterator values <var>i</var> in that range,
           <code>this-&gt;count(*<var>i</var>) == 1</code>.
    \post  <code><var>rng</var> == this-&gt;rng_engine()</code>
    \post  <code><var>cmp</var> == this-&gt;key_comp()</code>
    \post  <code><var>allocator</var> == this-&gt;get_allocator()</code>
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
template < typename InputIterator >
inline
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::hat
(
    InputIterator                  first,
    InputIterator                  last,
    RandomNumberGenerator const &  rng,   // = RandomNumberGenerator()
    Compare const &                cmp,   // = Compare()
    Allocator const &              alloc  // = Allocator()
)
    : elements_( first, last, make_tuple(make_tuple(
       multi_index::identity<Key>(), cmp ), make_tuple()),
       alloc )
    , generator_( rng, dist_type(0u, 0u) )
{
    function_requires< InputIteratorConcept<InputIterator> >();
    function_requires< ConvertibleConcept<typename
     std::iterator_traits<InputIterator>::value_type, Key> >();

    this->set_rng_bounds();
    BOOST_ASSERT( this->test_invariant() );
}


//  Hat random-access container operations member function definitions  ------//

/** Only one copy of the value will be contained at most, whether originally in
    the container or not.  The complexity is <b>O</b>(
    log(<code>this-&gt;size()</code>) ), amortized.

    \param x  The value of the new element.

    \return  A pair object <var>y</var> such that
             <code><var>y</var>.first</code> is an iterator to an element with
             value \p x and <code><var>y</var>.second</code> is a Boolean that
             is \c true if a new element had to be allocated with the desired
             value and \c false if an existing element's iterator was returned.

    \post  <code>this-&gt;count( <var>x</var> ) == 1</code>
    \post  <code>this-&gt;size() == <var>old_this</var>.size() +
           !<var>old_this</var>.count( <var>x</var> )</code>
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
inline
std::pair<typename boost::hats::hat<Key, RandomNumberGenerator, Compare,
 Allocator>::iterator, bool>
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::insert
(
    value_type const &  x
)
{
    std::pair<typename set_view::iterator, bool> const  result =
     this->element_set().insert( x );

    this->set_rng_bounds();
    BOOST_ASSERT( this->test_invariant() );
    return std::make_pair( const_iterator(result.first), result.second );
}

/** Only one copy of the value will be contained at most, whether originally in
    the container or not.  The complexity is <b>O</b>( 1 ) if the hint is
    perfect, <b>O</b>( log(<code>this-&gt;size()</code>) ) otherwise, both
    amortized.

    \pre  \p position must describe an iterator for this object.

    \param position  An iterator to be used as a hint.  It must point within
                     this container (either an element or the past-the-end
                     mark).  For best results, \p position should point to the
                     element that would immediately precede a newly inserted
                     element with an \p x value.
    \param x         The value of the new element.

    \return  If the value \p x already existed in the container, an iterator to
             the corresponding element; otherwise, an iterator to the newly
             added element with that value.

    \post  <code>this-&gt;count( <var>x</var> ) == 1</code>
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
inline
typename boost::hats::hat<Key, RandomNumberGenerator, Compare,
 Allocator>::iterator
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::insert
(
    iterator            position,
    value_type const &  x
)
{
    typename set_view::iterator const  result = this->element_set().insert(
     position.inner_, x );

    this->set_rng_bounds();
    BOOST_ASSERT( this->test_invariant() );
    return const_iterator( result );
}

/** Only one copy of each value will be contained at most, whether originally in
    the container or not.  The complexity is <b>O</b>(
    <code>std::distance(<var>first</var>, <var>last</var>)</code> *
    log(<code>this-&gt;size() + std::distance( <var>first</var>,
    <var>last</var> ))</code> ), amortized.

    \pre  \p InputIterator models the input-iterator concept (at least).
    \pre  [<var>first</var>, <var>last</var>) is a valid iterator range, and the
          iterated elements can be converted to \c #value_type objects.  The
          range's iterators must \em not point into this container.

    \param first  An iterator targeting the beginning of the list of elements to
                  be inserted.  If the list is empty, then it must equal the
                  value of the \p last iterator.  It must point to elements
                  outside of this container.
    \param last   An iterator targeting the past-the-end mark of the list of
                  elements to be inserted.  It must be reachable from \p first
                  through a nonnegative number of increments.  It must point to
                  elements, or the past-the-end mark, of something outside of
                  this container.

    \post  For each <var>i</var> in [<var>first</var>, <var>last</var>),
           <code>this-&gt;count( *<var>i</var> ) == 1</code>
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
template < typename InputIterator >
inline
void
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::insert
(
    InputIterator  first,
    InputIterator  last
)
{
    function_requires< InputIteratorConcept<InputIterator> >();
    function_requires< ConvertibleConcept<typename
     std::iterator_traits<InputIterator>::value_type, Key> >();

    this->element_set().insert( first, last );
    this->set_rng_bounds();
    BOOST_ASSERT( this->test_invariant() );
}

/** Iterators (and references) to the removed element are invalidated.  The
    complexity is <b>O</b>( <code>std::distance(<var>position</var>,
    this-&gt;end())</code> ), amortized.

    \pre  \p position must describe a valid iterator for this object.

    \param position  An iterator targeting the element to be deleted.  It must
                     point to an element (and not the past-the-end mark) of this
                     container.

    \post  <code><var>old_this</var>.size() - this-&gt;size() == 1</code>
    \post  <code>this-&gt;count( <var>old_value_at_position</var> ) == 0</code>
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
inline
void
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::erase
(
    iterator  position
)
{
    this->element_set().erase( position.inner_ );
    this->set_rng_bounds();
    BOOST_ASSERT( this->test_invariant() );
}

/** Iterators (and references) to the removed element, if there was a match,
    are invalidated.  The complexity is
    <b>O</b>( log(<code>this-&gt;size()</code>) +
    <code>std::distance(this-&gt;find( <var>x</var> ), this-&gt;end())</code> ),
    amortized.

    \param x  The key value that all elements to be deleted must have.

    \return  The number of elements removed.  For hat types, this number is
             always either 0 (no such key \p x was stored) or 1 (an element with
             key \p x existed).

    \post  <code>this-&gt;count( <var>x</var> ) == 0</code>
    \post  <code><var>return_value</var> == <var>old_this</var>.count(
           <var>x</var> )</code>
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
inline
typename boost::hats::hat<Key, RandomNumberGenerator, Compare,
 Allocator>::size_type
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::erase
(
    key_type const &  x
)
{
    size_type const  result = this->element_set().erase( x );

    this->set_rng_bounds();
    BOOST_ASSERT( this->test_invariant() );
    return result;
}

/** Iterators, and references, to the removed elements are invalidated.

    \pre  [<var>first</var>, <var>last</var>) must describe a vaild iterator
          range and said range must be for this object.

    \param first  An iterator targeting the beginning of the list of elements to
                  be deleted.  If the list is empty, then it must equal the
                  value of the \p last iterator.  It must point to an element,
                  or the past-the-end mark, of this container.
    \param last   An iterator targeting the past-the-end mark of the list of
                  elements to be deleted.  It must be reachable from \p first
                  through a nonnegative number of increments.  It must point to
                  an element, or the past-the-end mark, of this container.  Use
                  <code>this-&gt;end()</code> to cut off a tail section.

    \post  For each <var>v</var> that was a dereferenced value of <var>i</var>
           in [<var>first</var>, <var>last</var>), <code>this-&gt;count(
           <var>v</var> ) == 0</code>
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
inline
void
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::erase
(
    iterator  first,
    iterator  last
)
{
    this->element_set().erase( first.inner_, last.inner_ );
    this->set_rng_bounds();
    BOOST_ASSERT( this->test_invariant() );
}

/** All outstanding iterators and their references are invalidated.

    \post  <code>this-&gt;empty() == true</code> (i.e. <code>this-&gt;size() ==
           0</code>)
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
inline
void
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::clear
(
)
{
    this->element_set().clear();
    this->set_rng_bounds();
    BOOST_ASSERT( this->test_invariant() );
}

/** Swaps the content of this object with another.  Besides any elements, the
    comparator, random number generator, and allocator are switched too.

    \param other  The other object to trade state with this object

    \post  <code>*this == <var>old_other</var> &amp;&amp; <var>old_this</var> ==
           <var>other</var></code>

    \todo  Confirm that the allocator actually gets swapped.
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
void
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::swap
(
    hat &  other
)
{
    using std::swap;  // won't know how the RNG swaps

    swap( this->generator_, other.generator_ );

    try
    {
        this->elements_.swap( other.elements_ );
    }
    catch ( ... )
    {
        swap( this->generator_, other.generator_ );
        BOOST_ASSERT( this->test_invariant() );
        throw;
    }

    BOOST_ASSERT( this->test_invariant() );
}


//  Hat random-access container hat operation member function definitions  ---//

/** This is the core selection routine, but use the other three if later
    manipulation of the element with respect to its container is not required.
    A given value <var>v</var> has a <code>this-&gt;get_weight( <var>v</var> ) /
    this-&gt;total_weight()</code> fractional chance of its corresponding
    iterator being returned, assuming that the random number generator is
    accurately uniform.

    \return  A constant-access iterator to one of the elements of this
             container, or to <code>this-&gt;end()</code> if this container is
             empty

    \post  If <code>this-&gt;empty() == false</code>: <code>this-&gt;count(
           *<var>return_value</var> ) == 1</code>
    \post  The state of the internal random number generator may change.  (This
           is why this member function, and all similar ones, cannot be marked
           as suitable with \c const qualifying use.)

    \see  #sample #draw #discard
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
inline
typename boost::hats::hat<Key, RandomNumberGenerator, Compare,
 Allocator>::const_iterator
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::pick
(
)
{
    v_iterator const  result = this->choose();

    BOOST_ASSERT( this->test_invariant() );
    return const_iterator( this->elements_.template project<0>(result) );
}

/** Store the result (or use the \c #pick member function) if you need to change
    how the value relates to the container.

    \pre  <code>this-&gt;empty() == false</code>

    \return  An (immutable) reference to one of the elements of this container

    \post  Same post-conditions as returning <code>*this-&gt;pick()</code>,
           including <em>undefined behavior</em> if <code>this-&gt;empty() ==
           true</code> (since that would dereference an iterator for a
           past-the-end mark)

    \see  #pick
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
inline
typename boost::hats::hat<Key, RandomNumberGenerator, Compare,
 Allocator>::const_reference
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::sample
(
)
{
    v_iterator const  result = this->choose();

    BOOST_ASSERT( this->test_invariant() );
    return *result;
}

/** Iterators (and references) to the removed element, if any, are invalidated.
    Make sure to check the return value to see if the \p result is valid
    (especially if the definition of \p result wasn't initialized).

    \param result  A reference to a \c #value_type object that will receive a
                   copy of the chosen element

    \retval true   An element was removed (i.e. the container was not empty) and
                   \p result holds that element's value.
    \retval false  No element was removed because the container was empty.
                   Also, \p result was not altered.

    \post  For <code>this-&gt;empty() == false</code>, same post-conditions as:
           <code>this-&gt;erase( <var>result</var> = *this-&gt;pick() )</code>
           (assuming that the assignment operator carries the changed reference
           through, and doesn't return <code>void</code>); otherwise, no effect

    \see  #pick
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
inline
bool
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::draw
(
    reference  result
)
{
    if ( !this->empty() )
    {
        v_iterator const  chosen = this->choose();

        result = *chosen;
        this->element_vector().erase( chosen );
        this->set_rng_bounds();
        BOOST_ASSERT( this->test_invariant() );
        return true;
    }
    else
    {
        return false;  // no invariant check needed
    }
}

/** There is no way of retrieving the value of the element removed, if actually
    done, without external bookkeeping.  Iterators (and references) to that
    element are invalidated.  Use this member function instead of #draw if the
    value/identity of the removed element doesn't matter.

    \retval true   An element was removed (i.e. the container was not empty)
    \retval false  No element was removed because the container was empty

    \post  For <code>this-&gt;empty() == false</code>, same post-conditions as:
           <code>this-&gt;erase( this-&gt;pick() )</code>; otherwise, no effect

    \see  #pick
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
inline
bool
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::discard
(
)
{
    if ( !this->empty() )
    {
        this->element_vector().erase( this->choose() );
        this->set_rng_bounds();
        BOOST_ASSERT( this->test_invariant() );
        return true;
    }
    else
    {
        return false;  // no invariant check needed
    }
}

/** The \p weight is limited to <code>this-&gt;max_weight()</code>.  But a
    particular call may have a lower limit if the given \p weight would cause
    <code>this-&gt;total_weight() &lt;= this-&gt;max_total_weight()</code> to be
    violated.  Iterators (and references) to an element are invalidated if its
    weight goes from non-zero to zero.

    \pre  \p weight shouldn't exceed the weight limits.

    \param x       The key whose weight will be changed
    \param weight  The new weight

    \retval true   The new weight was accepted
    \retval false  The new weight was rejected (for exceeding limits); the
                   key's weight prior to this call is retained

    \post  If accepted: <code>this-&gt;get_weight( <var>x</var> ) ==
           weight</code>; otherwise: <code>this-&gt;get_weight( <var>x</var>
           )</code> is unchanged

    \note  For equal-opportunity hat types, <code>this-&gt;get_weight(
           <var>x</var> ) == this-&gt;count( <var>x</var> )</code>
 */
template < typename Key, class RandomNumberGenerator, class Compare,
 class Allocator >
inline
bool
boost::hats::hat<Key, RandomNumberGenerator, Compare, Allocator>::set_weight
(
    key_type const &  x,
    weight_type       weight
)
{
    switch ( weight )
    {
    case 0u:
        // Zero-weight elements don't contribute -> don't include
        this->erase( x );  // includes invariant check
        return true;
    case 1u:
        // Have an entry with this weight, no matter added or existing
        this->insert( x );  // includes invariant check
        return true;
    default:
        // All (included) weights are equal -> exclude off-sized ones
        return false;  // no change, no invariant check needed
    }
}


//  Hat random-access container non-member operator function definitions  ----//

/** \brief  Equals

    Compares two objects for equivalence.  Such objects are equal if they have
    the same element counts and, when matching up in an iteration order, their
    corresponding elements are equal.

    \pre  \c Key1 and \c Key2 must be comparible for equality, and
          <code>operator ==()</code> (which could put the types in either order)
          must define an equivalence relation suitable for that comparison.

    \param l  The left-side operand to be compared.
    \param r  The right-side operand to be compared.

    \retval true   \p l and \p r are equivalent.
    \retval false  \p l and \p r are not equivalent.

    \relates  boost::hats::hat

    \note  This comparison does not depend on the objects' RNG engines, own
           comparators, or allocators.  This operator (and <code>operator
           !=()</code>) will fail (to compile) if a suitable element-level
           <code>operator ==()</code> does not exist.
 */
template < typename Key1, typename Key2, class Rng1, class Rng2, class Cmp1,
 class Cmp2, class Al1, class Al2 >
inline
bool
boost::hats::operator ==
(
    boost::hats::hat<Key1, Rng1, Cmp1, Al1> const &  l,
    boost::hats::hat<Key2, Rng2, Cmp2, Al2> const &  r
)
{
    return ( l.size() == r.size() ) && ::std::equal( l.begin(), l.end(),
     r.begin() );
}

/** \brief  Not-equals

    Compares two objects for non-equivalence.  Such objects are unequal if
    either their element counts differ or, when matching up in an iteration
    order, their corresponding elements differ.

    \param l  The left-side operand to be compared.
    \param r  The right-side operand to be compared.

    \retval true   \p l and \p r are not equivalent.
    \retval false  \p l and \p r are equivalent.

    \see  boost::hats::operator==(hat const&,hat const&)

    \relates  boost::hats::hat
 */
template < typename Key1, typename Key2, class Rng1, class Rng2, class Cmp1,
 class Cmp2, class Al1, class Al2 >
inline
bool
boost::hats::operator !=
(
    boost::hats::hat<Key1, Rng1, Cmp1, Al1> const &  l,
    boost::hats::hat<Key2, Rng2, Cmp2, Al2> const &  r
)
{
    return !( l == r );
}

/** \brief  Less-than

    Compares two objects for their relative order.  One object is less than
    another if and only if the first object's forward-iterated list of elements
    lexicographically compares to be less than the second object's list.

    \pre  \c Key1 and \c Key2 must be comparible for order, and <code>operator
          &lt;()</code> (which could put the types in either order) must define
          a total ordering relation suitable for that comparison.

    \param l  The left-side operand to be compared.
    \param r  The right-side operand to be compared.

    \retval true   \p l is less than \p r
    \retval false  \p l is not less than \p r (either equal or greater than)

    \relates  boost::hats::hat

    \note  This comparison does not depend on the objects' RNG engines, own
           comparators, or allocators.  This operator (and <code>operator
           &gt;()</code>, <code>operator &gt;=()</code>, and <code>operator
           &lt;=()</code>) will fail (to compile) if a suitable element-level
           <code>operator &lt;()</code> does not exist.
 */
template < typename Key1, typename Key2, class Rng1, class Rng2, class Cmp1,
 class Cmp2, class Al1, class Al2 >
inline
bool
boost::hats::operator <
(
    boost::hats::hat<Key1, Rng1, Cmp1, Al1> const &  l,
    boost::hats::hat<Key2, Rng2, Cmp2, Al2> const &  r
)
{
    return ::std::lexicographical_compare( l.begin(), l.end(), r.begin(),
     r.end() );
}

/** \brief  Greater-than

    Compares two objects for their relative order.  One object is greater than
    another if and only if the second object's forward-iterated list of elements
    lexicographically compares to be less than the first object's list.

    \param l  The left-side operand to be compared.
    \param r  The right-side operand to be compared.

    \retval true   \p l is greater than \p r
    \retval false  \p l is not greater than \p r (either equal or less than)

    \see  boost::hats::operator<(hat const&,hat const&)

    \relates  boost::hats::hat
 */
template < typename Key1, typename Key2, class Rng1, class Rng2, class Cmp1,
 class Cmp2, class Al1, class Al2 >
inline
bool
boost::hats::operator >
(
    boost::hats::hat<Key1, Rng1, Cmp1, Al1> const &  l,
    boost::hats::hat<Key2, Rng2, Cmp2, Al2> const &  r
)
{
    return r < l;
}

/** \brief  Less-than-or-equal

    Compares two objects for their relative order.  One object is less than or
    equal to another if and only if the second object's forward-iterated list of
    elements does \em not lexicographically compare to be less than the first
    object's list.

    \param l  The left-side operand to be compared.
    \param r  The right-side operand to be compared.

    \retval true   \p l is equal or less than \p r
    \retval false  \p l is greater than \p r

    \see  boost::hats::operator<(hat const&,hat const&)

    \relates  boost::hats::hat
 */
template < typename Key1, typename Key2, class Rng1, class Rng2, class Cmp1,
 class Cmp2, class Al1, class Al2 >
inline
bool
boost::hats::operator <=
(
    boost::hats::hat<Key1, Rng1, Cmp1, Al1> const &  l,
    boost::hats::hat<Key2, Rng2, Cmp2, Al2> const &  r
)
{
    return !( l > r );
}

/** \brief  Greater-than-or-equal

    Compares two objects for their relative order.  One object is less than or
    equal to another if and only if the first object's forward-iterated list of
    elements does \em not lexicographically compare to be less than the second
    object's list.

    \param l  The left-side operand to be compared.
    \param r  The right-side operand to be compared.

    \retval true   \p l is equal or greater than \p r
    \retval false  \p l is less than \p r

    \see  boost::hats::operator<(hat const&,hat const&)

    \relates  boost::hats::hat
 */
template < typename Key1, typename Key2, class Rng1, class Rng2, class Cmp1,
 class Cmp2, class Al1, class Al2 >
inline
bool
boost::hats::operator >=
(
    boost::hats::hat<Key1, Rng1, Cmp1, Al1> const &  l,
    boost::hats::hat<Key2, Rng2, Cmp2, Al2> const &  r
)
{
    return !( l < r );
}


//  Hat random-access container miscellaneous function definitions  ----------//

/** \brief  Non-member swapping function for \c hat\<\>

    Exchanges the states of two \c hat\<\> objects.  This specialization of the
    algorithm can be called by generic code that uses free-function (template)
    swap assisted with Koenig lookup.

    \param a  The first object involved in the swap.
    \param b  The second object involved in the swap.

    \post  <code><var>a</var> == <var>old_b</var> &amp;&amp; <var>old_a</var> ==
           <var>b</var></code>

    \see  boost::hats::hat::swap

    \relates  boost::hats::hat
 */
template < typename Key, class Rng, class Cmp, class Al >
inline
void
boost::hats::swap
(
    boost::hats::hat<Key, Rng, Cmp, Al> &  a,
    boost::hats::hat<Key, Rng, Cmp, Al> &  b
)
{
    a.swap( b );
}


#endif  // BOOST_HAT_HAT_HPP
