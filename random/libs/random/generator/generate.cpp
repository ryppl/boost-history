// Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iomanip>
#include <map>
#include <cmath>
#include <ctime>

#include <boost/lexical_cast.hpp>

// Boost.Random includes
#include <boost/random.hpp>
#include <boost/random/distributions.hpp>

// Boost.Math includes
#include <boost/math/distributions.hpp>
#include <boost/math/distributions/extreme_value_i.hpp>
#include <boost/math/distributions/geometric.hpp>
#include <boost/math/distributions/skew_normal.hpp>

// This is a quick little trick to put all of the random distributions into
// a distinct namespace. I would have liked to use "random", but that's already
// taken...
namespace boost { namespace probability {
    // Discrete distributions
    using boost::bernoulli_distribution;
    using boost::binomial_distribution;
    using boost::poisson_distribution;
    using boost::geometric_distribution;

    // Continuous distributions
    using boost::normal_distribution;
    using boost::lognormal_distribution;
    using boost::skew_normal_distribution;
    using boost::exponential_distribution;
    using boost::gamma_distribution;
    using boost::beta_distribution;
    using boost::chi_squared_distribution;
    using boost::fisher_f_distribution;
    using boost::students_t_distribution;
    using boost::cauchy_distribution;
    using boost::extreme_value_i_distribution;
    using boost::weibull_distribution;
    using boost::rayleigh_distribution;
    using boost::pareto_distribution;
} }

using namespace std;
using namespace boost;

typedef map<int, size_t> Histogram;

template <typename Real>
inline Real infinity()
{ return numeric_limits<Real>::infinity(); }

template <typename Statistic>
inline typename Statistic::value_type get_mean(const Statistic& dist)
{ return math::mean(dist); }

template <typename R, typename P>
inline R get_mean(const math::cauchy_distribution<R,P>&)
{ return numeric_limits<R>::quiet_NaN(); }

template <typename R, typename P>
inline R get_mean(const math::fisher_f_distribution<R,P>& dist)
{ return dist.degrees_of_freedom2() <= 2 ? infinity<R>() : math::mean(dist); }

template <typename Statistic>
inline typename Statistic::value_type get_variance(const Statistic& dist)
{ return math::variance(dist); }

template <typename R, typename P>
inline R get_variance(const math::cauchy_distribution<R,P>&)
{ return numeric_limits<R>::quiet_NaN(); }

template <typename R, typename P>
inline R get_variance(const math::pareto_distribution<R,P>& dist)
{ return dist.shape() <= 2 ? infinity<R>() : math::variance(dist); }

template <typename R, typename P>
inline R get_variance(const math::fisher_f_distribution<R,P>& dist)
{ return dist.degrees_of_freedom2() <= 4 ? infinity<R>() : math::variance(dist); }

// It would be nice if I didn't have to pass the generating distribution
// separately from the probabilistic distribution. What I'm saying is that the
// Boost.Math distributions should have a function "generator()" that returns
// a distribution that can generate random numbers.
template <typename Probabilistic, typename Statistic, typename Engine, typename Histogram>
void
histogram(Probabilistic& dist,
          Statistic& stat,
          Engine& rng,
          Histogram& hist,
          size_t n,
          size_t p)
{
    // Generate n random numbers and bin them into p-precision bins.
    // Basically, we just multiply the random number by p - hopefully
    // it's a power of 10. And divide by p for the output.
    vector<double> data(n);
    for(size_t i = 0; i < n; ++i) {
        double x = dist(rng);
        data[i] = x;
        hist[int(round(x * p))] += 1;
    }

    // Sort the data for convenience of computation below.
    sort(data.begin(), data.end());

    // Compute the sum and mean in the first pass.
    double sum = 0.0;
    for(size_t i = 0; i < n; ++i) {
        sum += data[i];
    }
    double mean = sum / double(n);

    // Compute the (population) variance.
    sum = 0.0;
    for(size_t i = 0; i < n; ++i) {
        double x = data[i] - mean;
        sum += x * x;
    }
    double var = sum / double(n);
    // double stddev = sqrt(var);
    // double median = data[n / 2] + data[n / 2 + 1] / 2.0;

    cerr << setiosflags(ios::left) << setw(10) << "Measure"
         << setw(15) << "Expected" << setw(15) << "Observed" << endl;
    cerr << setiosflags(ios::left) << setw(10) << "Mean"
         << setw(15) << get_mean(stat) << setw(15) << mean << endl;
    cerr << setiosflags(ios::left) << setw(10) << "Variance"
         << setw(15) << get_variance(stat) << setw(15) << var << endl;
}

