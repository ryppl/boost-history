#ifndef BOOST_FT_CAN_BE_CALLED_HPP_INCLUDED
#define BOOST_FT_CAN_BE_CALLED_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//====================================================================
#include <boost/mpl/package_range_c.hpp>

namespace boost
{
namespace function_types
{
//Acknowledgments:
//  The code in this part of this namespace is basicly that posted here:
//    http://article.gmane.org/gmane.comp.lib.boost.devel/203956
//  with headers:
/*
From: Larry Evans <cppljevans <at> suddenlink.net>
Subject: Re: [proto][can_be_called]
Newsgroups: gmane.comp.lib.boost.devel
Date: 2010-05-08 18:26:38 GMT
 */
//
namespace detail_can_be_called
{
struct dont_care
{
    dont_care(...);
};

struct private_type
{
    private_type const &operator,(int) const;
};

typedef char yes_type;      // sizeof(yes_type) == 1
typedef char (&no_type)[2]; // sizeof(no_type)  == 2

template<typename T>
no_type is_private_type(T const &);

yes_type is_private_type(private_type const &);

template<typename Value, typename Indices>
struct repeat_indices
;
template<typename Value, unsigned... Indices>
struct repeat_indices<Value, boost::mpl::package_c<unsigned, Indices...> >
{
      template
      < unsigned IndexValue
      >
    struct always_value
    {
        typedef Value type;
    };
        typedef 
      boost::mpl::package
      < typename always_value<Indices>::type...
      >
    type
    ;
};
template<typename Value, unsigned N>
struct repeat
/**@brief
 *  Create package<typename... Values>
 *  with size=N and Values... are all Value.
 */
: repeat_indices<Value,typename boost::mpl::package_range_c<unsigned,0,N>::type>
{
};

template<typename Fun, typename DontCares>
struct funwrap_dont_care
;
template<typename Fun, typename... DontCares>
struct funwrap_dont_care<Fun, boost::mpl::package<DontCares...> >
: Fun
{
    funwrap_dont_care();
    typedef private_type const &(*pointer_to_function)(DontCares...);
    operator pointer_to_function() const;
};

template<typename Fun, typename Indices, typename... Args>
struct can_be_called_impl
;
template<typename Fun, unsigned... Indices, typename... Args>
struct can_be_called_impl<Fun, boost::mpl::package_c<unsigned, Indices...>, Args...>
{
 private:
    static unsigned const arity=sizeof...(Args);
    static funwrap_dont_care<Fun, typename repeat<dont_care,arity>::type> &fun;
      template
      < unsigned IndexValue
      >
        static
      typename mpl::arg
        < IndexValue
        + 1 //because mpl::arg<1> retrieves 1st index.
        >
      ::template apply<Args...>::type
    at_value(void)
      /**@brief
       *  Never used at runtime.  Only used to calculate value below.
       */
    ;
    static bool const value = 
    (
        sizeof(no_type) == 
        sizeof
        ( is_private_type
          ( ( fun 
              ( at_value<Indices>()...
              )
            , 0
            ) 
          )
        )
    );
 public:
    typedef boost::mpl::bool_<value> type;
};

}//exit detail_can_be_called

template<typename Sig>
struct can_be_called
;
template<typename Fun, typename... Args>
struct can_be_called<Fun(Args...)>
: detail_can_be_called::can_be_called_impl
  < Fun
  , typename boost::mpl::package_range_c
    < unsigned
    , 0
    , sizeof...(Args)
    >::type
  , Args...
  >
{
};
template<typename Fun, typename... Args>
struct can_be_called<Fun const(Args...)>
: detail_can_be_called::can_be_called_impl
  < Fun const
  , typename boost::mpl::package_range_c
    < unsigned
    , 0
    , sizeof...(Args)
    >::type
  , Args...
  >
{
};

}//exit function_types namespace
}//exit boost namespace
#endif
