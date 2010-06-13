//  Boost endian_pack.hpp header file -------------------------------------------------------//

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

//  Split of endian.hpp into endian.hpp  and endian_pack.hpp done by Vicente J. Botet Escriba.
//  Definition of native depending on BOOST_BIG_ENDIAN to big or little done by Vicente J. Botet Escriba.
//  Change the definition of endian_pack using types instead of enum endianness done by Vicente J. Botet Escriba.

// TODO: When a compiler supporting constexpr becomes available, try possible uses.

#ifndef BOOST_INTEGER_ENDIAN_PACK_HPP
#define BOOST_INTEGER_ENDIAN_PACK_HPP

#ifdef BOOST_ENDIAN_LOG
# include <iostream>
#endif

#if defined(__BORLANDC__) || defined( __CODEGEARC__)
# pragma pack(push, 1)
#endif

#include <boost/config.hpp>
#include <boost/detail/endian.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/integer_traits.hpp>
#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>
#include <boost/detail/scoped_enum_emulation.hpp>
//~ #include <iosfwd>
#include <climits>

# include <boost/endian/endian.hpp>

# if CHAR_BIT != 8
#   error Platforms with CHAR_BIT != 8 are not supported
# endif

# ifdef BOOST_NO_DEFAULTED_FUNCTIONS
#   define BOOST_ENDIAN_DEFAULT_CONSTRUCT {}          // C++03
# else
#   define BOOST_ENDIAN_DEFAULT_CONSTRUCT = default;  // C++0x
# endif

# if defined(BOOST_NO_DEFAULTED_FUNCTIONS) && defined(BOOST_ENDIAN_FORCE_PODNESS)
#   define BOOST_ENDIAN_NO_CTORS
# endif


namespace boost
{
  namespace detail
  {
    // Unrolled loops for loading and storing streams of bytes.

    template <typename T, std::size_t n_bytes,
      bool sign=boost::is_signed<T>::value >
    struct unrolled_byte_loops
    {
      typedef unrolled_byte_loops<T, n_bytes - 1, sign> next;

      static T load_big(const unsigned char* bytes)
        { return *(bytes - 1) | (next::load_big(bytes - 1) << 8); }
      static T load_little(const unsigned char* bytes)
        { return *bytes | (next::load_little(bytes + 1) << 8); }

      static void store_big(char* bytes, T value)
        {
          *(bytes - 1) = static_cast<char>(value);
          next::store_big(bytes - 1, value >> 8);
        }
      static void store_little(char* bytes, T value)
        {
          *bytes = static_cast<char>(value);
          next::store_little(bytes + 1, value >> 8);
        }
    };

    template <typename T>
    struct unrolled_byte_loops<T, 1, false>
    {
      static T load_big(const unsigned char* bytes)
        { return *(bytes - 1); }
      static T load_little(const unsigned char* bytes)
        { return *bytes; }
      static void store_big(char* bytes, T value)
        { *(bytes - 1) = static_cast<char>(value); }
      static void store_little(char* bytes, T value)
        { *bytes = static_cast<char>(value); }

    };

    template <typename T>
    struct unrolled_byte_loops<T, 1, true>
    {
      static T load_big(const unsigned char* bytes)
        { return *reinterpret_cast<const signed char*>(bytes - 1); }
      static T load_little(const unsigned char* bytes)
        { return *reinterpret_cast<const signed char*>(bytes); }
      static void store_big(char* bytes, T value)
        { *(bytes - 1) = static_cast<char>(value); }
      static void store_little(char* bytes, T value)
        { *bytes = static_cast<char>(value); }
    };

    template <typename T, std::size_t n_bytes>
    inline
    T load_big_endian(const void* bytes)
    {
      return unrolled_byte_loops<T, n_bytes>::load_big
        (static_cast<const unsigned char*>(bytes) + n_bytes);
    }

    template <typename T, std::size_t n_bytes>
    inline
    T load_little_endian(const void* bytes)
    {
      return unrolled_byte_loops<T, n_bytes>::load_little
        (static_cast<const unsigned char*>(bytes));
    }

    template <typename T, std::size_t n_bytes>
    inline
    void store_big_endian(void* bytes, T value)
    {
      unrolled_byte_loops<T, n_bytes>::store_big
        (static_cast<char*>(bytes) + n_bytes, value);
    }

    template <typename T, std::size_t n_bytes>
    inline
    void store_little_endian(void* bytes, T value)
    {
      unrolled_byte_loops<T, n_bytes>::store_little
        (static_cast<char*>(bytes), value);
    }

  } // namespace detail

