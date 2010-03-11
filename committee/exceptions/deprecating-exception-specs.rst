======================================
 Deprecating Exception Specifications
======================================

:Author: Doug Gregor
:Contact: doug.gregor@gmail.com
:date: 2010-03-11

:Number: D3051=10-0041

.. build HTML with:

   rst2html.py --footnote-references=superscript \
     --stylesheet-path=./rst.css --embed-stylesheet throwing-move.rst \
     N3051.html

.. contents:: index

Introduction
************

UK-136
  Exception specifications have proven close to worthless in practice, while adding a measurable overhead to programs. The feature should be deprecated. The one exception to the rule is the empty throw specification which could serve a legitimate optimizing role if the requirement to call the runtime unexpected mechanism was relaxed in this case.

As expressed in the national body comment above, exception
specifications have not proven useful in practice. There are numerous
discussions of the problems with exception specifications in C++ (see,
e.g., [Sutter02]_, [Boost03]_), but the main issues are:

* *Run-time checking*: C++ exception specifications are checked at runtime
  rather than at compile time, so they offer no programmer guarantees
  that all exceptions have been handled. The run-time failure mode
  (calling ``std::unexpected()``) does not lend itself to recovery.
* *Run-time overhead*: Run-time checking requires the compiler to
  produce additional code that also hampers optimizations. 
* *Unusable in generic code*: Within generic code, it is not generally
  possible to know what types of exceptions may be thrown from
  operations on template arguments, so a precise exception
  specification cannot be written.

In practice, only two forms of exception-throwing guarantees are
useful: an operation might throw an exception (any exception) or an
operation will never throw any exception. The former is expressed by
omitting the exception-specification entirely, while the latter *can* be
expressed as ``throw()`` but rarely is, due to performance
considerations.

[N3050]_ introduces a new kind of exception specification, ``noexcept``,
the specifies that the function will not throw any exceptions. Unlike
``throw()``, ``noexcept`` does not require the compiler to introduce
code to check whether an exception is thrown. Rather, if a function
specified as ``noexcept`` is exited via an exception, the result is
undefined behavior, which permits compilers to optimize based on the
knowledge that a ``noexcept`` function will not propagate an
exception.

With the introduction of ``noexcept``, programmers can now express the
two kinds of exception guarantees that are useful in practice, without
additional overhead. This paper therefore proposes to deprecate
"dynamic" exception specifications, i.e., those based on
``noexcept``.

Approach
********

The general approach taken by this paper is to separate the wording
required for dynamic exception specifications (those using ``throw``)
into a new, deprecated section D.5, while maintaining the description
of ``noexception`` and the general behavior of exception
specifications in 15.4. The intent of the revised 15.4 (and core
language in general) is to minimize the number of dependencies on
the deprecated section D.5, and to mark each of those with a
cross-reference.

To aid in the transition from dynamic exception specifications to
``noexcept``, the wording provides somewhat loose compatibility rules
for redeclarations of functions that have exception
specifications. Two rules stand out:

  1) All "non-throwing" forms of exception specifications
  (``throw()``, ``noexcept``, ``noexcept(true)``) are considered
  compatible, but the exception specification on the definition is
  what affects code generation:

  .. parsed-literal::

    // header ultramodern.h
    void f() noexcept;
    
    // source plodding.cpp
    #include "ultramodern.h"
    struct X { };
    void f() throw() { // okay, compatible with noexcept
      throw X(); // calls std::unexpected()
    }


  2) noexcept(false) is considered compatible with ``throw(``
  *type-id-list* ``)``:
  
  .. parsed-literal::

    // header ultramodern.h
    void g() noexcept(false);

    // source plodding.cpp
    #include "ultramodern.h"
    struct X { };
    void g() throw(X) { // okay, compatible with noexcept(false)
      throw X(); // okay
    }

These compatibility rules allow a gradual migration from dynamic
exception specifications to ``noexcept``, since a declaration of a
function can choose to use the new or old syntax independently in the
declaration and in the definition. This is particularly important with
the implicit declarations of ``operator new``, ``operator new[]``,
``operator delete``, and ``operator delete[]``, which this document
changes to use ``noexcept``. 

Proposed Changes to Standard Wording
************************************

.. role:: sub

.. role:: ins

.. role:: del

.. role:: ed

.. role:: insc(ins)
   :class: ins code

