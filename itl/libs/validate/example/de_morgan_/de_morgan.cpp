/*-----------------------------------------------------------------------------+
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

/** Example de_morgan.cpp \file de_morgan.cpp

Example de_morgan.cpp demonstrates some tests of 
<a href="http://en.wikipedia.org/wiki/De_Morgan%27s_laws">
De Morgan's law</a>
on interval_set.
This law is selected, because we can show some interesting semantical 
distinctions that reveal themselves in different instantiations of the law.

We do not use De Morgan's laws in it's original form
\code
~(b + c) == ~b & ~c ;
~(b & c) == ~b + ~c ;
// +: union  &: intersection  ~: complement
\endcode

but in an incarnation that uses the set difference or <b>relative complement</b>
instead of the unary complement operator.

\code
a - (b + c) == (a - b) & (a - c) ;
a - (b & c) == (a - b) + (a - c) ;
// +: union  &: intersection  -: relative complement
\endcode

As you can see there are two variants for the law depending on the placement 
of the operators + and &. So we can define the operations +, & and also the 
equality operation == as parameters of the law.

\code
// Shorthand pseudocode notation for a Law template
RelativeDeMorgan<S,o,$,=v= >: S a,b,c; a - (b o c) =v= (a - b) $ (a - c) ;
// o : First operator, $ : Second operator, =v= : An equality relation
\endcode

In this example, we show how to test De Morgan's law for different kinds
of interval_sets. Some laws will have violations because they do not hold.
We will then use the law template with a different equality operation
to achive a valid form of the law.

\include de_morgan_/de_morgan.cpp

The output of this example program informs about sucessful law validations
or validation failure. In case of failure the program presents the simplest of
all the violation that have been detected. So the first validation of 
De Morgan's law is reported here:

\code
===== validater 1 ====================================
Validation of DeMorgan<itv_set<int>,&=,+=,==>. Start
Stop. Time elapsed: 00:00:00.193000
Law successfully validated
\endcode

For pragmatic reasons we use abbreviations so
<tt>DeMorgan<itv_set<int>,&=,+=,==></tt> stands for
<tt>InplaceDeMorgan<interval_set<int>,&=,+=,==></tt> 

For the 1000 test cases no violations of the law were found.

Validater 2 uses the same law and operations on a different
type \c split_interval_set (abbreviated \c sp_itv_set). Here LaBatea
found 379 violations in 1000 generated test cases.

\code
===== validater 2 ====================================
Validation of DeMorgan<sp_itv_set<int>,&=,+=,==>. Start
379 violations
\endcode

The program informs about the type of law and it's 
formula. The tuple of input variables
<tt>(a,b,c) = ({[0,7)}, {[3,4)}, {})</tt>
and the output variables. In our case the left hand side
and the right hand side of the equation.
<tt>(lhs,rhs) = ({[0,7)}, {[0,3)[3,4)[4,7)})</tt>

\code
Violation of: DeMorgan<sp_itv_set<int>,&=,+=,==>
a - (b o c) == (a - b) $ (a - c) 'inplace'
({[0,7)}, {[3,4)}, {})
({[0,7)}, {[0,3)[3,4)[4,7)})
\endcode

This is the minimal report that is always given by LaBatea
for detected law violations. Next it enters a function
called <tt>debug_holds</tt> which is the debug version of a 
boolean function \c holds(). Function <tt>debug_holds</tt> 
can be coded in a way that supports the debugging of 
functions called by the law and provides readable output
for law violations which is done here:

\code
--- function debug_holds -----------------------------
          a = {[0,7)}
          b = {[3,4)}
          c = {}
lhs:
    (b o c) = {}
a - (b o c) = {[0,7)}
rhs:
      a - b = {[0,3)[4,7)}
      a - c = {[0,7)}
(a-b)$(a-c) = {[0,3)[3,4)[4,7)}
------------------------------------------------------
Stop. Time elapsed: 00:00:00.323000
\endcode

You can see from the output that the left and the right hand side of the
laws evaluation show the same elements but these elements are partitioned
in different segmentations. So the lexicographical equality on segments
which is computed by \c operator== evaluates to false.

Finally we use the law with the equality relation \c element_equal,
that checks the equality of elements but abstracts from the segmentation.
Note that \c element_equal is represented by a single \c '=' in the output.

\code
===== validater 3 ====================================
Validation of DeMorgan<sp_itv_set<int>,&=,+=,=>. Start
Stop. Time elapsed: 00:00:00.205000
Law successfully validated
\endcode

*/

