#ifndef BIGINT_GMP_RG08272003_HPP
#define BIGINT_GMP_RG08272003_HPP

//
// bigint_gmp.hpp - an arbitrary magnitude integer type. This code uses the same
// interface as bigint.hpp, though it is implemented using the GNU MP library
//

#include "gmp.h"
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

class bigint : boost::operators<bigint> {
  mpz_t gmp_value_;

  friend std::ostream& operator<<(std::ostream& os, bigint const& rhs);
  friend std::istream& operator>>(std::istream& is, bigint& rhs);
  
  std::ostream& to_ostream(std::ostream& os) const {
    
    if(!os.good()) return os;

    std::ostream::sentry opfx(os);

    if(opfx) {
    
      std::ios_base::fmtflags flags = os.flags();

      std::string format("%");
      if(flags & std::ios_base::showbase) format.push_back('#');
      if(flags & std::ios_base::showpos) format.push_back('+');

      format.push_back('Z');

      if (flags & std::ios_base::dec) {
        format.push_back('d');
      } else if (flags & std::ios_base::hex) {
        if(flags & std::ios_base::uppercase)
          format.push_back('X');
        else
          format.push_back('x');
      } else {// if (flags & std::ios_base::oct)
        format.push_back('o');
      }

      // Copy the string representation and stream it.
      std::vector<char> buffer;
      int length = gmp_snprintf(&buffer[0],0,format.c_str(),gmp_value_);
      buffer.resize(length+1);
      int length2 =
        gmp_snprintf(&buffer[0],length+1,format.c_str(),gmp_value_);
      assert(length == length2);
      os << &buffer[0];
    }
    return os;
  }
  
  std::istream& from_istream(std::istream& is) {

    if(!is.good()) return is;

    // Manage prefix and postfix operations (RAII object)
    std::istream::sentry ipfx(is);
    
    if(ipfx) {
      std::ios_base::fmtflags flags = is.flags();
      base_type base;
      if (flags & std::ios_base::dec) {
        base = decimal;
      } else if (flags & std::ios_base::hex) {
        base = hexadecimal;
      } else { // if (flags & std::ios_base::oct)
        base = octal;
      }

      // Read the number into a string, then initialize from that.
      std::string str;
      char c;

      // check for minus sign.
      is.get(c);
      if(!(c == '-' || std::isxdigit(c))) {
        is.putback(c);
        // signal error
        is.clear(is.rdstate() | std::ios::failbit);
      } else {
        str.push_back(c);

        // read in ASCII (hexadecimal) digits till it stops.
        while(is.get(c) && isxdigit(c))
          str.push_back(c);
        if(is.fail())
          // clear error state
          is.clear();
        else
          // put back the last character retrieved
          is.putback(c);
        *this = bigint(str,base);
      }
    }
    return is;
  }


  friend bigint operator-(bigint const& rhs);
  bigint& negate() {
    mpz_neg(gmp_value_,gmp_value_);
    return *this;
  }
  
  
public:
  enum base_type {octal=8, decimal=10, hexadecimal=16};
  
  bigint() {
    mpz_init(gmp_value_);
  }
  
  explicit bigint(std::string const& str, base_type base = decimal) {
    if(mpz_init_set_str(gmp_value_, str.c_str(),base) < 0) {
      mpz_clear(gmp_value_);
      throw std::runtime_error("Bad value");
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


#endif // BIGINT_GMP_RG08272003_HPP
