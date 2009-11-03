=====================================
 Allowing Move Constructors to Throw
=====================================

:Author: David Abrahams, Rani Sharoni, Doug Gregor
:Contact: dave@boostpro.com, rani_sharoni@hotmail.com, doug.gregor@gmail.com
:organization: `BoostPro Computing`_
:date: 2009-10-14

:Number: D2983=09-0173

.. _`BoostPro Computing`: http://www.boostpro.com

.. build HTML with:

   rst2html.py --footnote-references=superscript \
     --stylesheet-path=./rst.css --embed-stylesheet N2983-throwing-move.rst \
     N2983.html

.. contents:: index

Introduction
************

In N2855_, Doug Gregor and Dave Abrahams discussed a problematic
interaction between move constructors, templates, and certain standard
library member functions.  To date, attempts to solve the problem
(including N2855_ itself) have taken one basic approach: *ban throwing
move constructors, and be sure never to generate one*.  

Motivation
**********

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

* It hobbles the optimization potential of move constructors, because
  efficient move constructors that might throw (but almost never will)
  can't be generated or written.

* It adds another "Gotcha" that people have to worry about.

* It places tight restrictions on the order of code evolution: one
  cannot move-enable a class until all its sub-objects have been
  move-enabled.

* It forces the weakening of class invariants for some move-enabled
  classes: if move construction is not allowed to throw, the invariant
  must include a resource-less state in which we can leave the source
  object.

* In some cases class maintainers may need to choose between
  move-enabling their class and maintaining its current guarantees to
  users.  For example, Dinkumware's list implementation currently
  derives QOI benefits from having no resource-less state—“begin” and
  “end” iterators maintain their relationships even after lists are
  swapped.  If throwing move constructors were banned, that would no
  longer be possible.

