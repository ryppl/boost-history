
BOOST OVERLOAD    (updated 2007/10/28)

=================================================================
Copyright (c) 2007 Marco Cecchetti

Use, modification and distribution of this library is 
subject to the Boost Software License, Version 1.0 .
(See accompanying file LICENSE_1_0.txt or 
copy at http://www.boost.org/LICENSE_1_0.txt)
=================================================================

=================================================================
WARNING 
this library is a proof of concept implementation only,
and it comes without ABSOLUTELY ANY WARRANTY !
=================================================================

WARNING: at present Boost Overload IS NOT a Boost library !
===========================================================

=================================================================
Please report any bugs, compiler issues, comment or suggestion
to mrcekets@gmail.com
=================================================================

Library Description
===================
Boost Overload behaves as an overloaded Boost Function. 
Actually it wraps multiple Boost Function objects of different 
signatures.

Library Documentation
=====================
http://docs.google.com/View?docid=dhd386mp_16gxsfxn

Library Tutorial
================
http://docs.google.com/Doc?id=dfxvjncs_1x6456m


**************************************
* version 0.3.0 (overload-0.3.0.zip) *
**************************************

tested with boost version: 1.33.1, 1.34.1

Compiler support table  
======================

compiler             OS                test
-------------------------------------------
gcc 4.1.2      under Linux             pass
intel 9.1      under Linux             pass
mingw 3.4.2    under Windows           pass
mingw 3.2.3    under Windows           fail
MS VC++ 8.0    under Windows           pass
MS VC++ 7.1    under Windows           partial*

*all test passed but signature deduction of multi-signature function objects
you have to use the signature based syntax with them.

what's new
===========

- added partial support for MSVC 7.1, all work but signature deduction of
  multi-signature function objects, you have to use the signature based syntax
  in order to set multi-signature function objects as callable targets;

- you can set a free function overload or a member function
  overload as callable target through the signature based syntax
  for more info give a look to the new paragraph in the tuturial;
  the same applies to template free functions and template member functions;

- added two new helper methods
  - the empty_all method that tell us if all callable targets are 
    empty;
  - the clear_all method that clears all the callable targets;

- in the folder libs/overload/example you can find all the examples
  presented in the tutorial;

- fixed some minor issue.


**************************************
* version 0.2.2 (overload-0.2.2.zip) *
**************************************

tested with boost version: 1.33.1, 1.34.1

Compiler support table 
======================

compiler             OS                test
-------------------------------------------
gcc 4.1.2      under Linux             pass
intel 9.1      under Linux             pass
mingw 3.4.2    under Windows           pass
mingw 3.2.3    under Windows           fail
MS VC++ 8.0    under Windows           pass
MS VC++ 7.1    under Windows           fail


description
===========
I went on working on the second implementation only.
The source code has gone through refactoring and modularization.
The signature deduction policy used by the "set" method is changed in the 
part concerned with multi-signature function objects.
This is a complete and detailed description of the signature deduction policy:

Let be "ov" an instantiation of the template class overload for
a given set of signatures "ss" used as template arguments:
- if the new candidate callable target "f" is a free function or 
  a member function then:
  - if "f" is not overloaded and its signature is included in "ss"
    then the new target (i.e. "f") replaces the target tied 
    to the matching signature;
  - if the signature of "f" is not included in "ss" a compile time
    error is produced;
  - if "f" is overloaded an "unresolved overloaded function type"
    compile time error is produced; (we can't rely on the Mesonides'
    trick, or equivalent for free functions, because they require
    to know the function name in advance); this is the same behaviour
    followed by boost.function;
  note: when I say that a function member signature is included in
        "ss" I actually mean that a const or non-const explicit signature
        of the given member function is included in "ss";
- if the new candidate callable target "f" is a function object (possibly a 
  multi-signature one ) then:
  - let be "rs" the set of signatures that are defined by its overload
    resolution set (or it would be better to say "by the overload
    resolution set of f::operator()" ); note that for a function
    object with a template operator() "rs" is an infinite set;
  - if the intersection between "rs" and "ss" is empty the assignment
    fails and a compile time error is produced;
  - if the intersection between "rs" and "ss" is not empty then the new
    target (i.e. "f") replaces all the targets related to the
    signatures included in the intersection.


**********************************
* version 0.1 (overload-0.1.zip) *
**********************************

tested on gcc 4.1.2 under Linux

description
===========
The archive includes two implementations.

- in the first implementation, I deduce function object signature 
by exploiting template function argument deduction; 
if the signature is not discovered no error is reported, 
and error handling is demanded to boost::function at call time; 
signature deduction of multi-signature function objects cause 
an "unresolved overloaded function type" compile error;

- in the second implementation, if the signature is not deduced 
a compile time error is produced; 
signature deduction of multi-signature function objects 
is successfull if it exists one and only one signature registered 
in the overload that matches with the function object, else 
an ad-hoc compile time error, pointing out the ambiguity, 
is produced.
