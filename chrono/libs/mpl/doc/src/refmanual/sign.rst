.. Metafunctions/Arithmetic Operations//sign |80

sign
====

Synopsis
--------

.. parsed-literal::
    
    template<
          typename T
        >
    struct sign
    {
        typedef |unspecified| type;
    };



Description
-----------

Returns the sign of its argument.


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

+---------------+---------------------------+-----------------------------------------------+
| Parameter     | Requirement               | Description                                   |
+===============+===========================+===============================================+
| ``T``         | |Integral Constant|       | Operation's argument.                         |
+---------------+---------------------------+-----------------------------------------------+

|Note:| |numeric metafunction note| |-- end note|


Expression semantics
--------------------

For any |Integral Constant| ``c``:

.. parsed-literal::

    typedef sign<c>::type r; 

:Return type:
    |Integral Constant|.

:Semantics:
    Equivalent to 

    .. parsed-literal::
    
        typedef integral_c< c::value_type, ( (c::value<0)?-1:(0<c::value)?1:0 ) > r;

.. ..........................................................................

.. parsed-literal::

    typedef sign<c> r; 

:Return type:
    |Integral Constant|.

:Semantics:
    Equivalent to
    
    .. parsed-literal::

        struct r : sign<c>::type {};


Complexity
----------

Amortized constant time.


Example
-------

.. parsed-literal::
    
    typedef sign< int_<-10> >::type r;
    BOOST_MPL_ASSERT_RELATION( r::value, ==, -1 );
    BOOST_MPL_ASSERT(( is_same< r::value_type, int > ));


See also
--------

|Arithmetic Operations|, |Numeric Metafunction|, |numeric_cast|, |plus|, |minus|, |times|


.. copyright:: Copyright ©  2010 Vicente J. Botet Escriba
   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
