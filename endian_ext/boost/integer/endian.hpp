//  Boost endian.hpp header file -------------------------------------------------------//

//  (C) Copyright Darin Adler 2000
//  (C) Copyright Beman Dawes 2006, 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/endian

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

#include <boost/integer/endian_pack.hpp>


#if defined(__BORLANDC__) || defined( __CODEGEARC__)
# pragma pack(push, 1)
#endif

#include <boost/config.hpp>
#define BOOST_MINIMAL_INTEGER_COVER_OPERATORS
#define BOOST_NO_IO_COVER_OPERATORS
#include <boost/integer/cover_operators.hpp>
#undef  BOOST_NO_IO_COVER_OPERATORS
#undef  BOOST_MINIMAL_INTEGER_COVER_OPERATORS
#include <boost/type_traits/is_signed.hpp>
#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>
#include <iosfwd>
#include <climits>

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
        typedef T value_type;
#     ifndef BOOST_ENDIAN_NO_CTORS
        endian() BOOST_ENDIAN_DEFAULT_CONSTRUCT
        explicit endian(T val)
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

  } // namespace integer
} // namespace boost

#if defined(__BORLANDC__) || defined( __CODEGEARC__)
# pragma pack(pop)
#endif

#endif // BOOST_ENDIAN_HPP
