#ifdef USE_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>

#ifdef NUMERICS_PRIVATE
#include "../config.h"
#include "../vector.h"
#include "../vector_sp.h"
#include "../matrix.h"
#include "../matrix_sp.h"
#include "../io.h"
#else
#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/vector_sp.h>
#include <boost/numeric/ublas/matrix.h>
#include <boost/numeric/ublas/matrix_sp.h>
#include <boost/numeric/ublas/io.h>
#endif

#include "test3.h"

int main () {
    test_vector ();
    test_matrix_vector ();
    test_matrix ();
    return 0;
}

