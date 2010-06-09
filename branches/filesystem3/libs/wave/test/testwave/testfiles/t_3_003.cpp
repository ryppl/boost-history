/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    http://www.boost.org/

    Copyright (c) 2001-2010 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

//O --variadics

// Tests predefined macros

//R #line 15 "t_3_003.cpp"
__STDC__                    //R 1 
__STDC_VERSION__            //R __STDC_VERSION__ 
__cplusplus                 //R 199711L 
__STDC_HOSTED__             //R __STDC_HOSTED__ 
__LINE__                    //R 19 
__FILE__                    //R "$P" 
__BASE_FILE__               //R "$F" 
__WAVE_HAS_VARIADICS__      //R 1 
__INCLUDE_LEVEL__           //R 0 
//R #line 50 "test.cpp"
#line 50 "test.cpp"
__LINE__                    //R 50 
__FILE__                    //R "test.cpp" 
__BASE_FILE__               //R "$F" 


//R #line 56 "test.cpp"
__LINE__                    //R 56 
__FILE__                    //R "test.cpp" 
__BASE_FILE__               //R "$F" 

//H 01: <built-in>(1): __STDC__
//H 02: 1
//H 03: 1
//H 01: <built-in>(1): __cplusplus
//H 02: 199711L
//H 03: 199711L
//H 01: <built-in>(1): __BASE_FILE__
//H 02: "$F"
//H 03: "$F"
//H 01: <built-in>(1): __WAVE_HAS_VARIADICS__
//H 02: 1
//H 03: 1
//H 10: t_3_003.cpp(25): #line
//H 17: 50 "test.cpp" (50, "test.cpp")
//H 01: <built-in>(1): __BASE_FILE__
//H 02: "$F"
//H 03: "$F"
//H 01: <built-in>(1): __BASE_FILE__
//H 02: "$F"
//H 03: "$F"