.. role:: delc(del)
   :class: ins code

.. role:: raw-html(raw)
   :format: html

The wording in this paper is based on the current working paper
(N3035) as amended by N3050.

3.7.4 Dynamic storage duration [basic.stc.dynamic]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Modify paragraph 2 as follows:

  2 The library provides default definitions for the global allocation
  and deallocation functions. Some global allocation and deallocation
  functions are replaceable (18.6.1). A C++ program shall provide at
  most one definition of a replaceable allocation or deallocation
  function. Any such function definition replaces the default version
  provided in the library (17.6.3.6). The following allocation and
  deallocation functions (18.6) are implicitly declared in global
  scope in each translation unit of a program.

  .. parsed-literal::

    void* operator new(std::size_t) :del:`throw(std::bad_alloc)` :ins:`noexcept(false)`;
    void* operator new[](std::size_t) :del:`throw(std::bad_alloc)` :ins:`noexcept(false)`;
    void operator delete(void*) :del:`throw()` :ins:`noexcept`;
    void operator delete[](void*) :del:`throw()` :ins:`noexcept`;

  These implicit declarations introduce only the function names
  ``operator new``, ``operator new[]``, ``operator delete``, 
  ``operator delete[]`` [*Note*: the implicit declarations do not
  introduce the names ``std`` :del:`, std::bad_alloc,` and
  ``std::size_t``, or any other names that the
  library uses to declare these names. Thus, a *new-expression*,
  *delete-expression* or function call that refers to one of these
  functions without including the header ``<new>`` is
  well-formed. However, referring to ``std`` :del:`, std::bad_alloc,` and
  ``std::size_t`` is ill-formed unless the name has been declared by
  including the appropriate header. -- *end note*] Allocation and/or
  deallocation functions can also be declared and defined for any
  class (12.5).

