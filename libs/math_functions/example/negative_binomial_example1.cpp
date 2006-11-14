// negative_binomial_example1.cpp

// Copyright Paul A. Bristow 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_MATH_THROW_ON_DOMAIN_ERROR

#ifdef _MSC_VER
#  pragma warning(disable: 4127) // conditional expression is constant.
#  pragma warning(disable: 4512) // assignment operator could not be generated.
#  pragma warning(disable: 4996) // 'std::char_traits<char>::copy' was declared deprecated.
#endif

// Example 1 of using negative_binomial distribution.

// http://en.wikipedia.org/wiki/Negative_binomial_distribution

// (After a problem by Dr. Diane Evans,
// Professor of mathematics at Rose-Hulman Institute of Technology)

// Pat is required to sell candy bars to raise money for the 6th grade field trip.
// There are thirty houses in the neighborhood,
// and Pat is not supposed to return home until five candy bars have been sold.
// So the child goes door to door, selling candy bars.
// At each house, there is a 0.4 probability of selling one candy bar
// and a 0.6 probability of selling nothing.

// What's the probability mass function for selling the last candy bar at the nth house?

// Recall that the Negative Binomial(r, p) distribution describes the probability of k failures
// and r successes in k+r Bernoulli(p) trials with success on the last trial.
// Selling five candy bars means getting five successes, so r = 5.
// The total number of trials (in this case, houses) this takes is therefore
//    sucesses + failures or k + r = k + 5 = n.
// The random variable we are interested in is the number of houses k
// that must be visited to sell five candy bars,
// so we substitute k = n 5 into a NegBin(5, 0.4) mass function
// and obtain the following mass function of the distribution of houses (for n ? 5):

// What's the probability that Pat finishes ON the tenth house?

//    f(10) = 0.1003290624, or about 1 in 10

// What's the probability that Pat finishes ON OR BEFORE reaching the eighth house?

// To finish on or before the eighth house,
// Pat must finish at the fifth, sixth, seventh, or eighth house.
// Sum those probabilities:

    // f(5) = 0.01024
    // f(6) = 0.03072
    // f(7) = 0.055296
    // f(8) = 0.0774144
    // sum {j=5 to 8} f(j) = 0.17367

// What's the probability that Pat exhausts all 30 houses in the neighborhood,
// and doesn't sell the required 5 candy bars?

// 1 - sum{j=5 to 30} f(j) = 1 - incomplete beta (p = 0.4)(5, 30-5+1) =~ 1 - 0.99849 = 0.00151

// see also http://www.math.uah.edu/stat/bernoulli/Introduction.xhtml
// http://www.codecogs.com/pages/catgen.php?category=stats/dists/discrete


#include <boost/math/distributions/negative_binomial.hpp> // for negative_binomial_distribution
  using boost::math::negative_binomial_distribution;
	using boost::math::negative_binomial; // typedef provides default type is double.
  using  ::boost::math::cdf;
  using  ::boost::math::pdf; // Probability of negative_binomial.
  using  ::boost::math::quantile;

#include <iostream>
	using std::cout;
	using std::endl;
	using std::noshowpoint;
#include <iomanip>
	using std::setprecision;

#include <cassert>

