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
#include "../triangular.h"
#include "../symmetric.h"
#include "../io.h"
#else
#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/matrix.h>
#include <boost/numeric/ublas/triangular.h>
#include <boost/numeric/ublas/symmetric.h>
#include <boost/numeric/ublas/io.h>
#endif

#include "test6.h"

int main () {
    test_vector ();
    test_matrix_vector ();
    test_matrix ();
    return 0;
}

