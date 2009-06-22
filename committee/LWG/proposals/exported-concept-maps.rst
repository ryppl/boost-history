=====================
Exported Concept Maps
=====================

:Authors: Dave Abrahams <dave@boostpro.com> and Doug Gregor <doug.gregor@gmail.com>
:Number:  D2918=09-0108
:Abstract:
:Date: 2009-06-22

:Abstract: We propose a mechanism that allows default associated
           functions to be used in unconstrained code.

Summary
=======

Concepts and concept maps can provide default implementations of
associated functions.  For example, in the following code, ``Num``
models the ``LessThanComparable`` concept even though it only supplies
one of four required operators::

  concept LessThanComparable<typename T>
  {
      bool operator<(T const& x, T const& y);
      bool operator>(T const& x, T const& y) { return y < x; }
      bool operator<=(T const& x, T const& y) { return !(y < x); }
      bool operator>=(T const& x, T const& y) { return !(x < y); }
  }

  class Num
  {
       friend bool operator<(Num const& x, Num const& y)
       { ... }
  };
  
  concept_map LessThanComparable<Num>; // OK
  
Now all four operators required by ``LessThanComparable`` can be
applied to ``Num`` in a constrained template where
``LessThanComparable`` is required::

  template <LessThanComparable X>
  int f(X& a, X& b)
  {
      while (b > a) // Uses operator > from LessThanComparable
      { ... }
  }

  Num a, b;
  int z = f(a, b);  // OK, LessThanComparable supplies operator>

Unfortunately, the same cannot be said of contexts where ``Num`` has
not been constrained by ``LessThanComparable``::

  int f(Num& a, Num& b)
  {
      while (b > a) // error: no operator> defined
      { ... }
  }

We propose to allow ``concept_map``\ s to be explicitly “exported” to unconstrained
contexts like this::

  export concept_map LessThanComparable<Num>; // OK
  
Then the unconstrained definition of ``f`` above will work as might be
expected.  We also propose that concept maps generated with the
“intentional concept mapping” syntax described in D2916=09-0106 (if it
is accepted) be exported, so ``Num`` could be declared this way::

  class Num -> LessThanComparable
  {
       friend bool operator<(Num const& x, Num const& y)
       { ... }
  };

Motivation
==========

The status quo creates a disturbing divide between constrained and
unconstrained code.

* Divide between constrained and unconstrained code.
* Is Num LessThanComparable?  In constrained code, yes.  In unconstrained code, no.

Rationales
==========

We considered automatically exporting all ``concept_map``\ s, but that
could change the meaning of ordinary unconstrained code since
``concept_map``\ s can be used for post-hoc adaptation, that would
definitely cause problems

We considered making “``export``” the default, but we believe that the
“intentional concept mapping syntax” proposed by D2916=09-0106 covers
a large majority of the cases, so there's no compelling reason to
increase the risk associated with declaring a ``concept_map``.

Proposed Wording
================


Acknowledgements
================