int main()
{
	cout << "Example 1 using the Negative_binomial Distribution.";
  #if defined(__FILE__) && defined(__TIMESTAMP__)
  	cout << "  " << __FILE__ << ' ' << __TIMESTAMP__ << ' '<< _MSC_FULL_VER << "\n";
  #endif
	cout << endl;

  // Some examples of constructing distribution, for example negative binomial:
  // Fundamentally constructed like this:
  boost::math::negative_binomial_distribution<double> mydist0(8., 0.25);
  // But is inconveniently long.

  using boost::math::negative_binomial_distribution;
  // Allows convenient reference to negative_binomial_distribution.

  // You can provide the type explicitly thus:
  negative_binomial_distribution<double> mydist1(8., 0.25); // Explicit double.
  negative_binomial_distribution<float>  mydist2(8., 0.25); // Explicit float, double arguments -> float.
  negative_binomial_distribution<float>  mydist3(8, 0.25); // Explicit float, integer & double arguments -> float.
  negative_binomial_distribution<float>  mydist4(8.F, 0.25F); // Explicit float, float arguments, no conversion.
  negative_binomial_distribution<float>  mydist5(8, 1); // Explicit integer, integer arguments -> float.
  // And if you have your own RealType then:
  // negative_binomial_distribution<YourType>  mydist6(8, 1); // Integer arguments -> YourType.

  // negative_binomial_distribution<> mydist8; // error C2512 no appropriate default constructor available.
  // Since there are no accessor functions, no default constructor are provided,
  // because it is difficult to chose any sensible default values.

  negative_binomial_distribution<>  mydist9(8., 0.25); // Uses default RealType = double.
  // But the name "negative_binomial_distribution" is still inconveniently long.

  // Some examples using the provided typedef:
  // typedef negative_binomial_distribution<double> negative_binomial; // Reserved name of type double.

  using boost::math::negative_binomial; // Convenient access to the name.
  // Allows convenient reference to negative_binomial of default type double.
  negative_binomial mydist10(5., 0.4); // Both arguments double.
  // And automatic conversion takes place, so you can use integers and floats:
  negative_binomial mydist11(5, 0.4); // Using provided typedef double, int and double arguments.
  // This is probably the most common usage.
  negative_binomial mydist12(5., 0.4F); // Double and float arguments. 
  negative_binomial mydist13(5, 1); // Both arguments integer.

  try
  {
    double sales_quota = 5; // Pat's sales quota - successes (r).
    double success_fraction = 0.4; // success_fraction (p) - so fail_fraction is 0.6.
    negative_binomial nb(5, 0.4); // double
    
    int all_houses = 30; // The number of houses on the estate.

    cout <<"Selling candy bars - an example of using the negative binomial distribution. " << setprecision(3)
      << "\n""An example by Dr. Diane Evans,"
      "\n""Professor of Mathematics at Rose-Hulman Institute of Technology,"
      << "\n""see http://en.wikipedia.org/wiki/Negative_binomial_distribution""\n"
      << endl;
    cout << "Pat has a sales per house success rate of " << success_fraction
      << ",""\n""so he would, on average, sell " << nb.success_fraction() * 100 << " bars after trying 100 houses." << endl;

    cout << "With a success rate of " << nb.success_fraction()  << ", he might expect, on average, to need to visit about "
      << success_fraction * all_houses << " houses in order to sell all " << nb.successes() << " candy bars. " << endl;

    // To finish on or before the 8th house, Pat must finish at the 5th, 6th, 7th or 8th house.
    // (Obviously he could not finish on fewer than 5 houses because he must sell 5 candy bars.
    // so the 5th house is the first possible that he could finish on).
    // Sum those probabilities to find the probability that he will finish exactly on the tenth house.
    // The probability that he will finish on exactly any house is the Probability Density Function (pdf).
    cout << "Probability that Pat finishes on the " << sales_quota << "th house is " << "f(5) = " << pdf(nb, nb.successes()) << endl;
    cout << "Probability that Pat finishes on the 6th house is " << "f(6) = " << pdf(nb, 6 - sales_quota) << endl;
    cout << "Probability that Pat finishes on the 7th house is " << "f(7) = " << pdf(nb, 7 - sales_quota) << endl;
    cout << "Probability that Pat finishes on the 8th house is " << "f(8) = " << pdf(nb, 8 - sales_quota) << endl;

    // The sum of the probabilities for these houses is the Cumulative Distribution Function (cdf).
    cout << "Probability that Pat finishes on or before the 8th house is " << "f(sales_quota) + f(6) + f(7) + f(8) = "
      // Sum each of the mass/density probabilities for houses sales_quota, 6, 7, & 8.
      << pdf(nb, sales_quota - sales_quota) // 0
      + pdf(nb, 6 - sales_quota) // 1
      + pdf(nb, 7 - sales_quota) // 2
      + pdf(nb, 8 - sales_quota) // 3
      << endl;

    // Or using the negative binomial **cumulative** distribution function (cdf instead sum of the pdfs):
    cout << "\n""Probability of selling his quota of " << sales_quota
      << " candy bars""\n""on or before the " << 8 << "th house is "
      << cdf(nb, 8 - sales_quota) << endl;


    cout << "Probability that Pat finishes on the 10th house is " << "f(10) = " << pdf(nb, 10 - sales_quota) << endl;
    cout << "\n""Probability of selling his quota of " << sales_quota
      << " candy bars""\n""on or before the " << 10 << "th house is "
      << cdf(nb, 10 - sales_quota) << endl;

    cout << "Probability that Pat finishes on the 11th house is " << "f(11) = " << pdf(nb, 11 - sales_quota) << endl;
    cout << "\n""Probability of selling his quota of " << sales_quota
      << " candy bars""\n""on or before the " << 11 << "th house is "
      << cdf(nb, 11 - sales_quota) << endl;

    cout << "Probability that Pat finishes on the 12th house is " << "f(10) = " << pdf(nb, 12 - sales_quota) << endl;
    cout << "\n""Probability of selling his quota of " << sales_quota
      << " candy bars""\n""on or before the " << 12 << "th house is "
      << cdf(nb, 12 - sales_quota) << endl;

    // Finally consider the risk of Pat not setting his quota of 5 bars even after visiting all the houses.
    cout << "\n""Probability of failing to sell his quota of " << sales_quota
      << " candy bars""\n""even after visiting all " << all_houses << "  houses is "
      << 1 - cdf(nb, all_houses - sales_quota) << endl;

    // This is the Cephes and Codecogs inverse.
    cout.precision(17);
    double p = cdf(nb, (8 - sales_quota)); 
    cout << "Probability of meeting sales quota on or before 8th house is "<< p << endl;
    // Probability of meeting sales quota on or before 8th house is 0.17367040000000009
    cout.precision(3);

    double z = quantile(nb, p);
    cout << "quantile(nb, " << p << ") = " << z + sales_quota << endl;
    cout << "If the confidence of meeting sales quota is " << p
        << ", then the finishing house is " << quantile(nb, p) + sales_quota << endl;

    cout << "If confidence of meeting quota is zero (we assume all " << sales_quota << " sales are successes)" 
      ", then finishing house is " << sales_quota << endl;


    int const pssize  = 11;
    double ps[pssize] = {0., 0.001, 0.01, 0.05, 0.1, 0.5, 0.9, 0.95, 0.99, 0.999, 1.};
    for (int i = 0; i < pssize; i++)
    {
      cout << "If confidence of meeting quota is " << ps[i]
        << ", then finishing house is " << ceil(quantile(nb, ps[i])) + sales_quota << endl;
    }

    cout << "If we demand a confidence of meeting sales quota of unity"
      " (so we never achieve " << sales_quota << " sales)" 
      ", then we can never be certain, so the finishing house is infinite!"  << endl;
  }
  catch(const std::exception& e)
   {
      std::cout <<
          "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
   }

	return 0;
}  // int main()

