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

:Abstract: We describe a library for binding C++ to dynamic languages.

.. _`Boost Consulting`: http://www.boost-consulting.com

.. contents:: index

.. sectnum::

.. role:: concept
   :class: interpreted

.. |copy| unicode:: 0xA9 .. copyright sign

=========================
 Introduction
=========================

Boost.Langbinding is a generalization of earlier attempts at binding 
libraries, specifically Boost.Python [#]_ and Luabind [#]_.

This library is spawned from the realization that the language specific
code, which has previously been intertwined with the template generated
wrappers, can be factored out into a separately compiled library. This
means it is possible to define bindings that are agnostic to the
target language, and later expose them without any need for recompilation.
 
The goals for this project include:

* Language neutral binding of classes and functions. 
* Efficient wrapper code generation. 
* Language-specific pluggable back ends. 
* No preprocessing stage; everything done inside C++. 
 
To accomplish these goals the library uses template metaprogramming
to allow for simple declararative syntax while maintaining
efficiency and flexibility, and type-erasure to normalize multiple
plugins.

.. [#] Boost.Python, Copyright |copy| David Abrahams, http://www.boost.org/libs/python/
.. [#] Luabind, Boost.Python derivative for Lua. Copyright |copy| Daniel Wallin 
       and Arvid Norberg, http://luabind.sf.net/

=========================
 User Interface
=========================

Here we describe the interface authors that are adding a language binding to 
their library or application use. This includes support for modules, 
namespaces, classes, member functions, free functions and operators.

Where example code for a target language is neccessary, code for both Python
and Lua is given.

Every entity is derived from ``module_description``, which is the abstract
representation that is passed to the target language for binding.
``module_description`` and it's derivatives are handle classes with deep copy
value semantics.

------------------------------
 Modules
------------------------------

The ``module`` is the top level entity in a registration. Entities are
added to the module using ``operator[]``.

.. topic:: Synopsis

 .. parsed-literal::

    class **module** : public module_description
    {
    public:
        module()
        module(char const* name)
        ...
        module& operator[](module_description const&);
    };

------------------------------
 Functions
------------------------------

Functions are exposing using ``def``.

.. topic:: Synopsis

 .. parsed-literal::

    class **def** : public module_description
    {
    public:
        template<class Fn>
        def(char const* name, Fn fn);

        template<class Fn, class CallPolicies>
        def(char const* name, Fn fn, CallPolicies);

        template<class Fn, class CallPolicies, class SignatureTransformation>
        def(char const* name, Fn fn, CallPolicies, SignatureTransformation);
    };

 * ``fn`` is a function or member function pointer.
 * ``CallPolicies`` and ``SignatureTransformation`` may be given in any
   order.

If a function named ``name`` has already been exposed, an overload is added to
that function object.

.. parsed-literal::

    int timestwo(int x) { return x * 2; }

    ..

    def("timestwo", &timestwo)

.. note:: Signatures
    
    Function and member function pointers are treated the same by the library.
    A member function::

        R(X::*)(A0, .., AN)

    Is treated as::

        R(X&, A0, ..., AN)

    Because of this, member functions can be used as free functions with an
    additional first argument, and free functions can be exposed as class member
    functions.    

.. parsed-literal::

    struct X
    {
        void f();
    };

    def("f", &X::f)

Will register a unary function that expects an ``X&`` as it's parameter.

Parameters
==========

Parameters of all primitive types are automatically handled. Class types need to
be registered. 

For class types, ``derived->base`` conversions are handled. In the case of a
polymorphic type, ``base->derived`` conversions are also considered, based on
the dynamic type of the parameter.

Return Values
=============

Again, primitive types are automatically converted and class types need to be 
registered.

Class types can be returned by value, or held in a smart pointer. If returned
by value, the object will be copied into a new instance. If a smart pointer is
returned, the smart pointer will be copied and held in a new instance.

The return type is considered to be a smart pointer **iff** it has an overload of
``get_pointer()`` that returns a raw pointer to a class type. In other words, 
given that ``x`` is a smart pointer::

    *get_pointer(x)

Must be well formed, and the type of that expression is considered the pointee
type that is converted to the target language.

When references or pointers are returned, an ownership strategy need to be
explicitly specified. This is to prevent dangling references and leaked
objects. The ownership strategy is specified as the call policy parameter in 
the ``def()`` call.

.. parsed-literal::

    def("f", &f, adopt(result)) // Manage the ownership over the returned pointer.
    def("f", &f, reference_existing(result)) // Reference an existing object.
    def("f", &f, internal_reference(result, _1)) // Returns a reference to something inside
                                                 // the object given as parameter one, make sure
                                                 // that object doesn't disappear leaving
                                                 // a dangling reference.

Notice how placeholders are used to indicate which elements are involved.

..  Signature Transformations
    =========================

    .. parsed-literal::

        def("f", &f, signature(_1, _1))
        def("f", &f, signature(_1[_1], _1[_2]))

------------------------------
 Classes
------------------------------

Classes are exposed using ``class_``.

.. topic:: Synopsis

 .. parsed-literal::

    template<class TAndBases, class HolderType = */\* implementation defined \*/*>
    class **class_** : public module_description
    {
    public:
        class\_(char const* name);

        template<class Fn>
        class\_& def(char const\* name, Fn fn);

        template<class Fn, class CallPolicies>
        class\_& def(char const* name, Fn fn, CallPolicies);

        template<class Fn, class CallPolicies, class SignatureTransformation>
        class\_& def(char const* name, Fn fn, CallPolicies, SignatureTransformation);

        class\_& scope(module_description const&);
    };

 TAndBases
    This is the class type being exposed. Inheritance relationships are indicated
    using a function type: ``Derived(Base)``.

 HolderType
    This is the pointer type used to hold instances of ``T``. For example if
    ``HolderType ::= std::auto_ptr<T>``, all new instances created in the target
    language will be held by an ``auto_ptr<T>``. Defaults to an owning pointer.

Constructors
============

.. topic:: Init Synopsis

 .. parsed-literal::

    template<class A0 = */\* implementation defined \*/*, ..., class AN = */\* implementation defined \*/*>
    struct init
    {
        init();
        ...
    };

 The template parameters ``A0`` .. ``AN`` indicate the positional constructor arguments.

Exposing constructors is done by calling ``def()``, passing an instance of ``init<>``
with the desired constructor signature:

.. parsed-literal::

    class_<X>("X")
        .def(**init<>()**)
        .def(**init<int, int>()**)

Creates a wrapper for the class type ``X``, with a default constructor and a
constructor taking two ``int`` parameters.


Member Functions
================

Member functions are exposed using one of the ``class_<>::def()`` overloads.
The parameters are exactly the same as with the global ``def()`` described
in the previous section.

For example::

    class_<X>("X")
        .def("f", &f)

Will expose the class ``X`` with a single member function ``f``.


Holder Types
============

Sometimes an interface passes instances of a class managed by smart pointers.
In these cases it is important to be able to pass instances created in the
target language environment to functions expecting a smart pointer. ::

    void f(boost::shared_ptr<X> const&);

To handle this we specify that our class instances is to be held with 
``boost::shared_ptr<X>``::

    class_<X, boost::shared_ptr<X> >("X")

Now instances of ``X`` created in the target language can be safely passed to functions
that expects a ``boost::shared_ptr``.


Inheritance
===========

To indicate inheritance relationships the function type syntax is used. It
was choosen to emulate the Python class declaration syntax.

For example::

    class_<Derived(Base)>("Derived")

Multiple inheritance is exposed by simply adding more argument types to
the function type::

    class_<Derived(Base1, Base2)>("Derived")

This will register the relationships in a cast-graph, with ``derived->base``, 
and possibly ``base->derived`` conversions (if the registered class is 
polymorphic). The derived class will also automatically inherit any registered
member functions from it's base.

..  Inheritance2
    ============

     and control how the class instances is held within
    the target language. This is discussed in greater depth in Inheritance_ and
    HolderTypes. For polymorphic types it is possible, with a little extra effort,
    to expose virtual functions to the target language, where they can be called
    and overridden. This is discussed in `Overridable Virtual Functions`_.

------------------------------
 Overridable Virtual Functions
------------------------------

To be able to expose overridable virtual functions for a class ``T`` without
being intrusive on the exposed class, we need to define a wrapper-class. This
class must derive from ``polymorphic<T>`` and implement virtual dispatch
overrides, as well as default implementation functions for every virtual
function.

A typical wrapper-class for a class ``Base`` will look something like this:

.. parsed-literal::

    struct Base
    {
        virtual int f() { return 0; }
    }

    struct BaseWrap : polymorphic<Base>
    {
        int f()
        {
            if (override f = this->find_override("f"))
                return f();
            else
                return Base::f();
        }

        int default_f()
        {
            return Base::f();
        }
    };

The virtual dispatch override looks if there is an override with the given
function name in the target language representation of the instance. If
there is one it is called using ``operator()``. If there is no overload,
the default implementation in Base is called instead.

``default_f`` is needed for when there is actually an override defined in
the target language, but we want to call the base class function statically
anyway. This happens when virtual overrides wants to call their base
implementation::

    class Derived(Base):
        def f():
            return Base.f(self) + 10

If not for ``default_f``, this would call the virtual function ending up in
an infinite loop.

To expose the class above and it's virtual function ``f``, we use ``class_`` 
like this::

    class_<BaseWrap>("Base")
        .def("f", &Base::f, &BaseWrap::default_f)

Now instances of derived classes defined in a target language can be passed
in place of an ``Base``. ::

    int g(Base& x)
    {
        return x.f();
    }

    ...

    def("g", &g);

**Python code:**

.. parsed-literal::

    class Derived(Base):
        def f():
            return 10

    g(Derived())    *Returns 10*

**Lua code:**

.. parsed-literal::

    class "Derived" (Base)
        function Derived:f()
            return 10
        end

    g(Derived())    *Returns 10*

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

.. |ModuleBuilder| replace:: :concept:`Module Builder`
.. |ModuleBuilders| replace:: :concept:`Module Builders`

|ModuleBuilders| use a visitation interface to explore
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
valid for |ModuleBuilder| type ``B`` and instance ``b``,
with the access rights of ``backend::module_builder_access``.

Module Creation
===============

::

  b.visit(backend::module const& m);
  b.leave(backend::module const& m);

Function Creation
=================

::

  b.visit(backend::function<B> const& f);
  b.leave(backend::function<B> const& f);

This interface is used both for functions bound at module scope and
for member functions bound within classes.  Functions visited while
a class is being visited should be treated as member functions.

Typically, upon visiting a function the |ModuleBuilder|
will want to create a new callable object (in its target language)
that, when called, invokes ``f`` ::

  B::function_result result = f(a, i);

where ``a`` is an object of type ``B::argument_package`` and ``i``
is an object of type ``B::interpreter``.

.. Likewise, no need for this either.

   The lifetime of the ``backend::function`` object is guaranteed to
   be at least that of the front-end ``module`` object (not
   ``backend::module`` but the object bound to the
   ``module_description&`` shown `here`__).

   __ basics_

Function Result
---------------

::

  typedef B::function_result R;

A function result is a copyable type representing the result of
calling a function in the target language.  In a Python binding
``R`` might be as simple as ``PyObject*``.  This type is also used
by C++ to target language data converters.

Argument Package
----------------

::

  typedef B::argument_package A;

This type represents the package of function arguments passed from
the target language.  For a Python binding it might be as simple
as ``Python* [2]``, representing a positional argument tuple and
keyword argument dictionary.  Argument packages need not be
copyable types.  This type will also be used by target language to
C++ data converters.

Interpreter
-----------

::

  typedef B::interpreter;

This can be any type.  Languages that support multiple simulateous
interpreter states may need to identify the interpreter when
creating new objects, as typically happens when converting C++
objects (like function return values) into the target language.
Typically the interpreter identification would be passed to the
``B``\ 's constructor and then stored in each target language
function object that it creates so it can be easily retrieved.
Languages that don't need this information can use ``int`` and
always pass zero.

Class Creation
==============

::

  b.visit(backend::class_ const& c);
  b.leave(backend::class_ const& c);

A unique integer id has been allocated to each class wrapped by the
front-end from the sequence of numbers starting with zero.  The id
can be accessed via::

  c.id()

The backend will typically want to create an appropriately-named
class object in the target module.  The integer id will be 
It should store a reference to
this class in an object of type ``B::class_weak_reference``. ::

  typedef B::class_reference C;

Instances of this type should maintain the lifetime of the created
class object, or if that's not possible, should be automatically
notified when the created class object is destroyed so that the
backend code can throw an appropriate exception if an attempt is
made to use the destroyed class.  If target language interpreters
can be destroyed and reconstituted (e.g. with ``PyFinalize``), it
may be neccessary for all ``C`` instances associated with a given
interpreter to explicitly release their reference to the created
class

can be destroyed ``C`` exhibit typical "weak
reference behavior;" that is, 

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


