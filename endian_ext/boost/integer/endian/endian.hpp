//  Boost endian.hpp header file -------------------------------------------------------//

//  (C) Copyright Darin Adler 2000
//  (C) Copyright Beman Dawes 2006, 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/integer/endian

//--------------------------------------------------------------------------------------//

//  Original design developed by Darin Adler based on classes developed by Mark
//  Borgerding. Four original class templates were combined into a single endian
//  class template by Beman Dawes, who also added the unrolled_byte_loops sign
//  partial specialization to correctly extend the sign when cover integer size
//  differs from endian representation size.

//  Split of endian.hpp into endian.hpp  and endian_pack?hpp done by Vicente J. Botet Escriba.

// TODO: When a compiler supporting constexpr becomes available, try possible uses.

#ifndef BOOST_INTEGER_ENDIAN_HPP
#define BOOST_INTEGER_ENDIAN_HPP

#include <boost/integer/endian/endian_pack.hpp>


#if defined(__BORLANDC__) || defined( __CODEGEARC__)
# pragma pack(push, 1)
#endif

#include <boost/config.hpp>
#define BOOST_MINIMAL_INTEGER_COVER_OPERATORS
#define BOOST_NO_IO_COVER_OPERATORS
#include <boost/integer/endian/cover_operators.hpp>
#undef  BOOST_NO_IO_COVER_OPERATORS
#undef  BOOST_MINIMAL_INTEGER_COVER_OPERATORS
#include <boost/type_traits/is_signed.hpp>
#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>
#include <iosfwd>
#include <climits>
#include <limits>
#include <cstddef>

namespace boost
{

  namespace integer
  {

  template <typename E, 
        typename T, 
        std::size_t n_bits=sizeof(T)*8,
        BOOST_SCOPED_ENUM(alignment) A = alignment::aligned
    > class endian
      : cover_operators< endian< E, T, n_bits, A>, T >
    {
      endian_pack<E, T, n_bits, A> pack_;
        
      public:
        typedef E endian_type;
        typedef T value_type;
        BOOST_STATIC_CONSTEXPR std::size_t width = n_bits;
        BOOST_STATIC_CONSTEXPR BOOST_SCOPED_ENUM(alignment) alignment_value = A;
        
#     ifndef BOOST_ENDIAN_NO_CTORS
        endian() BOOST_ENDIAN_DEFAULT_CONSTRUCT
        template <typename T2>
        explicit endian(T2 val)
            : pack_(val)
        { 
        }
#     endif
        endian & operator=(T val) { 
            pack_=val; 
            return *this; 
        }
        operator T() const
        { 
          return T(pack_);
        }
        const char* data() const  { return pack_.data(); }

    };
        
  //  naming convention typedefs  ------------------------------------------------------//

    // unaligned big endian signed integer types
    typedef endian< big_endian, int_least8_t, 8, alignment::unaligned >           big8_t;
    typedef endian< big_endian, int_least16_t, 16, alignment::unaligned >         big16_t;
    typedef endian< big_endian, int_least32_t, 24, alignment::unaligned >         big24_t;
    typedef endian< big_endian, int_least32_t, 32, alignment::unaligned >         big32_t;
    typedef endian< big_endian, int_least64_t, 40, alignment::unaligned >         big40_t;
    typedef endian< big_endian, int_least64_t, 48, alignment::unaligned >         big48_t;
    typedef endian< big_endian, int_least64_t, 56, alignment::unaligned >         big56_t;
    typedef endian< big_endian, int_least64_t, 64, alignment::unaligned >         big64_t;

    // unaligned big endian unsigned integer types
    typedef endian< big_endian, uint_least8_t, 8, alignment::unaligned >          ubig8_t;
    typedef endian< big_endian, uint_least16_t, 16, alignment::unaligned >        ubig16_t;
    typedef endian< big_endian, uint_least32_t, 24, alignment::unaligned >        ubig24_t;
    typedef endian< big_endian, uint_least32_t, 32, alignment::unaligned >        ubig32_t;
    typedef endian< big_endian, uint_least64_t, 40, alignment::unaligned >        ubig40_t;
    typedef endian< big_endian, uint_least64_t, 48, alignment::unaligned >        ubig48_t;
    typedef endian< big_endian, uint_least64_t, 56, alignment::unaligned >        ubig56_t;
    typedef endian< big_endian, uint_least64_t, 64, alignment::unaligned >        ubig64_t;

