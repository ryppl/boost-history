//  Boost-sandbox simple unittest implementation file  ----------------------
//  Purpose: only test of facilities in ntree.hpp

//  (C) Copyright Marc Cromme 2002, marc@cromme.dk. Permission to
//  copy, use, modify, sell and distribute this software is granted
//  provided this copyright notice appears in all copies. This
//  software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

#include "unittest.hpp"
#include <iostream>

/* --- Static variables of UnitTest class ---------------------------------- */

long unsigned int UnitTest::countOk = 0;
long unsigned int UnitTest::countFailure = 0;
long unsigned int UnitTest::countException = 0;


/* --- UnitTest functions -------------------------------------------------- */

/**
 * Resets internal(static) counters.
 */
void UnitTest::reset(void)
{
  countOk = 0;
  countFailure = 0; 
  countException = 0;
}
  
/**
 * Adds the result of a unittest to the internal(static) counters.
 */
void UnitTest::addresult(resulttype result)
{
  if 
    (result == OK) countOk++;
  else if 
    (result == FAILURE) countFailure++;
  else if 
    (result == EXCEPTION) countException++;
}

/**
 * Creates a report entry, containing the position and the test result.
 */
void UnitTest::print(char* fileName, long int lineNumber, char* exprstring,
		     char* teststring, resulttype result) 
{
  std::cout << fileName   << "\t" 
            << lineNumber << "\t" 
            << teststring << "\t" 
            << exprstring << " :   \t";
  if 
    (result == OK) std::cout << "OK" << std::endl;
  else if 
    (result == FAILURE) std::cout << "FAILURE" << std::endl;
  else if 
    (result == EXCEPTION) std::cout << "EXCEPTION" << std::endl;
}

/**
 * Prints the internal(static) counters using cout. 
 */
bool UnitTest::report(void) 
{
  std::cout << std::endl << "UNIT TEST RESULTS" << std::endl;
  std::cout << countOk + countFailure + countException << " TOTAL"
	    << std::endl;
  std::cout << countOk << " OK" << std::endl;
  std::cout << countFailure << " FAILURE " << std::endl;
  std::cout << countException << " EXCEPTION " << std::endl; 
  bool passed = ((countFailure == 0) && (countException == 0));
  return passed;  
}









