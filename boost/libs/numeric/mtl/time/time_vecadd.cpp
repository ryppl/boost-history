#include <iostream>
#include <mtl/vector.hpp>
#include <mtl/hires_timer.hpp>


extern "C" void daxpy_(int*, double*, double*, int*, double*, int*);
                      

struct close_enough {
  bool operator()(double x, double y) {
    return std::abs(x - y)/std::abs(x + y) < 10e-1;
  }
};

inline double calc_mflops(double time, int N) {
  return 2.0*N/(1000000.0*time);
}

int
main(int argc, char* argv[])
{
  if (argc < 3) {
    std::cerr << "time_vecadd <N> <Trials>" << std::endl;
    return EXIT_FAILURE;
  }
  int n = atoi(argv[1]);
  int t = atoi(argv[2]);
  int i, c, j;

  mtl::vector<double, column>::type y(n), x(n);
  
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
  
  double alpha = drand48();
  std::vector<double> mtl_times(t), blas_times(t);
  mtl::hires_timer timer;

  int iters = 0;

  // figure out a good length of time to run the test
  double good_interval = timer.elapsed_min() * 100;
  for (; timer.elapsed() < good_interval; ++iters)
    y += alpha * x;
  std::copy(dy, dy + n, y.begin());

  for (j = 0; j < t; ++j) {
    timer.restart();
    for (c = 0; c < iters; ++c)
      y += alpha * x;
    mtl_times[j] = timer.elapsed();
  }

  for (j = 0; j < t; ++j) {
    timer.restart();
    int inc = 1;
    for (c = 0; c < iters; ++c)
      daxpy_(&n, &alpha, dx, &inc, dy, &inc);
    blas_times[j] = timer.elapsed();
  }

  // make sure the results agree
  if( !std::equal(y.begin(), y.end(), dy, close_enough()) ) {
    std::cerr << "results not equal" << std::endl;
    std::cerr << "y: ";
    std::copy(y.begin(), y.end(), 
	      std::ostream_iterator<double>(std::cerr, " "));
    std::cerr << std::endl;
    std::cerr << "dy: ";
    std::copy(dy, dy + n, 
	      std::ostream_iterator<double>(std::cerr, " "));
    std::cerr << std::endl;
  }

  double mtl_min_time = *std::min_element(mtl_times.begin(),
					  mtl_times.end());
  double blas_min_time = *std::min_element(blas_times.begin(),
					   blas_times.end());

  std::cout << n << '\t' << calc_mflops(mtl_min_time, n * iters)
	    << '\t' << calc_mflops(blas_min_time, n * iters)
	    << std::endl;

  delete [] dy;
  delete [] dx;

  return 0;
}
