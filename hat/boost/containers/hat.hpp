// Boost containers/hat.hpp header file  -------------------------------------//
/** \file
    \brief  Declaration of the \c hat container adaptor class template.

    \author  Daryle Walker

    Contains the declaration (and definition) of the \c boost::containers::hat
    container adaptor class template.  The adaptor enables random selection of
    its contents.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */
// See <http://www.boost.org/libs/containers> for documentation.

#ifndef BOOST_CONTAINERS_HAT_HPP
#define BOOST_CONTAINERS_HAT_HPP

#include <boost/containers_fwd.hpp>

#include <boost/concept/assert.hpp>           // for BOOST_CONCEPT_ASSERT
#include <boost/concept_check.hpp>            // for boost::SGIAssignable, etc.
#include <boost/mpl/assert.hpp>               // for BOOST_MPL_ASSERT
#include <boost/random/mersenne_twister.hpp>  // for boost::mt19937
#include <boost/random/uniform_int.hpp>       // for boost::uniform_int
#include <boost/swap.hpp>                     // for boost::swap
#include <boost/type_traits/is_same.hpp>      // for boost::is_same

#include <deque>     // for std::deque
#include <iterator>  // for std::iterator_traits, advance, distance


namespace boost
{
namespace containers
{


//  Hat (really) random-access container class template definition  ----------//

/** \brief  A container adaptor that provides random selection of its contents,
            similar to pulling names from a hat.

    \author  Daryle Walker (Boost implementation)
    \author  Jonathan Wooldridge [a.k.a. AngleWyrm] (original concept)

    The interface is similar to that of \c std::priority_queue.  The call to
    \c #top will choose an element based on a randomly selected index.  To refer
    to a specific element repeatedly, token objects can be returned from random
    draws instead.  These tokens are required to remove a specific element after
    it's first chosen.  (The fact that calls to \c #top and the regular \c #pop
    use separate random draws is why the token system was added.)  All elements
    are given equal consideration (i.e. weight) for selection, modulo any bias
    in the random number generation.

    Example containers that can be adapted are \c std::vector, \c std::deque,
    \c std::list, and \c std::basic_string.

    \pre  Besides being suitable for container elements, \p T must be Swappable.
    \pre  \p Container must be a sequence-style container type.  \p T must be
          its element type.
    \pre  \p RandomNumberGenerator must have an interface compatible with the
          engines that are part of the Boost.Random library, conforming to
          UniformRandomNumberGenerator, CopyConstructable, and Assignable.

    \tparam T                      The type of the elements.
    \tparam Container              The type of the container.  If not given,
                                   <code>std::deque&lt;<var>T</var>&gt;</code>
                                   is the default.
    \tparam RandomNumberGenerator  The type of the uniform random number
                                   generation engine.  If not given,
                                   \c boost::mt19937 is the default.

    \todo  Need to add a concept check for Swappable on T.
    \todo  Need to add concept check for UniformRandomNumberGenerator,
           CopyConstructable, and Assignable on RandomNumberGenerator.
    \todo  Create constructors that use the new move semantics (i.e. r-value
           &quot;&amp;&amp;&quot; references) for the components.
    \todo  Can the container's size get big enough, or the RNG engine's
           randomness small enough, that gaps will exist between chosen indices?
           If so, how can we tell?  Is there some general parameter that can be
           read from the engine so we'll know when this problem happens?
 */
template < typename T, class Container = std::deque<T>, class
 RandomNumberGenerator = boost::mt19937 >
class hat
{
    // Concept checks
    BOOST_CONCEPT_ASSERT(( boost::SGIAssignable<T> ));

    BOOST_CONCEPT_ASSERT(( boost::Sequence<Container> ));

    BOOST_MPL_ASSERT(( boost::is_same<T, typename Container::value_type> ));

    // Helper type-aliases
    typedef typename Container::iterator              iterator;
    typedef typename Container::const_iterator  const_iterator;

    // Hidden implmentation member data
    mutable RandomNumberGenerator  e;

public:
    // Types
    /** \brief  Type of the container

        Represents the class used for containing element objects.
     */
    typedef Container           container_type;
    /** \brief  Type of the random number generation engine

        Represents the class used for choosing randomly-selected numbers.
     */
    typedef RandomNumberGenerator  engine_type;

