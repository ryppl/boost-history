===================================
Explicit Refinement and Type Safety
===================================

:Authors: Doug Gregor <doug.gregor@gmail.com> and Dave Abrahams <dave@boostpro.com>
:Number:  xxxx=xx-xxxx
:Date: 2009-07-04

This paper describes a significant type-safety problem with explicit
refinement, a feature proposed in N2906_ as a replacement for explicit
concepts. The problem itself was first noted by Chris Jefferson in
reflector message c++std-lib-24237; this paper merely expands upon
Chris' observation.

Overloading and Type-Safety
===========================

In N2831_, we described a type-safety problem with the (then current)
rvalue-reference binding rules, wherein an rvalue reference could bind
to an lvalue, silently stealing resources from the lvalue. The
rvalue-references problem highlighted an important principle of type
safety:

.. Admonition:: Principle of Type-safe Overloading (PTO)

   Every function must be type-safe in isolation, *without regard to
   how it has been overloaded.*

The interpretation of this principle is quite simple: one should be
able to look at a single function (template) and determine from its
signature whether that function can be invoked for a given set of
arguments. One must not rely on the existence of other overloaded
functions to ensure that the single function (template) is correct,
because those other overloads will not always be available within the
overload set, due to language rules (such as SFINAE) or physical
separation in different headers. 

This principle is certainly not new. Back when overloading was
introduced into C++, various uses of the "overload" keyword were tried
(and subsequently removed). Bjarne Stroustrup summarizes the issue
with the "overload" keyword succinctly (from c++std-lib-23985):

    In particular, we *must* pick the right version of an algorithm
    without a single point of definition of all versions.

The principle of type-safe overloading is based on that same idea.

Explicit Refinement
===================

Explicit refinement [N2906_] is an attempt to eliminate the need for
explicit (non-auto) concepts by introducing a new kind of concept
refinement, *explicit refinement*, that limits when a concept map can
be implicitly generated. In particular, when a concept ``B``
explicitly refines a concept ``A``, a type ``X`` that meets the syntactic
requirements of both ``B`` and ``A`` will be considered a ``B`` unless
we are performing some kind of partial ordering (of class template
partial specialization, concept map templates, or function
templates). In these partial-ordering cases, ``X`` will only be
considered a ``B`` when there is an explicit concept map ``B<X>`` or
when the other candidates in the ordering do not involve ``A<X>``.

Explicit refinement can best be understood through examples. N2906_
uses the example of a ``copy()`` algorithm that is overloaded both for
input iterators (as in the Standard Library) and for contiguous
iterators. The two constrained templates are::

  // #1
  template<InputIterator InIter, OutputIterator<auto, InIter::reference> OutIter> 
    OutIter copy(InIter first, InIter last, OutIter result) {
      while (first != last) {
        *result = *first;  
        ++first; 
        ++result;
      }
      return result;
    }

  // #2
  template<ContiguousIterator InIter, ContiguousIterator OutIter>
    requires SameType<InIter::value_type, OutIter::value_type>
          && POD<InIter::value_type>
    OutIter copy(InIter first, InIter last, OutIter result) {
      if (first != last) 
        memmove(&*result, *&first, (last - first) * sizeof(InIter::value_type));
      return result + (last - first); 
    }

The first ``copy()`` implementation copies element-by-element, which
works for any ``InputIterator``/``OutputIterator`` pair with suitable
value types. The second ``copy()`` implementation uses ``memmove()``
for far greater performance with both iterators point to contiguous
blocks with the same POD data type.

Next, we consider two calls to ``copy()``::

  void test_copy(deque<int>::iterator di, int* ip, int n, int *result) {
    copy(di, di + n, result);
    copy(ip, ip + n, result);
  }

Ideally, the first call should invoke the ``copy()`` marked #1, since a deque
does not store its contents in contiguous memory. The second call
should invoke the ``copy())`` marked #2, providing the ``memmove()``
optimization automatically. Any other solution either misses an
optimization opportunity or leads to run-time errors. 

