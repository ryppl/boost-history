#include "test_global_data.hpp"

char gld_char(' ');
int gld_int(0);
double gld_double(0.0);
bool gld_bool(false);
int * gld_pointer(0);
test_enum gld_enum(e_test_default);
test_pod gld_pod;

char gld_char2('0');
int gld_int2(1000);
double gld_double2(3.1416);
bool gld_bool2(true);
int * gld_pointer2(0);
test_enum gld_enum2(e_test_third);
test_pod gld_pod2((test_pod('*',565,2.045,true)));
