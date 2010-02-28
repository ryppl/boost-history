//////////////////////////////////////////////////////////////////////////////
// test::speed_common.h                                                     //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_ASSIGN_TEST_SPEED_COMMON_CSV_ER_2010_H
#define LIBS_ASSIGN_TEST_SPEED_COMMON_CSV_ER_2010_H
#include <vector>
#include <string>

double uniform_deviate ( int seed );
int rand(int M, int N);
char rand_letter();
std::string rand_str(int len);
std::vector<int> rand_vec();

#endif