    /** \brief  Type of elements

        Represents the type of the container's elements.  Copied from the
        container's type profile. 
     */
    typedef typename Container::value_type  value_type;
    /** \brief  Type of size values

        Represents the unsigned integral type used for sizing returns.  Copied
        from the container's type profile.
     */
    typedef typename Container::size_type    size_type;

    /** \brief  Type of element references

        Represents the type of references to the container's elements.  Copied
        from the container's type profile.
     */
    typedef typename Container::reference              reference;
    /** \brief  Type of element references with immutable access

        Represents the type of references to the container's elements with
        immutable access.  Copied from the container's type profile.
     */
    typedef typename Container::const_reference  const_reference;

    /** \brief  Type of markers towards a container's element with immutable
                access

        Represents the type for access to a specific element after it's randomly
        chosen.  The main way to access an element, \c #top, changes its
        referent after each call, so using tokens is the only method of later
        referral.  It is similar to an immutable-access iterator, except all
        forms of traversal are disabled.
     */
    class const_token
    {
    public:
        /** \brief  The inner iterator

            Accesses a particular element of the adaptor's container.  Public to
            avoid complicated (friend) definitions.
         */
        const_iterator  i;

        /** \brief  Indirection

            Dereferences the token to provide &quot;direct&quot; access to the
            target element.

            \return  A <code>const</code> reference to this object's target.
         */
        const_reference     operator  *() const  { return *this->i; }
        /** \brief  Class member access (via pointer)

            Dereferences the token to provide access to the target element in
            a pointer-like style.

            \return  A <code>const</code> pointer to this object's target.
         */
        value_type const *  operator ->() const  { return &this->operator *(); }

        /** \brief  Equals

            Compares this token with another for equivalence.  The tokens are
            equal if they refer to the same element object.  (This token acts as
            the left-side operand.)  This operator is also used when \p o is
            really a \c boost::containers::hat::token object.

            \param o  The right-side operand to be compared.

            \retval true   \c *this and \p o are equivalent.
            \retval false  \c *this and \p o are not equivalent.
         */
        bool  operator ==( const_token o ) const  { return this->i == o.i; }
        /** \brief  Not-equals

            Compares this token with another for non-equivalence.  The tokens
            are unequal if they refer to different element objects.  (This token
            acts as the left-side operand.)  This operator is also used when
            \p o is really a \c boost::containers::hat::token object.

            \param o  The right-side operand to be compared.

            \retval true   \c *this and \p o are not equivalent.
            \retval false  \c *this and \p o are equivalent.
         */
        bool  operator !=( const_token o ) const  {return !this->operator==(o);}
    };
    /** \brief  Type of markers towards a container's element

        Represents the type for access to a specific element after it's randomly
        chosen.  The main way to access an element, \c #top, changes its
        referent after each call, so using tokens is the only method of later
        referral.  It is similar to a mutable-access iterator, except all forms
        of traversal are disabled.
     */
    class token
    {
    public:
        /** \brief  The inner iterator

            Accesses a particular element of the adaptor's container.  Public to
            avoid complicated (friend) definitions.
         */
        iterator  i;

        /** \brief  Indirection

            Dereferences the token to provide &quot;direct&quot; access to the
            target element.

            \return  A non-<code>const</code> reference to this object's target.
         */
        reference     operator  *() const  { return *this->i; }
        /** \brief  Class member access (via pointer)

            Dereferences the token to provide access to the target element in
            a pointer-like style.

            \return  A non-<code>const</code> pointer to this object's target.
         */
        value_type *  operator ->() const  { return &this->operator *(); }

        /** \brief  Conversion, immutable-access token

            To meet the requirements of a container, a mutable-access iterator
            has to be able convert to a immutable-access one of the same
            element.  Similarly, this ability is granted to tokens.

            \return  A token with immutable access (i.e. \c const) to the same
                     element that this token refers to.
         */
        operator const_token() const
        {
            const_token  result = { this->i };

            return result;
        }

