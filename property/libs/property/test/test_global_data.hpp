#if !defined(TEST_GLOBAL_DATA_HPP)
#define TEST_GLOBAL_DATA_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"

extern char gld_char;
extern int gld_int;
extern double gld_double;
extern bool gld_bool;
extern int * gld_pointer;
extern test_enum gld_enum;
extern test_pod gld_pod;

extern char gld_char2;
extern int gld_int2;
extern double gld_double2;
extern bool gld_bool2;
extern int * gld_pointer2;
extern test_enum gld_enum2;
extern test_pod gld_pod2;

#endif
