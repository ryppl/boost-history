=====================================
 Allowing Move Constructors to Throw
=====================================

:Author: David Abrahams, Rani Sharoni
:Contact: dave@boostpro.com, ranis@microsoft.com
:organization: `BoostPro Computing`_, Microsoft
:date: 2009-10-14

:Number: D2983=09-0173

.. _`BoostPro Computing`: http://www.boostpro.com

.. contents:: index

Introduction
============

In N2855_, Doug Gregor and Dave Abrahams discussed a problematic
interaction between move constructors, templates, and certain standard
library member functions.  To date, attempts to solve the problem
(including N2855_ itself) have taken one basic approach: *ban throwing
move constructors, and be sure never to generate one*.  

Consider, for a moment, the actual magnitude of the problem we're
addressing: it's a backward-compatibility/code evolution issue that
only arises when *all* these conditions are satisfied:

* An existing operation today gives the strong guarantee
* The operation is being *move-enabled* (altered to use move operations)
* An existing type that the operation manipulates acquires a move constructor
* That move constructor can throw
* The particular move-enabled operation can only offer the basic
  guarantee if a move constructor throws [#x]_

In light of the extremely narrow set of circumstances under which the
problem can arise, it seems rather heavy-handed to ban throwing move
constructors altogether:

* It can be a hard rule for users to follow.

* Generating move constructors by default as proposed in N2904_
  requires a carefully-engineered set of rules about when such a
  default can be generated.
 
* It hobbles the optimization potential of move constructors, because
  efficient move constructors that might throw (but almost never will)
  can't be generated or written.

* It adds another "Gotcha" that people have to worry about.

* It places tight restrictions on the order of code evolution: one
  cannot move-enable a class until all its sub-objects have been
  move-enabled.

* It forces the weakening of class invariants for some move-enabled
  classes: if move construction can throw, it must be possible to
  leave the source object in a resource-less state.  

* In some cases class maintainers may need to choose between
  move-enabling their class and maintaining its current guarantees to
  users.  For example, dinkumware's list implementation currently
  derives QOI benefits from having no resource-less state—“begin” and
  “end” iterators maintain their relationships even after lists are
  swapped.  If throwing move constructors are banned, that would no
  longer be possible.

* When it is necessary to manually write a move constructor for a
  templated class, it would in many cases require either expert-level
  metaprogramming skill or the addition of a language feature, just
  for move constructors, whose only purpose is in solving this
  particular problem. [#attribute]_

.. _N2904: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2009/n2904.pdf

Fortunately, there is a better way.  Instead of imposing this burden
on every class author, we can deal with the issue more selectively in
the operation being move-enabled.  There, we know whether a throwing
move can disturb existing guarantees.  We propose that instead of
using ``std::move(x)`` in those cases, thus granting permission for the
compiler to use *any* available move constructor, maintainers of these
particular operations should use ``std::nothrow_move(x)``, which only
grants permission to use non-throwing move constructors.  Unless ``x``
is known to have a nonthrowing move constructor, the operation would
fall back to copying ``x``, just as though ``x`` had never acquired a move
constructor at all.

For example, ``std::pair``\ 's move constructor, were it to be written
out manually, could remain as it was before this issue was
discovered::

    template <class First2, Second2>
    pair( pair<First2,Second2>&& rhs )
      : first( move(rhs.first) ), 
        second( move(rhs.second) )
    {}

However, ``std::vector::reserve`` would be move-enabled this way:

.. parsed-literal:

  void reserve(size_type n)
  {
      if (n > this->capacity())
      {
          pointer new_begin = this->allocate( n );
          size_type s = this->size(), i = 0;
          try
          {
              for (;i < s; ++i)
                   new ((void*)(new_begin + i)) value_type( **nothrow_move(** (\*this)[i]) **)** );
          }
          catch(...)
          {
              for (;i > 0; --i)                 // clean up new elements
                 (new_begin + i)->~value_type();

              this->deallocate( new_begin );    // release storage
              throw;
          }
          // -------- irreversible mutation starts here -----------
          this->deallocate( this->begin_ );
          this->begin_ = new_begin;
          this->end_ = new_begin + s;
          this->cap_ = new_begin + n;
      }
  }


We stress again that the use of ``nothrow_move`` as opposed to ``move``
would only be necessary under an *extremely* limited set of
circumstances.  In particular, it would never be required in new code,
which could simply give a *conditional* strong guarantee, e.g. “if an
exception is thrown other than by ``T``\ 's move constructor, there are
no effects.”

Implementing ``nothrow_move``
=============================

One reasonable implementation of ``std::nothrow_move`` might be::

  template <class T>
  typename enable_if<has_nothrow_move_constructor<T>, T&&>::type 
  nothrow_move(T& x)
  {
      return std::move(x);
  }
  
  template <class T>
  typename disable_if<has_nothrow_move_constructor<T>, T&>::type 
  nothrow_move(T& x)
  {
      return x;
  }

However, that begs the question of how
``has_nothrow_move_constructor<T>`` might be implemented.  We propose
that ``has_nothrow_move_constructor<T>`` be a conservative trait very
much like ``has_nothrow_copy_constructor<T>`` from the current working
draft; it would be identical to the proposed
``is_nothrow_constructible<T,T&&>`` from N2953_.  In other words, it is
*allowed* to return ``false`` even when the proper result is ``true``.

An Optimization Hint
====================

To help the library deduce the correct result for these traits, we
propose to add a new kind of exception-specification, spelled:

.. parsed-literal::

   throw( *integral constant expression* )

The only impact of such an exception-specification is this: if a
function decorated with `throw(false)` throws an exception, the
behavior is undefined. [#no-diagnostic]_ That effect is sufficient to
allow these *xxx*\ ``_nothrow_``\ *xxx* traits to report ``true`` for
any operation decorated with `throw(false)`.  Class maintainers could
label their move constructors `throw(false)` to indicate non-throwing
behavior, and the library is permitted to take advantage of that
labelling if it can be detected (via “compiler magic”).

Note that the usefulness of `throw(false)` as an optimization hint
goes way beyond the narrow case introduced by N2855_.  In fact, it
goes beyond move construction: when the compiler can detect
non-throwing operations with certainty, it can optimize away a great
deal of code and/or data that is devoted to exception handling.  Some
compilers already do that for `throw()` specifications, but since
those incur the overhead of an implicit try/catch block to handle
unexpected exceptions, the benefits are limited.

The advantage of the integral constant expression parameter is that
one can easily offer accurate hints in templated move constructors.
For example, `std::pair`\ 's converting move constructor could be
written as follows:

.. parsed-literal::

    template <class First2, Second2>
    pair( pair<First2,Second2>&& rhs ) 
          **throw( !is_nothrow_constructible<First,First2&&>::value
                 || !is_nothrow_constructible<Second,Second2&&>::value )** [#is_nothrow_constructible]_
      : first( move(rhs.first) ), 
        second( move(rhs.second) )
    {}

Although the above is reminiscent of the `enable_if` clause that would
be *required* if there is a ban on throwing move constructors, the
exception specification above is entirely optional; its presence or
absence doesn't affect the correctness of a move constructor.

Interactions with Other Proposals
=================================

The generation of default move constructors, first proposed by Bjarne
Stroustrup in N2904_, and again by Bjarne Stroustrup and Lawrence
Crowl in N2953_, is harmonious with our proposal.  For example, since
throwing move constructors are allowed, default move constructors will
be generated in more cases, with performance benefits if *any*
subobjects have been move-enabled.

As a matter of QOI, a default move constructor would probably gain an
exception specification whose boolean constant parameter is computed
from the results of has_nothrow_move on all subobjects, but, being a
matter of QOI, that doesn't have any effect on standard text.

The proposed ``[[nothrow]]`` attribute is just a less-powerful version
of this feature.  In particular, it can't express the hint shown for
`pair`\ 's move constructor above.  We suggest it be dropped.

Existing Practice
=================

The Microsoft compiler has always treated empty
exception-specifications as though they have the same meaning we
propose for `throw(false)`.  That is, Microsoft omits the
standard-mandated runtime behavior if the function throws, and it
performs optimizations based on the assumption that the function
doesn't throw.  This interpretation of `throw()` has proven to be
successful in practice and is regarded by many as superior to the one
in the standard.  Standardizing `throw(false)` gives everyone access
to this optimization tool.

Proposed Changes to Standard Wording
====================================

.. Rani fills in this part


-------

.. [#attribute] In Frankfurt, Dave proposed that we use the attribute
   syntax ``[[moves(subobj1,subobj2)]]`` for this purpose.  Aside from
   being controversial, it's a wart regardless of the syntax used,
   adding a whole new mechanism just for move constructors but useless
   elsewhere.

.. _N2855: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2009/n2855.html

.. _N2953: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2009/n2953.html

.. [#x] Many move-enabled operations can give the strong guarantee
   regardless of whether move construction throws.  One example is
   ``std::list<T>::push_back``.  This issue affects only the narrow
   subset of operations that need to make *multiple* explicit moves
   from locations observable by the caller.

.. [#no-diagnostic] In particular, we are not proposing to mandate
   static checking: a `throw(false)` function can call a `throw(true)`
   function without causing the program to become ill-formed or
   generating a diagnostic.  Generating a diagnostic in such cases
   can, of course, be implemented by any compiler as a matter of QOI.

.. [#is_nothrow_constructible] See N2953_ for a definition of
   `is_nothrow_constructible`.
