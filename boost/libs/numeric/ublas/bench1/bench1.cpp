#include <iostream>

#include "../config.h"
#include "../vector.h"
#include "../matrix.h"

#include "../timer.h"

#include "bench1.h"

int main () {
	std::cout << "float" << std::endl;

    bench_1<float, 3, 1000000 * SCALE> () ();
    bench_2<float, 3, 300000 * SCALE> () ();
    bench_3<float, 3, 100000 * SCALE> () ();

    bench_1<float, 10, 300000 * SCALE> () ();
    bench_2<float, 10, 30000 * SCALE> () ();
    bench_3<float, 10, 3000 * SCALE> () ();

    bench_1<float, 30, 100000 * SCALE> () ();
    bench_2<float, 30, 3000 * SCALE> () ();
    bench_3<float, 30, 100 * SCALE> () ();

    bench_1<float, 100, 30000 * SCALE> () ();
    bench_2<float, 100, 300 * SCALE> () ();
    bench_3<float, 100, 3 * SCALE> () ();

	std::cout << "double" << std::endl;

    bench_1<double, 3, 1000000 * SCALE> () ();
    bench_2<double, 3, 300000 * SCALE> () ();
    bench_3<double, 3, 100000 * SCALE> () ();

    bench_1<double, 10, 300000 * SCALE> () ();
    bench_2<double, 10, 30000 * SCALE> () ();
    bench_3<double, 10, 3000 * SCALE> () ();

    bench_1<double, 30, 100000 * SCALE> () ();
    bench_2<double, 30, 3000 * SCALE> () ();
    bench_3<double, 30, 100 * SCALE> () ();

    bench_1<double, 100, 30000 * SCALE> () ();
    bench_2<double, 100, 300 * SCALE> () ();
    bench_3<double, 100, 3 * SCALE> () ();

    std::cout << "std::complex<float>" << std::endl;

    bench_1<std::complex<float>, 3, 1000000 * SCALE> () ();
    bench_2<std::complex<float>, 3, 300000 * SCALE> () ();
    bench_3<std::complex<float>, 3, 100000 * SCALE> () ();

    bench_1<std::complex<float>, 10, 300000 * SCALE> () ();
    bench_2<std::complex<float>, 10, 30000 * SCALE> () ();
    bench_3<std::complex<float>, 10, 3000 * SCALE> () ();

    bench_1<std::complex<float>, 30, 100000 * SCALE> () ();
    bench_2<std::complex<float>, 30, 3000 * SCALE> () ();
    bench_3<std::complex<float>, 30, 100 * SCALE> () ();

    bench_1<std::complex<float>, 100, 30000 * SCALE> () ();
    bench_2<std::complex<float>, 100, 300 * SCALE> () ();
    bench_3<std::complex<float>, 100, 3 * SCALE> () ();

    std::cout << "std::complex<double>" << std::endl;

    bench_1<std::complex<double>, 3, 1000000 * SCALE> () ();
    bench_2<std::complex<double>, 3, 300000 * SCALE> () ();
    bench_3<std::complex<double>, 3, 100000 * SCALE> () ();

    bench_1<std::complex<double>, 10, 300000 * SCALE> () ();
    bench_2<std::complex<double>, 10, 30000 * SCALE> () ();
    bench_3<std::complex<double>, 10, 3000 * SCALE> () ();

    bench_1<std::complex<double>, 30, 100000 * SCALE> () ();
    bench_2<std::complex<double>, 30, 3000 * SCALE> () ();
    bench_3<std::complex<double>, 30, 100 * SCALE> () ();

    bench_1<std::complex<double>, 100, 30000 * SCALE> () ();
    bench_2<std::complex<double>, 100, 300 * SCALE> () ();
    bench_3<std::complex<double>, 100, 3 * SCALE> () ();
    return 0;
}

