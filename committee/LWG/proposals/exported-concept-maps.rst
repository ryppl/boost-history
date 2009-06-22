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

We propose to allow ``concept_map``\ s to be explicitly “exported” to
unconstrained contexts like this::

  export concept_map LessThanComparable<Num> { } // OK
  
Then the unconstrained definition of ``f`` above will work as
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
unconstrained code.  Does the original definition of ``Num`` model
``LessThanComparable``?  In constrained code, yes; in unconstrained
code, no.  In order to make ``Num`` model ``LessThanComparable``
everywhere, one actually needs to duplicate all the default
definitions that have already been supplied by the template for
constrained code::

  class Num
  {
      friend bool operator<(Num const& x, Num const& y)
      { ... }
      // copy-paste-munge from LessThanComparable
      friend bool operator>(Num const& x, Num const& y) { return y < x; }
      friend bool operator<=(Num const& x, Num const& y) { return !(y < x); }
      friend bool operator>=(Num const& x, Num const& y) { return !(x < y); }
  };

Unlike an empty concept map, whose verbosity has caused some concern,
this boilerplate code truly adds zero value (except inasmuch as it
provides the desired operators for unconstrained code) and carries
with it all the usual disadvantages of duplicated code.  

Default implementations of associated functions arise in concepts like
``LessThanComparable`` whose interfaces are **non-minimal**, i.e. they
contain elements that can be implemented entirely in terms of other
interface elements.  In C++03 the usual way to avoid repeating this
boilerplate in each model of a concept is to capture the redundancy in
a base class template::

  // Derive your Model from this class (that's the Curiously Recurring
  // Template Pattern, CRTP) to implement redundant interface elements
  template <class Model>
  struct less_than_comparable
  {
      friend bool operator>(Model const& x, Model const& y) { return y < x; }
      friend bool operator<=(Model const& x, Model const& y) { return !(y < x); }
      friend bool operator>=(Model const& x, Model const& y) { return !(x < y); }
  };
  
  struct Num : less_than_comparable<Num>
  {
      friend bool operator<(Num const&, Num const&);
  };

  struct String : less_than_comparable<String>
  {
      friend bool operator<(String const&, String const&);
  };

If this proposal is accepted, all such CRTP base classes templates
could be discarded, the redundant interface being implemented directly
by the concept.  The Boost.Operators library, for example, could be
eliminated for C++0x, and the Boost.Iterator library would shrink
substantially.

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

