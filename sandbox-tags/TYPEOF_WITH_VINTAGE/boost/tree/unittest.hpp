//  Boost-sandbox simple unittest header file  ----------------------
//  Purpose: only test of facilities in ntree.hpp

//  (C) Copyright Marc Cromme 2002, marc@cromme.dk. Permission to
//  copy, use, modify, sell and distribute this software is granted
//  provided this copyright notice appears in all copies. This
//  software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

#ifndef UNITTEST_HPP
#define UNITTEST_HPP


//#include <string>
#include <limits>


/* --- UnitTest class ------------------------------------------------------ */

/**
 * Used for keeping track of unittests, and to generate unittest reports.
 */
class UnitTest 
{
  public:
    enum resulttype  {OK, FAILURE, EXCEPTION};

  private:
    /// Counter for succeeded unit tests.
    static long unsigned int countOk;            
    /// Counter for failed unit tests.
    static long unsigned int countFailure;       
    /// Counter for unit tests that failed with an exception.
    static long unsigned int countException;     

  public:
    static void reset(void);
    static void addresult(resulttype result);
    static void print(char* fileName, long int lineNumber, char* exprstring,
		      char* teststring, resulttype result);
    static bool report(void);
};

/* --- almost equal function ----------------------------------------------- */
template <class C>
bool almost(C v1, C v2, C factor = 10){
  C diff = 0<(v1 - v2) ? (v1 -v2) : (v2 - v1);
  C sum = 0<(v1 + v2) ? (v1 + v2) : -(v1 + v2);
  if (sum < diff) return false;
  if (diff <= std::numeric_limits<C>::epsilon()*factor*sum) return true;
  return false;
}



/* --- Macros for unit testing --------------------------------------------- */

#ifndef __STRING
#define __STRING(expression) #expression
#endif

#undef UNITTEST

/**
 * Macro used for unit testing.
 */
#define UNITTEST(teststring,expression)        \
{                                              \
 UnitTest::resulttype result;                  \
 try {                                         \
   if(expression)                              \
      result = UnitTest::OK;                   \
   else                                        \
      result = UnitTest::FAILURE;              \
 }                                             \
 catch (...) { result = UnitTest::EXCEPTION ;} \
 UnitTest::addresult(result);                  \
 UnitTest::print( __FILE__, __LINE__, __STRING(expression),teststring,result); \
} 

#endif // UNITTEST_HPP