    // unaligned little endian signed integer types
    typedef endian< little_endian, int_least8_t, 8, alignment::unaligned >        little8_t;
    typedef endian< little_endian, int_least16_t, 16, alignment::unaligned >      little16_t;
    typedef endian< little_endian, int_least32_t, 24, alignment::unaligned >      little24_t;
    typedef endian< little_endian, int_least32_t, 32, alignment::unaligned >      little32_t;
    typedef endian< little_endian, int_least64_t, 40, alignment::unaligned >      little40_t;
    typedef endian< little_endian, int_least64_t, 48, alignment::unaligned >      little48_t;
    typedef endian< little_endian, int_least64_t, 56, alignment::unaligned >      little56_t;
    typedef endian< little_endian, int_least64_t, 64, alignment::unaligned >      little64_t;

    // unaligned little endian unsigned integer types
    typedef endian< little_endian, uint_least8_t, 8, alignment::unaligned >       ulittle8_t;
    typedef endian< little_endian, uint_least16_t, 16, alignment::unaligned >     ulittle16_t;
    typedef endian< little_endian, uint_least32_t, 24, alignment::unaligned >     ulittle24_t;
    typedef endian< little_endian, uint_least32_t, 32, alignment::unaligned >     ulittle32_t;
    typedef endian< little_endian, uint_least64_t, 40, alignment::unaligned >     ulittle40_t;
    typedef endian< little_endian, uint_least64_t, 48, alignment::unaligned >     ulittle48_t;
    typedef endian< little_endian, uint_least64_t, 56, alignment::unaligned >     ulittle56_t;
    typedef endian< little_endian, uint_least64_t, 64, alignment::unaligned >     ulittle64_t;

    // unaligned native endian signed integer types
    typedef endian< native_endian, int_least8_t, 8, alignment::unaligned >        native8_t;
    typedef endian< native_endian, int_least16_t, 16, alignment::unaligned >      native16_t;
    typedef endian< native_endian, int_least32_t, 24, alignment::unaligned >      native24_t;
    typedef endian< native_endian, int_least32_t, 32, alignment::unaligned >      native32_t;
    typedef endian< native_endian, int_least64_t, 40, alignment::unaligned >      native40_t;
    typedef endian< native_endian, int_least64_t, 48, alignment::unaligned >      native48_t;
    typedef endian< native_endian, int_least64_t, 56, alignment::unaligned >      native56_t;
    typedef endian< native_endian, int_least64_t, 64, alignment::unaligned >      native64_t;

    // unaligned native endian unsigned integer types
    typedef endian< native_endian, uint_least8_t, 8, alignment::unaligned >       unative8_t;
    typedef endian< native_endian, uint_least16_t, 16, alignment::unaligned >     unative16_t;
    typedef endian< native_endian, uint_least32_t, 24, alignment::unaligned >     unative24_t;
    typedef endian< native_endian, uint_least32_t, 32, alignment::unaligned >     unative32_t;
    typedef endian< native_endian, uint_least64_t, 40, alignment::unaligned >     unative40_t;
    typedef endian< native_endian, uint_least64_t, 48, alignment::unaligned >     unative48_t;
    typedef endian< native_endian, uint_least64_t, 56, alignment::unaligned >     unative56_t;
    typedef endian< native_endian, uint_least64_t, 64, alignment::unaligned >     unative64_t;

#define BOOST_HAS_INT16_T
#define BOOST_HAS_INT32_T
#define BOOST_HAS_INT64_T
  
  //  These types only present if platform has exact size integers:
  //     aligned big endian signed integer types
  //     aligned big endian unsigned integer types
  //     aligned little endian signed integer types
  //     aligned little endian unsigned integer types