#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/gentor/gentorprofile.hpp>

#include <boost/validate/laws/set_laws.hpp>
#include <boost/itl/interval_set.hpp>
#include <boost/itl/split_interval_map.hpp>

using namespace std;
using namespace boost;
using namespace boost::itl;
using namespace boost::posix_time;


template<class LawT, template<typename>class Generator>
void test_law(LawValidater<LawT, Generator>& validater)
{
    // You can set parameters for the generator that generates test data.
    // set_std_profile allows to set size parameter that determines the
    // container size and the ranges of generated values.
    int data_size = 8;
    GentorProfileSgl::it()->set_std_profile(data_size);

    // Here we set the number of law instances that we want
    // to be tested.
    int test_count = 1000;
    validater.set_trials_count(test_count);

    ptime start, stop;
    cout << "Validation of " << LawT().typeString();
    cout << ". Start\n";
    start = ptime(microsec_clock::local_time());
    validater.run();
    stop = ptime(microsec_clock::local_time());
    cout << "Stop. Time elapsed: " << stop - start << endl;

    if(validater.violation_count() == 0)
        cout << "Law successfully validated\n";
}

//------------------------------------------------------------------------------
// Test single laws using this
void test_LawValidater()
{
    // Here we instantiate the law InplaceDeMorgan for a test on
    // interval_set<int>
    // First  operation : inplace_et   implements intersection
    // Second operation : inplace_plus implements union
    // Equality relation: std_equal    implements segmental equality (default)

    LawValidater<InplaceDeMorgan<interval_set<int>, inplace_et, inplace_plus >, 
                 RandomGentor> validater1;
    cout << "===== validater 1 ====================================\n";
    test_law(validater1);
    // Ok the law was successfully validated.

    // Next we check the same law for split_interval_sets
    LawValidater<InplaceDeMorgan<split_interval_set<int>, inplace_et, inplace_plus >, 
                 RandomGentor> validater2;
    cout << "\n===== validater 2 ====================================\n";
    test_law(validater2);
    // The law does not hold for split_interval_sets: There were violations.
    
    // Checking the smallest of all violations, that is presented by the
    // validater in the output, we can see that the left and right hand
    // side of the equation show interval sets of equal elements but with
    // a different segmentation, a different sequence of intervals.


    // Using element equality that abstracts from the segmentation ...
    LawValidater<InplaceDeMorgan<split_interval_set<int>, inplace_et, inplace_plus, 
                                 inplace_minus, element_equal>, 
                 RandomGentor> validater3;
    cout << "\n===== validater 3 ====================================\n";
    test_law(validater3);
    // finally leads to an instantiation of the law that holds for
    // split_interval_set.
}

int main()
{
    cout << ">> A Law Based Test Atomaton (LaBatea): Sample de_morgan.cpp <<\n";
    cout << "---------------------------------------------------------------\n";
    test_LawValidater();
    return 0;
}

// Program output:
/*-------------------------------------------------------------
>> A Law Based Test Atomaton (LaBatea): Sample de_morgan.cpp <<
---------------------------------------------------------------
===== validater 1 ====================================
Validation of DeMorgan<itv_set<int>,&=,+=,==>. Start
Stop. Time elapsed: 00:00:00.193000
Law successfully validated

===== validater 2 ====================================
Validation of DeMorgan<sp_itv_set<int>,&=,+=,==>. Start
379 violations
Violation of: DeMorgan<sp_itv_set<int>,&=,+=,==>
a - (b o c) == (a - b) $ (a - c) 'inplace'
({[0,7)}, {[3,4)}, {})
({[0,7)}, {[0,3)[3,4)[4,7)})
--- function debug_holds -----------------------------
          a = {[0,7)}
          b = {[3,4)}
          c = {}
lhs:
    (b o c) = {}
a - (b o c) = {[0,7)}
rhs:
      a - b = {[0,3)[4,7)}
      a - c = {[0,7)}
(a-b)$(a-c) = {[0,3)[3,4)[4,7)}
------------------------------------------------------
Stop. Time elapsed: 00:00:00.323000

===== validater 3 ====================================
Validation of DeMorgan<sp_itv_set<int>,&=,+=,=>. Start
Stop. Time elapsed: 00:00:00.205000
Law successfully validated
-------------------------------------------------------------*/

