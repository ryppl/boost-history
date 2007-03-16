//  Boost-sandbox simple unittest implementation file  ----------------------
//  Purpose: only test of facilities in ntree.hpp

//  (C) Copyright Marc Cromme 2002, marc@cromme.dk. Permission to
//  copy, use, modify, sell and distribute this software is granted
//  provided this copyright notice appears in all copies. This
//  software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.



#include "unittest.hpp"

/* === Extern declarations of unit test functions ========================== */

//extern void utest_array(void); 
//extern void utest_tree(void); 
extern void utest_mappediterator(void);
extern void utest_ntree(void); 
//extern void utest_linalg(void); 
//extern void utest_projective(void); 
//extern void utest_matrixutils(void); 

/* === Main function ======================================================= */

int main(void)
{
  // Reset
  UnitTest::reset();

  // Perform unit tests
  //utest_array(); 
  //utest_tree();
  utest_mappediterator();
  utest_ntree(); 
  //utest_linalg(); 
  //utest_projective(); 
  //utest_matrixutils(); 

  // Report summarized status of unit tests
  UnitTest::report();

  return 0;
}