  namespace integer
  {

# ifdef BOOST_ENDIAN_LOG
    bool endian_log(true);
# endif

 
    BOOST_SCOPED_ENUM_START(alignment) { unaligned, aligned }; BOOST_SCOPED_ENUM_END

    template <typename E,
        typename T, 
        std::size_t n_bits=sizeof(T)*8,
        BOOST_SCOPED_ENUM(alignment) A = alignment::aligned
    > class endian_pack;

    //  Specializations that represent unaligned bytes.
    //  Taking an integer type as a parameter provides a nice way to pass both
    //  the size and signedness of the desired integer and get the appropriate
    //  corresponding integer type for the interface.

    //  unaligned big endian specialization
    template <typename T, std::size_t n_bits>
    class endian_pack<big_endian, T, n_bits, alignment::unaligned >
      
    {
        BOOST_STATIC_ASSERT( (n_bits/8)*8 == n_bits );
      public:
        typedef T value_type;
#     ifndef BOOST_ENDIAN_NO_CTORS
        endian_pack() BOOST_ENDIAN_DEFAULT_CONSTRUCT
        explicit endian_pack(T val)
        { 
#       ifdef BOOST_ENDIAN_LOG
          if ( endian_log )
            std::clog << "big, unaligned, " << n_bits << "-bits, construct(" << val << ")\n";
#       endif
          detail::store_big_endian<T, n_bits/8>(m_value, val);
        }
#     endif
        endian_pack & operator=(T val) { detail::store_big_endian<T, n_bits/8>(m_value, val); return *this; }
        operator T() const
        { 
#       ifdef BOOST_ENDIAN_LOG
          if ( endian_log )
            std::clog << "big, unaligned, " << n_bits << "-bits, convert(" << detail::load_big_endian<T, n_bits/8>(m_value) << ")\n";
#       endif
          return detail::load_big_endian<T, n_bits/8>(m_value);
        }
      private:
  	    char m_value[n_bits/8];
    };

    //  unaligned little endian specialization
    template <typename T, std::size_t n_bits>
    class endian_pack< little_endian, T, n_bits, alignment::unaligned >
    {
        BOOST_STATIC_ASSERT( (n_bits/8)*8 == n_bits );
      public:
        typedef T value_type;
#     ifndef BOOST_ENDIAN_NO_CTORS
        endian_pack() BOOST_ENDIAN_DEFAULT_CONSTRUCT
        explicit endian_pack(T val)
        { 
#       ifdef BOOST_ENDIAN_LOG
          if ( endian_log )
            std::clog << "little, unaligned, " << n_bits << "-bits, construct(" << val << ")\n";
#       endif
          detail::store_little_endian<T, n_bits/8>(m_value, val);
        }
#     endif
        endian_pack & operator=(T val) { detail::store_little_endian<T, n_bits/8>(m_value, val); return *this; }
        operator T() const
        { 
#       ifdef BOOST_ENDIAN_LOG
          if ( endian_log )
            std::clog << "little, unaligned, " << n_bits << "-bits, convert(" << detail::load_little_endian<T, n_bits/8>(m_value) << ")\n";
#       endif
          return detail::load_little_endian<T, n_bits/8>(m_value);
        }
      private:
  	    char m_value[n_bits/8];
    };


    //  Specializations that mimic built-in integer types.
    //  These typically have the same alignment as the underlying types.

    //  aligned big endian specialization
    template <typename T, std::size_t n_bits>
    class endian_pack< big_endian, T, n_bits, alignment::aligned  >
    {
        BOOST_STATIC_ASSERT( (n_bits/8)*8 == n_bits );
        BOOST_STATIC_ASSERT( sizeof(T) == n_bits/8 );
      public:
        typedef T value_type;
#   ifndef BOOST_ENDIAN_NO_CTORS
        endian_pack() BOOST_ENDIAN_DEFAULT_CONSTRUCT
#     ifdef BOOST_BIG_ENDIAN
        endian_pack(T val) : m_value(val) { }
#     else
        explicit endian_pack(T val)    { detail::store_big_endian<T, sizeof(T)>(&m_value, val); }
#     endif
#   endif
#   ifdef BOOST_BIG_ENDIAN  
        endian_pack & operator=(T val) { m_value = val); return *this; }
        operator T() const        { return m_value; }
#   else  
        endian_pack & operator=(T val) { detail::store_big_endian<T, sizeof(T)>(&m_value, val); return *this; }
        operator T() const        { return detail::load_big_endian<T, sizeof(T)>(&m_value); }
#   endif  
      private:
  	    T m_value;
    };