With explicit refinement, one would express the iterator concepts
hierarchy using implicit concepts and some explicit refinement as::

  /*auto*/ concept InputIterator<typename T> { /* ... */ }
  /*auto*/ concept ForwardIterator<typename T> : explicit InputIterator<T> { /* ... */ }
  /*auto*/ concept BidirectionalIterator<typename T> : ForwardIterator<T> { /* ... */ }
  /*auto*/ concept RandomAccessIterator<typename T> : BidirectionalIterator<T> { /* ... */ }
  /*auto*/ concept ContiguousIterator<typename T> : explicit RandomAccessIterator<T> { 
    requires LvalueReference<reference> && LvalueReference<subscript_reference>; 
  }

Now, consider the call to ``copy()`` that provides ``deque<int>``
iterator arguments. Both versions of ``copy()`` could be called,
because ``deque<int>::iterator`` syntactically meets the requirements
of both ``InputIterator`` and ``ContiguousIterator``. However, the
explicit refinement of ``ContiguousIterator`` from
``RandomAccessIterator`` (which, transitively, refines
``InputIterator``) prevents ``copy()`` #2 from being selected by
partial ordering. That's good: if we called ``copy()`` #2, the result
would be a run-time failure as the code attempts to ``memmove()`` the
contents of a deque.

With the call to ``copy()`` that provides a pointer to ``int``, both
versions of ``copy()`` can again be called, with the same
result. However, we assume that the author of the
``ContiguousIterator`` concept will write the appropriate concept map
template to explicitly state that all pointers are
``ContiguousIterators``, e.g.,

::

  template<ObjectType T> concept_map ContiguousIterator<T*> { }

With this explicitly-written concept map template, the ``copy()`` call
provided with pointers to ``int`` will select the optimized ``copy()``
#2. 

A Type-Safety Problem with Explicit Refinement
==============================================

The Principle of Type-Safe Overloading says that each overload should
be type-safe in isolation, without the presence of other
overloads. Therefore, we separate the two ``copy()`` algorithms in
some logical manner:

  1. The first copy (for ``InputIterators``) is placed in the standard
  header ``<algorithm>``, where it already resides.

  2. The second copy (for ``ContiguousIterators``) is placed into the
  header ``<mylib/contiguous.h>``, along with the
  ``ContiguousIterator`` concept and any required concept maps. This
  is a likely scenario where third-party libraries introduce new
  semantic concepts along with optimizations based on those concepts.

Now, consider our ``test_copy`` function compiled in three different
translation units, where the set of includes varies from one to the
next::

  #include <deque>
  #include <algorithm> // A
  #include <mylib/contiguous.h> // B
  #include <mylib/trie.h>
  using namespace mylib;
  using namespace std;

  void test_copy(deque<int>::iterator di, int* ip, int n, int *result) {
    copy(di, di + n, result);
    copy(ip, ip + n, result);
  }

When lines A and B are present, both ``copy()`` algorithms will be
visible and ``test_copy`` will have the intended semantics.

When line A is present but line B is not present, only the
``InputIterator`` version of ``copy()`` is visible, so both calls in
``test_copy`` will use that function. The resulting performance will
be lower, because we're missing the ``memmove()``
optimization. However, the program is still correct.

When line B is present but line A is not present, only the
``ContiguousIterator`` version of ``copy()`` is visible. Since both
``deque<int>`` iterators and ``int`` pointers meet the syntactic
requirements of ``ContiguousIterator``, we end up performing the
``memmove`` optimization in both cases, leading to erroneous run-time
behavior for the deque.

What happened? The optimized ``copy()`` violates the principle of
type-safe overloading, because it only properly rejects iterators that 
syntactically meet the requirements of ``ContiguousIterator`` (but
don't semantically meet those requirements) when the
less-specialized ``InputIterator`` copy algorithm is also
visible. 

Conclusion
==========

The use of explicit refinement leads to violations of the principle of
type-safe overloading, leading to unsafe run-time behavior. If the
introduction of explicit refinement also implies the removal of
explicit concepts (as suggested in N2906_), programmers will not be
able to safely provide optimized versions algorithms based on new
semantic concepts, as we have shown in the ``copy()`` above, for fear
that those optimizations will be silently applied when they should not
be.

-----

.. [#N2906] Bjarne Stroustrup, *Simplifying the use of concepts*, ISO C++ committee document N2906=09-0096, June, 2009.

.. [#N2831] David Abrahams and Doug Gregor, *Fixing a Safety Problem with Rvalue References: Proposed Wording*, ISO C++ committee document N2831=09-0021, December, 2008.
