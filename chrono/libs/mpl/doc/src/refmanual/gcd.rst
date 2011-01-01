.. Metafunctions/Arithmetic Operations//gcd |90

gcd
====

Synopsis
--------

.. parsed-literal::
    
    template<
          typename T1
        , typename T2
        , typename T3 = |unspecified|
        |...|
        , typename T\ *n* = |unspecified|
        >
    struct gcd
    {
        typedef |unspecified| type;
    };



Description
-----------

Returns the greatest common divisor (gcd) of its arguments.


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

+---------------+---------------------------+-----------------------------------------------+
| Parameter     | Requirement               | Description                                   |
+===============+===========================+===============================================+
| |T1...Tn|     | |Integral Constant|       | Operation's arguments.                        |
+---------------+---------------------------+-----------------------------------------------+

|Note:| |numeric metafunction note| |-- end note|


Expression semantics
--------------------

For any |Integral Constant|\ s |c1...cn|:


.. parsed-literal::

    typedef gcd<c1,\ |...|\ c\ *n*\>::type r; 

:Return type:
    |Integral Constant|.

:Semantics:
    Equivalent to 
        
    .. parsed-literal::
    
        typedef 
            if_c<c1::value==0, c2,
            if_c<c2::value==0, c1,
                gcd<c2, integral_c<
                    typeof(c1::value % c2::value)
                ,   ( c1::value % c2::value )>
                >
            > > c;
            
        typedef gcd<c,c3,\ |...|\c\ *n*\>::type r; 

.. ..........................................................................

.. parsed-literal::

    typedef gcd<c1,\ |...|\ c\ *n*\> r;

:Return type:
    |Integral Constant|.

:Semantics:
    Equivalent to
    
    .. parsed-literal::

        struct r : gcd<c1,\ |...|\ c\ *n*\>::type {};




Complexity
----------

Amortized constant time.


Example
-------

.. parsed-literal::
    
    typedef gcd< int_<10>, int_<4>, long_<6> >::type r;
    BOOST_MPL_ASSERT_RELATION( r::value, ==, 2 );
    BOOST_MPL_ASSERT(( is_same< r::value_type, long > ));


See also
--------

|Arithmetic Operations|, |Numeric Metafunction|, |numeric_cast|, |divides|, |lcm|, |times|


.. copyright:: Copyright ©  2010 Vicente J. Botet Escriba
   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