/*

Output is:

Example 1 using the Negative_binomial Distribution.  ..\..\..\..\..\..\boost-san
dbox\libs\math_functions\example\negative_binomial_example1.cpp Mon Oct 23 13:57
:11 2006 140050727

Selling candy bars - an example of using the negative binomial distribution.
An example by Dr. Diane Evans,
Professor of Mathematics at Rose-Hulman Institute of Technology,
see http://en.wikipedia.org/wiki/Negative_binomial_distribution

Pat has a sales per house success rate of 0.4,
so he would, on average, sell 40 bars after trying 100 houses.
With a success rate of 0.4, he might expect, on average, to need to visit about
12 houses in order to sell all 5 candy bars.
Probability that Pat finishes on the 5th house is f(5) = 0.1
Probability that Pat finishes on the 6th house is f(6) = 0.0307
Probability that Pat finishes on the 7th house is f(7) = 0.0553
Probability that Pat finishes on the 8th house is f(8) = 0.0774
Probability that Pat finishes on or before the 8th house is f(sales_quota) + f(6
) + f(7) + f(8) = 0.174

Probability of selling his quota of 5 candy bars
on or before the 8th house is 0.174
Probability that Pat finishes on the 10th house is f(10) = 0.1

Probability of selling his quota of 5 candy bars
on or before the 10th house is 0.367
Probability that Pat finishes on the 11th house is f(11) = 0.1

Probability of selling his quota of 5 candy bars
on or before the 11th house is 0.467
Probability that Pat finishes on the 12th house is f(10) = 0.0946

Probability of selling his quota of 5 candy bars
on or before the 12th house is 0.562

Probability of failing to sell his quota of 5 candy bars
even after visiting all 30  houses is 0.00151
Probability of meeting sales quota on or before 8th house is 0.17367040000000003

quantile(nb, 0.174) = 8
If the confidence of meeting sales quota is 0.174, then the finishing house is 8

If confidence of meeting quota is zero (we assume all 5 sales are successes), th
en finishing house is 5
If confidence of meeting quota is 0, then finishing house is 5
If confidence of meeting quota is 0.001, then finishing house is 5
If confidence of meeting quota is 0.01, then finishing house is 5
If confidence of meeting quota is 0.05, then finishing house is 7
If confidence of meeting quota is 0.1, then finishing house is 8
If confidence of meeting quota is 0.5, then finishing house is 12
If confidence of meeting quota is 0.9, then finishing house is 18
If confidence of meeting quota is 0.95, then finishing house is 21
If confidence of meeting quota is 0.99, then finishing house is 25
If confidence of meeting quota is 0.999, then finishing house is 32
If confidence of meeting quota is 1, then finishing house is 1.#J
If we demand a confidence of meeting sales quota of unity (so we never achieve 5 sales),
then we can never be certain, so the finishing house is infinite!
Press any key to continue . . .


*/





