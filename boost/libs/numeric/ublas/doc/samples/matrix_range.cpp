#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

int main () {
    using namespace boost::numeric::ublas;
    matrix<double> m (3, 3);
    matrix_range<matrix<double> > mr (m, range (0, 3), range (0, 3));
    for (unsigned i = 0; i < mr.size1 (); ++ i)
        for (unsigned j = 0; j < mr.size2 (); ++ j)
            mr (i, j) = 3 * i + j;
    std::cout << mr << std::endl;
}

