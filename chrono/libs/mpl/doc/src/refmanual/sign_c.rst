.. Metafunctions/Arithmetic Operations//sign_c |110

sign_c
======

Synopsis
--------

.. parsed-literal::
    
    template<
          typename T
          T N
        >
    struct sign_c
    {
        typedef |unspecified| type;
    };



Description
-----------

Returns the sign of its argument and differs only from |sign| in the way the parameters are specified.


Header
------

.. parsed-literal::
    
    #include <boost/mpl/sign.hpp>
    #include <boost/mpl/arithmetic.hpp>


Model of
--------

|Numeric Metafunction|


Parameters
----------

+---------------+-------------------------------+----------------------------------+
| Parameter     | Requirement                   | Description                      |
+===============+===============================+==================================+
| ``T``         | An integral type              | Operation's argument value type. |
+---------------+-------------------------------+----------------------------------+
| ``N``         | An integral constant          | Operation's argument.            | 
+---------------+-------------------------------+----------------------------------+

|Note:| |numeric metafunction note| |-- end note|


Expression semantics
--------------------

For any Integral Type ``t`` and integral constant ``n`` of type ``t``:

.. parsed-literal::

    typedef sign_c<t,n>::type r; 

:Return type:
    |Integral Constant|.

:Semantics:
    Equivalent to 

    .. parsed-literal::
    
        typedef sign< integral_c<t,n> > r;

.. ..........................................................................

.. parsed-literal::

    typedef sign_c<t,n> r; 

:Return type:
    |Integral Constant|.

:Semantics:
    Equivalent to
    
    .. parsed-literal::

        struct r : sign_c<t,n>::type {};


Complexity
----------

Amortized constant time.


Example
-------

.. parsed-literal::
    
    typedef sign_c< int, -10 >::type r;
    BOOST_MPL_ASSERT_RELATION( r::value, ==, -1 );
    BOOST_MPL_ASSERT(( is_same< r::value_type, int > ));


See also
--------

|Arithmetic Operations|, |Numeric Metafunction|, |numeric_cast|, |sign|


.. copyright:: Copyright ©  2010 Vicente J. Botet Escriba
   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