        /** \brief  Equals

            Compares this token with another for equivalence.  The tokens are
            equal if they refer to the same element object.  (This token acts as
            the left-side operand.)

            \param o  The right-side operand to be compared.

            \retval true   \c *this and \p o are equivalent.
            \retval false  \c *this and \p o are not equivalent.
         */
        bool  operator ==( token o ) const  { return this->i == o.i; }
        /** \brief  Not-equals

            Compares this token with another for non-equivalence.  The tokens
            are unequal if they refer to different element objects.  (This token
            acts as the left-side operand.)

            \param o  The right-side operand to be compared.

            \retval true   \c *this and \p o are not equivalent.
            \retval false  \c *this and \p o are equivalent.
         */
        bool  operator !=( token o ) const  { return !this->operator ==( o ); }

        /** \brief  Equals, cross-mutability

            Compares this token with a \c boost::containers::hat::const_token for equivalence.  The
            tokens are equal if they refer to the same element objects.  (This
            token acts as the left-side operand.)

            \param o  The right-side operand to be compared.

            \retval true   \c *this and \p o are equivalent.
            \retval false  \c *this and \p o are not equivalent.
         */
        bool  operator ==( const_token o ) const  {return o.operator ==(*this);}
        /** \brief  Not-equals, cross-mutability

            Compares this token with a \c boost::containers::hat::const_token for non-equivalence.  The
            tokens are unequal if they refer to different element objects.
            (This token acts as the left-side operand.)

            \param o  The right-side operand to be compared.

            \retval true   \c *this and \p o are not equivalent.
            \retval false  \c *this and \p o are equivalent.
         */
        bool  operator !=( const_token o ) const  {return o.operator !=(*this);}
    };

protected:
    // Exposed member data or access
    /** \brief  The inner container

        Accesses the adaptor's container.  Enables a derived class to
        arbitrarily mess with the container.
     */
    Container  c;

    /** \brief  The random number generator engine, mutable access

        Accesses the adaptor's random number generator engine.  Enables a
        derived class to arbitrarily mess with the engine.  Conventional access,
        like \c #c, is not provided due to how the engine is set up in this
        class.

        \return  A non-<code>const</code> reference to inner R.N.G. engine.
     */
    RandomNumberGenerator &        engine()        { return this->e; }
    /** \brief  The random number generator engine, immutable access

        Accesses the adaptor's random number generator engine.  Enables a
        derived class to arbitrarily inspect the engine.  Conventional access,
        like \c #c, is not provided due to how the engine is set up in this
        class.

        \return  A \c const reference to inner R.N.G. engine.
     */
    RandomNumberGenerator const &  engine() const  { return this->e; }

    /** \brief  Index random-selection

        Generates an index value at random.  The value is constrained to be a
        valid offset (for non-empty containers).  Even though this member
        function is marked \c const, the inner R.N.G. engine may still change.
        Enables derived classes to choose random elements without creating
        either a token or a reference to an element.

        \return  An index to an element in \c #c, or zero if
                 <code>this-&gt;c.empty()</code>.
     */
    size_type  choice() const
    {
        return uniform_int<size_type>( 0u, this->size() - !this->empty() )(
         this->e );
    }

public:
    // Lifetime management (use automatic copy constructor and destructor)
    /** Constructs a \c #hat object with the given R.N.G. engine and container.
        It can also either construct an empty hat with a given engine or serve
        as the default constructor to an empty hat with default R.N.G. settings.

        \param en  The engine to copy settings.  If not given, a default
                   constructed engine is used.
        \param co  The container to copy elements.  If not given, a default
                   constructed container is used.

        \post  <code><var>en</var> == this-&gt;engine()</code>.
        \post  <code><var>co</var> == this-&gt;c</code>.
     */
    explicit  hat( RandomNumberGenerator const &en = RandomNumberGenerator(),
     Container const &co = Container() )  : c( co ), e( en )  {}

