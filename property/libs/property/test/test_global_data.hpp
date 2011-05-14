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

#endif
