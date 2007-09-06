// (C) Copyright Jeremy Siek 1999-2001.
// (C) Copyright Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_MAP_PROPERTY_TRAITS_HPP
#define BOOST_PROPERTY_MAP_PROPERTY_TRAITS_HPP

#include <cstddef>
#include <boost/config.hpp>

namespace boost { namespace property_map {

    template <typename PropertyMap>
    struct property_traits
    {
        typedef typename PropertyMap::key_type key_type;
        typedef typename PropertyMap::value_type value_type;
        typedef typename PropertyMap::reference reference;
        typedef typename PropertyMap::category category;
    };

    namespace detail {
        // This enumeration provides identifier values that can be used
        // to determine the specific type of property map at compile time.
        //
        // TODO: It doesn't look like these are actually used anywhere. So
        // it may be entirely possible to get rid of them. They don't actually
        // contribute much to the code below...
        enum property_map_id {
            READABLE_PA,
            WRITABLE_PA,
            READ_WRITE_PA,
            LVALUE_PA,
            OP_BRACKET_PA,          // What's this?
            RAND_ACCESS_ITER_PA,    // Random access?
            LAST_PA
        };
    }

    // Property tags. These tags are used to constrain and specialize put
    // and get operations for property maps with different capabilities.
    struct readable_property_map_tag { enum { id = detail::READABLE_PA }; };
    struct writable_property_map_tag { enum { id = detail::WRITABLE_PA }; };
    struct read_write_property_map_tag
        : public readable_property_map_tag
        , public writable_property_map_tag
    { enum { id = detail::READ_WRITE_PA }; };
    struct lvalue_property_map_tag
        : public read_write_property_map_tag
          { enum { id = detail::LVALUE_PA }; };


    // Prebuilt specializations...

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    // The user will just have to create their own specializations for
    // other pointers types if the compiler does not have partial
    // specializations. Sorry!
#define BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(TYPE) \
    template <> \
    struct property_traits<TYPE*> { \
    typedef TYPE value_type; \
    typedef value_type& reference; \
    typedef std::ptrdiff_t key_type; \
    typedef lvalue_property_map_tag   category; \
}; \
    template <> \
    struct property_traits<const TYPE*> { \
    typedef TYPE value_type; \
    typedef const value_type& reference; \
    typedef std::ptrdiff_t key_type; \
    typedef lvalue_property_map_tag   category; \
}

    // Automatically generate specializations for fundamental types.
    BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(long);
    BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(unsigned long);
    BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(int);
    BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(unsigned int);
    BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(short);
    BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(unsigned short);
    BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(char);
    BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(unsigned char);
    BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(signed char);
    BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(bool);
    BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(float);
    BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(double);
    BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(long double);

  // This may need to be turned off for some older compilers that don't have
  // wchar_t intrinsically.
# ifndef BOOST_NO_INTRINSIC_WCHAR_T
    template <>
    struct property_traits<wchar_t*>
    {
        typedef wchar_t value_type;
        typedef value_type& reference;
        typedef std::ptrdiff_t key_type;
        typedef lvalue_property_map_tag   category;
    };

    template <>
    struct property_traits<const wchar_t*>
    {
        typedef wchar_t value_type;
        typedef const value_type& reference;
        typedef std::ptrdiff_t key_type;
        typedef lvalue_property_map_tag   category;
    };
# endif
#else // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    // Automatically specialize for all pointer types. This allows us to
    // treat array types (ptrs) as property maps.
    template <class T>
    struct property_traits<T*>
    {
        typedef T value_type;
        typedef value_type& reference;
        typedef std::ptrdiff_t key_type;
        typedef lvalue_property_map_tag category;
    };

    template <class T>
    struct property_traits<const T*> {
        typedef T value_type;
        typedef const value_type& reference;
        typedef std::ptrdiff_t key_type;
        typedef lvalue_property_map_tag category;
    };


#endif
} }

// These variants of the put() and get() functions provide specializations for
// pointer types. In this case, the "property map" is essentially an address,
// the key is an offset from that address and the value, is... well, the value.
// Note that the value type (V) must be convertible to the property map type (T).
// Also, these need to go in global namespace because Koenig lookup does not
// apply to T*.

template <class T, class V>
inline void put(T* pa, std::ptrdiff_t k, const V& val)
{
    pa[k] = val;
}

template <class T>
inline const T& get(const T* pa, std::ptrdiff_t k)
{
    return pa[k];
}

#if !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
// Re-insert the put and get functions into to the property_map namespace.
namespace boost { namespace property_map {
    using ::put;
    using ::get;
} }
#endif

#endif
