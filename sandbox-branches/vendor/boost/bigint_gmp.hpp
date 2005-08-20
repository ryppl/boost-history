#ifndef BIGINT_GMP_RG08272003_HPP
#define BIGINT_GMP_RG08272003_HPP

// Use, modification and distribution is subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Bigint Library
//  Authors: Ronald Garcia
//           Andrew Lumsdaine

//
// bigint_gmp.hpp - an arbitrary magnitude integer type. This code uses the same
// interface as bigint.hpp, though it is implemented using the GNU MP library
//

#include "gmp.h"
#include "boost/bigint/bigint_base.hpp"
#include "boost/config.hpp"
#include "boost/operators.hpp"
#include <iostream>
#include <iomanip>
#include <cassert>
#include <string>
#include <cctype>
#include <stdexcept>
#include <vector>

namespace boost {

class bigint : public boost::detail::bigint::bigint_base<bigint>, // for op>>
               boost::operators<bigint> {
  mpz_t gmp_value_;

  friend std::ostream& operator<<(std::ostream& os, bigint const& rhs);
  std::ostream& to_ostream(std::ostream& os) const {
    
    if(!os.good()) return os;

    std::ostream::sentry opfx(os);

    if(opfx) {
    
      std::ios_base::fmtflags flags = os.flags();

      std::string format("%");
      if(flags & std::ios_base::showbase) format.push_back('#');
      if(flags & std::ios_base::showpos) format.push_back('+');

      format.push_back('Z');

      int base;
      if (flags & std::ios_base::dec) {
        base = 10;
        format.push_back('d');
      } else if (flags & std::ios_base::hex) {
        base = 16;
        if(flags & std::ios_base::uppercase)
          format.push_back('X');
        else
          format.push_back('x');
      } else {// if (flags & std::ios_base::oct)
        base = 8;
        format.push_back('o');
      }

      // Copy the string representation and stream it.
      std::vector<char> buffer;
      int const extra_chars = 4; // 4 -> "-0x"
      int length = mpz_sizeinbase(gmp_value_,base) + extra_chars; 
      buffer.resize(length); 
      (void)gmp_snprintf(&buffer[0],length,format.c_str(),gmp_value_);
      os << &buffer[0];
    }
    return os;
  }

  friend bigint operator-(bigint const& rhs);
  bigint& negate() {
    mpz_neg(gmp_value_,gmp_value_);
    return *this;
  }
  
  
public:

  typedef boost::detail::bigint::bigint_base<bigint>::base_type base_type;

  bigint() {
    mpz_init(gmp_value_);
  }
  
  explicit bigint(std::string const& str, base_type base = decimal) {
    
    // GMP only recognizes "0x" if you auto-detect bases.
    char const* p = str.c_str();
    if(base == hexadecimal && str.size() >= 2 &&
       p[0] == '0' && tolower(p[1]) == 'x')
      p += 2;
    if(mpz_init_set_str(gmp_value_,p,base) < 0) {
      mpz_clear(gmp_value_);
      throw bigint::bad_value(str);
    }
  }
  
  bigint(long value) {
    mpz_init_set_si(gmp_value_,value);
  }
  
  bigint(const bigint& other) {
    mpz_init_set(gmp_value_,other.gmp_value_);
  }

  bigint& operator=(const bigint& other) {
    if (&other != this) {
      mpz_init_set(gmp_value_,other.gmp_value_);
    }
  }

  ~bigint() {
    mpz_clear(gmp_value_);
  }


  bool is_zero() const {
    return mpz_sgn(gmp_value_) == 0;
  }
    
  bool positive() const {
    return mpz_sgn(gmp_value_) > 0;
  }

  bool negative() const {
    return mpz_sgn(gmp_value_) < 0;
  }

  bool operator!() const {
    return this->is_zero();
  }
  
  bool operator<(bigint const& other) const {
    return mpz_cmp(gmp_value_,other.gmp_value_) < 0;
  }

  bool operator==(bigint const& other) const {
    return mpz_cmp(gmp_value_,other.gmp_value_) == 0;
  }

  bigint& operator+=(bigint const& other) {
    mpz_add(gmp_value_,gmp_value_,other.gmp_value_);
    return *this;
  }

  bigint& operator-=(bigint const& other) {
    mpz_sub(gmp_value_,gmp_value_,other.gmp_value_);
    return *this;
  }

  bigint& operator*=(bigint const& other) {
    mpz_mul(gmp_value_,gmp_value_,other.gmp_value_);
    return *this;
  }

  bigint& operator/=(bigint const& other) {

    mpz_tdiv_q(gmp_value_,gmp_value_,other.gmp_value_);
    return *this;
  }

  bigint& operator%=(bigint const& other) {
    mpz_tdiv_r(gmp_value_,gmp_value_,other.gmp_value_);
    return *this;
  }

  bigint& operator++() {
    mpz_add_ui(gmp_value_,gmp_value_,1);
    return *this;
  }

  bigint& operator--() {
    mpz_sub_ui(gmp_value_,gmp_value_,1);
    return *this;
  }

  void swap(bigint& other) {
    mpz_swap(gmp_value_,other.gmp_value_);
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


#endif // BIGINT_GMP_RG08272003_HPP
