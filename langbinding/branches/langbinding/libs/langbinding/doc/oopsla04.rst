++++++++++++++++++++++++++
 Boost.Langbinding
++++++++++++++++++++++++++

:Author: David Abrahams, Daniel Wallin
:Contact: dave@boost-consulting.com, dalwan01@student.umu.se
:organization: `Boost Consulting`_
:date: $Date$
:copyright: Copyright David Abrahams and Daniel Wallin 2004.
  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

.. _`Boost Consulting`: http://www.boost-consulting.com

.. contents:: index

.. sectnum::

:abstract: We describe a library for binding C++ to dynamic languages.

=========================
 Introduction
=========================

Boost.Langbinding is a generalization of earlier attempts at binding 
libraries, specifically Boost.Python and Luabind. The goals for this 
project include: 
 
* Language neutral binding of classes and functions. 
* Efficient wrapper code generation. 
* Language-specific pluggable back ends. 
* No preprocessing stage; everything done inside C++. 
 
To accomplish these goals the library uses template metaprogramming
to allow for simple declararative syntax while maintaining
efficiency and flexibility, and type-erasure to normalize multiple
plugins.

=========================
 User Interface
=========================

------------------------------
 Classes
------------------------------

------------------------------
 Overridable Virtual Functions
------------------------------

=========================
 Backend Interface
=========================

This section describes the interface used by authors of back-ends
for binding to specific languages.  A backend implements operations
such as conversions of data with certain primitive types between
the backend language and C++, the creation of classes and class
instances in the backend language, and 

-----------------
 Backend Plugins
-----------------

A backend plugin is a class that 

derived from a CRTP base class allows the library access to nested type information that
encapsulates language-specific resources.

-------------------------
 Registering Conversions
-------------------------

===========================
 Implementation Techniques
===========================