inline double to_double(const char* str)
{
    return lexical_cast<double>(str);
}

inline int to_int(const char* str)
{
    return lexical_cast<int>(str);
}

int
main(int argc, char *argv[])
{
    // Setup the base random number generators. Here, we have a random bit
    // generator (rbg) that will feed numbers into the a uniform [0, 1) random
    // number generator (rng). Note that the rbg is seeded against the current
    // time.
    minstd_rand rbg(time(0));
    uniform_01<minstd_rand> rng(rbg);

    // See if the user wants to use a different distribution.
    string which;
    if(argc > 1) {
        which = argv[1];
    }

    // Basic histogram configuration.
    Histogram hist;
    size_t n = 10000;
    size_t p = 100;

    // Which distribution?
    if(which == "bernoulli") {
        double prob = argc > 2 ? to_double(argv[2]) : 0.5;
        cerr << "X ~ Bernoulli(" << prob << ")" << endl;

        probability::bernoulli_distribution<> dist(prob);
        math::bernoulli_distribution<> stat(prob);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "binomial") {
        int num = argc > 2 ? to_int(argv[2]) : 1;
        double prob = argc > 3 ? to_double(argv[3]) : 0.5;
        cerr << "X ~ Binomial(" << num << "," << prob << ")" << endl;

        probability::binomial_distribution<> dist(num, prob);
        math::binomial_distribution<> stat(num, prob);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "poisson") {
        double mean = argc > 2 ? to_double(argv[2]) : 1.0;
        cerr << "X ~ Poisson(" << mean << ")" << endl;

        probability::poisson_distribution<> dist(mean);
        math::poisson_distribution<> stat(mean);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "geometric") {
        double prob = argc > 2 ? to_double(argv[2]) : 0.5;
        cerr << "X ~ Geometric(" << prob << ")" << endl;

        probability::geometric_distribution<> dist(prob);
        math::geometric_distribution<> stat(prob);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "normal") {
        double mean = argc > 2 ? to_double(argv[2]) : 0.0;
        double sigma = argc > 3 ? to_double(argv[3]) : 1.0;
        cerr << "X ~ Normal(" << mean << "," << sigma << ")" << endl;

        probability::normal_distribution<> dist(mean, sigma);
        math::normal_distribution<> stat(mean, sigma);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "lognormal") {
        double mean = argc > 2 ? to_double(argv[2]) : 0.0;
        double sigma = argc > 3 ? to_double(argv[3]) : 1.0;
        cerr << "X ~ LogNormal(" << mean << "," << sigma << ")" << endl;

        probability::lognormal_distribution<> dist(mean, sigma);
        math::lognormal_distribution<> stat(mean, sigma);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "skew-normal") {
        double mean = argc > 2 ? to_double(argv[2]) : 0.0;
        double sigma = argc > 3 ? to_double(argv[3]) : 1.0;
        double alpha = argc > 4 ? to_double(argv[4]) : 0.0;
        cerr << "X ~ SkewNormal(" << mean << "," << sigma << "," << alpha << ")" << endl;

        probability::skew_normal_distribution<> dist(mean, sigma, alpha);
        math::skew_normal_distribution<> stat(mean, sigma, alpha);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "exponential") {
        double l = argc > 2 ? to_double(argv[2]) : 1.0;
        cerr << "X ~ Exponential(" << l << ")" << endl;

        probability::exponential_distribution<> dist(l);
        math::exponential_distribution<> stat(l);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "gamma") {
        double alpha = argc > 2 ? to_double(argv[2]) : 1.0;
        double theta = argc > 3 ? to_double(argv[3]) : 1.0;
        cerr << "X ~ Gamma(" << alpha << "," << theta << ")" << endl;

        probability::gamma_distribution<> dist(alpha, theta);
        math::gamma_distribution<> stat(alpha, theta);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "beta") {
        double alpha = argc > 2 ? to_double(argv[2]) : 1.0;
        double beta = argc > 3 ? to_double(argv[3]) : 1.0;
        cerr << "X ~ Beta(" << alpha << "," << beta << ")" << endl;

        probability::beta_distribution<> dist(alpha, beta);
        math::beta_distribution<> stat(alpha, beta);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "chi-squared") {
        double df = argc > 2 ? to_double(argv[2]) : 1.0;
        cerr << "X ~ ChiSquared(" << df << ")" << endl;

        probability::chi_squared_distribution<> dist(df);
        math::chi_squared_distribution<> stat(df);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "f") {
        double d1 = argc > 2 ? to_double(argv[2]) : 1.0;
        double d2 = argc > 3 ? to_double(argv[3]) : 1.0;
        cerr << "X ~ F(" << d1 << "," << d2 << ")" << endl;

        probability::fisher_f_distribution<> dist(d1, d2);
        math::fisher_f_distribution<> stat(d1, d2);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "students-t") {
        double df = argc > 2 ? to_double(argv[2]) : 1.0;
        cerr << "X ~ StudentsT(" << df << ")" << endl;

        probability::students_t_distribution<> dist(df);
        math::students_t_distribution<> stat(df);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "cauchy") {
        // Strange distribution... There are some significant outliers that can
        // make it's plot look a little misleading. It's not (really).
        double med = argc > 2 ? to_double(argv[2]) : 0.0;
        double sigma = argc > 3 ? to_double(argv[3]) : 1.0;
        cerr << "X ~ Cauchy(" << med << "," << sigma << ")" << endl;

        probability::cauchy_distribution<> dist(med, sigma);
        math::cauchy_distribution<> stat(med, sigma);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "extreme-value") {
        double mu = argc > 2 ? to_double(argv[2]) : 0.0;
        double beta = argc > 3 ? to_double(argv[3]) : 1.0;
        double sign = argc > 4 ? to_double(argv[4]) : 1.0;
        cerr << "X ~ ExtremeValue(" << mu << "," << beta << "," << sign << ")" << endl;

        probability::extreme_value_i_distribution<> dist(mu, beta, sign);
        math::extreme_value_i_distribution<> stat(mu, beta, sign);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "weibull") {
        double alpha = argc > 2 ? to_double(argv[2]) : 1.0;
        double beta = argc > 3 ? to_double(argv[3]) : 1.0;
        cerr << "X ~ Weibull(" << alpha << "," << beta << ")" << endl;

        probability::weibull_distribution<> dist(alpha, beta);
        math::weibull_distribution<> stat(alpha, beta);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "rayleigh") {
        double sigma = argc > 2 ? to_double(argv[2]) : 1.0;
        cerr << "X ~ Rayleigh(" << sigma << ")" << endl;

        probability::rayleigh_distribution<> dist(sigma);
        math::rayleigh_distribution<> stat(sigma);
        histogram(dist, stat, rng, hist, n, p);
    }
    else if(which == "pareto") {
        double loc = argc > 2 ? to_double(argv[2]) : 1.0;
        double shape = argc > 3 ? to_double(argv[3]) : 1.0;
        cerr << "X ~ Pareto(" << loc << "," << shape << ")" << endl;

        probability::pareto_distribution<> dist(loc, shape);
        math::pareto_distribution<> stat(loc, shape);
        histogram(dist, stat, rng, hist, n, p);
    }
    else {
        cout << "Usage: hist [distribution [parameters]]" << endl << endl;
        cout << "Available distributions and parameters:" << endl;
        cout << "\tbernoulli        [prob = 0.5]" << endl;
        cout << "\tbinomial         [num = 1] [prob = 0.5]" << endl;
        cout << "\tpoisson          [mean = 1]" << endl;
        cout << "\tnormal           [mean = 0] [sigma = 1]" << endl;
        cout << "\tlognormal        [mean = 0] [sigma = 1]" << endl;
        cout << "\tskew-normal      [mean = 0] [sigma = 1] [alpha = 0]" << endl;
        cout << "\texponential      [lambda = 1]" << endl;
        cout << "\tgamma            [alpha = 1] [theta = 1]" << endl;
        cout << "\tbeta             [alpha = 1] [beta = 1]" << endl;
        cout << "\tchi-squared      [deg = 1]" << endl;
        cout << "\tfisher-f         [deg1 = 1] [deg2 = 1]" << endl;
        cout << "\tstudents-t       [deg1 = 1]" << endl;
        cout << "\tcauchy           [median = 0] [sigma = 1]" << endl;
        cout << "\textreme-value    [alpha = 0] [beta = 1] [sign = 1]" << endl;
        cout << "\tweibull          [alpha = 1] [beta = 1]" << endl;
        cout << "\trayleigh         [sigma = 1]" << endl;
        cout << "\tpareto           [k = 1] [alpha = 1]" << endl;
    }

    // Compute a precision based on the log of p.
    double sum = 0.0;
    int prec = int(log10(p));
    Histogram::iterator i, end = hist.end();
    for(i = hist.begin(); i != hist.end(); ++i) {
        double x = double(i->first) / double(p);
        double p = double(i->second) / double(n);
        sum += p;
        cout << fixed << setprecision(prec)
             << x << " "
             << setprecision(6) << p << " " << sum << " "
             << i->second << endl;
    }
    // cerr << "check cumulative prob: " << sum << endl;

    return 0;
}