  //     aligned native endian typedefs are not provided because
  //     <cstdint> types are superior for this use case

# if defined(BOOST_HAS_INT16_T)
    typedef endian< big_endian, int16_t, 16, alignment::aligned >      aligned_big16_t;
    typedef endian< big_endian, uint16_t, 16, alignment::aligned >     aligned_ubig16_t;
    typedef endian< little_endian, int16_t, 16, alignment::aligned >   aligned_little16_t;
    typedef endian< little_endian, uint16_t, 16, alignment::aligned >  aligned_ulittle16_t;
# endif

# if defined(BOOST_HAS_INT32_T)
    typedef endian< big_endian, int32_t, 32, alignment::aligned >      aligned_big32_t;
    typedef endian< big_endian, uint32_t, 32, alignment::aligned >     aligned_ubig32_t;
    typedef endian< little_endian, int32_t, 32, alignment::aligned >   aligned_little32_t;
    typedef endian< little_endian, uint32_t, 32, alignment::aligned >  aligned_ulittle32_t;
# endif

# if defined(BOOST_HAS_INT64_T)
    typedef endian< big_endian, int64_t, 64, alignment::aligned >      aligned_big64_t;
    typedef endian< big_endian, uint64_t, 64, alignment::aligned >     aligned_ubig64_t;
    typedef endian< little_endian, int64_t, 64, alignment::aligned >   aligned_little64_t;
    typedef endian< little_endian, uint64_t, 64, alignment::aligned >  aligned_ulittle64_t;
# endif

    template<typename OSTREAM, typename E,
        typename T, std::size_t N,
        BOOST_SCOPED_ENUM(alignment) A> 
      OSTREAM& 
      operator<<(OSTREAM & os, 
                 const endian< E,T,N,A > & aval) {
        os << T(aval);
        return os;
                     
    }


  } // namespace integer
} // namespace boost

#if defined(__BORLANDC__) || defined( __CODEGEARC__)
# pragma pack(pop)
#endif

