#include <iostream>
#include <mtl/vector.hpp>
#include <mtl/hires_timer.hpp>


extern "C" double ddot_(int*, double*, int*, double*, int*);
                      

struct close_enough {
  bool operator()(double x, double y) {
    return std::abs(x - y)/std::abs(x + y) < 10e-4;
  }
};

inline double calc_mflops(double time, int N) {
  return 2.0*N/(1000000.0*time);
}

int
main(int argc, char* argv[])
{
  if (argc < 3) {
    std::cerr << "time_dot <N> <Trials>" << std::endl;
    return EXIT_FAILURE;
  }
  int n = atoi(argv[1]);
  int t = atoi(argv[2]);
  int i, c, j;

  mtl::vector<double, column>::type y(n);
  mtl::vector<double, row>::type x(n);
  
  double* dy = new double[n];
  double* dx = new double[n];

  // Fill in the matrices and vectors
  for (i = 0; i < n; ++i) {
    double elt = drand48();
    y[i] = elt;
    dy[i] = elt;
    elt = drand48();
    x[i] = elt;
    dx[i] = elt;
  }

  double mtl_dot, blas_dot;
  std::vector<double> mtl_times(t), blas_times(t);
  int iters = 0;
  mtl::hires_timer timer;

  // figure out a good length of time to run the test
  double good_interval = timer.elapsed_min() * 100;
  for (; timer.elapsed() < good_interval; ++iters)
    mtl_dot = x * y;
  mtl_dot = 0;

  for (j = 0; j < t; ++j) {
    timer.restart();
    for (c = 0; c < iters; ++c)
      mtl_dot = x * y;
    mtl_times[j] = timer.elapsed();
  }

  for (j = 0; j < t; ++j) {
    timer.restart();
    int inc = 1;
    for (c = 0; c < iters; ++c)
      blas_dot = ddot_(&n, dx, &inc, dy, &inc);
    blas_times[j] = timer.elapsed();
  }

  // make sure the results agree
  if( !close_enough()(mtl_dot, blas_dot) ) {
    std::cerr << "results not equal" << std::endl;
    std::cerr << "mtl_dot: " << mtl_dot << std::endl;
    std::cerr << "blas_dot: " << blas_dot << std::endl;
  }

  double mtl_min_time = *std::min_element(mtl_times.begin(),
					  mtl_times.end());
  double blas_min_time = *std::min_element(blas_times.begin(),
					   blas_times.end());

  std::cout << n << '\t' << calc_mflops(mtl_min_time, n * iters)
	    << '\t' << calc_mflops(blas_min_time, n * iters) << std::endl;
  
  delete [] dy;
  delete [] dx;

  return 0;
}
