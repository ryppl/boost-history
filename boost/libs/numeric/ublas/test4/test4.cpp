#ifdef BOOST_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>

#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/matrix.h>
#include <boost/numeric/ublas/banded.h>
#include <boost/numeric/ublas/io.h>

#include "test4.h"

int main () {
    test_vector ();
    test_matrix_vector ();
    test_matrix ();
    return 0;
}

