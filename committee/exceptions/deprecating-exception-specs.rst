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
a call to ``std::terminate()``.

With the introduction of ``noexcept``, programmers can now express the
two kinds of exception guarantees that are useful in practice, without
additional overhead. This paper therefore proposes to deprecate
"dynamic" exception specifications, i.e., those that are written as 
:raw-html:`<code>throw(</code><i>type-id-list<sub>opt</sub></i><code>)</code>`.

Approach
********

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
declaration and in the definition. 

Proposed Changes to Standard Wording
************************************

The wording in this paper is based on the current working paper
(N3035) as amended by N3050.

15.4 Exception specifications [except.spec]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Modify the paragraphs in this section as follows. 

  3 If any declaration of a function has an *exception-specification*
  :raw-html:`<span class="ins">that is not a <i>noexcept-specification</i> allowing all exceptions</span>`, 
  all declarations, including the definition and an explicit
  specialization, of that function shall have :raw-html:`a<span class="del">n</span> <span
  class="ins">compatible</span> <i>exception-specification</i>
  <span class="del">with the same set of <i>type-id</i>s</span>.` If
  any declaration of a pointer to function, reference to function, or
  pointer to member function has an *exception-specification*,
  all occurrences of that declaration shall have :raw-html:`a<span class="del">n</span> <span
  class="ins">compatible</span> <i>exception-specification</i>
  <span class="del">with the same set of <i>type-id</i>s</span>.` In an
  explicit instantiation an *exception-specification* may be
  specified, but is not required. If an *exception-specification* is
  specified in an explicit instantiation directive, it shall 
  :raw-html:`<span class="del">have the
  same set of <i>type-id</i>s as</span><span class="ins">be compatible
  with the <i>exception-specification</i>s of</span>` other declarations
  of that function. 
  A diagnostic is required only if the :raw-html:`<span class="del">sets of <i>type-id</i>s are
  different</span><span class="ins"><i>exception-specifications</i>
  are not compatible</span>` within a single translation unit.

  :ed:`[Insert a new paragraph before paragraph 5]` :raw-html:`<span class="ins">Two
  <i>exception-specifications</i> are <i>compatible</i> if:</span>`

    * :raw-html:`<span class="ins">both are non-throwing (regardless of their form), </span>`
    * :raw-html:`<span class="ins">both have the form <code>noexcept(<i>constant-expression</i>)</code> and the <i>constant-expression</i>s are equivalent,</span>`
    * :raw-html:`<span class="ins">one <i>exception-specification</i> is a <i>noexcept-specification</i> allowing all exceptions and the other is of the form <code>throw(<i>type-id-list</i>)</code>, or</span>`
    * :raw-html:`<span class="ins">both are <i>dynamic-exception-specifications</i> that have the same set of <i>type-id</i>s.</span>`

  5 In such an assignment or initialization, *exception-specifications* on return types and parameter types shall :del:`match exactly` :ins:`be compatible`. In other assignments or initializations, *exception-specifications* shall :del:`match exactly` :ins:`be compatible`.

  :ed:`[Insert a new paragraph at the end of 15.4]` :raw-html:`<span class="ins">[ <i>Note</i>: The use of <i>dynamic-exception-specification</i>s is deprecated (see annex D). - <i>end note</i> ]</span>`
  
:ins:`D.5 Dynamic exception specifications [depr.except.spec]`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  :ins:`1` :raw-html:`<span class="ins">The use of <i>dynamic-exception-specification</i>s is deprecated.</span>`

------

.. [Sutter02] A Pragmatic Look at Exception Specifications. http://www.gotw.ca/publications/mill22.htm

.. [Boost03] http://www.boost.org/development/requirements.html#Exception-specification

.. [N3050] D. Abrahams, R. Sharoni, and D. Gregor. *Allowing Move Constructors to Throw*. Document number N3050=10-0040, ISO C++ Committee Post-Pittsburgh Mailing, March, 2010.