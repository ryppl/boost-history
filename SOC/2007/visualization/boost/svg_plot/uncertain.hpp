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
using std::ostream;
#include <limits>
using std::numeric_limits;

namespace boost
{
namespace svg
{
class unc
{ /*! \brief Class for storing an observed or measured value together with information
    about its uncertainty (previously called 'error' or 'plusminus').
    \details This version assumes uncorrelated uncertainties (by far the most common case).
*/
public:
  unc(); // Default constructor.
  unc(double v, float u, short unsigned df, short unsigned ty);
  friend ostream& operator<< (ostream& os, const unc& u);
  friend ostream& operator<< (ostream& os, const std::pair<unc, unc>& u);
  bool operator<(const unc& rhs)const;
  bool operator<(unc& rhs)const;
  bool operator==(const unc& rhs) const;
  unc& operator=(const unc& rhs);

  // Get and set member functions.
  double value(); //!< Get most likely value, typically the mean.
  float uncertainty(); //!< Get estimate of uncertainty, typically standard deviation.
  short unsigned deg_free(); //!< Get degrees of freedom, usually = number of observations -1;
  short unsigned types(); //!< Get other information about the value.

  void value(double);//!< Set most likely value, typically the mean.
  void uncertainty(float);//!< Set estimate of uncertainty, typically standard deviation.
  void deg_free(short unsigned); //!< Set degrees of freedom, usually = number of observations -1;
  void types(short unsigned); //!< Set other information about the value.

  // private?
  // Note that this should fit into 128 bytes, same as two 64 bit doubles.
  double value_; //!< Most likely value, typically the mean.
  float uncertainty_; //!< Estimate of uncertainty, typically standard deviation.
  //! Negative values mean that uncertainty is not defined.
  short unsigned deg_free_;	//!< Degrees of freedom, usually = number of observations -1;
	//! so for 2 observations, 1 degree of freedom.
  //! Range from 0 (1 observation) to 65534 = (std::numeric_limits<unsigned short int>::max)() - 1
  //! Highest value (std::numeric_limits<unsigned short int>::max)() == 0xFFFF == 65535
  //! is used to indicate deg_free_ is NOT meaningful.
  //! Note this is an integral type - some functions may provide a floating-point type,
  //! but this must be converted, even if a marginal amount of information is lost.
  short unsigned types_; //!< Reserved for other information about the value_
  // (Hopefully, this only uses up the bits that would otherwise be padding).
};

unc::unc(double v, float u = -1.f, short unsigned df = (std::numeric_limits<unsigned short int>::max)(), short unsigned ty = 0U)
:
  value_(v), uncertainty_(u), deg_free_(df), types_(ty)
{ // Constructor.
  // Note the defaults.
}

unc::unc()
:
  value_(0.), uncertainty_(-1.F), deg_free_((std::numeric_limits<unsigned short int>::max)()), types_(0U)
{ // Default constructor.
  // Note the defaults so that value is zero, but others have 'unknown' status.
}

bool unc::operator<(const unc& rhs) const
{
  return value_ <rhs.value_;
}

bool unc::operator<(unc& rhs) const
{
  return value_ <rhs.value_;
}

bool unc::operator ==(const unc& rhs) const
{
  return value_ == rhs.value_; // Might check ALL are equal?
}

unc& unc::operator=(const unc& rhs)
{
  value_ = rhs.value_;
  uncertainty_ = rhs.uncertainty_;
  deg_free_ = rhs.deg_free_;
  types_ = rhs.types_ ; 
  return *this; //! to make chainable.
}


double unc::value() //!< Get most likely value, typically the mean.
{
  return value_;
}

float unc::uncertainty() //!< Get estimate of uncertainty, typically standard deviation.
{
  return uncertainty_;
}

short unsigned unc::deg_free() //!< Get degrees of freedom, usually = number of observations -1;
{
  return deg_free_;
}

short unsigned unc::types() //!< Get other information about the value.
{
  return types_;
}

void unc::value(double v)//!< Set most likely value, typically the mean.
{
  value_ = v;
}

void unc::uncertainty(float u)//!< Set estimate of uncertainty, typically standard deviation.
{
  uncertainty_ = u;
}
void unc::deg_free(short unsigned df) //!< Set degrees of freedom, usually = number of observations -1;
{
  deg_free_ = df;
}

void unc::types(short unsigned  t) //!< Set other information about the value.
{
  types_ = t;
}

ostream& operator<< (ostream& os, const unc& u)
{ //! Output an value with (if defined )uncertainty and degrees of freedom.
  //! For example: "1.23 +/- 0.01 (13)".
  os << u.value_;
  if (u.uncertainty_ > 0)
  { // Uncertainty is defined, so output it.
    //os << char(241)  //! 256 bit codepage plusminus symbol 0xF1.
    os << '\361'  //! 256 bit codepage plusminus symbol 0xF1.
      << u.uncertainty_; 
    //os << " +or-" << u.uncertainty_; 
    // os <<  "&#x00A0;&#x00B1;" //! Unicode space plusminus glyph.
  };
  if (u.deg_free_ != (std::numeric_limits<unsigned short int>::max)())
  { // Degrees of freedom is defined, so output it.
    os << " (" << u.deg_free_ << ")";
  };
  return os;
} // ostream& operator<< (ostream& os, const unc& u)

ostream& operator<< (ostream& os, const std::pair<unc, unc>& u)
{ //! Output a pair (X and Y) value with (if defined) uncertainty and degrees of freedom.
  //! For example: "1.23 +/- 0.01 (13), 3.45 +/- 0.06 (78)".
  os << u.first << ", " << u.second;
  return os;
} // ostream& operator<< (ostream& os, const pair<unc, unc>& u)

} // namespace svg
} // namespace boost
#endif // BOOST_SVG_UNCERTAIN_HPP
