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
optimization automatically.

With explicit refinement, one would express the iterator concepts
hierarchy using implicit concepts and some explicit refinement as:

  /*auto*/ concept InputIterator<typename T> { /* ... */ }
  /*auto*/ concept ForwardIterator<typename T> : explicit InputIterator<T> { /* ... */ }
  /*auto*/ concept BidirectionalIterator<typename T> : ForwardIterator<T> { /* ... */ }
  /*auto*/ concept RandomAccessIterator<typename T> : BidirectionalIterator<T> { /* ... */ }
  /*auto*/ concept ContiguousIterator<typename T> : explicit RandomAccessIterator<T>  { 
   requires LvalueReference<reference> && LvalueReference<subscript_reference>; 
  }

Now, consider the call to ``copy()`` that provides ``deque<int>``
iterator arguments. Both versions of ``copy()`` could be called,
because ``deque<int>::iterator`` syntactically meets the requirements
of both ``InputIterator`` and ``ContiguousIterator``. However, the
explicit refinement of ``ContiguousIterator`` from
``RandomAccessIterator`` (which, transitively, refines
``InputIterator``) prevents ``copy()`` #2 from being selected by
partial ordering. Thus, we get the behavior we want.

With the call to ``copy()`` that provides a pointer to ``int``, both
versions of ``copy()`` can again be called, with the same
result. However, we assume that the author of the
``ContiguousIterator`` concept will write the appropriate concept map
template to explicitly state that all pointers are
``ContiguousIterator``s, e.g.,

::

  template<ObjectType T> concept_map ContiguousIterator<T*> { }

With this explicitly-written concept map template, the ``copy()`` call
provided with pointers to ``int`` will select the optimized ``copy()``
#2. 

A Type-Safety Problem with Explicit Refinement
==============================================

Rewrite this stupid section by separating the copy() algorithms into
different headers, then pointing out the PTO violation.


The Principle of Type-Safe Overloading says that each overload should
be type-safe in isolation, without the presence of other
overloads. The first of our ``copy()`` algorithms (#1) clearly follows
this principle: the algorithm is safe for any applicable
``InputIterator``/``OutputIterator`` combination, because every
``ContiguousIterator`` is also an ``InputIterator``. In the worst
case, if ``copy()`` #2 is not present, we will have sub-optimal copy
performance from contiguous memory regions to other contiguous memory
regions.

The second, optimized  ``copy()`` algorithm (#2) violates the PTO when
the concepts hierarchy makes use of explicit refinement. When
``copy()`` #2 is called with ``deque<int>`` iterators, the iterators
syntactically match the ``ContiguousIterator``concept and, therefore,
we get a run-time failure when the algorithm tries to ``memmove`` the
contents of the 


-----

.. N2906: Bjarne Stroustrup, *Simplifying the use of concepts *, ISO C++ committee document N2906=09-0096, June, 2009.

.. N2831: David Abrahams and Doug Gregor, *Fixing a Safety Problem with Rvalue References: Proposed Wording*, ISO C++ committee document N2831=09-0021, December, 2008.