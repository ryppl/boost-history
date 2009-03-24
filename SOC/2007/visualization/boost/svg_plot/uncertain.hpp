/*! \file uncertain.hpp
  \brief Class for storing Uncertainties and handling simple Propagation of according to a pure Gaussian model.
  \details
    This simplifed version assuming uncorrelated uncertainties (the common case)
    is based on code by Evan Manning (manning@alumni.caltech.edu)
    Evan Marshal Manning, C/C++ Users Journal, March 1996 page 29 to 38.
    original downloaded from ftp://beowulf.jpl.nasa.gov/pub/manning
    This is a simple model of uncertainties, designed to
    accompany an article published in C/C++ Users Journal March 1996.
    A fuller collection of even fancier classes also given in UReal.h.
    And also based on a extended version including uncertainty as standard deviation & its uncertainty 
    as degrees of freedom, and other information about the value added Paul A Bristow from 31 Mar 98.

  \author Paul A. Bristow
  \date Mar 2009
*/
// Copyright Paul A. Bristow 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_UNCERTAIN_HPP
#define BOOST_SVG_UNCERTAIN_HPP

#include <boost/svg_plot/detail/pair.hpp>

#include <iostream>
//using std::ostream;
#include <limits>
//using std::numeric_limits;
#include <utility> // using std::pair;

namespace boost
{
namespace svg
{
   static const double plusminus = 2.; //! Number of standard deviations used for plusminus text display. Nominal factor of 2 (strictly 1.96) corresponds to 95% confidence limit.\n

//  template <bool correlated = false>
class unc
{ /*! \brief Class for storing an observed or measured value together with information
    about its uncertainty (previously called 'error' or 'plusminus').
    \details This version assumes uncorrelated uncertainties (by far the most common case).
    \see http://www.measurementuncertainty.org/ \n
    International Vocabulary of Basic and General Terms in Metrology; ISO/TAG 4 1994\n
    ISO, Guide to the expression of uncertainty in measurement, ISO, Geneva, 1993.\n
    Eurochem, Quantifying uncertainty in analytical measurements.
*/
public:
  unc(); // Default constructor.
  unc(double v, float u, short unsigned df, short unsigned ty);
  friend std::ostream& operator<< (std::ostream& os, const unc& u);
  friend std::ostream& operator<< (std::ostream& os, const std::pair<unc, unc>& u);
  bool operator<(const unc& rhs)const;
  bool operator<(unc& rhs)const;
  bool operator==(const unc& rhs) const;
  unc& operator=(const unc& rhs);

  // Get and set member functions.
  double value() const; // return most likely value, typically the mean.
  float uncertainty() const; // returnestimate of uncertainty, typically one standard deviation.
  short unsigned deg_free() const; // return degrees of freedom, usually = number of observations -1;
  short unsigned types() const; // return other information about the value.

