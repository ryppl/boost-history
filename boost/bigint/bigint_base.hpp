#ifndef BIGINT_BASE_RG09212003_HPP
#define BIGINT_BASE_RG09212003_HPP

//
// bigint_gmp.hpp - an arbitrary magnitude integer type. This code uses the same
// interface as bigint.hpp, though it is implemented using the GNU MP library
//

#include <iostream>
#include <iomanip>
#include <cassert>
#include <string>
#include <cctype>
#include <stdexcept>
#include <vector>

namespace boost {
namespace detail {
namespace bigint {

template <typename Derived>
class bigint_base {

  Derived& derived() {
    return static_cast<Derived&>(*this);
  }

  Derived const& derived() const {
    return static_cast<Derived const&>(*this);
  }

public:
  enum base_type {octal=8, decimal=10, hexadecimal=16};

  struct bad_value : public std::runtime_error {
    bad_value(std::string const& number) :
      std::runtime_error("Bad value: " + number) { }
  };
  
protected:  

  template <typename T>
  friend std::istream& operator>>(std::istream& is, bigint_base<T>& rhs);

  std::istream& from_istream(std::istream& is) {
    using std::ios_base;
    ios_base::iostate err = ios_base::goodbit;

    if(!is.good()) return is;

    try {
      // Manage prefix and postfix operations (RAII object)
      std::istream::sentry ipfx(is);
    
      if(ipfx) {
        ios_base::fmtflags flags = is.flags();
        base_type base;
        if (flags & ios_base::dec) {
          base = decimal;
        } else if (flags & ios_base::hex) {
          base = hexadecimal;
        } else { // if (flags & ios_base::oct)
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
          is.setstate(std::ios::failbit);
        } else {
          str.push_back(c);
          
          // Don't let failbit exceptions propogate for the following loop
          ios_base::iostate user_exceptions = is.exceptions();
          is.exceptions(user_exceptions & (~ios_base::failbit));
          // read in ASCII (hexadecimal) digits till it stops.
          while(is.get(c) && isxdigit(c))
            str.push_back(c);
          if(is.fail())
            // clear error state
            is.clear(is.rdstate() & ~ios_base::failbit);
          else
            // put back the last character retrieved
            is.putback(c);
          // restore user exceptions
          is.exceptions(user_exceptions);
          this->derived() = Derived(str,base);
        }
      }
    } catch (std::bad_alloc& ) {
      // Error handling code taken from Kreft & Langer
      err |= ios_base::badbit;
      ios_base::iostate exception_mask = is.exceptions();

      if ((exception_mask & ios_base::failbit) &&
          !(exception_mask & ios_base::badbit)) {
        is.setstate(err);
      } else if (exception_mask & ios_base::badbit) {
        // set bad_bit, but rethrow the original exception
        try { is.setstate(err); }
        catch (ios_base::failure&) { }
        throw;
      }
    } catch (...) {
      err |= ios_base::failbit;
      ios_base::iostate exception_mask = is.exceptions();

      if ((exception_mask & ios_base::badbit) &&
          (err & ios_base::badbit)) {
        is.setstate(err);
      } else if (exception_mask & ios_base::failbit) {
        // set fail_bit, but rethrow the original exception
        try { is.setstate(err); }
        catch ( ios_base::failure& ) { }
        throw;
      }
    }
    if(err != ios_base::goodbit) is.setstate(err);
    return is;
  }

};

template <typename T>
inline
std::istream& operator>>(std::istream& is, bigint_base<T>& rhs) {
  return rhs.from_istream(is);
}

} // namespace bigint
} // namespace detail
} // namespace boost


#endif // BIGINT_BASE_RG09212003_HPP
