#include <iostream>
#include <mtl/matrix.hpp>
#include <mtl/vector.hpp>
#include <mtl/hires_timer.hpp>


extern "C" void dgemv_(char*, int*, int*, double*, double*, int*,
                       double*, int*, double*, double*, int*);

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
    std::cerr << "time_matvec <M> <N> <Trials>" << std::endl;
    return EXIT_FAILURE;
  }
  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int t = atoi(argv[3]);

  std::vector<double> mtl_times(t), blas_times(t);

  mtl::matrix<double, row>::type A(m, n);
  mtl::vector<double, column>::type y(m), x(n);
  
  double* dA = new double[m * n];
  double* dy = new double[m];
  double* dx = new double[n];

  // Fill in the matrices and vectors
  int i, j, c;
  for (i = 0; i < m; ++i)
    for (j = 0; j < n; ++j) {
      double elt = drand48();
      A[i][j] = elt;
      dA[j * m + i] = elt; // dA is column major
    }
  for (i = 0; i < m; ++i) {
    double elt = drand48();
    y[i] = elt;
    dy[i] = elt;
  }
  for (j = 0; j < n; ++j) {
    double elt = drand48();
    x[j] = elt;
    dx[j] = elt;
  }
  
  double alpha = 1;
  double beta = 1;

  mtl::hires_timer timer;
  int iters = 0;

  // figure out a good length of time to run the test
  double good_interval = timer.elapsed_min() * 100;
  for (; timer.elapsed() < good_interval; ++iters)
      y += A * x;

  std::copy(dy, dy + m, y.begin());

  for (j = 0; j < t; ++j) {
    timer.restart();
    for (c = 0; c < iters; ++c)
      y += A * x;
    mtl_times[j] = timer.elapsed();
  }
  
  for (j = 0; j < t; ++j) {
    timer.restart();
    int inc = 1;
    for (c = 0; c < iters; ++c)
      dgemv_("N", &m, &n, &alpha, dA, &m, dx, &inc, &beta, dy, &inc);
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
    std::copy(dy, dy + m, 
	      std::ostream_iterator<double>(std::cerr, " "));
    std::cerr << std::endl;
  }

  double mtl_min_time = *std::min_element(mtl_times.begin(),
					  mtl_times.end());
  double blas_min_time = *std::min_element(blas_times.begin(),
					   blas_times.end());

  std::cout << m << '\t' << calc_mflops(mtl_min_time, m * n * iters)
	    << '\t' << calc_mflops(blas_min_time, m * n * iters)
	    << std::endl;

  delete [] dA;
  delete [] dy;
  delete [] dx;

  return 0;
}