  void value(double); // Set most likely value, typically the mean.
  void uncertainty(float); // Set estimate of uncertainty, typically standard deviation.
  void deg_free(short unsigned); // Set degrees of freedom, usually = number of observations -1;
  void types(short unsigned); // Set other information about the value.

private:
  // Note that this class should fit into 128 bytes, same as two 64 bit doubles, so it only doubles the memory required.
  double value_; //!< Most likely value, typically the mean.
  float uncertainty_; //!< Estimate of uncertainty, typically one standard deviation.
  //! Negative values mean that uncertainty is not defined.
  //! 
  short unsigned deg_free_;  /*!< Degrees of freedom, usually = number of observations -1;
  so for 2 observations, 1 degree of freedom.
  Range from 0 (1 observation) to 65534 = (std::numeric_limits<unsigned short int>::max)() - 1\n
  Highest value (std::numeric_limits<unsigned short int>::max)() == 0xFFFF == 65535
  is used to indicate deg_free_ is NOT meaningful.\n
  Note this is an integral type - some functions may provide a floating-point type,
  but this must be converted, even if a marginal amount of information is lost.
  */
  short unsigned types_; //!< Reserved for other information about the value.
  // (Hopefully, this only uses up the bits that would otherwise be padding).
};

unc::unc(double v, float u = -1.f, short unsigned df = (std::numeric_limits<unsigned short int>::max)(), short unsigned ty = 0U)
:
  value_(v), uncertainty_(u), deg_free_(df), types_(ty)
{ // Constructor.
  // Note the defaults so that unspecified variables have 'undefined' status.
}

unc::unc()
:
  value_(0.), uncertainty_(-1.F), deg_free_((std::numeric_limits<unsigned short int>::max)()), types_(0U)
{ // Default constructor.
  // Note the defaults so that value is zero, but others have 'undefined' status.
}

bool unc::operator<(const unc& u) const
{ //! Less operator only compares the value, ignoring any uncertainty information.
  return value_ < u.value_;
}

bool unc::operator<(unc& u) const
{ //! Less operator only compares the value, ignoring any uncertainty information.
  return value_ < u.value_;
}

bool unc::operator ==(const unc& u) const
{ //! Equality operator only compares the value, ignoring any uncertainty information.
  return value_ == u.value_; // But might check ALL are equal? Or that are approximately equal taking uncertainty infor account?
}

unc& unc::operator=(const unc& u)
{ //! Assignment simply copies all values, including those with 'undefined' status.
  value_ = u.value_;
  uncertainty_ = u.uncertainty_;
  deg_free_ = u.deg_free_;
  types_ = u.types_ ; 
  return *this; //! to make chainable.
}

double unc::value() const
{ //! \return Most likely value, typically the mean.
  return value_;
}

float unc::uncertainty() const
{ //! \return Estimate of uncertainty, typically standard deviation.
  return uncertainty_;
}

short unsigned unc::deg_free() const
{ //! \return Degrees of freedom, usually the number of observations -1.
  return deg_free_;
}

short unsigned unc::types() const
{ //! \return Other information about the uncertaint value.
  return types_;
}

void unc::value(double v)
{ //! Set most likely value, typically the mean.
  value_ = v;
}

void unc::uncertainty(float u)
{ //! Set estimate of uncertainty, typically standard deviation.
  uncertainty_ = u;
}
void unc::deg_free(short unsigned df)
{ //! Set degrees of freedom, usually = number of observations -1;
  deg_free_ = df;
}

void unc::types(short unsigned  t)
{ //! Set other information about the uncertain value.
  types_ = t;
}

std::ostream& operator<< (std::ostream& os, const unc& u)
{ /*! Output an value with (if defined) uncertainty and degrees of freedom (and type).
     For example: "1.23 +/- 0.01 (13)".\n
     Note that the uncertainty is input and stored as one standard deviation,
     but output multiplied for a user configurable 'confidence factor' plusminus,
     default two for about 95% confidence (but could also be one for 67% or 3 for 99% confidence).
  */
  os << u.value_;
  if (u.uncertainty_ > 0.F)
  { // Uncertainty is defined, so output it.
    //! Note that the plus or minus can be output using several methods.
    os << '\361'  //! 256 character 8-bit codepage plusminus symbol octal 361, or
    // os << char(241)
    //! decimal 241 or
    // os << char(0xF1)
    //! hexadecimal F1, or
    // os <<  "&#x00A0;&#x00B1;"
    //! Unicode space plusminus glyph, or\n
    //! os << " +or-" << u.uncertainty_; 
    //! Plain ANSI 7 bit code chars.
      << u.uncertainty_ * plusminus; // Typically two standard deviation.
  };
  if (u.deg_free_ != (std::numeric_limits<unsigned short int>::max)())
  { // Degrees of freedom is defined, so output it.
    os << " (" << u.deg_free_ << ")";
  };

  if (u.types_ != 0U)
  { // If defined, output other type information (as yet unspecified).
    os << " [" << u.types_ << "] ";
  }
  return os;
} // ostream& operator<< (ostream& os, const unc& u)

std::ostream& operator<< (std::ostream& os, const std::pair<unc, unc>& u)
{ //! Output a pair (X and Y) value with (if defined) uncertainty and degrees of freedom.
  //! \details For example: "1.23 +/- 0.01 (13), 3.45 +/- 0.06 (78)".
  os << u.first << ", " << u.second;
  return os;
} // std::ostream& operator<< (ostream& os, const pair<unc, unc>& u)

/*! Allow value part of variables of class unc to be assigned to, and compared with double.
\tparam T Built-in floating-point type, float, double or long double, or uncertain type unc.
*/
template <class T>
double value_of(T v);

template <class T>
double value_of(T v)
{ //! \return value as a double.
  return double(v);
}

template<>
double value_of(unc v)
{ //! \return unc.value() as a double.
  return v.value();
}

/*! Allow uncertainty (standard deviation) part of variables of class unc to be assigned to, and compared with float.
\tparam T Built-in floating-point type, float, double or long double, or uncertain type unc.
*/
template <class T>
float unc_of(T);

template <class T>
float unc_of(T)
{ //! \return zero always (because no uncertainty information is available for built-in double, float, or long double).
  return float(0.);
}

template<>
float unc_of(unc v)
{ //! \return unc.uncertainty() as a float.
  return v.uncertainty();
}

// Two helper functions to provide values and uncertainties as pairs.
// Note names plural valueS_of

template <class T>
std::pair<double, double> values_of(T); // Declaration.

template <class T>
std::pair<double, double> values_of(std::pair<const T, T> vp)
{
  return std::make_pair(value_of(vp.first), value_of(vp.second));
}

template <class T>
std::pair<double, double> values_of(std::pair<const unc, unc> up)
{ //! \return value (part) as a pair of doubles.
  // so can write
  // std::pair<const double, double> minmax = value_of(*result.first); // x min & max
  // whether T is double or unc.

  double vp1 = up.first.value();
  double vp2 = up.second.value();
  std::pair<double, double> minmax = std::make_pair(up.first.value(), up.second.value());
  return minmax;  
}

template <class T>
std::pair<double, double> values_of(std::pair<T, T> vp)
{
  return std::make_pair(value_of(vp.first), value_of(vp.second));
}

template <class T>
std::pair<double, double> values_of(std::pair<unc, unc> up)
{ //! \return value (part) as a pair of doubles.
  // so can write
  // std::pair<const double, double> minmax = value_of(*result.first); // x min & max
  // whether T is double or unc.
  return std::make_pair<double, double>(up.first.value(), up.second.value());
}

template <class T>
std::pair<float, float> uncs_of(T); // Declaration.

template <class T>
std::pair<float, float> uncs_of(std::pair<T, T> vp)
{
  return std::make_pair<float, float>(unc_of(vp.first), unc_of(vp.second));
}

template <class T>
std::pair<const float, float> uncs_of(std::pair<const T, T> vp)
{ //! \return uncertainty parts (if any) as a pair of floats.
  // so can write
  // std::pair<const float, float> minmax = value_of(*result.first); // min unc & max unc for example/
  // whether T is built-in or unc.
  return std::make_pair(vp.first.unc(), vp.second.unc());
}

} // namespace svg
} // namespace boost
#endif // BOOST_SVG_UNCERTAIN_HPP
