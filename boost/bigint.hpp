#ifndef BIGINT_RG08272002_HPP
#define BIGINT_RG08272002_HPP

//
// bigint.hpp - an arbitrary magnitude integer type
//

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
  // radix of 10,000 for easy conversion to and from ascii strings.

  // Bigits (base 10,000 digits) are stored in reverse order (most significant
  // bigits at the end of the vector) to take advantage of std::vector's fast
  // pushing and popping at the end of the vector.

  // Summary: Positive numbers are stored obviously as base 10,000
  // numbers. Negative numbers are stored in complement form, with -1 as the
  // most significant bigit.

class bigint : boost::operators<bigint> {
  BOOST_STATIC_CONSTANT(int, radix = 10000);
  enum { chars_per_digit = 4}; // RG: gcc3 was giving trouble...
  typedef std::vector<int> buffer_t;
  buffer_t buffer;

  friend std::ostream& operator<<(std::ostream& os, bigint const& rhs);
  friend std::istream& operator>>(std::istream& is, bigint& rhs);

  std::ostream& print(std::ostream& os) const {

    if(!os.good()) return os;

    // Is the bigint negative?
    if(buffer.back() == -1)
      return os << '-' << -(*this);
    
    for(buffer_t::const_reverse_iterator i = buffer.rbegin();
	i != buffer.rend(); ++i) {
      assert(*i < radix);
      if(i != buffer.rbegin())
	os << std::setw(chars_per_digit) << std::setfill('0');
      os << *i;
    }
    return os;
  }


  std::istream& from_istream(std::istream& is) {

    if(!is.good()) return is;
    
    // Read into an std::string, then set.
    std::string str;

    // skip whitespace
    char c;
    is >> c;
    // check for minus sign.
    if(!(c == '-' || std::isdigit(c))) {
      is.putback(c);
      // signal error
      return is;
    } else {
      str.push_back(c);

      // read in ASCII digits till it stops.
      while(is.get(c) && isdigit(c))
	str.push_back(c);
      is.putback(c);
      *this = bigint(str);
      return is;
    }
  }

  friend bigint operator-(bigint const& rhs);

  bigint& negate() {
    std::transform(buffer.begin(),buffer.end(),buffer.begin(),
		   std::negate<int>());
    normalize();
    return *this;
  }


  void carry(buffer_t& buffer) {
    // fix carries - \forall i \in buffer (i < radix )
    int carry = 0;
    for(buffer_t::iterator i = buffer.begin();
	i != buffer.end(); ++i) {

      int new_carry = integral_div((carry + *i),radix);
      *i = integral_mod((carry + *i),radix);
      carry = new_carry;
    }
    if(carry)
      buffer.push_back(carry);
  }

  void normalize() {

    // Adjust carries.
    carry(buffer);

    // remove excess leading zeros
    while(!buffer.empty() && buffer.back() == 0)
      buffer.pop_back();
    if (buffer.empty())
      buffer.push_back(0);
  }


  void align_to(bigint const& other) {
    if(buffer.size() < other.buffer.size())
      buffer.resize(other.buffer.size(),0);
    // postcondition: buffer.size() >= other.buffer.size()
  }

  template <typename OP>
  bigint& operate(bigint const& other, OP op) {
    align_to(other);

    // other.buffer.size() <= buffer.size() 
    std::transform(other.buffer.begin(),other.buffer.end(),
		   buffer.begin(),
		   buffer.begin(),op);

    normalize();

    return *this;
  }


  // integral ops:
  //  any integer x can be represented as: x = qy + r. Then:
  //   - integral_div(x,y) = q
  //   - integral_mod(x,y) = r
  //  where r has the same sign as y.

  int integral_div(int lhs, int rhs) {
    return (lhs - integral_mod(lhs,rhs)) / rhs;
  }

  // This is adapted from the Tcl library source code.
  int integral_mod(int lhs, int rhs) {
    
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
  
  void from_string(std::string const& str) {
    assert(!str.empty());
    assert(str.find_first_not_of("-0123456789") == std::string::npos);
    assert(str[0] != '-' ||
	   str.find_first_not_of("0123456789",1) == std::string::npos);
    assert(str[0] != '-' || str.size() > 1);

    std::string::const_iterator start = str.begin();

    bool negative = str[0] == '-';
    if(negative) 
      ++start;

    std::string::size_type size = std::distance(start,str.end());
    buffer.clear();
    buffer.resize( (size + chars_per_digit - 1)/chars_per_digit, 0 );

    // how many chars x go with the "first" bunch? (1 <= x <= 4)
    int trailing_digits = size % chars_per_digit;
    int first_marker = trailing_digits ? trailing_digits : chars_per_digit; 

    // invariant: [i,j) refers to the next "digit" to be processed
    // p points to the next bigit to be filled
    buffer_t::reverse_iterator p = buffer.rbegin();
    std::string::const_iterator i = start;
    std::string::const_iterator j = start + first_marker;
    while (i != str.end()) {
      int digit = 0;
      while (i != j) {
	digit *= 10;
	digit += *i - '0';
	++i;
      }
      *p++ = digit;
      if(i != str.end())
	 j += chars_per_digit;
    }

    if(negative)
      negate();
  }


  void quotient_remainder(bigint const& other,
			  bigint& quotient,
			  bigint& remainder) {

    bigint xs(*this);
    bigint ys(other);
    
    int d = integral_div(radix,ys.buffer.back()+1);
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
	  q = integral_div(xxs.buffer.back() *
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
  }

  bigint& bdiv(int const& other) {

    // generate rs's
    int r = 0;
    for(buffer_t::reverse_iterator i = buffer.rbegin();
	i != buffer.rend(); ++i) {
      r = radix * integral_mod(r,other) + *i;
      *i = r;
    }
    for(buffer_t::reverse_iterator i = buffer.rbegin();
	i != buffer.rend(); ++i) {
      *i = integral_div(*i,other);
    }    
    normalize();
    return *this;
  }

public:
  
  bigint() : buffer(1,0) { }

  explicit bigint(std::string const& str) {
    from_string(str);
  }

  bigint(long value) {
    std::ostringstream os;
    os << value;
    from_string(os.str());
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
    this->is_zero();
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
  return rhs.print(os);
}

inline
std::istream& operator>>(std::istream& is, bigint& rhs) {
  return rhs.from_istream(is);
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