* When it is necessary to manually write a move constructor for a
  templated class, it would in many cases require either expert-level
  metaprogramming skill or the addition of a language feature, just
  for move constructors, whose only purpose is in solving this
  particular problem. [#attribute]_

.. _N2904: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2009/n2904.pdf

Solution
********

Fortunately, there is a better way.  Instead of imposing this burden
on every class author, we can deal with the issue more selectively in
the operation being move-enabled.  There, we know whether a throwing
move can disturb existing guarantees.  We propose that instead of
using ``std::move(x)`` in those cases, thus granting permission for
the compiler to use *any* available move constructor, maintainers of
these particular operations should use ``std::move_if_noexcept(x)``, which
grants permission move *unless* it could throw and the type is
copyable.  Unless ``x`` is a move-only type, or is known to have a
nonthrowing move constructor, the operation would fall back to copying
``x``, just as though ``x`` had never acquired a move constructor at
all.

For example, ``std::pair``\ 's move constructor, were it to be written
out manually, could remain as it was before this issue was
discovered::

    template <class First2, Second2>
    pair( pair<First2,Second2>&& rhs )
      : first( move(rhs.first) ), 
        second( move(rhs.second) )
    {}

However, ``std::vector::reserve`` could be move-enabled this way: [#default-construct-swap]_

.. parsed-literal::

  void reserve(size_type n)
  {
      if (n > this->capacity())
      {
          pointer new_begin = this->allocate( n );
          size_type s = this->size(), i = 0;
          try
          {
              for (;i < s; ++i)
                   new ((void*)(new_begin + i)) value_type( **std::move_if_noexcept(** (\*this)[i]) **)** );
          }
          catch(...)
          {
              while (i > 0)                 // clean up new elements
                 (new_begin + --i)->~value_type();

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


We stress again that the use of ``std::move_if_noexcept`` as opposed to
``move`` would only be necessary under an *extremely* limited set of
circumstances.  In particular, it would never be required in new code,
which could simply give a *conditional* strong guarantee, e.g. “if an
exception is thrown other than by ``T``\ 's move constructor, there
are no effects.”  We recommend that approach as best practice for new
code.

Implementing ``std::move_if_noexcept``
**************************************

One possible implementation of ``std::move_if_noexcept`` might be::

  template <class T>
  typename conditional<
      !nothrow_move_constructible<T>::value
      && has_copy_constructor<T>::value,
      T const&,
      T&&
  >::type
  move_if_noexcept(T& x)
  {
      return std::move(x);
  }

We propose that ``nothrow_move_constructible<T>`` be a conservative
trait very much like ``has_nothrow_copy_constructor<T>`` from the
current working draft; it would be identical to the proposed
``is_nothrow_constructible<T,T&&>`` from N2953_.  In other words, it
returns ``true`` only when it can prove the move constructor doesn't
throw, and is *allowed* to return ``false`` even when the move
constructor can throw.

An Optimization Hint
********************

To help the library deduce the correct result for these traits, we
propose to add a new kind of exception-specification, spelled:

.. parsed-literal::

   noexcept( *integral constant expression* )

The only impact of such an exception-specification is this: if a
function decorated with ``noexcept(true)`` throws an exception, the
behavior is undefined. [#no-diagnostic]_ That effect is sufficient to
allow these *xxx*\ ``_nothrow_``\ *xxx* traits to report ``true`` for
any operation decorated with ``noexcept(true)``.  Class maintainers could
label their move constructors ``noexcept(true)`` to indicate non-throwing
behavior, and the library is permitted to take advantage of that
labelling if it can be detected (via “compiler magic”).

Note that the usefulness of ``noexcept(true)`` as an optimization hint
goes way beyond the narrow case introduced by N2855_.  In fact, it
goes beyond move construction: when the compiler can detect
non-throwing operations with certainty, it can optimize away a great
deal of code and/or data that is devoted to exception handling.  Some
compilers already do that for ``throw()`` specifications, but since
those incur the overhead of an implicit try/catch block to handle
unexpected exceptions, the benefits are limited.

The advantage of the integral constant expression parameter is that
one can easily offer accurate hints in templated move constructors.
For example, ``std::pair``\ 's converting move constructor could be
written as follows:

.. parsed-literal::

    template <class First2, Second2>
    pair( pair<First2,Second2>&& rhs ) 
          **noexcept( is_nothrow_constructible<First,First2&&>::value
                      && is_nothrow_constructible<Second,Second2&&>::value )** [#is_nothrow_constructible]_
      : first( move(rhs.first) ), 
        second( move(rhs.second) )
    {}

Although the above is reminiscent of the ``enable_if`` clause that would
be *required* if there is a ban on throwing move constructors, the
exception specification above is entirely optional; its presence or
absence doesn't affect the correctness of a move constructor.

operator ``noexcept(``\ *expression*\ ``)``
*******************************************

It seems that ``has_nothrow_``\ *xxx* traits are proliferating (and
not just in this proposal).  Once we have ``noexcept(``\
*bool-constant-expr*\ ``)`` available to make the information
available, it makes sense to generalize the traits into an operator
similar to ``sizeof`` and ``typeof`` that can give us answers about
*any* expression.


Interactions with Other Proposals
*********************************

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
``pair``\ 's move constructor above.  We suggest it be dropped.

Existing Practice
*****************

The Microsoft compiler has always treated empty
exception-specifications as though they have the same meaning we
propose for ``noexcept(true)``.  That is, Microsoft omits the
standard-mandated runtime behavior if the function throws, and it
performs optimizations based on the assumption that the function
doesn't throw.  This interpretation of ``throw()`` has proven to be
successful in practice and is regarded by many as superior to the one
in the standard.  Standardizing ``noexcept(true)`` gives everyone access
to this optimization tool.

Implicit ``noexcept(true)`` for Destructors
*******************************************

So few destructors can throw exceptions that the default
exception-specification for destructors could be changed from nothing
(i.e. ``noexcept(false)``) to ``noexcept(true)`` with only a tiny
amount of code breakage.  Such code is already very dangerous, and
where used properly, ought to be a well-known “caution area” that is
reasonably easily migrated.  However, we don't think this change would
be appropriate for C++0x at this late date, so we're not proposing it.

FIXME: Add some discussion about implementation freedom with noexcept(true).

Proposed Changes to Standard Wording
************************************

.. role:: sub

.. role:: ins

.. role:: del

.. role:: insc(ins)
   :class: ins code

.. role:: delc(del)
   :class: ins code

.. role:: raw-html(raw)
   :format: html

2.12 Keywords [lex.key]
=======================

Add the new ``noexcept`` keyword to Table 3 - Keywords.

3.7.4.1 Allocation functions [basic.stc.dynamic.allocation]
===========================================================

Modify paragraph 3 as follows:

  3 An allocation function that fails to allocate storage can invoke the currently installed new-handler function (18.6.2.3), if any. [ *Note*: A program-supplied allocation function can obtain the address of the currently installed new_handler using the ``std::set_new_handler`` function (18.6.2.4). -- *end note* ] If an allocation function declared with :del:`an empty` :ins:`a non-throwing` *exception-specification* (15.4), :del:`throw(),` fails to allocate storage, it shall return a null pointer. Any other allocation function that fails to allocate storage shall indicate failure only by throwing an exception of a type that would match a handler (15.3) of type ``std::bad_alloc`` (18.6.2.1).

5.3 Unary expressions [expr.unary]
==================================

Modify the grammar in paragraph 1 as follows:

  1 Expressions with unary operators group right-to-left.

  .. parsed-literal::

    *unary-expression*: 
      *postfix-expression*
      ++ *cast-expression* 
      -- *cast-expression* 
      *unary-operator* *cast-expression*
      sizeof *unary-expression*
      sizeof ( *type-id* ) 
      sizeof ... ( *identifier* ) 
      alignof ( *type-id* ) 
      :raw-html:`<span class="ins"><i>noexcept-expression</i></span>`
      *new-expression*
      *delete-expression*

5.3.4 New [expr.new]
====================

Modify paragraph 13 as follows:

  13 [*Note*: unless an allocation function is declared with :del:`an empty` :ins:`a non-throwing` *exception-specification* (15.4), :del:`throw(),` it indicates failure to allocate storage by throwing a ``std::bad_alloc`` exception (Clause 15, 18.6.2.1); it returns a non-null pointer otherwise. If the allocation function is declared with :del:`an empty` :ins:`a non-throwing` *exception-specification*, :del:`throw(),` it returns null to indicate failure to allocate storage and a non-null pointer otherwise. -- *end note*] If the allocation function returns null, initialization shall not be done, the deallocation function shall not be called, and the value of the new-expression shall be null.

5.3.7 noexcept operator [expr.unary.noexcept]
=============================================

(Add this new section)

  1 :ins:`The noexcept operator determines whether the evaluation of its operand, which is an unevaluated operand ([expr] Clause 5), can throw an exception ([except.throw]).`

  .. parsed-literal::

    :raw-html:`<span class="ins"><i>noexcept-expression</i></span>`
      :raw-html:`<span class="ins">noexcept ( <i>expression</i> )</span>`

  2 :raw-html:`<span class="ins">The result of the <code>noexcept</code> operator is a constant of type <code>bool</code>.</span>`

  3 :raw-html:`<span class="ins">The result of the <code>noexcept</code> operator is <code>false</code> if in an evaluated context the <i>expression</i> would contain</span>`

  * :raw-html:`<span class="ins">a potentially evaluated call [<i>Footnote</i>: This includes implicit calls, e.g., the call to an allocation function in a <i>new-expression</i>. -- <i>end footnote</i>] to a function, member function, function pointer, or member function pointer that does not have a non-throwing <i>exception-specification</i> ([except.spec]),</span>`

  * :raw-html:`<span class="ins">a potentially evaluated <i>throw-expression</i> ([except.throw]),</span>`

  * :raw-html:`<span class="ins">a potentially evaluated <code>dynamic_cast</code> expression <code>dynamic_cast&lt;T&gt;(v)</code>, where <code>T</code> is a reference type, that requires a run-time check ([expr.dynamic.cast]), or</span>`

  * :raw-html:`<span class="ins">a potentially evaluated <code>typeid</code> expression ([expr.typeid]) applied to an expression whose type is a polymorphic class type ([class.virtual]).</span>`

  :ins:`Otherwise, the result is true.`

14.6.3 Variadic templates [temp.variadic]
=========================================

Modify the fifth bullet of paragraph 4 as follows:

  4 A *pack expansion* is a sequence of tokens that names one or more parameter packs, followed by an ellipsis. The sequence of tokens is called the *pattern of the expansion*; its syntax depends on the context in which the expansion occurs. Pack expansions can occur in the following contexts:

    * In :raw-html:`a<span class="del">n</span> <i><span class="ins">dynamic-</span>exception-specification</i>` (15.4); the pattern is a *type-id*.

14.7.2.2 Type-dependent expressions [temp.dep.expr]
===================================================

Add the following case to the list in paragraph 4:

  4 Expressions of the following forms are never type-dependent (because the type of the expression cannot be dependent):

  .. parsed-literal::

    :raw-html:`<span class="ins">noexcept ( <i>expression</i> )</span>`

14.7.2.3 Value-dependent expressions [temp.dep.constexpr]
=========================================================

Modify paragraph 2 as follows:

  2 Expressions of the following form are value-dependent if the *unary-expression* :raw-html:`<span class="ins">or <i>expression</i></span>` is type-dependent or the *type-id* is dependent:

  .. parsed-literal::

    sizeof *unary-expression*
    sizeof ( *type-id* ) 
    alignof ( *type-id* )
    :raw-html:`<span class="ins">noexcept ( <i>expression</i> )</span>`

15.4 Exception specifications [except.spec]
===========================================

Change the following paragraphs as follows:

  1 A function declaration lists exceptions that its function might directly 
  or indirectly throw by using an *exception-specification* as a suffix of its 
  declarator.

  .. parsed-literal::

    *exception-specification:*
      :raw-html:`<span class="ins"><i>dynamic-exception-specification</i></span>`
      :raw-html:`<span class="ins"><i>noexcept-specification</i></span>`

    :raw-html:`<span class="ins"><i>dynamic-exception-specification</i>:</span>`
      :raw-html:`<span class="ins"><code>throw (</code> <em>type-id-list<sub>opt</sub></em> <code>)</code></span>`

    *type-id-list*:
      *type-id* :raw-html:`<code>...</code><sub><i>opt</i></sub>`
      *type-id-list*, *type-id* :raw-html:`<code>...</code><sub><i>opt</i></sub>`

    :raw-html:`<span class="ins"><em>noexcept-specification:</em></span>`
      :raw-html:`<span class="ins"><code>noexcept (</code> <em>constant-expression<span class="sub">opt</span></em> <code>)</code></span>`

  :raw-html:`<span class="ins">In a <i>noexcept-specification</i>, the
  <i>constant-expression</i>, if supplied, shall be a constant expression
  ([expr.const]) that is contextually converted to <code>bool</code>
  ([conv] Clause 4). A <i>noexcept-specification</i>
  <code>noexcept()</code> is equivalent to <code>noexcept(true)</code>.</span>`

  7 A function is said to *allow* an exception of type ``E`` if its :raw-html:`<i><span class="ins">dynamic-</span>exception-specification</i>` contains a type ``T`` for which a handler of type ``T`` would be a match (15.3) for an exception of type ``E``.

  .. comment :raw-html:`<span class="ins">, if its <i>noexcept-specification</i> is <code>noexcept(false)</code>, or if the function has no <i>exception-specification</i>`.

  11 A function with no *exception-specification* :raw-html:`<span class="ins">, or with an <i>exception-specification</i> of the form <code>noexcept(<i>constant-expression</i>)</code> where the <i>constant-expression</i> yields <code>false</code>,</span>` allows all exceptions. :raw-html:`<span class="ins">An <i>exception-specification</i> is <i>non-throwing</i> if it is of the form <code>throw()</code>, <code>noexcept()</code>, or <code>noexcept(<i>constant-expression</i>)</code> where the <i>constant-expression</i> yields <code>true</code>.</span>` A function with :del:`an empty` :ins:`a non-throwing` *exception-specification* :raw-html:`<span class="del">, <code>throw()</code>,</span>` does not allow any exceptions.

  14 In :raw-html:`a<span class="del">n</span> <i><span class="ins">dynamic-</span>exception-specification</i>,` a *type-id* followed by an ellipsis is a pack expansion (14.6.3).

Add the following new paragraph:

    :raw-html:`<span class="ins">15 If a function with a
    <i>noexcept-specification</i> whose <i>constant-expression</i>
    yields <code>true</code> throws an exception, the behavior is
    undefined.  A <i>noexcept-specification</i> whose
    <i>constant-expression</i> yields <code>true</code> is in all
    other respects equivalent to the <i>exception-specification</i>
    <code>throw()</code>.  A <i>noexcept-specification</i> whose
    <i>constant-expression</i> yields <code>false</code> is equivalent
    to omitting the <i>exception-specification</i> altogether.</span>`

.. comment

  17.6.4.10 Restrictions on exception handling [res.on.exception.handling]
  ========================================================================

  Modify footnote 192 (the first footnote in paragraph 2) as follows:

    192) That is, the C library functions can all be treated as if they have :del:`a throw()` :ins:`:del:`an empty` :ins:`a non-throwing`` exception-specification. This allows implementations to make performance optimizations based on the absence of exceptions at runtime.

18.8.2.2 Type unexpected_handler [unexpected.handler]
=====================================================

Modify paragraph 1 as follows:

  1 The type of a handler function to be called by ``unexpected()`` when a function attempts to throw an exception not listed in its :raw-html:`<i><span class="ins">dynamic-</span>exception-specification</i>.`

20.3 Utility components [utility]
=================================

Change Header ``<utility>`` synopsis as follows:

.. parsed-literal::

  // 20.3.2, forward/move: 
  template <class T> struct identity; 
  template <class T> T&& forward(typename identity<T>::type&&); 
  template <class T> typename remove_reference<T>::type&& move(T&&);
  :ins:`template <class T> typename conditional<
    !nothrow_move_constructible<T>::value && has_copy_constructor<T>::value, 
    T const&, T&&>::type move_if_noexcept(T& x);`

20.3.2 forward/move helpers [forward]
=====================================

Append the following:

  .. parsed-literal::

    :ins:`template <class T> typename conditional<
      !nothrow_move_constructible<T>::value && has_copy_constructor<T>::value, 
      T const&, T&&>::type move_if_noexcept(T& x);`

  :raw-html:`<span class="ins">10 <em>Returns:</em> <code>std::move(t)</code></span>`

20.6.2 Header ``<type_traits>`` synopsis [meta.type.synop]
==========================================================

.. parsed-literal::

    template <class T> struct has_nothrow_assign;
    :ins:`template <class T> struct has_move_constructor; 
    template <class T> struct nothrow_move_constructible;

    template <class T> struct has_move_assign; 
    template <class T> struct nothrow_move_assignable;

    template <class T> struct has_copy_constructor; 
    template <class T> struct has_default_constructor; 
    template <class T> struct has_copy_assign;`

    template <class T> struct has_virtual_destructor;



20.6.4.3 Type properties [meta.unary.prop]
==========================================

Add entries to table 43:

+--------------------------------+-----------------------------------+-----------------------------------+
| Template                       |Condition                          |Preconditions                      |
+================================+===================================+===================================+
| ``template <class T>           |``T`` has a move constructor       |``T`` shall be a complete type.    |
| struct has_move_constructor;`` |(17.3.14).                         |                                   |
+--------------------------------+-----------------------------------+-----------------------------------+
| ``template <class T>           |``noexcept( T( make<T>() ) )``     |``T`` shall be a complete type.    |
| struct                         |                                   |                                   |
| nothrow_move_constructible;``  |                                   |                                   |
|                                |                                   |                                   |
|                                |                                   |                                   |
+--------------------------------+-----------------------------------+-----------------------------------+
| ``template <class T>           |``T`` has a move assignment        |``T`` shall be a complete type.    |
| struct has_move_assign;``      |operator (17.3.13).                |                                   |
+--------------------------------+-----------------------------------+-----------------------------------+
| ``template <class T>           |``noexcept( *(T*)0 = make<T> )``   |``T`` shall be a complete type.    |
| struct                         |                                   |                                   |
| nothrow_move_assignable;``     |                                   |                                   |
|                                |                                   |                                   |
+--------------------------------+-----------------------------------+-----------------------------------+
| ``template <class T>           |``T`` has a copy constructor       |``T`` shall be a complete type, an | 
| struct has_copy_constructor;`` |(12.8).                            |array of unknown bound, or         |
|                                |                                   |(possibly cv-qualified) ``void.``  |
|                                |                                   |                                   |
+--------------------------------+-----------------------------------+-----------------------------------+
| ``template <class T>           |``T`` has a default constructor    |``T`` shall be a complete type, an |
| struct                         |(12.1).                            |array of unknown bound, or         |
| has_default_constructor;``     |                                   |(possibly cv-qualified) ``void.``  |
|                                |                                   |                                   |
+--------------------------------+-----------------------------------+-----------------------------------+
| ``template <class T>           |``T`` has a copy assignment        |``T`` shall be a complete type, an |
| struct has_copy_assign;``      |operator (12.8).                   |array of unknown bound, or         |
|                                |                                   |(possibly cv-qualified) ``void``.  |
|                                |                                   |                                   |
+--------------------------------+-----------------------------------+-----------------------------------+

23.3.2.3 deque modifiers [deque.modifiers]
==========================================

Context::

    iterator insert(const_iterator position, const T& x);
    iterator insert(const_iterator position, T&& x);
    void insert(const_iterator position, size_type n, const T& x);
    template <class InputIterator>;
       void insert(const_iterator position, ;
                   InputIterator first, InputIterator last);

    template <class... Args> void emplace_front(Args&&... args);
    template <class... Args> void emplace_back(Args&&... args);
    template <class... Args> iterator emplace(const_iterator position, Args&&... args);
    void push_front(const T& x);
    void push_front(T&& x);
    void push_back(const T& x);
    void push_back(T&& x);`

Change Paragraph 2 as follows:

  2 Remarks: If an exception is thrown other than by the copy
  constructor\ :ins:`, move constructor, move assignment operator` or
  assignment operator of ``T`` there are no effects.  :raw-html:`<span
  class="ins">If an exception is thrown by the move constructor of a
  non-CopyConstructible <code>T</code>, the effects are
  unspecified.</span>`

-----

Context::

  iterator erase(const_iterator position); 
  iterator erase(const_iterator first, const_iterator last);

Change paragraph 6 as follows:

    6 Throws: Nothing unless an exception is thrown by the copy constructor,
    :ins:`move constructor, move assignment operator`
    or assignment operator of ``T``.

23.3.6.2 vector capacity [vector.capacity]
==========================================

Context::

   void reserve(size_type n);

Remove paragraph 2:

    :del:`2 Requires: If value_type has a move constructor, that
    constructor shall not throw any exceptions.`

Change paragraph 3 as follows:

    :del:`3`:ins:`2` Effects: A directive that informs a vector of a
    planned change in size, so that it can manage the storage
    allocation accordingly. After ``reserve()``, ``capacity()`` is
    greater or equal to the argument of reserve if reallocation
    happens; and equal to the previous value of ``capacity()``
    otherwise.  Reallocation happens at this point if and only if the
    current capacity is less than the argument of ``reserve()``. If an
    exception is thrown :raw-html:`<span class="ins">other than by the
    move constructor of a non-CopyConstructible <code>T</code>` there
    are no effects.

-----

Context::

      void resize(size_type sz, const T& c);

Change paragraph 13 to say:

    If an exception is thrown :raw-html:`<span class="ins">other than
    by the move constructor of a non-CopyConstructible
    <code>T</code></span>` there are no effects.

23.3.6.4 vector modifiers [vector.modifiers]
============================================

Change the section as follows:

  .. parsed-literal::

    iterator insert(const_iterator position, const T& x); 
    iterator insert(const_iterator position, T&& x); 
    void insert(const_iterator position, size_type n, const T& x); 
    template <class InputIterator>
      void insert(const_iterator position, InputIterator first, InputIterator last);
    template <class... Args> void emplace_back(Args&&... args);
    template <class... Args> iterator emplace(const_iterator position, Args&&... args);
    void push_back(const T& x); 
    void push_back(T&& x);

  :del:`1 Requires: If value_type has a move constructor, that constructor shall
  not throw any exceptions.`

  :del:`2`:ins:`1` Remarks: Causes reallocation if the new size is
  greater than the old capacity. If no reallocation happens, all the
  iterators and references before the insertion point remain valid.
  If an exception is thrown other than by the copy constructor
  :ins:`move constructor, move assignment operator,` or assignment
  operator of ``T`` or by any InputIterator operation there are no
  effects.  :raw-html:`<span class="ins">if an exception is thrown by
  the move constructor of a non-CopyConstructible <code>T</code>, the
  effects are unspecified.`

  :del:`3`:ins:`2` Complexity: The complexity is linear in the number
  of elements inserted plus the distance to the end of the vector.

**Note to proposal reader:** The strong guarantee of ``push_back`` for
CopyConstructible ``T``\ s is maintained by virtue of 23.2.1
[container.requirements.general] paragraph 11.

-----

Context::

  iterator erase(const_iterator position); 
  iterator erase(const_iterator first, const_iterator last);

Change paragraph 6 as follows:

    6 Throws: Nothing unless an exception is thrown by the copy
    constructor, :ins:`move constructor, move assignment operator`, or
    assignment operator of ``T``.

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
   static checking: a ``noexcept(true)`` function can call a ``noexcept(false)``
   function without causing the program to become ill-formed or
   generating a diagnostic.  Generating a diagnostic in such cases
   can, of course, be implemented by any compiler as a matter of QOI.

.. [#is_nothrow_constructible] See N2953_ for a definition of
   ``is_nothrow_constructible``.

.. [#default-construct-swap] Actually ``reserve`` and other such
   operations can be optimized even for a type without non-throwing
   move constructors but with a default constructor and a non-throwing
   swap, by first default-constructing elements in the new array and
   swapping each element into place.