#if 0
namespace std {
      template <typename E, 
        typename T, 
        size_t N
    > class numeric_limits<boost::integer::endian<E,T,N,boost::integer::alignment::aligned> > 
  {
      typedef T value_type;
   public:

      BOOST_STATIC_CONSTANT(bool, is_specialized = true);
      static value_type min BOOST_PREVENT_MACRO_SUBSTITUTION (){ return numeric_limits<value_type>::(min)(); }
      static value_type max BOOST_PREVENT_MACRO_SUBSTITUTION (){ return numeric_limits<value_type>::(max)(); }
      BOOST_STATIC_CONSTANT(int, digits = numeric_limits<value_type>::digits);
      BOOST_STATIC_CONSTANT(int, digits10 = numeric_limits<value_type>::digits10);
      BOOST_STATIC_CONSTANT(bool, is_signed = numeric_limits<value_type>::is_signed);
      BOOST_STATIC_CONSTANT(bool, is_integer = numeric_limits<value_type>::is_integer);
      BOOST_STATIC_CONSTANT(bool, is_exact = numeric_limits<value_type>::is_exact);
      BOOST_STATIC_CONSTANT(int, radix = numeric_limits<value_type>::radix);
      static value_type epsilon() throw() { return numeric_limits<value_type>::epsilon();  };
      static value_type round_error() throw() { return numeric_limits<value_type>::round_error(); };

      BOOST_STATIC_CONSTANT(int, min_exponent = numeric_limits<value_type>::min_exponent);
      BOOST_STATIC_CONSTANT(int, min_exponent10 = numeric_limits<value_type>::min_exponent10);
      BOOST_STATIC_CONSTANT(int, max_exponent = numeric_limits<value_type>::max_exponent);
      BOOST_STATIC_CONSTANT(int, max_exponent10 = numeric_limits<value_type>::max_exponent10);

      BOOST_STATIC_CONSTANT(bool, has_infinity = numeric_limits<value_type>::has_infinity);
      BOOST_STATIC_CONSTANT(bool, has_quiet_NaN = numeric_limits<value_type>::has_quiet_NaN);
      BOOST_STATIC_CONSTANT(bool, has_signaling_NaN = numeric_limits<value_type>::has_signaling_NaN);
      BOOST_STATIC_CONSTANT(bool, has_denorm = numeric_limits<value_type>::has_denorm);
      BOOST_STATIC_CONSTANT(bool, has_denorm_loss = numeric_limits<value_type>::has_denorm_loss);
      static value_type infinity() throw() { return numeric_limits<value_type>::infinity(); };
      static value_type quiet_NaN() throw() { return numeric_limits<value_type>::quiet_NaN(); };
      static value_type signaling_NaN() throw() { return numeric_limits<value_type>::signaling_NaN(); };
      static value_type denorm_min() throw() { return numeric_limits<value_type>::denorm_min(); };

      BOOST_STATIC_CONSTANT(bool, is_iec559 = numeric_limits<value_type>::is_iec559);
      BOOST_STATIC_CONSTANT(bool, is_bounded = numeric_limits<value_type>::is_bounded);
      BOOST_STATIC_CONSTANT(bool, is_modulo = numeric_limits<value_type>::is_modulo);

      BOOST_STATIC_CONSTANT(bool, traps = numeric_limits<value_type>::traps);
      BOOST_STATIC_CONSTANT(bool, tinyness_before = numeric_limits<value_type>::tinyness_before);
      BOOST_STATIC_CONSTANT(float_round_style, round_style = numeric_limits<value_type>::round_style);
      
  };

namespace std {
      template <typename E, 
        typename T, 
        size_t N
    > class numeric_limits<boost::integer::endian<E,T,N,boost::integer::alignment::aligned> > 
  {
      typedef T value_type;
   public:

      BOOST_STATIC_CONSTANT(bool, is_specialized = true);
      static value_type min BOOST_PREVENT_MACRO_SUBSTITUTION (){ return numeric_limits<value_type>::(min)(); }
      static value_type max BOOST_PREVENT_MACRO_SUBSTITUTION (){ return numeric_limits<value_type>::(max)(); }
      BOOST_STATIC_CONSTANT(int, digits = numeric_limits<value_type>::digits);
      BOOST_STATIC_CONSTANT(int, digits10 = numeric_limits<value_type>::digits10);
      BOOST_STATIC_CONSTANT(bool, is_signed = numeric_limits<value_type>::is_signed);
      BOOST_STATIC_CONSTANT(bool, is_integer = numeric_limits<value_type>::is_integer);
      BOOST_STATIC_CONSTANT(bool, is_exact = numeric_limits<value_type>::is_exact);
      BOOST_STATIC_CONSTANT(int, radix = numeric_limits<value_type>::radix);
      static value_type epsilon() throw() { return numeric_limits<value_type>::epsilon();  };
      static value_type round_error() throw() { return numeric_limits<value_type>::round_error(); };

      BOOST_STATIC_CONSTANT(int, min_exponent = numeric_limits<value_type>::min_exponent);
      BOOST_STATIC_CONSTANT(int, min_exponent10 = numeric_limits<value_type>::min_exponent10);
      BOOST_STATIC_CONSTANT(int, max_exponent = numeric_limits<value_type>::max_exponent);
      BOOST_STATIC_CONSTANT(int, max_exponent10 = numeric_limits<value_type>::max_exponent10);

      BOOST_STATIC_CONSTANT(bool, has_infinity = numeric_limits<value_type>::has_infinity);
      BOOST_STATIC_CONSTANT(bool, has_quiet_NaN = numeric_limits<value_type>::has_quiet_NaN);
      BOOST_STATIC_CONSTANT(bool, has_signaling_NaN = numeric_limits<value_type>::has_signaling_NaN);
      BOOST_STATIC_CONSTANT(bool, has_denorm = numeric_limits<value_type>::has_denorm);
      BOOST_STATIC_CONSTANT(bool, has_denorm_loss = numeric_limits<value_type>::has_denorm_loss);
      static value_type infinity() throw() { return numeric_limits<value_type>::infinity(); };
      static value_type quiet_NaN() throw() { return numeric_limits<value_type>::quiet_NaN(); };
      static value_type signaling_NaN() throw() { return numeric_limits<value_type>::signaling_NaN(); };
      static value_type denorm_min() throw() { return numeric_limits<value_type>::denorm_min(); };

      BOOST_STATIC_CONSTANT(bool, is_iec559 = numeric_limits<value_type>::is_iec559);
      BOOST_STATIC_CONSTANT(bool, is_bounded = numeric_limits<value_type>::is_bounded);
      BOOST_STATIC_CONSTANT(bool, is_modulo = numeric_limits<value_type>::is_modulo);

      BOOST_STATIC_CONSTANT(bool, traps = numeric_limits<value_type>::traps);
      BOOST_STATIC_CONSTANT(bool, tinyness_before = numeric_limits<value_type>::tinyness_before);
      BOOST_STATIC_CONSTANT(float_round_style, round_style = numeric_limits<value_type>::round_style);
      
  };
    

namespace std {
      template <typename E, 
        typename T, 
        size_t N
    > class numeric_limits<boost::integer::endian<E,T,N,boost::integer::alignment::unaligned> > 
  {
      typedef T value_type;
   public:

      BOOST_STATIC_CONSTANT(bool, is_specialized = true);
      static value_type min BOOST_PREVENT_MACRO_SUBSTITUTION (){ 
          return numeric_limits<value_type>::is_signed
            ? low_bit_mask<value_type, N>::value
            : 0; 
      }
      static value_type max BOOST_PREVENT_MACRO_SUBSTITUTION (){ 
          return numeric_limits<value_type>::is_signed
            ?low_bit_mask<value_type, N-1>::value
            :low_bit_mask<value_type, N>::value; 
      }
      BOOST_STATIC_CONSTANT(int, digits = N);
      BOOST_STATIC_CONSTANT(int, digits10 = 2<<N);
      BOOST_STATIC_CONSTANT(bool, is_signed = numeric_limits<value_type>::is_signed);
      BOOST_STATIC_CONSTANT(bool, is_integer = numeric_limits<value_type>::is_integer);
      BOOST_STATIC_CONSTANT(bool, is_exact = numeric_limits<value_type>::is_exact);
      BOOST_STATIC_CONSTANT(int, radix = numeric_limits<value_type>::radix);
      static value_type epsilon() throw() { return numeric_limits<value_type>::epsilon();  };
      static value_type round_error() throw() { return numeric_limits<value_type>::round_error(); };

      BOOST_STATIC_CONSTANT(int, min_exponent = numeric_limits<value_type>::min_exponent);
      BOOST_STATIC_CONSTANT(int, min_exponent10 = numeric_limits<value_type>::min_exponent10);
      BOOST_STATIC_CONSTANT(int, max_exponent = numeric_limits<value_type>::max_exponent);
      BOOST_STATIC_CONSTANT(int, max_exponent10 = numeric_limits<value_type>::max_exponent10);

      BOOST_STATIC_CONSTANT(bool, has_infinity = numeric_limits<value_type>::has_infinity);
      BOOST_STATIC_CONSTANT(bool, has_quiet_NaN = numeric_limits<value_type>::has_quiet_NaN);
      BOOST_STATIC_CONSTANT(bool, has_signaling_NaN = numeric_limits<value_type>::has_signaling_NaN);
      BOOST_STATIC_CONSTANT(bool, has_denorm = numeric_limits<value_type>::has_denorm);
      BOOST_STATIC_CONSTANT(bool, has_denorm_loss = numeric_limits<value_type>::has_denorm_loss);
      static value_type infinity() throw() { return numeric_limits<value_type>::infinity(); };
      static value_type quiet_NaN() throw() { return numeric_limits<value_type>::quiet_NaN(); };
      static value_type signaling_NaN() throw() { return numeric_limits<value_type>::signaling_NaN(); };
      static value_type denorm_min() throw() { return numeric_limits<value_type>::denorm_min(); };

      BOOST_STATIC_CONSTANT(bool, is_iec559 = numeric_limits<value_type>::is_iec559);
      BOOST_STATIC_CONSTANT(bool, is_bounded = numeric_limits<value_type>::is_bounded);
      BOOST_STATIC_CONSTANT(bool, is_modulo = numeric_limits<value_type>::is_modulo);

      BOOST_STATIC_CONSTANT(bool, traps = numeric_limits<value_type>::traps);
      BOOST_STATIC_CONSTANT(bool, tinyness_before = numeric_limits<value_type>::tinyness_before);
      BOOST_STATIC_CONSTANT(float_round_style, round_style = numeric_limits<value_type>::round_style);
      
  };
    
  
} // namespace std

#endif
#endif // BOOST_ENDIAN_HPP
