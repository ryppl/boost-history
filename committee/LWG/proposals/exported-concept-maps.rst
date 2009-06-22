=====================
Exported Concept Maps
=====================

:Authors: Dave Abrahams <dave@boostpro.com> and Doug Gregor <doug.gregor@gmail.com>
:Number:  D2918=09-0108
:Date: 2009-06-22

:Abstract: We propose a mechanism that allows default associated
           functions to be used in unconstrained code.

.. role:: ins
   :class: ins

.. role:: ins-emphasis
   :class: ins emphasis

.. role:: ins-pre
   :class: ins pre

.. role:: del
   :class: del

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
  
  concept_map LessThanComparable<Num> { }// OK
  
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

  export concept_map LessThanComparable<Num> { } // OK
  
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

Modify the grammar in 14.10.2 [concept.map] as follows:

  *concept-map-definition*: 
    :ins:`export`\ |opt| *concept_map* ::\ |opt| *nested-name-specifier*\ |opt| *concept-id* { *concept-map-member-specification*\ |opt| } ;\ |opt|

Modify paragraph 6 of [concept.map] as follows:

6. A concept map member that satisfies a requirement member cannot be found by any form of name lookup (3.4) :ins:`unless it is a member of an exported concept map ([concept.map.export])`.

Add a new paragraph at the end of [concept.map]:

15. :ins:`A concept-map-definition that starts with` :ins-pre:`export` :ins:`defines an exported concept map ([concept.map.export]).`

Add a new section 14.10.2.3 Exported concept maps [concept.map.export]:

1. :ins:`An` :ins-emphasis:`exported concept map` :ins:`is a concept map for which each concept map member that satisfies a requirement member corresponding to an associated function requirement can be found by name lookup in the namespace enclosing the concept map. These concept map members are called` :ins-emphasis:`exported associated member function definitions`\ :ins:`. [Example:` ::

    concept EQ<typename T> {
      bool operator==(const T& x, const T& y);
      bool operator!=(const T& x, const T& y) { return !(x == y); }
    }

    struct X { bool operator==(const X&) const; };
  
    export concept_map EQ<X> { }

    bool f(X x, X y) { 
      return x != y; // okay: name lookup finds EQ<X>::operator!=
    }

  :ins:`- end example]`

2. Deal with template argument deduction issues for concept map templates.

3. Deal with member function requirements.


Acknowledgements
================


.. |opt| replace:: :sub:`opt`

