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
  
The remaining three operators (>, <=, >=) are provided with default
definitions within the concept map. Now all four operators required by
``LessThanComparable`` can be 
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
  
Here, the synthesized definitions for the >, <=, and >= operators are
exported from the concept map so that they are visible to normal name
lookup. Then, the unconstrained definition of ``f`` above will work as
expected.  We also propose that concept maps generated with the
“intentional concept mapping” syntax described in N2916=09-0106 (if it
is accepted) be exported, so ``Num`` could be declared this way:

.. parsed-literal::

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
interface elements.  Such interfaces are quite common—in Boost you can
find entire libraries devoted to implementing the redundant parts of
non-minimal interfaces ([Operators]_, [Iterator]_).  The usual way to
avoid repeating this
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

In C++0x as defined today, one would need both the CRTP base *and* the
``concept_map`` to achieve uniform behavior in constrained and
unconstrained code.  If this proposal is accepted, though, all such
CRTP base class templates could be discarded, the redundant interface
being implemented directly by the concept.  The Boost.Operators
library, for example, could be eliminated for C++0x, and the
Boost.Iterator library would shrink substantially—a massive reduction
in verbosity that would make C++0x code that uses (exported) concept
maps substantially shorter than the equivalent non-concepts code.


Risks, Opportunities, and Rationale
===================================

In general, adding definitions to a system increases complexity and
the risk of unexpected effects (the safest code is no code).  Exported
``concept_map``\ s, in particular, add candidates to overload sets.
These new definitions can potentially change the meaning of
unconstrained code, which currently has no dependency on the
``concept_map``.  That risk is mitigated by the fact that the exported
definitions are only found when the exported concept map is itself
visible, so these exported definitions follow the same rules as scoped
concept maps.
If one can assume the type author has control over definitions in his
namespace, then any such semantic change (e.g., introducing another concept 
map into that namespace) would likely be intentional.
However, if one lumps everything together into the global namespace or starts
writing ``concept_map``\s in namespaces they do not control, the
potential for surprise is greater.

We considered automatically exporting all ``concept_map``\s, to
provide the greatest consistency between constrained and unconstrained
code. However, we do not recommend this approach because it is
generally wrong when concept maps are used to adapt syntax, since
syntax adaptations for the sake of concepts aren't typically meant to
be part of the public interface of a type. More importantly, we found
that those types whose public interface involves defaults from
concepts maps were almost always good candidates for the “intentional
concept mapping syntax” proposed by N2916=09-0106, which already
explicitly ties the interface of a type to a particular set of
concepts (including their defaults).

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

2. :ins:`The exported associated function definitions in an exported concept map or exported concept map template are only visible when the exported concept map (or concept map template) is visible via concept map lookup (14.11.1.1). [Example:` ::

    struct Y { bool operator==(const Y&) const; };

    namespace N {
      export concept_map EQ<Y> { }; // okay

      bool f(Y y1, Y y2) {
        return y1 != y2; // okay: name lookup finds N::EQ<Y>::operator!=
      }
    }

    bool g(Y y1, Y y2) {
      return y1 != y2; // error: N::EQ<Y> is not visible to concept map lookup,
                       // therefore N::EQ<Y>::operator!= is not visible.
    }
    
  :ins:`- end example]`

3. :ins:`An exported associated function definition that corresponds to an associated non-member function requirement is visible in the namespace enclosing the exported concept map. [Note: the exported associated function definition can be found by any form of name lookup that would find a function declaration with the same name and signature, including unqualified name lookup (3.4.1), argument-dependent name lookup (3.4.2), and qualified name lookup into a namespace (3.4.3.2). --end note]`

4. :ins:`An exported associated function definition that corresponds to an associatd member function requirement is visible in the class nominated by the exported associated function definition. The exported associated function definition is treated as a public member of the nominated class. [Example:` ::

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

5. :ins:`An exported associated function definition of an exported concept map template is visible when the concept map template's template parameters can be deduced (14.9.2) from the corresponding associated function requirement, as specified below.The concept map template is then instantiated with the deduced template arguments; the resulting concept map is an exported concept map whose exported associated function requirements are visible. Deduction of the concept map template's template arguments depends on the form of the associated function requirement:`

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

  * :ins:`When the associated function requirement is an associated member function requirement, template argument deduction attempts to deduce the concept map template's template parameters from the nominated class of the associated member function requirement. [Example:` ::

      concept C<typename T> { }

      concept M1<typename T> {
        void T::f();
      }

      concept M2<typename T, typename U> {
        void T::f(U);
      }

      template<C T> struct X { };

      template<C T>
      export concept_map M1<X<T>> {
        void X<T>::f() { }
      }

      template<C T, typename U>
      export concept_map M2<X<T>, U> {
        void X<T>::f(U) { }
      }

      void f(X<int> x, int y) {
        x.f(); // okay: template argument deduction deduces T=int  X<T>
        x.f(y); // okay?
      }

    :ins:`- end example]`


Acknowledgements
================

Bibliography
============

.. [Operators] The Boost.Operators library. http://www.boost.org/doc/libs/1_39_0/libs/utility/operators.htm.

.. [Iterator] The Boost.Iterator library. http://www.boost.org/doc/libs/1_39_0/libs/iterator/doc/index.html.

.. |opt| replace:: :sub:`opt`

