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

.. role:: concept
   :class: interpreted

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

This section describes the interface used by authors of back ends
for binding to specific languages.  A back end implements
operations such as conversions of data with certain primitive types
between the backend language and C++ and the creation of classes
and class instances in the backend language, and the management of
language-specific resources such as functions and data.

------------
 The Basics
------------

A module author creates modules in the target language by passing a
language-specific module-building object to a
``module_description``\ 's ``::bind`` member function.  For
example:

.. _basics:

::

  // front-end binding code
  module_description my_module =
  
     module("my_module")
     [
         def("f", &f)
     ];

  // Python module initialization function
  init_mymodule()
  {
      my_module.bind(python::build_module());
  }

The ``module_description`` is typically a namespace-scope object
with static storage duration, and is initialized with a ``module``
instance by front end binding code.  This initialization may occur
in a shared library, making it pluggable.  If a shared library is
not used, or if the system does not guarantee that shared library
initialization happens once and only once, additional measures may
be needed to avoid race conditions in multithreaded environments.

-----------------
 Module Builders
-----------------

Module builders must be instances of a class derived from a CRTP
base class::

  namespace python {

  class build_module
    : public backend::module_builder< module_creator > 
  {
      friend class backend::module_builder_access;
      ...
  };

  }

The friend declaration allows the bulk of the ``build_module``
interface to be declared ``private``.

Each distinct parameter to module_visitor is associated with a
unique backend ID, so each backend should only declare one
``build_module``, or at least one such class at the root of an
inheritance hierarchy.  

.. I see no reason to do this.  The ``xxx_fn`` (now
   ``backend::function`` object) can just be cheaply copyable.  Use
   reference counting if neccessary.  We'll pass it to the visitor,
   and the visitor will store it in the XXX function.

   The backend ID is accessible through
   ``backend::module_builder``\ 's ``::backend_id()`` member
   function.  Backend authors will only need to use this interface in
   `one place`__.

   __ `Function Creation`_

:concept:`Module Builders` use a visitation interface to explore
the ``module_description`` passed to them.  The visited items
represent entities such as functions and classes.  For each visited
item ``v``, the following expressions are valid::

  std::string name(v.name());
  std::map<std::string,boost::any> const& attributes = v.attributes();

The item's attributes are used to hold information such as
documentation strings.  An agreed-upon naming and type protocol
for holding attributes commonly-needed across target languages
will be established.

Expressions described in the following sections are required to be
valid for :concept:`Module Builder` type ``B`` and instance ``b``,
with the access rights of ``backend::module_builder_access``.
``std::string``

Module Creation
===============

::

  b.visit(backend::module const& m);
  b.leave(backend::module const& m);

Function Creation
=================

::

  b.visit(backend::function const& f);                     
  b.leave(backend::function const& f);

This interface is used both for functions bound at module scope and
for member functions bound within classes.  Functions visited while
a class is being visited should be treated as member functions.

Typically, upon visiting a function the :concept:`Module Builder`
will want to create a new object that is callable in its target
language and that, when called, invokes ``f`` by passing an object
of type ``B::argument_package``, yielding an object of type
``B::function_result``.

.. Likewise, no need for this either.

   The lifetime of the ``backend::function`` object is guaranteed to
   be at least that of the front-end ``module`` object (not
   ``backend::module`` but the object bound to the
   ``module_description&`` shown `here`__).

   __ basics_


Responsibilities of the backend:

* Build objects that represent classes and functions in the dynamic
  language and that can hold the library's representations of
  classes and functions, to which the dynamic language's
  operations are dispatched.

* Provide a type that represents an argument package.  For Python
  this might be a pair of PyObject*s representing positional and
  keyword arguments.

* Provide a function that, given an argument package, can
  determine whether a given argument is 

* Provide types that manage language resources such as classes,
  instances, and function overrides.

* define wrappers for C++ classes and functions 
* given an argument package and an arg index, find out if that argument is a 
* provide a type representing an argument package
* define a visitor that translates the registrations to the target language 
* register built-in converters 
* create some function that can create instances of wrapped classes using the holder_installers and class_*.. I guess that's part of (1)
* provide a type that represents a virtual function override in the target language
* provide a type that represents a function call result in the target language


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