    /** Constructs a \c #hat object with a given iterator range, R.N.G. engine,
        and container.  (The initial engine and container are optional.)  The
        elements of the iterator range are copied to the end of the inner
        container in order.

        \pre  \p InputIterator models the input-iterator (or above) concept.
        \pre  The element type of \p InputIterator has to be compatible with
              \c #value_type.
        \pre  \p last has to be reachable from \p first via a finite nonnegative
              number of forward iterations.

        \tparam InputIterator  The type of the range's iterators.

        \param first  An iterator targeting the beginning of a list of entries
                      to be appended to \c #c.  If this list is empty, then it
                      must equal \p last instead.
        \param last   An iterator targeting the past-the-end mark of a list of
                      entries to be appended to \c #c.
        \param en     The engine to copy settings.  If not given, a default
                      constructed engine is used.
        \param co     The container to copy elements.  If not given, a default
                      constructed container is used.

        \post  <code><var>en</var> == this-&gt;engine()</code>.
        \post  <code>std::equal( <var>co</var>.begin(), <var>co</var>.end(),
               this-&gt;c.begin() ) &amp;&amp; std::equal( <var>first</var>,
               <var>last</var>, this-&gt;c.begin() + <var>co</var>.size()
               )</code>, excusing the assumptions that \p first and \p last are
               reusable and that \c #container_type supports random-access
               iterators.
     */
    template < typename InputIterator >
    hat( InputIterator first, InputIterator last, RandomNumberGenerator const
     &en = RandomNumberGenerator(), Container const &co = Container() )
        : c( co ), e( en )
    {
        BOOST_CONCEPT_ASSERT(( boost::InputIterator<InputIterator> ));
        BOOST_CONCEPT_ASSERT(( boost::Convertible<typename
         std::iterator_traits<InputIterator>::value_type, value_type> ));

        c.insert( c.end(), first, last );
    }

    // Sizing
    /** \brief  Checks if any elements are contained

        This may be faster than using <code>this-&gt;size()</code> and an
        equality-comparison with zero.

        \return  <code>this-&gt;size() == 0</code>

        \see  #size
     */
    bool       empty() const  { return this->c.empty(); }
    /** \brief  Returns the number of elements contained

        \return  The number of elements of the inner container.

        \see  #empty
     */
    size_type  size() const   { return this->c.size(); }

    // Access
    /** \brief  Generates a marker to a randomly-chosen element

        An element from \c #c is arbitrarily chosen, using the random number
        generator \c #engine().  A token based off an iterator to that element
        is created.  Like container iterators, this token should be considered
        invalid after any operation that changes the number of elements in
        \c #c, even temporarily.  (This includes \c #push and \c #pop,
        especially if the latter removes the token's element.)

        \pre  <code>this-&gt;empty() == false</code>.

        \return  A \c boost::containers::hat::token to an element.

        \post  The R.N.G. engine may change.

        \see  #choice
        \see  #boost::containers::hat::token
     */
    token        pick()
    {
        token  result = { this->c.begin() };

        std::advance( result.i, this->choice() );
        return result;
    }
    /** \brief  Generates marker to randomly-chosen element, immutable access

        An element from \c #c is arbitrarily chosen, using the random number
        generator \c #engine().  A token based off an iterator to that element
        is created.  Like container iterators, this token should be considered
        invalid after any operation that changes the number of elements in
        \c #c, even temporarily.  (This includes \c #push and \c #pop,
        especially if the latter removes the token's element.)

        \pre  <code>this-&gt;empty() == false</code>.

        \return  A token (actually a \c boost::containers::hat::const_token) to
                 an element.

        \post  The R.N.G. engine may change, even with \c const.

        \see  #choice
        \see  #boost::containers::hat::const_token
     */
    const_token  pick() const
    {
        const_token  result = { this->c.begin() };

        std::advance( result.i, this->choice() );
        return result;
    }

    /** \brief  Generates a reference to a randomly-chosen element

        An element from \c #c is arbitrarily chosen, using the random number
        generator \c #engine().  Like direct container element references, this
        reference should be considered invalid after any operations that change
        the number of elements, even temporarily.  (This includes \c #push and
        \c #pop, especially if the latter removes the referenced element.)

        \pre  <code>this-&gt;empty() == false</code>.

        \return  A non-<code>const</code> reference to an element.

        \post  The R.N.G. engine may change.

        \see  #choice
     */
    reference        top()        { return *this->pick(); }
    /** \brief  Generates reference to randomly-chosen element, immutable access

        An element from \c #c is arbitrarily chosen, using the random number
        generator \c #engine().  Like direct container element references, this
        reference should be considered invalid after any operations that change
        the number of elements, even temporarily.  (This includes \c #push and
        \c #pop, especially if the latter removes the referenced element.)

        \pre  <code>this-&gt;empty() == false</code>.

        \return  A \c const reference to an element.

        \post  The R.N.G. engine may change, even with \c const.

        \see  #choice
     */
    const_reference  top() const  { return *this->pick(); }

