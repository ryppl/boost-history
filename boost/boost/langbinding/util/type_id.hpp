// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef TYPE_ID_DWA2002517_HPP
# define TYPE_ID_DWA2002517_HPP

# include <boost/langbinding/util/config.hpp>
# include <boost/langbinding/util/aux_/msvc_typeinfo.hpp>
# include <boost/langbinding/module/id.hpp>
# include <boost/operators.hpp>
# include <typeinfo>
# include <cstring>
# include <boost/static_assert.hpp>
# include <boost/detail/workaround.hpp>
# include <boost/type_traits/same_traits.hpp>

#  ifndef BOOST_LANGBINDING_UTIL_HAVE_GCC_CP_DEMANGLE
#   if defined(__GNUC__)                                                \
    && ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))   \
    && !defined(__EDG_VERSION__)
#    define BOOST_LANGBINDING_UTIL_HAVE_GCC_CP_DEMANGLE
#   endif
#  endif

namespace boost { namespace langbinding { namespace util {

// for this compiler at least, cross-shared-library type_info
// comparisons don't work, so use typeid(x).name() instead. It's not
// yet clear what the best default strategy is.
# if (defined(__GNUC__) && __GNUC__ >= 3) \
 || defined(_AIX) \
 || (   defined(__sgi) && defined(__host_mips)) \
 || (defined(linux) && defined(__INTEL_COMPILER) && defined(__ICC))
#  define BOOST_LANGBINDING_UTIL_TYPE_ID_NAME
# endif 

// type ids which represent the same information as std::type_info
// (i.e. the top-level reference and cv-qualifiers are stripped), but
// which works across shared libraries.
struct type_info : private totally_ordered<type_info>
{
    inline type_info(std::type_info const& = typeid(void), int module = module::id());

    inline bool operator<(type_info const& rhs) const;
    inline bool operator==(type_info const& rhs) const;

    char const* name() const;
    friend BOOST_LANGBINDING_DECL std::ostream& operator<<(
        std::ostream&, type_info const&);

    int module() const;

 private: // data members
#  ifdef BOOST_LANGBINDING_UTIL_TYPE_ID_NAME
    typedef char const* base_id_t;
#  else
    typedef std::type_info const* base_id_t;
#  endif
    
    base_id_t m_base_type;
    int m_module;
};

template<class T>
struct type_id_
{
    static type_info id;
};

template<class T>
type_info type_id_<T>::id(
#  if !defined(_MSC_VER)                                       \
      || (!BOOST_WORKAROUND(BOOST_MSVC, <= 1300)                \
          && !BOOST_WORKAROUND(BOOST_INTEL_CXX_VERSION, <= 700))
    typeid(T)
#  else // strip the decoration which msvc and Intel mistakenly leave in
    aux_::msvc_typeid((boost::type<T>*)0)
#endif
  , module::id()
  );

#  ifdef BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
#   define BOOST_LANGBINDING_UTIL_EXPLICIT_TT_DEF(T) ::boost::type<T>*
#  else
#   define BOOST_LANGBINDING_UTIL_EXPLICIT_TT_DEF(T)
#  endif

template <class T>
inline type_info const& type_id(BOOST_EXPLICIT_TEMPLATE_TYPE(T))
{
    return type_id_<T>::id;
}

#  if (defined(__EDG_VERSION__) && __EDG_VERSION__ < 245) \
   || (defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 741)
// Older EDG-based compilers seems to mistakenly distinguish "int" from
// "signed int", etc., but only in typeid() expressions. However
// though int == signed int, the "signed" decoration is propagated
// down into template instantiations. Explicit specialization stops
// that from taking hold.

#   define BOOST_LANGBINDING_UTIL_SIGNED_INTEGRAL_TYPE_ID(T)                  \
template <>                                                                   \
inline type_info const& type_id<T>(BOOST_LANGBINDING_UTIL_EXPLICIT_TT_DEF(T)) \
{                                                                             \
    return type_id_<T>::id;                                                   \
}

BOOST_LANGBINDING_UTIL_SIGNED_INTEGRAL_TYPE_ID(short)
BOOST_LANGBINDING_UTIL_SIGNED_INTEGRAL_TYPE_ID(int)
BOOST_LANGBINDING_UTIL_SIGNED_INTEGRAL_TYPE_ID(long)
// using Python's macro instead of Boost's - we don't seem to get the
// config right all the time.
#   ifdef HAVE_LONG_LONG
BOOST_LANGBINDING_UTIL_SIGNED_INTEGRAL_TYPE_ID(long long)
#   endif
#   undef BOOST_PYTHON_SIGNED_INTEGRAL_TYPE_ID
#  endif

//
inline type_info::type_info(std::type_info const& id, int module)
    : m_base_type(
#  ifdef BOOST_LANGBINDING_UTIL_TYPE_ID_NAME
        id.name()
#  else
        &id
#  endif
      )
    , m_module(module)
{
}

inline bool type_info::operator<(type_info const& rhs) const
{
#  ifdef BOOST_LANGBINDING_UTIL_TYPE_ID_NAME
    return m_module < rhs.m_module
        && std::strcmp(m_base_type, rhs.m_base_type) < 0;
#  else
    return m_module < rhs.m_module 
        && m_base_type->before(*rhs.m_base_type);
#  endif 
}

inline bool type_info::operator==(type_info const& rhs) const
{
#  ifdef BOOST_LANGBINDING_UTIL_TYPE_ID_NAME
    return m_module == rhs.m_module
        && !std::strcmp(m_base_type, rhs.m_base_type);
#  else
    return m_module == rhs.m_module
        && *m_base_type == *rhs.m_base_type;
#  endif 
}

#  ifdef BOOST_LANGBINDING_UTIL_HAVE_GCC_CP_DEMANGLE
namespace aux
{
  BOOST_LANGBINDING_DECL char const* gcc_demangle(char const*);
} // namespace aux
#  endif
    
inline char const* type_info::name() const
{
    char const* raw_name
        = m_base_type
#  ifndef BOOST_LANGBINDING_UTIL_TYPE_ID_NAME
          ->name()
#  endif
        ;
    
#  ifdef BOOST_LANGBINDING_UTIL_HAVE_GCC_CP_DEMANGLE
    return aux::gcc_demangle(raw_name);
#  else
    return raw_name;
#  endif 
}

inline int type_info::module() const
{
    return m_module;
}

BOOST_LANGBINDING_DECL std::ostream& operator<<(std::ostream&, type_info const&);

}}} // namespace boost::langbinding::util

#endif // TYPE_ID_DWA2002517_HPP
