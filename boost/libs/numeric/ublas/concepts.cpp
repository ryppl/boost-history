#ifdef BOOST_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <cassert>
#include <iostream>

#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/vector_sp.h>
#include <boost/numeric/ublas/matrix.h>
#include <boost/numeric/ublas/banded.h>
#include <boost/numeric/ublas/triangular.h>
#include <boost/numeric/ublas/symmetric.h>
#include <boost/numeric/ublas/hermitean.h>
#include <boost/numeric/ublas/matrix_sp.h>
#include <boost/numeric/ublas/io.h>

#include <boost/numeric/ublas/concepts.h>

namespace numerics = boost::numerics;

int main () {
    void (* check) (void) = numerics::concept_checks;
    numerics::ignore_unused_variable_warning (check);
    return 0;
}


