15.4 Exception specifications [except.spec]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Modify the paragraphs in this section as follows. Note that every
paragraph in this section is accounted for (even those that have not
changed), to ease review. Editorial notes are :ed:`[Yellow]` and will
describe, e.g., when specific paragraphs have been moved. The
paragraphs are numbered with old and new numbers (old/new), where the
old numbers refer to the current working paper and the new numbers
refer to the desired paragraph numbers after this paper is applied to
the working paper.

  :del:`1`/:ins:`1` A function declaration lists exceptions that its function might
  directly or indirectly throw by using an *exception-specification*
  as a suffix of its declarator. 
  :raw-html:`<span class="ins">A function is said to <i>allow</i> an
  exception of type <code>E</code> if such an exception will propagate
  from the outermost block of that function out of the function.</span>`

  .. parsed-literal::

    *exception-specification*:
      *dynamic-exception-specification*
      *noexcept-specification*

    :ed:`[Moved to D.5p1]` :raw-html:`<span class="del"><i>dynamic-exception-specification</i>:</span>`
      :raw-html:`<span class="del"><code>throw (</code> <i>type-id-list<sub>opt</sub></i> <code>)</code></span>`

    :ed:`[Moved to D.5p1]` :raw-html:`<span class="del"><i>type-id-list</i>:</span>`
      :raw-html:`<span class="del"><i>type-id</i> <code>...</code><sub><i>opt</i></sub></span>`
      :raw-html:`<span class="del"><i>type-id-list</i> <code>,</code> <i>type-id</i> <code>...</code><sub><i>opt</i></sub></span>`

    *noexcept-specification*:
      ``noexcept (`` *constant-expression* ``)``
      ``noexcept``

  :ins:`2` In a *noexcept-specification*, the *constant-expression*, if supplied,
  shall be a constant expression ([expr.const]) that is contextually
  converted to ``bool`` ([conv] Clause 4). A *noexcept-specification*
  ``noexcept`` is equivalent to ``noexcept(true)``.

  :del:`2`/:ins:`3` An *exception-specification* shall appear only on a function
  declarator for a function type, pointer to function type, reference
  to function type, or pointer to member function type that is the
  top-level type of a declaration or definition, or on such a type
  appearing as a parameter or return type in a function declarator. An
  *exception-specification* shall not appear in a typedef declaration
  or *alias-declaration*. [ *Example* :

  .. parsed-literal::

    void f() :del:`throw(int)`:ins:`noexcept`;              // OK
    void (\*fp)() :del:`throw (int)`:ins:`noexcept`;        // OK
    void g(void pfa() :del:`throw(int))`:ins:`noexcept`;    // OK
    typedef int (\*pf)() :del:`throw(int)`:ins:`noexcept`;  // ill-formed

  - *end example* ] 

  :ed:`[Moved to D.5p2]` :del:`A type denoted in an exception-specification shall not
  denote an incomplete type. A type denoted in an
  exception-specification shall not denote a pointer or reference to an
  incomplete type, other than void\*, const void\*, volatile void\*, or
  const volatile void\*.`

  6 :ed:`[Moved to D.5p3]` :raw-html:`<span class="del">An
  <i>exception-specification</i> can include the same type more than
  once and can include classes that are related by inheritance, even
  though doing so is redundant. [ <i>Note</i>: An
  <i>exception-specification</i> can also include the class
  <code>std::bad_exception</code> (18.8.2.1). - <i>end note</i>
  ]</span>`

  7 :ed:`[Moved to D.5p4]` :raw-html:`<span class="del">A function is
  said to <i>allow</i> an exception of type <code>E</code> if its
  <i>dynamic-exception-specification</i> contains a type
  <code>T</code> for which a handler of type <code>T</code> would be a
  match (15.3) for an exception of type <code>E</code>.</span>`

  8 :ed:`[Moved to D.5p5]` :raw-html:`<span class="del">Whenever an
  exception is thrown and the search for a handler (15.3) encounters
  the outermost block of a function with an <i>exception-specification</i>,
  the function <code>std::unexpected()</code> is called (15.5.2) if
  the <i>exception-specification</i> does not allow the exception. [
  <i>Example</i>:</span>`

  .. parsed-literal::

    :del:`class X { };`
    :del:`class Y { };`
    :del:`class Z: public X { };`
    :del:`class W { };`

    :del:`void f() throw (X, Y) {`
      :del:`int n = 0;`
      :del:`if (n) throw X(); // OK`
      :del:`if (n) throw Z(); // also OK`
      :del:`throw W();        // will call std::unexpected()`
    :del:`}`

  :raw-html:`<span class="del">-- <i>end example</i></span>]`

  9 :ed:`[Moved to D.5p6]` :raw-html:`<span class="del">The function
  <code>std::unexpected()</code> may throw an exception that will
  satisfy the <i>dynamic-exception-specification</i> for which it
  was invoked, and in this case the search for another handler will
  continue at the call of the function with this
  <i>dynamic-exception-specification</i> (see 15.5.2), or it may call
  <code>std::terminate()</code>.</span>`

  10 :ed:`[Moved to D.5p7]` :raw-html:`<span class="del">An implementation shall not reject an expression merely because when executed it throws or might throw an exception that the containing function does not allow. [ <i>Example</i>:</span>`

    .. parsed-literal::

      :del:`extern void f() throw(X, Y);`
      :del:`void g() throw(X) {`
        :del:`f(); // OK`
      :del:`}`

  :raw-html:`<span class="del">the call to <code>f</code> is well-formed even though when called, <code>f</code> might throw exception <code>Y</code> that <code>g</code> does not allow. - <i>end example</i> ]</span>`

  :del:`11`/:ins:`4` A function with no *exception-specification*, or with an
  *exception-specification* of the form ``noexcept(``
  *constant-expression* ``)`` where the *constant-expression* yields
  ``false``, allows all exceptions. An *exception-specification* is
  non-throwing if it is of the form :del:`throw(),` ``noexcept``,
  :del:`or` ``noexcept(`` *constant-expression* ``)`` where the
  *constant-expression* yields ``true``
  :raw-html:`<span class="ins">, or <code>throw()</code> (D.5)</span>`
  . A function with a
  non-throwing *exception-specification* does not allow any
  exceptions.

  :ins:`5` :raw-html:`<span class="ins">Two
  <i>exception-specifications</i> are <i>compatible</i> if:</span>`

    * :raw-html:`<span class="ins">both are non-throwing (regardless of their form), </span>`
    * :raw-html:`<span class="ins">both have the form <code>noexcept(<i>constant-expression</i>)</code> and the <i>constant-expression</i>s are equivalent,</span>`
    * :raw-html:`<span class="ins">one <i>exception-specification</i> is of the form <code>noexcept(false)</code> and the other is of the form <code>throw(<i>type-id-list</i>)</code> (D.5), or</span>`
    * :raw-html:`<span class="ins">both are <i>dynamic-exception-specifications</i> (D.5) that have the same set of <i>type-id</i>s.</span>`

  :del:`3`/:ins:`6` If any declaration of a function has an *exception-specification*,
  all declarations, including the definition and an explicit
  specialization, of that function shall have an :raw-html:`<span
  class="ins">compatible</span> <i>exception-specification</i>
  <span class="del">with the same set of <i>type-id</i>s</span>.` If
  any declaration of a pointer to function, reference to function, or
  pointer to member function has an *exception-specification*, all
  occurrences of that declaration shall have an :raw-html:`<span
  class="ins">compatible</span> <i>exception-specification</i>
  <span class="del">with the same set of <i>type-id</i>s</span>.` In an
  explicit instantiation an *exception-specification* may be
  specified, but is not required. If an *exception-specification* is
  specified in an explicit instantiation directive, it shall 
  :raw-html:`<span class="del">have the
  same set of <i>type-id</i>s as</span><span class="ins">be compatible
  to the <i>exception-specification</i>s of</span>` other declarations
  of that function. 
  A diagnostic is required only if the :raw-html:`<span class="del">sets of <i>type-id</i>s are
  different</span><span class="ins"><i>exception-specifications</i>
  are not compatible</span>` within a single translation unit.

  :del:`4`/:ins:`7` If a virtual function has an *exception-specification*, all declarations, including the definition, of any function that overrides that virtual function in any derived class shall only allow exceptions that are allowed by the *exception-specification* of the base class virtual function. [ *Example*:

    .. parsed-literal::

      struct B { 
        virtual void f() throw (int, double);
        virtual void g();
        :ins:`virtual void h() noexcept;`
        :ins:`virtual void i() noexcept(false);`
      };
      
      struct D: B { 
        void f();                 // ill-formed
        void g() throw (int);     // OK
        :ins:`void h() noexcept(false); // ill-formed`
        :ins:`void i() noexcept;        // OK`
      };

  The declaration of ``D::f`` is ill-formed because it allows all exceptions, whereas ``B::f`` allows only ``int`` and`` double``. :raw-html:`<span class="ins">Similarly, the declaration of <code>D::h</code> is ill-formed because it allows all exceptions, whereas <code>B::h</code> does not allow any exceptions.</span>` - *end example*] A similar restriction applies to assignment to and initialization of pointers to functions, pointers to member functions, and references to functions: the target entity shall allow at least the exceptions allowed by the source value in the assignment or initialization. [ *Example*:

    .. parsed-literal::

      class A { /\*...\*/ }; 
      void (\*pf1)();    // no exception specification 
      void (\*pf2)() throw(A);
      :ins:`void (\*pf3)() noexcept;`
      void f() { 
        pf1 = pf2;  // OK: pf1 is less restrictive 
        :ins:`pf1 = pf3;  // OK: pf1 is less restrictive`
        pf2 = pf1;  // error: pf2 is more restrictive
        :ins:`pf3 = pf1;  // error: pf3 is more restrictive`
        :ins:`pf3 = pf2;  // error: pf3 is more restrictive`
      }

  - *end example* ]

  :del:`5`/:ins:`8` In such an assignment or initialization, *exception-specifications* on return types and parameter types shall :del:`match exactly` :ins:`be compatible`. In other assignments or initializations, *exception-specifications* shall :del:`match exactly` :ins:`be compatible`.

  :del:`12`/:ins:`9` An *exception-specification* is not considered part of a function's
  type.

  :del:`13`/:ins:`10` An implicitly declared special member function (Clause 12)
  :del:`shall` :ins:`may` have an *exception-specification*. :del:`If`
  :ins:`Let` ``f`` :del:`is` :ins:`be` an implicitly declared default
  constructor, copy constructor, destructor, or copy assignment
  operator, :ins:`then:`

    * ``f`` shall allow all exceptions if any function it directly invokes allows all exceptions :ins:`,`
    * ``f`` shall :del:`allow no exceptions` :raw-html:`<span class="ins">have the implicit <i>exception-specification</i> <code>noexcept</code></span>` if every function it directly invokes allows no exceptions :ins:`, otherwise`
    * its implicit *exception-specification* :raw-html:`<span class="ins">is a <i>dynamic-exception-specification</i> (D.5) that </span>` specifies the *type-id* ``T`` if and only if ``T`` is allowed by the *exception-specification* of a function directly invoked by ``f``'s implicit definition.

  [ *Example*:

    .. parsed-literal::

      struct A { 
        A();
        A(const A&) throw();
        ~A() throw(X); 
      };

      struct B { 
        B() throw(); 
        B(const B&) throw(); 
        ~B() throw(Y);
      };

      struct D : public A, public B {
        // Implicit declaration of D::D(); 
        // Implicit declaration of D::D(const D&) throw(); 
        // Implicit declaration of D::~D() throw(X,Y);
      };

  Furthermore, if ``A::~A()`` or ``B::~B()`` were virtual, ``D::~D()`` would not be as restrictive as that of ``A::~A``, and the program would be ill-formed since a function that overrides a virtual function from a base class shall have an exception-specification at least as restrictive as that in the base class. - *end example* ]

  :del:`14`/:ins:`11` :ed:`[Moved to D.5p8]` :raw-html:`<span class="del">In a <i>dynamic-exception-specification</i>, a <i>type-id</i> followed by an ellipsis is a pack expansion (14.6.3).</span>`

  :del:`15`/:ins:`12` If a function with a *noexcept-specification* whose
  *constant-expression* yields ``true`` throws an exception, the
  behavior is undefined. :raw-html:`<span
  class="ins">[<i>Note</i>:</span> A <i>noexcept-specification</i>
  whose <i>constant-expression</i> yields <code>true</code> is in all
  other respects equivalent to the <i><span
  class="ins">dynamic-</span>exception-specification</i>
  <code>throw()</code> <span class="ins">(D.5)</span>.<span
  class="ins"> - <i>end note</i>]</span>` A *noexcept-specification*
  whose *constant-expression* yields ``false`` is equivalent to omitting the
  *exception-specification* altogether.

15.5 Special functions [except.special]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  1 The :raw-html:`function<span class="del">s</span> <code>std::terminate()</code> (15.5.1) <span class="del">and <code>std::unexpected()</code> (15.5.2) are</span><span class="ins">is</span>` used by the exception handling mechanism for coping with errors related to the exception handling mechanism itself. The function ``std::current_exception()`` (18.8.5) and the class ``std::nested_exception`` (18.8.6) can be used by a program to capture the currently handled exception.

15.5.2 The ``std::unexpected()`` function [except.unexpected]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Move this section to D.5 to become D.5.1
[depr.except.unexpected]. Then modify the following paragraphs as
follows:

  1 If a function with :del:`an` :ins:`a` :raw-html:`<i><span class="ins">dynamic-</span>exception-specification</i> throws an exception that is not listed in the <i><span class="ins">dynamic-</span>exception-specification</i>, the function <code>std::unexpected()</code> is called (18.8.2) immediately after completing the stack unwinding for the former function.`

  3 The ``std::unexpected()`` function shall not return, but it can throw (or re-throw) an exception. If it throws a new exception which is allowed by the exception specification which previously was violated, then the search for another handler will continue at the call of the function whose exception specification was violated. If it throws or rethrows an exception that the *exception-specification* does not allow then the following happens: If the :raw-html:`<i><span class="ins">dynamic-</span>exception-specification</i> does not include the class <code>std::bad_exception</code> (18.8.2.1) then the function <code>std::terminate()</code> is called, otherwise the thrown exception is replaced by an implementation-defined object of the type <code>std::bad_exception</code> and the search for another handler will continue at the call of the function whose <i><span class="ins">dynamic-</span>exception-specification</i> was violated.`

  4 Thus, :del:`an` :ins:`a` :raw-html:`<i><span class="ins">dynamic-</span>exception-specification</i> guarantees that only the listed exceptions will be thrown. If the <i><span class="ins">dynamic-</span>exception-specification</i> includes the type <code>std::bad_exception</code> then any exception not on the list may be replaced by <code>std::bad_exception</code> within the function <code>std::unexpected()</code>.`

17.6.4.11 Restrictions on exception handling [res.on.exception.handling]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Modify the paragaphs shown as follows:

  1 Any of the functions defined in the C++ standard library can
  report a failure by throwing an exception of a type described in its
  **Throws**: paragraph or its *exception-specification* (15.4). An
  implementation may strengthen the *exception-specification* for a
  non-virtual function by removing listed exceptions :raw-html:`<span
  class="ins">or replacing an empty <i>dynamic-exception-specification</i> (D.5 [depr.except.spec.dynamic]) <code>throw()</code> with a compatible <code>noexcept</code> specification (15.4)</span>`.

  3 Functions from the C standard library shall not throw exceptions [191]_ except when such a function calls a program-supplied function that throws an exception.

.. [191] That is, the C library functions can all be treated as if they have a :del:`throw()` :ins:`non-throwing` *exception-specification*. This allows implementations to make performance optimizations based on the absence of exceptions at runtime.

18.6 Dynamic memory management [support.dynamic]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Modify the header ``<new>`` synopsis as follows.

.. parsed-literal::

  void* operator new(std::size_t size) :del:`throw(std::bad_alloc)`:ins:`noexcept(false)`;
  void* operator new(std::size_t size, const std::nothrow_t&) :del:`throw()``:ins:`noexcept`; 
  void operator delete(void* ptr) :del:`throw()``:ins:`noexcept`; 
  void operator delete(void* ptr, const std::nothrow_t&) :del:`throw()``:ins:`noexcept`; 
  void* operator new[](std::size_t size) :del:`throw(std::bad_alloc)``:ins:`noexcept(false)`; 
  void* operator new[](std::size_t size, const std::nothrow_t&) :del:`throw()``:ins:`noexcept`; 
  void operator delete[](void* ptr) :del:`throw()``:ins:`noexcept`; 
  void operator delete[](void* ptr, const std::nothrow_t&) :del:`throw()``:ins:`noexcept`;
  void* operator new (std::size_t size, void* ptr) :del:`throw()``:ins:`noexcept`; 
  void* operator new[](std::size_t size, void* ptr) :del:`throw()``:ins:`noexcept`; 
  void operator delete (void* ptr, void*) :del:`throw()``:ins:`noexcept`;
  void operator delete[](void* ptr, void*) :del:`throw()``:ins:`noexcept`;

18.6.1.1 Single-object forms [new.delete.single]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Update the signatures of ``operator new`` and ``operator delete`` in this section to reflect the changes to the synopsis.

18.6.1.2 Array forms [new.delete.array]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Update the signatures of ``operator new[]`` and ``operator delete[]`` in this section to reflect the changes to the synopsis.

18.6.1.3 Placement forms [new.delete.placement]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Update the signatures of ``operator new``, ``operator delete``, ``operator new[]``, and ``operator delete[]`` in this section to reflect the changes to the synopsis.

18.8 Exception handling [support.exception]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Modify paragraph 1 as follows:

  1 The header ``<exception>`` defines several types and functions related to the handling of exceptions in a C++ program.

  Header ``<exception>`` synopsis

  .. parsed-literal::

    namespace std { 
      class exception; 
      :del:`class bad_exception;`
      class nested_exception;

      :ins:`// D.5.2, violating exception-specifications (deprecated):`
      :ins:`class bad_exception;`
      typedef void (\*unexpected_handler)(); 
      unexpected_handler set_unexpected(unexpected_handler f) throw(); 
      void unexpected [[noreturn]] ();

      typedef void (\*terminate_handler)(); 
      terminate_handler set_terminate(terminate_handler f) throw(); 
      void terminate [[noreturn]] ();

      bool uncaught_exception() throw(); 

      typedef *unspecified* exception_ptr;

      exception_ptr current_exception(); 
      void rethrow_exception [[noreturn]] (exception_ptr p); 
      template<class E> exception_ptr copy_exception(E e);
      template <class T> void throw_with_nested [[noreturn]] (T&& t); 
      template <class E> void rethrow_if_nested(const E& e);
    }

  SEE ALSO: 15.5 :ins:`, D.5.1 [depr.except.unexpected]`.




