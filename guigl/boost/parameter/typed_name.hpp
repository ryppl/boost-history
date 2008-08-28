/*=================================---------------------------------------------
    Copyright Daniel Wallin 2006.
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST_PARAMETER_TYPED_NAME_060806_HPP
# define BOOST_PARAMETER_TYPED_NAME_060806_HPP

# include <boost/parameter/typed_keyword.hpp>
# include <boost/parameter/name.hpp>
# include <boost/detail/workaround.hpp>
# include <boost/preprocessor/cat.hpp>
# include <boost/preprocessor/stringize.hpp>
# include <boost/preprocessor/control/iif.hpp>
# include <boost/preprocessor/tuple/eat.hpp>
# include <boost/preprocessor/tuple/elem.hpp>
# include <boost/mpl/placeholders.hpp>

# if !defined(BOOST_NO_SFINAE) \
  && !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
  && !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x592))

#  include <boost/utility/enable_if.hpp>
#  include <boost/mpl/lambda.hpp>

namespace boost { namespace parameter { namespace aux {

// Tag type passed to MPL lambda.
struct lambda_tag;

struct typed_name_tag_base 
{};

template <class Tag>
struct typed_name_tag
{};

template <class T>
struct is_typed_name_tag
  : mpl::false_
{};

}}} // namespace boost::parameter::aux

namespace boost { namespace mpl {

template <class T>
struct lambda<
    T
  , typename enable_if<
        parameter::aux::is_typed_name_tag<T>, parameter::aux::lambda_tag
    >::type
>
{
    typedef true_ is_le;
    typedef bind3< quote3<parameter::value_type>, arg<2>, T, void> result_;
    typedef result_ type;
};

}} // namespace boost::mpl

# endif


# if BOOST_WORKAROUND(BOOST_MSVC, < 1300)
#  define BOOST_PARAMETER_TYPED_NAME_OBJECT(tag, name)                    \
    static ::boost::parameter::typed_keyword<tag> const& name             \
       = ::boost::parameter::typed_keyword<tag>::instance;
# else
#  define BOOST_PARAMETER_TYPED_NAME_OBJECT(tag, name)                    \
    namespace                                                       \
    {                                                               \
       ::boost::parameter::typed_keyword<tag> const& name                 \
       = ::boost::parameter::typed_keyword<tag>::instance;                \
    }
# endif

# define BOOST_PARAMETER_BASIC_TYPED_NAME(tag_namespace, tag, name, type)       \
    namespace tag_namespace                                         \
    {                                                               \
      struct tag                                                    \
      {                                                             \
          static char const* keyword_name()                         \
          {                                                         \
              return BOOST_PP_STRINGIZE(tag);                       \
          }                                                         \
                                                                    \
          typedef boost::parameter::value_type<                     \
              boost::mpl::_2, tag, boost::parameter::void_          \
          > _;                                                      \
                                                                    \
          typedef boost::parameter::value_type<                     \
              boost::mpl::_2, tag, boost::parameter::void_          \
          > _1;                                                     \
                                                                    \
          typedef type value_type;                                                          \
      };                                                            \
    }                                                               \
    BOOST_PARAMETER_TYPED_NAME_OBJECT(tag_namespace::tag, name)

# define BOOST_PARAMETER_BASIC_TYPED_NAME_WDEFAULT(tag_namespace, tag, name, type, default_v)       \
    namespace tag_namespace                                         \
    {                                                               \
      struct tag                                                    \
      {                                                             \
          static char const* keyword_name()                         \
          {                                                         \
              return BOOST_PP_STRINGIZE(tag);                       \
          }                                                         \
                                                                    \
          typedef boost::parameter::value_type<                     \
              boost::mpl::_2, tag, boost::parameter::void_          \
          > _;                                                      \
                                                                    \
          typedef boost::parameter::value_type<                     \
              boost::mpl::_2, tag, boost::parameter::void_          \
          > _1;                                                     \
                                                                    \
          typedef type value_type;                                  \
                                                                    \
          static value_type &default_value()                        \
          {                                                         \
               static value_type value = default_v;                 \
               return value;                                        \
          }                                                         \
                                                                    \
      };                                                            \
    }                                                               \
    BOOST_PARAMETER_TYPED_NAME_OBJECT(tag_namespace::tag, name)


# define BOOST_PARAMETER_TYPED_NAME(name,type)                          \
    BOOST_PARAMETER_BASIC_TYPED_NAME(tag, name, BOOST_PP_CAT(_, name), type)

# define BOOST_PARAMETER_TYPED_NAME_WDEFAULT(name,type,default_v)                \
    BOOST_PARAMETER_BASIC_TYPED_NAME_WDEFAULT(tag, name, BOOST_PP_CAT(_, name), type,default_v)

/**/

#endif // BOOST_PARAMETER_NAME_060806_HPP

