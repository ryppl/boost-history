#ifndef BIGINT_RG08272002_HPP
#define BIGINT_RG08272002_HPP

// Use, modification and distribution is subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Bigint Library
//  Authors: Ronald Garcia
//           Andrew Lumsdaine

//
// bigint.hpp - an arbitrary magnitude integer type
//

#include "boost/bigint/bigint_base.hpp"
#include "boost/config.hpp"
#include "boost/operators.hpp"
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <cassert>
#include <string>
#include <sstream>
#include <cctype>

namespace boost {


  // bigint is based on information found in The Art of Computer Programming
  // Volume 2: Seminumerical Algorithms, Chapter 4.3 by Donald Knuth.  

  // The implementation uses std::vector<int> as its buffer format and a fixed
  // radix of 10,000, which was used early on for easy conversion to 
  // and from ascii strings, but is now subject to change.

  // Bigits (base 10,000 digits) are stored in reverse order (most significant
  // bigits at the end of the vector) to take advantage of std::vector's fast
  // pushing and popping at the end of the vector.

  // Summary: Positive numbers are stored obviously as base 10,000
  // numbers. Negative numbers are stored in complement form, with -1 as the
  // most significant bigit.

class bigint : public boost::detail::bigint::bigint_base<bigint>, // for op>>
               boost::operators<bigint> {
  BOOST_STATIC_CONSTANT(int, radix = 10000);
  typedef std::vector<int> buffer_t;
  buffer_t buffer;

  friend std::ostream& operator<<(std::ostream& os, bigint const& rhs);

  std::ostream& to_ostream(std::ostream& os) const {
    using std::ios_base;
    ios_base::iostate err = ios_base::goodbit;

    if(!os.good()) return os;

    try {
      // Manage prefix and postfix operations (RAII object)
      std::ostream::sentry opfx(os);
    
      if(opfx) {
        // Build the string representation, then stream it out.
        std::string stringrep;
        ios_base::fmtflags flags = os.flags();

        if(this->negative()) {
          std::ostringstream ostr;
          ostr.flags(flags);
          ostr << '-' << std::noshowpos;
          (-(*this)).to_ostream(ostr);
          stringrep += ostr.str();
        } else {
          if(flags & ios_base::showpos) 
            stringrep.push_back('+');
        
          // Add base representation if needed
          if (flags & ios_base::showbase) {
            if (flags & ios_base::hex) 
              stringrep += ((flags & ios_base::uppercase) ? "0X" : "0x");
            else if (flags & ios_base::oct)
              stringrep.push_back('0');
          }
        
          // Valid digits for each base
          static char const decimals[] = {'0','1','2','3','4','5',
                                          '6','7','8','9'};
          static char const lowerhex[] = {'0','1','2','3','4','5','6','7','8',
                                          '9','a','b','c','d','e','f'};
          static char const upperhex[] = {'0','1','2','3','4','5','6','7','8',
                                          '9','A','B','C','D','E','F'};
          static char const octals[] = {'0','1','2','3','4','5','6','7'};
        
          int base;
          char const* digits;
          if (flags & ios_base::dec) {
            base = 10;
            digits = decimals;
          } else if (flags & ios_base::hex) {
            base = 16;
            digits = (flags & ios_base::uppercase) ? upperhex : lowerhex;
          } else { // if (flags & ios_base::oct)
            base = 8;
            digits = octals;
          }
          std::vector<char> text;
          // RG - calculate number of characters and reserve that space.
          bigint quotient = *this;
          bigint remainder;
          quotient.quotient_remainder(bigint(base),quotient,remainder);
          // invariant: assert(radix > 16) -> 
          //    'remainder.buffer.back()' is the remainder.
          // invariant: remainder < length of digits.
          text.push_back(digits[remainder.buffer.back()]);
          while (!quotient.is_zero()) {
            quotient.quotient_remainder(bigint(base),quotient,remainder);
            text.push_back(digits[remainder.buffer.back()]);
          }
#if 0 // Why does this fail under c++ (GCC) 3.1 20020420 (prerelease)?
          stringrep.append(text.rbegin(),text.rend());
#else
          std::copy(text.rbegin(),text.rend(),std::back_inserter(stringrep));
#endif
        }
        os << stringrep;
      }
    } catch (std::bad_alloc& ) {
      // Error handling code taken from Kreft & Langer
      err |= ios_base::badbit;
      ios_base::iostate exception_mask = os.exceptions();

      if ((exception_mask & ios_base::failbit) &&
          !(exception_mask & ios_base::badbit)) {
        os.setstate(err);
      } else if (exception_mask & ios_base::badbit) {
        // set bad_bit, but rethrow the original exception
        try { os.setstate(err); }
        catch (ios_base::failure&) { }
        throw;
      }
    } catch (...) {
      err |= ios_base::failbit;
      ios_base::iostate exception_mask = os.exceptions();

      if ((exception_mask & ios_base::badbit) &&
          (err & ios_base::badbit)) {
        os.setstate(err);
      } else if (exception_mask & ios_base::failbit) {
        // set fail_bit, but rethrow the original exception
        try { os.setstate(err); }
        catch ( ios_base::failure& ) { }
        throw;
      }
    }
    if(err != ios_base::goodbit) os.setstate(err);
    return os; 
  }

  friend bigint operator-(bigint const& rhs);

  bigint& negate() {
    std::transform(buffer.begin(),buffer.end(),buffer.begin(),
		   std::negate<int>());
    this->normalize();
    return *this;
  }


  // Fix carries -> ensure that \forall i \in buffer (i < radix )
  void carry(buffer_t& buffer) {

    int carry = 0;
    for(buffer_t::iterator i = buffer.begin();
	i != buffer.end(); ++i) {

      int new_carry = floor_div((carry + *i),radix);
      *i = floor_mod((carry + *i),radix);
      carry = new_carry;
    }
    if(carry)
      buffer.push_back(carry);
  }

  void normalize() {

    // Adjust carries.
    this->carry(buffer);

    // Remove excess leading zeros (Though a true zero is one 0-valued bigit)
    while(buffer.size() > 1 && buffer.back() == 0)
      buffer.pop_back();

  }


  void align_to(bigint const& other) {
    if(buffer.size() < other.buffer.size())
      buffer.resize(other.buffer.size(),0);
    // postcondition: buffer.size() >= other.buffer.size()
  }

  template <typename OP>
  bigint& operate(bigint const& other, OP op) {
    this->align_to(other);

    // other.buffer.size() <= buffer.size() 
    std::transform(other.buffer.begin(),other.buffer.end(),
		   buffer.begin(),
		   buffer.begin(),op);

    this->normalize();

    return *this;
  }


  // Any integer x can be represented as: x = qy + r, 0 <= |r| < |d|. 
  // Then:
  //   - floor_div(x,y) = q
  //   - floor_mod(x,y) = r.
  // Specifically, this is "floor" division:
  // the quotient q is equal to the "x/y" rounded towards negative infinity
  // (the floor function). The remainder r always has the same sign as y.
  int floor_div(int lhs, int rhs) {
    return (lhs - floor_mod(lhs,rhs)) / rhs;
  }

  int floor_mod(int lhs, int rhs) {
    
    bool negative = false;
    if (rhs < 0) {
      rhs = -rhs;
      lhs = -lhs;
      negative = true;
    }
    
    int rem = lhs % rhs;
    if (rem < 0) {
      rem += rhs;
    }
    
    if (negative) {
      rem = -rem;
    }
    
    return rem;
  }

public:
  typedef boost::detail::bigint::bigint_base<bigint>::base_type base_type;
private:

  void from_string(std::string const& str, base_type a_base) {

    // check(bool) throws bad_value on a failed assertion.
    struct {
      std::string const& str_;
      void operator()(bool b) {
        if (!b) throw bigint::bad_value(str_);
      }
    } check = { str };

    // Verify the goodness of the number
    char const* valid_octals = "01234567";
    char const* valid_decimals = "0123456789";
    char const* valid_hexadecimals = "0123456789abcdefABCDEF";

    char const* valid;
    int factor;
    switch(a_base) {
    case octal:
      valid = valid_octals;
      factor = 8;
      break;
    case decimal:
      valid = valid_decimals;
      factor = 10;
      break;
    case hexadecimal:
      valid = valid_hexadecimals;
      factor = 16;
      break;
    default:
      assert(false);
      break;
    }

    //
    // String Validation
    //
    std::string::size_type idx = 0;

    // Negative number?
    bool negative = false;
    if(!str.empty() && str[0] == '-') {
      negative = true;
      ++idx;
    }

    // Accept hexadecimal base markers.
    if(a_base == hexadecimal && str.size() >= idx+2 &&
       str[idx] == '0' && tolower(str[idx+1]) == 'x')
      idx += 2;

    // Error checking: Empty strings are invalid!
    check(str.size()-idx > 0);
    check(str.find_first_not_of(valid,idx) == std::string::npos);


    // Heart of the algorithm.
    std::string::const_iterator start = str.begin()+idx;

    buffer.clear();
    buffer.push_back(0);

    std::string::const_iterator i = start;
    while (i != str.end()) {
      for(buffer_t::iterator j = buffer.begin(); j != buffer.end(); ++j)
        *j *= factor;
      buffer.front() += isdigit(*i) ? *i - '0' : 10 + tolower(*i) - 'a';
      this->normalize();
      ++i;
    }

    if(negative)
      this->negate();
  }


  void quotient_remainder(bigint const& other,
			  bigint& quotient,
			  bigint& remainder) {

    // WARNING: either "other" or "this" may alias
    // with "quotient" or "remainder"
    bigint xs(*this);
    bigint ys(other);


    // This algorithm works specifically for non-negative numbers. 
    // Quotient and remainder signs are fixed at the end.
    bool const negative_dividend = this->negative();
    bool const negative_divisor = other.negative();

    if (negative_dividend)
      xs.negate();

    if(negative_divisor)
      ys.negate();

    int d = floor_div(radix,ys.buffer.back()+1);
    xs *= bigint(d);
    ys *= bigint(d);

    buffer_t& quotients = quotient.buffer;
    quotients.resize(xs.buffer.size());

    remainder = 0;

    // for each next bigit:
    // invariants:
    //  quotients holds the first i quotients
    //  remainder holds the ith remainder

    buffer_t::reverse_iterator p = quotients.rbegin();
    for(buffer_t::reverse_iterator i = xs.buffer.rbegin();
	i != xs.buffer.rend(); ++i) {
      bigint xxs = remainder * bigint(radix) + bigint(*i);
      if(xxs.buffer.size() < ys.buffer.size()) {
	*p++ = 0;
	remainder = xxs;
      } else if (xxs.buffer.size() == ys.buffer.size()) {
	bigint zs = xxs - ys;
	if (zs < bigint(0)) {
	  *p++ = 0;
	  remainder = xxs;
	} else {
	  *p++ = 1;
	  remainder = zs;
	}
      } else { // xxs.buffer.size() > ys.buffer.size()

	// Guess a value for q, based on Knuth, Volume 2:Section 4.3.1
	int q;
	if(xxs.buffer.back() >= ys.buffer.back()) 
	  q = radix-1;
	else
	  q = floor_div(xxs.buffer.back() *
			     radix + *(++xxs.buffer.rbegin()),
			   ys.buffer.back());

	bigint rs = xxs - ys * bigint(q);
	while (rs >= ys) {
	  ++q;
	  rs -= ys;
	}
	*p++ = q;
	remainder = rs;
      }
    }
    quotient.normalize();

    if(remainder != bigint(0))
      remainder.bdiv(d);

    // Fix the quotient and remainder signs
    if(negative_dividend) remainder.negate();

    if((negative_dividend && !negative_divisor) ||
       (!negative_dividend && negative_divisor))
      quotient.negate();
  }

  bigint& bdiv(int const& other) {

    // generate rs's
    int r = 0;
    for(buffer_t::reverse_iterator i = buffer.rbegin();
	i != buffer.rend(); ++i) {
      r = radix * floor_mod(r,other) + *i;
      *i = r;
    }
    for(buffer_t::reverse_iterator i = buffer.rbegin();
	i != buffer.rend(); ++i) {
      *i = floor_div(*i,other);
    }    
    normalize();
    return *this;
  }

public:

    
  bigint() : buffer(1,0) { }

  explicit bigint(std::string const& str, base_type a_base=decimal) {
    from_string(str,a_base);
  }

  bigint(long value) {
    std::ostringstream os;
    os << value;
    from_string(os.str(),decimal);
  }

  bool is_zero() const {
    return buffer.size() == 1 && buffer.back() == 0;
  }

  bool negative() const {
    return buffer.back() == -1;
  }

  bool positive() const {
    return !(this->negative() || this->is_zero());
  }

  bool operator!() const {
    return this->is_zero();
  }

  bool operator<(bigint const& other) const {
    if(buffer.size() == other.buffer.size())
      return
	std::lexicographical_compare(buffer.rbegin(),buffer.rend(),
	  other.buffer.rbegin(),other.buffer.rend());
    else if(buffer.back() < 0 && other.buffer.back() >= 0)
      return true;
    else if(buffer.back() >= 0 && other.buffer.back() < 0)
      return false;
    else if(buffer.back() < other.buffer.back())
      return true;
    else return false;      
  }

  bool operator==(bigint const& other) const {
    if(buffer.size() != other.buffer.size())
      return false;
    else
      return
	std::equal(buffer.begin(),buffer.end(),other.buffer.begin());
  }

  bigint& operator+=(bigint const& other) {
    return operate(other, std::plus<int>());
  }

  struct subtract_backwards {
    int operator()(int lhs, int rhs) {
      return rhs - lhs;
    }
  };
  bigint& operator-=(bigint const& other) {
    return operate(other, subtract_backwards());
  }

  bigint& operator*=(bigint const& other) {
    buffer_t term(buffer.size());
    buffer_t partial_sum(buffer.size()+other.buffer.size(),0);
    buffer_t::iterator shift = partial_sum.begin();
    buffer_t::const_iterator m = other.buffer.begin();
    while(m != other.buffer.end()) {
      // multiply every bigit of this.buffer into term
      std::transform(buffer.begin(),buffer.end(),term.begin(),
		     std::bind2nd(std::multiplies<int>(),*m));
      
      // add to partial_sum, properly shifted
      std::transform(term.begin(),term.end(),shift,shift,
		     std::plus<int>());
      
      carry(partial_sum);
      ++shift;
      ++m;
    }
    
    buffer = partial_sum;
    normalize();
    return *this;
  }

  bigint& operator/=(bigint const& other) {

    bigint rem;
    quotient_remainder(other,*this,rem);
    return *this;
  }

  bigint& operator%=(bigint const& other) {

    bigint quotient;
    bigint remainder;
    quotient_remainder(other,quotient,remainder);
    *this = remainder;
    return *this;
  }

  bigint& operator++() {
    return *this += bigint(1);
  }

  bigint& operator--() {
    return *this -= bigint(1);
  }

  void swap(bigint& other) {
    buffer.swap(other.buffer);
  }
};


void swap(bigint& lhs, bigint& rhs) {
  lhs.swap(rhs);
}

inline
std::ostream& operator<<(std::ostream& os, bigint const& rhs) {
  return rhs.to_ostream(os);
}

inline
bigint operator-(bigint const& rhs) {
  bigint result(rhs);
  return result.negate();
}

inline
bigint operator+(bigint const& rhs) {
  return rhs;
}


} // namespace boost


#endif // BIGINT_RG08272002_HPP