18.8.2	Violating exception-specifications [exception.unexpected]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Move this section to D.5 to become D.5.2
[depr.exception.unexpected].

:ins:`D.5 Dynamic exception specifications [depr.except.spec.dynamic]`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Insert this new section. :ins:`Green underlined text` is used to
indicate new wording, while normal text is used whenever text was
moved from another section (15.4, 15.5).

  1 :ins:`A dynamic exception specification lists exceptions that its function
  might directly or indirectly throw.`

  .. parsed-literal::

    :ed:`[Moved from 15.4p1]` :raw-html:`<span class="ins"><i>dynamic-exception-specification</i>:</span>`
      :raw-html:`<span class="ins"><code>throw (</code> <i>type-id-list<sub>opt</sub></i> <code>)</code></span>`

    :ed:`[Moved from 15.4p1]` :raw-html:`<span class="ins"><i>type-id-list</i>:</span>`
      :raw-html:`<span class="ins"><i>type-id</i> <code>...</code><sub><i>opt</i></sub></span>`
      :raw-html:`<span class="ins"><i>type-id-list</i> <code>,</code> <i>type-id</i> <code>...</code><sub><i>opt</i></sub></span>`

  2 :ed:`[Moved from 15.4p2]` :raw-html:`A type denoted in a
  <i><span class="ins">dynamic-</span>exception-specification</i> shall not denote an
  incomplete type. A type denoted in a
  <i><span class="ins">dynamic-</span>exception-specification</i> shall not denote a pointer or
  reference to an incomplete type, other than <code>void*</code>,
  <code>const void*</code>, <code>volatile void*</code>, or
  <code>const volatile void*</code>.`

  3 :ed:`[Moved from 15.4p6]` :raw-html:`A
  <i><span class="ins">dynamic-</span>exception-specification</i> can include the same type more than
  once and can include classes that are related by inheritance, even
  though doing so is redundant. [ <i>Note</i>: A
  <i><span class="ins">dynamic-</span>exception-specification</i> can also include the class
  <code>std::bad_exception</code> (18.8.2.1). - <i>end note</i>
  ]`

  4 :ed:`[Moved from 15.4p7]` :raw-html:`A function <span class="del">is
  said to <i>allow</i></span><span class="ins">allows</span> an exception of type <code>E</code> if its
  <i>dynamic-exception-specification</i> contains a type
  <code>T</code> for which a handler of type <code>T</code> would be a
  match (15.3) for an exception of type <code>E</code>.`

  5 :ed:`[Moved from 15.4p8]` :raw-html:`Whenever an
  exception is thrown and the search for a handler (15.3) encounters
  the outermost block of a function with a<span class="del">n</span>
  <i><span class="ins">dynamic-</span>exception-specification</i>, the function
  <code>std::unexpected()</code> is called (15.5.2) if the
  <i><span class="ins">dynamic-</span>exception-specification</i> does not allow the exception. [
  <i>Example</i>:`

  .. parsed-literal::

    class X { };
    class Y { };
    class Z: public X { };
    class W { };

    void f() throw (X, Y) {
      int n = 0;
      if (n) throw X(); // OK
      if (n) throw Z(); // also OK
      throw W();        // will call std::unexpected()
    }

  :raw-html:`-- <i>end example</i>]`

  6 :ed:`[Moved from 15.4p9]` :raw-html:`The function
  <code>std::unexpected()</code> may throw an exception that will
  satisfy the <i><span class="ins">dynamic-</span>exception-specification</i> for which it
  was invoked, and in this case the search for another handler will
  continue at the call of the function with this
  <i><span class="ins">dynamic-</span>exception-specification</i> (see 15.5.2), or it may call
  <code>std::terminate()</code>.`

  7 :ed:`[Moved from 15.4p10]` :raw-html:`An implementation shall not reject an expression merely because when executed it throws or might throw an exception that the containing function <span class="ins">whose <i>exception-specification</i> is a <i>dynamic-exception-specification</i></span> does not allow. [<i>Example</i>:`

    .. parsed-literal::

      extern void f() throw(X, Y);
      void g() throw(X) {
        f(); // OK
      }

  :raw-html:`the call to <code>f</code> is well-formed even though when called, <code>f</code> might throw exception <code>Y</code> that <code>g</code> does not allow. - <i>end example</i>]`

  8 :ed:`[Moved from 15.4p14]` :raw-html:`In a <i>dynamic-exception-specification</i>, a <i>type-id</i> followed by an ellipsis is a pack expansion (14.6.3).`


------

.. [Sutter02] A Pragmatic Look at Exception Specifications. http://www.gotw.ca/publications/mill22.htm

.. [Boost03] http://www.boost.org/development/requirements.html#Exception-specification

.. [N3050] D. Abrahams, R. Sharoni, and D. Gregor. *Allowing Move Constructors to Throw*. Document number N3050=10-0040, ISO C++ Committee Post-Pittsburgh Mailing, March, 2010.