    // Element insertion/removal
    /** \brief  Adds an element

        An element with the value \p x is added to the end of \c #c.  Since the
        \c push_back member function is optional for sequence-style containers,
        the element is added with the inner container's \c insert member
        function at the \c end point.  Assume that it will invalidate any
        outstanding element tokens and references.

        \param x  The value that will be copied into this object.

        \post  <code>this-&gt;size() == <var>old_this</var>.size() + 1</code>.
        \post  <code>*--this-&gt;c.end() == <var>x</var></code>.
     */
    void  push( const_reference x )  { this->c.insert( this->c.end(), x ); }
    /** \brief  Removes a randomly-chosen element by marker, mutable access

        The element referred to by \p t is removed from \c #c.  Actually, its
        value is swapped (using <code>boost::swap</code>) with the rear element
        and then that rear element is removed, for efficiency's sake.  (Since
        the \c pop_back member function is optional for sequence-style
        containers, the \c erase member function is used with an iterator to the
        rear element.)  This \e will ruin any sorting scheme that may be
        maintained in the container, unless the rear element happened to be the
        one chosen.  Assume that it will invalidate any outstanding element
        tokens and references.

        \pre  \p t is valid and it refers to a current element in \c c.  (This
              definately means that <code>this-&gt;empty() == false</code>.)

        \param t  A token to the element to be removed.

        \post  <code>this-&gt;size() == <var>old_this</var>.size() - 1</code>.
        \post  <code>std::count( this-&gt;c.begin(), this-&gt;c.end(), *
               <var>t</var> ) + 1 == std::count( <var>old_this</var>.c.begin(),
               <var>old_this</var>.c.end(), *<var>t</var> )</code>.

        \see  #boost::containers::hat::token
     */
    void  pop( token t )
    {
        iterator  rear = this->c.end();

        boost::swap( *--rear, *t );
        this->c.erase( rear );
    }
    /** \brief  Removes a randomly-chosen element by marker, immutable access

        The element referred to by \p t is removed from \c #c.  Actually, this
        operation is meant for mutable-access tokens, so \p t is translated to
        such a token and then \c #pop(token) is called.  This means that this
        version may be slower.  Any sorting scheme that may be maintained in the
        container \e will be ruined, unless the rear element happened to be the
        one chosen.  Assume that it will invalidate any outstanding element
        tokens and references.

        \pre  \p t is valid and it refers to a current element in \c c.  (This
              definately means that <code>this-&gt;empty() == false</code>.)

        \param t  A token to the element to be removed.

        \post  <code>this-&gt;size() == <var>old_this</var>.size() - 1</code>.
        \post  <code>std::count( this-&gt;c.begin(), this-&gt;c.end(), *
               <var>t</var> ) + 1 == std::count( <var>old_this</var>.c.begin(),
               <var>old_this</var>.c.end(), *<var>t</var> )</code>.

        \see  #boost::containers::hat::const_token
     */
    void  pop( const_token t )
    {
        token  location = { this->c.begin() };

        std::advance(location.i,std::distance<const_iterator>(location.i, t.i));
        this->pop( location );
    }
    /** \brief  Removes a randomly-chosen element

        Some element in \c #c is removed.  There's no way to determine which one
        without a bunch of comparisons and a save of the pre-<code>pop</code>
        state.  The removed element acts as if its value was swapped with the
        rear element and then that one was removed.  This means that any sorting
        scheme maintained \e will be ruined (unless the rear element was the one
        initially chosen).  Assume that it will invalidate any outstanding
        element tokens and references.

        \pre  <code>this-&gt;empty() == false</code>.

        \post  <code>this-&gt;size() == <var>old_this</var>.size() - 1</code>.
        \post  <code>0 &lt;= std::mismatch( this-&gt;c.begin(),
               this-&gt;c.end(), <var>old_this</var>.c.begin() ).first -
               this-&gt;c.begin() &lt;= this-&gt;size()</code>.
     */
    void  pop()  { this->pop( this->pick() ); }

};  // boost::containers::hat


}  // namespace containers
}  // namespace boost


#endif  // BOOST_CONTAINERS_HAT_HPP
