#ifdef USE_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>

#include "../config.h"
#include "../vector.h"
#include "../matrix.h"
#include "../io.h"

#include "test4.h"

int main () {
    test_vector ();
    test_matrix_vector ();
    test_matrix ();
    return 0;
}