    //  aligned little endian specialization
    template <typename T, std::size_t n_bits>
    class endian_pack< little_endian, T, n_bits, alignment::aligned  >
    {
        BOOST_STATIC_ASSERT( (n_bits/8)*8 == n_bits );
        BOOST_STATIC_ASSERT( sizeof(T) == n_bits/8 );
      public:
        typedef T value_type;
#   ifndef BOOST_ENDIAN_NO_CTORS
        endian_pack() BOOST_ENDIAN_DEFAULT_CONSTRUCT
#     ifdef BOOST_LITTLE_ENDIAN
        endian_pack(T val) : m_value(val) { }
#     else
        explicit endian_pack(T val)    { detail::store_little_endian<T, sizeof(T)>(&m_value, val); }
#     endif
#   endif
#   ifdef BOOST_LITTLE_ENDIAN
        endian_pack & operator=(T val) { m_value = val; return *this; }
        operator T() const        { return m_value; }
    #else
        endian_pack & operator=(T val) { detail::store_little_endian<T, sizeof(T)>(&m_value, val); return *this; }
        operator T() const        { return detail::load_little_endian<T, sizeof(T)>(&m_value); }
    #endif
      private:
  	    T m_value;
    };

  //  naming convention typedefs  ------------------------------------------------------//

    // unaligned big endian_pack signed integer types
    typedef endian_pack< big_endian, int_least8_t, 8, alignment::unaligned >           big8_pt;
    typedef endian_pack< big_endian, int_least16_t, 16, alignment::unaligned >         big16_pt;
    typedef endian_pack< big_endian, int_least32_t, 24, alignment::unaligned >         big24_pt;
    typedef endian_pack< big_endian, int_least32_t, 32, alignment::unaligned >         big32_pt;
    typedef endian_pack< big_endian, int_least64_t, 40, alignment::unaligned >         big40_pt;
    typedef endian_pack< big_endian, int_least64_t, 48, alignment::unaligned >         big48_pt;
    typedef endian_pack< big_endian, int_least64_t, 56, alignment::unaligned >         big56_pt;
    typedef endian_pack< big_endian, int_least64_t, 64, alignment::unaligned >         big64_pt;

    // unaligned big endian_pack unsigned integer types
    typedef endian_pack< big_endian, uint_least8_t, 8, alignment::unaligned >          ubig8_pt;
    typedef endian_pack< big_endian, uint_least16_t, 16, alignment::unaligned >        ubig16_pt;
    typedef endian_pack< big_endian, uint_least32_t, 24, alignment::unaligned >        ubig24_pt;
    typedef endian_pack< big_endian, uint_least32_t, 32, alignment::unaligned >        ubig32_pt;
    typedef endian_pack< big_endian, uint_least64_t, 40, alignment::unaligned >        ubig40_pt;
    typedef endian_pack< big_endian, uint_least64_t, 48, alignment::unaligned >        ubig48_pt;
    typedef endian_pack< big_endian, uint_least64_t, 56, alignment::unaligned >        ubig56_pt;
    typedef endian_pack< big_endian, uint_least64_t, 64, alignment::unaligned >        ubig64_pt;

    // unaligned little endian_pack signed integer types
    typedef endian_pack< little_endian, int_least8_t, 8, alignment::unaligned >        little8_pt;
    typedef endian_pack< little_endian, int_least16_t, 16, alignment::unaligned >      little16_pt;
    typedef endian_pack< little_endian, int_least32_t, 24, alignment::unaligned >      little24_pt;
    typedef endian_pack< little_endian, int_least32_t, 32, alignment::unaligned >      little32_pt;
    typedef endian_pack< little_endian, int_least64_t, 40, alignment::unaligned >      little40_pt;
    typedef endian_pack< little_endian, int_least64_t, 48, alignment::unaligned >      little48_pt;
    typedef endian_pack< little_endian, int_least64_t, 56, alignment::unaligned >      little56_pt;
    typedef endian_pack< little_endian, int_least64_t, 64, alignment::unaligned >      little64_pt;

    // unaligned little endian_pack unsigned integer types
    typedef endian_pack< little_endian, uint_least8_t, 8, alignment::unaligned >       ulittle8_pt;
    typedef endian_pack< little_endian, uint_least16_t, 16, alignment::unaligned >     ulittle16_pt;
    typedef endian_pack< little_endian, uint_least32_t, 24, alignment::unaligned >     ulittle24_pt;
    typedef endian_pack< little_endian, uint_least32_t, 32, alignment::unaligned >     ulittle32_pt;
    typedef endian_pack< little_endian, uint_least64_t, 40, alignment::unaligned >     ulittle40_pt;
    typedef endian_pack< little_endian, uint_least64_t, 48, alignment::unaligned >     ulittle48_pt;
    typedef endian_pack< little_endian, uint_least64_t, 56, alignment::unaligned >     ulittle56_pt;
    typedef endian_pack< little_endian, uint_least64_t, 64, alignment::unaligned >     ulittle64_pt;

