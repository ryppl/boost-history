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
       friend bool operator<(Num const& x, Num const& y) { … }
  };
  
  concept_map LessThanComparable<Num> { } // OK
  
Now all four operators required by ``LessThanComparable`` can be
applied to ``Num`` in a constrained template where
``LessThanComparable`` is required::

  template <LessThanComparable X>
  int f(X& a, X& b)
  {
      while (b > a) // Uses operator > from LessThanComparable
      { … }
  }

  Num a, b;
  int z = f(a, b);  // OK, LessThanComparable supplies operator>

Unfortunately, the same cannot be said of contexts where ``Num`` has
not been constrained by ``LessThanComparable``:

.. parsed-literal::

  int f(Num& a, Num& b)
  {
      while (b > a) // **ERROR:** no operator> defined
      { … }
  }

We propose to allow ``concept_map``\ s to be explicitly “exported” to
unconstrained contexts like this::

  export concept_map LessThanComparable<Num> { } // OK
  
Then the unconstrained definition of ``f`` above will work as
expected.  We also propose that concept maps generated with the
“intentional concept mapping” syntax described in D2916=09-0106 (if it
is accepted) be exported, so ``Num`` could be declared this way:

parsed-literal::

  class Num **-> LessThanComparable**
  {
       friend bool operator<(Num const& x, Num const& y) { … }
  };

Motivation
==========

The status quo creates a disturbing divide between constrained and
unconstrained code.  Does the original definition of ``Num`` model
``LessThanComparable``?  In constrained code, the answer is “yes;” in
unconstrained code, “no.”  In order to make ``Num`` model
``LessThanComparable`` everywhere, one actually needs to duplicate all
the default definitions that have already been supplied by the
concept for constrained code:

.. parsed-literal::

  class Num
  {
      friend bool operator<(Num const& x, Num const& y) { … }
      **// copy-paste-munge from LessThanComparable**
      **friend bool operator>(Num const& x, Num const& y) { return y < x; }**
      **friend bool operator<=(Num const& x, Num const& y) { return !(y < x); }**
      **friend bool operator>=(Num const& x, Num const& y) { return !(x < y); }**
  };

Unlike an empty concept map, whose verbosity has caused some concern,
this redundant code truly adds zero value (except inasmuch as it
provides the desired operators for unconstrained code) and carries
with it all the usual disadvantages of duplicated code.  We hate the
idea of being forced to regurgitate boilerplate just to reach the
baseline of uniform behavior in constrained and unconstrained code,
and we predict that most users writing models will have the same
reaction (or worse).

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
      // captured boilerplate:
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

Risks, Opportunities, and Rationale
===================================

In general, adding definitions to a system increases complexity and
the risk of unexpected effects (the safest code is no code).  Exported
``concept_map``\ s, in particular, add candidates to overload sets.
These new definitions can potentially change the meaning of
unconstrained code, which by currently has no dependency on the
``concept_map``.  That risk is mitigated by the fact that the exported
defaults are looked up only through associated namespaces, so the
offending ``concept_map`` would have to be written in one of those.
If one can assume the type author has control over definitions in his
namespace, then any such semantic change would likely be intentional,
but if lump everything together into the global namespace or start
writing ``concept_map``s in namespaces they do not control, the
potential for surprise is greater.

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

1. :ins:`An` :ins-emphasis:`exported concept map` :ins:`is a concept map for which each concept map member that satisfies a requirement member corresponding to an associated function requirement can be found by name lookup. These concept map members are called` :ins-emphasis:`exported associated function definitions`\ :ins:`. [Example:` ::

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

2. :ins:`An exported associated function definition that corresponds to an associated non-member function requirement is visible in the namespace enclosing the exported concept map. [Note: the exported associated function definition can be found by any form of name lookup that would find a function declaration with the same name and signature, including unqualified name lookup (3.4.1), argument-dependent name lookup (3.4.2), and qualified name lookup into a namespace (3.4.3.2). --end note]`

3. :ins:`An exported associated function definition that corresponds to an associatd member function requirement is visible in the class nominated by the exported associated function definition. The exported associated function definition is treated as a public member of the nominated class. [Example:` ::

    concept C<typename T> {
      void T::f();
      void T::g() { this->f(); }
      void T::h() { this->g(); }
    }

    struct Y { 
      void h();
    };

    export concept_map C<Y> { } // Y::f and Y::g are now visible

    void f(X &x) {
      x.f(); // okay: calls C<Y>'s Y::f
      x.g(); // okay: calls C<Y>'s Y::g
      x.h(); // okay: calls Y::h
    }

  :ins:`- end example]`

4. :ins:`An exported associated function definition of an exported concept map template is visible when the concept map template's template parameters can be deduced (14.9.2) from the corresponding associated function requirement, as specified below.The concept map template is then instantiated with the deduced template arguments; the resulting concept map is an exported concept map whose exported associated function requirements are visible. Deduction of the concept map template's template arguments depending on the form of the associated function requirement:`

  * :ins:`When the associated function requirement is an associated non-member function requirement, template argument deduction attempts to deduce the concept map template's template parameters from the` :ins-emphasis:`parameter-type-list` :ins:`of the associated non-member function requirement. [Example:` ::

      concept EQ2<typename T, typename U> {
        bool operator==(const T&, const U&);
        bool operator!=(const T& t, const U& u) { return !(t == u); }
      }

      struct A { };
      struct B { };

      template<std::ObjectType T> struct ptr {
        T* m;
      };

      template<typename T, typename U>
      export concept_map EQ2<ptr<T>, ptr<U>> {
        bool operator==(const ptr<T>& t, const ptr<U>& u) { 
          return t.m == u.m;
        }
      }

      bool f(ptr<int> p1, ptr<float> p2) { 
        return p1 == p2; // okay: from operator==(const ptr<T>& t, const ptr<U>& u),
                         // deduces T=int and U=float
                         // instantiates concept_map EQ2<ptr<T>, ptr<U>> to find
                         // EQ2<ptr<int>, ptr<float>>::operator==(const ptr<int>& t, const ptr<float>& u)
      }

    :ins:`- end example]`

  * :ins:`When the associated function requirement is an associated member function requirement, template argument deduction attempts to deduce the concet map template's template parameters from the nominated class of the associated member function requirement. [Example:` ::

      concept M<typename T, typename U> {
        void T::f(U);
      }

      template<typename T>
      struct X { };

      template<typename.......FIXME

    :ins:`- end example]`


Acknowledgements
================


.. |opt| replace:: :sub:`opt`

