.. Metafunctions/Arithmetic Operations//gcd_c |130

gcd_c
=====

Synopsis
--------

.. parsed-literal::
    
    template<
          typename T
          T N1
        , T N2
        , T N3 = |unspecified|
        |...|
        , T N\ *n* = |unspecified|
        >
    struct gcd_c
    {
        typedef |unspecified| type;
    };



Description
-----------

Returns the greatest common divisor (gcd) of its arguments and differs only from |gcd| in the way the parameters are specified.


Header
------

.. parsed-literal::
    
    #include <boost/mpl/gcd.hpp>
    #include <boost/mpl/arithmetic.hpp>


Model of
--------

|Numeric Metafunction|



Parameters
----------

+---------------+-------------------------------+-----------------------------------+
| Parameter     | Requirement                   | Description                       |
+===============+===============================+===================================+
| ``T``         | An integral type              | Operation's arguments value type. |
+---------------+-------------------------------+-----------------------------------+
| ``N1...Nn``   | An integral constant          | Operation's arguments.            | 
+---------------+-------------------------------+-----------------------------------+

|Note:| |numeric metafunction note| |-- end note|


Expression semantics
--------------------

For any Integral Type ``t`` and integral constants |c1...cn| of type ``t``:


.. parsed-literal::

    typedef gcd_c<t, c1,\ |...|\ c\ *n*\>::type r; 

:Return type:
    |Integral Constant|.

:Semantics:
    Equivalent to 
        
    .. parsed-literal::
    
        typedef gcd<
                    integral_c<T,c1>, 
                    integral_c<T,c2>, 
                    ..., 
                    integral_c<T,cn>
            >::type r;
            
.. ..........................................................................

.. parsed-literal::

    typedef gcd_c<t, c1,\ |...|\ c\ *n*\>r; 

:Return type:
    |Integral Constant|.

:Semantics:
    Equivalent to
    
    .. parsed-literal::

        struct r : gcd_c<t, c1,\ |...|\ c\ *n*\>::type {};




Complexity
----------

Amortized constant time.


Example
-------

.. parsed-literal::
    
    typedef gcd_c<int, 10, 4, 6>::type r;
    BOOST_MPL_ASSERT_RELATION( r::value, ==, 2 );
    BOOST_MPL_ASSERT(( is_same< r::value_type, long > ));


See also
--------

|Arithmetic Operations|, |Numeric Metafunction|, |numeric_cast|, |gcd|


.. copyright:: Copyright ©  2010 Vicente J. Botet Escriba
   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