    // unaligned native endian_pack signed integer types
    typedef endian_pack< native_endian, int_least8_t, 8, alignment::unaligned >        native8_pt;
    typedef endian_pack< native_endian, int_least16_t, 16, alignment::unaligned >      native16_pt;
    typedef endian_pack< native_endian, int_least32_t, 24, alignment::unaligned >      native24_pt;
    typedef endian_pack< native_endian, int_least32_t, 32, alignment::unaligned >      native32_pt;
    typedef endian_pack< native_endian, int_least64_t, 40, alignment::unaligned >      native40_pt;
    typedef endian_pack< native_endian, int_least64_t, 48, alignment::unaligned >      native48_pt;
    typedef endian_pack< native_endian, int_least64_t, 56, alignment::unaligned >      native56_pt;
    typedef endian_pack< native_endian, int_least64_t, 64, alignment::unaligned >      native64_pt;

    // unaligned native endian_pack unsigned integer types
    typedef endian_pack< native_endian, uint_least8_t, 8, alignment::unaligned >       unative8_pt;
    typedef endian_pack< native_endian, uint_least16_t, 16, alignment::unaligned >     unative16_pt;
    typedef endian_pack< native_endian, uint_least32_t, 24, alignment::unaligned >     unative24_pt;
    typedef endian_pack< native_endian, uint_least32_t, 32, alignment::unaligned >     unative32_pt;
    typedef endian_pack< native_endian, uint_least64_t, 40, alignment::unaligned >     unative40_pt;
    typedef endian_pack< native_endian, uint_least64_t, 48, alignment::unaligned >     unative48_pt;
    typedef endian_pack< native_endian, uint_least64_t, 56, alignment::unaligned >     unative56_pt;
    typedef endian_pack< native_endian, uint_least64_t, 64, alignment::unaligned >     unative64_pt;

#define BOOST_HAS_INT16_T
#define BOOST_HAS_INT32_T
#define BOOST_HAS_INT64_T
  
  //  These types only present if platform has exact size integers:
  //     aligned big endian_pack signed integer types
  //     aligned big endian_pack unsigned integer types
  //     aligned little endian_pack signed integer types
  //     aligned little endian_pack unsigned integer types

  //     aligned native endian_pack typedefs are not provided because
  //     <cstdint> types are superior for this use case

# if defined(BOOST_HAS_INT16_T)
    typedef endian_pack< big_endian, int16_t, 16, alignment::aligned >      aligned_big16_pt;
    typedef endian_pack< big_endian, uint16_t, 16, alignment::aligned >     aligned_ubig16_pt;
    typedef endian_pack< little_endian, int16_t, 16, alignment::aligned >   aligned_little16_pt;
    typedef endian_pack< little_endian, uint16_t, 16, alignment::aligned >  aligned_ulittle16_pt;
# endif

# if defined(BOOST_HAS_INT32_T)
    typedef endian_pack< big_endian, int32_t, 32, alignment::aligned >      aligned_big32_pt;
    typedef endian_pack< big_endian, uint32_t, 32, alignment::aligned >     aligned_ubig32_pt;
    typedef endian_pack< little_endian, int32_t, 32, alignment::aligned >   aligned_little32_pt;
    typedef endian_pack< little_endian, uint32_t, 32, alignment::aligned >  aligned_ulittle32_pt;
# endif

# if defined(BOOST_HAS_INT64_T)
    typedef endian_pack< big_endian, int64_t, 64, alignment::aligned >      aligned_big64_pt;
    typedef endian_pack< big_endian, uint64_t, 64, alignment::aligned >     aligned_ubig64_pt;
    typedef endian_pack< little_endian, int64_t, 64, alignment::aligned >   aligned_little64_pt;
    typedef endian_pack< little_endian, uint64_t, 64, alignment::aligned >  aligned_ulittle64_pt;
# endif
    
  } // namespace integer
} // namespace boost

#if defined(__BORLANDC__) || defined( __CODEGEARC__)
# pragma pack(pop)
#endif

#endif // BOOST_INTEGER_ENDIAN_PACK_HPP
