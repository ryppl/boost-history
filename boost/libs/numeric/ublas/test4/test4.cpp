#ifdef USE_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>

#ifdef NUMERICS_PRIVATE
#include "../config.h"
#include "../vector.h"
#include "../matrix.h"
#include "../banded.h"
#include "../io.h"
#else
#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/matrix.h>
#include <boost/numeric/ublas/banded.h>
#include <boost/numeric/ublas/io.h>
#endif

#include "test4.h"

int main () {
    test_vector ();
    test_matrix_vector ();
    test_matrix ();
    return 0;
}

