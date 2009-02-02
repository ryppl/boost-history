/*=================================---------------------------------------------
    Copyright Daniel Wallin, David Abrahams 2005.
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef TYPED_ARG_LIST_050329_HPP
#define TYPED_ARG_LIST_050329_HPP

#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/aux_/result_of0.hpp>
#include <boost/parameter/aux_/default.hpp>
#include <boost/parameter/aux_/parameter_requirements.hpp>
#include <boost/parameter/aux_/yesno.hpp>
#include <boost/parameter/aux_/maybe.hpp>
#include <boost/parameter/aux_/arg_list.hpp>
#include <boost/parameter/config.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/iterator_tags.hpp>

#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

namespace boost { namespace parameter { 

// Forward declaration for aux::arg_list, below.
template<class T> struct keyword_base;

namespace aux {

// Tag type passed to MPL lambda.
struct lambda_tag;

//
// Structures used to build the tuple of actual arguments.  The
// tuple is a nested cons-style list of arg_list specializations
// terminated by an empty_arg_list.
//
// Each specialization of arg_list is derived from its successor in
// the list type.  This feature is used along with using
// declarations to build member function overload sets that can
// match against keywords.
//
  
// MPL sequence support
struct typed_arg_list_tag;

// Terminates typed_arg_list<> and represents an empty list.  Since this
// is just the terminating case you might want to look at typed_arg_list
// first, to get a feel for what's really happening here.

struct empty_typed_arg_list
{
    empty_typed_arg_list() {}

    // Constructor taking BOOST_PARAMETER_MAX_ARITY empty_typed_arg_list
    // arguments; this makes initialization
    empty_typed_arg_list(
        BOOST_PP_ENUM_PARAMS(
            BOOST_PARAMETER_MAX_ARITY, void_ BOOST_PP_INTERCEPT
        ))
    {}

    // A metafunction class that, given a keyword and a default
    // type, returns the appropriate result type for a keyword
    // lookup given that default
    struct binding
    {
        template<class KW, class Default, class Reference>
        struct apply
        {
            typedef Default type;
        };
    };

#if !BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
    // Terminator for has_key, indicating that the keyword is unique
    template <class KW>
    static no_tag has_key(KW*);
#endif
    
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300) \
    || (BOOST_WORKAROUND(__GNUC__, < 3)) \
    || BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    
    // The overload set technique doesn't work with these older
    // compilers, so they need some explicit handholding.
      
    // A metafunction class that, given a keyword, returns the type
    // of the base sublist whose get() function can produce the
    // value for that key
    struct key_owner
    {
        template<class KW>
        struct apply
        {
            typedef empty_typed_arg_list type;
        };
    };

    template <class K, class T>
    T& get(default_<K,T> x) const
    {
        return x.value;
    }

    template <class K, class F>
    typename result_of0<F>::type
    get(lazy_default<K,F> x) const
    {
        return x.compute_default();
    }
#endif

    // If this function is called, it means there is no argument
    // in the list that matches the supplied keyword. Just return
    // the default value.
    template <class Tag>
    const typename Tag::value_type &operator[](const keyword_base<Tag>&) const
    {
        return Tag::default_value();
    }

    // If this function is called, it means there is no argument
    // in the list that matches the supplied keyword. Just return
    // the default value.
    template <class K, class Default>
    Default& operator[](default_<K, Default> x) const
    {
        return x.value;
    }

    // If this function is called, it means there is no argument
    // in the list that matches the supplied keyword. Just evaluate
    // and return the default value.
    template <class K, class F>
    typename result_of0<F>::type
    operator[](
        BOOST_PARAMETER_lazy_default_fallback<K,F> x) const
    {
        return x.compute_default();
    }

    // No argument corresponding to ParameterRequirements::key_type
    // was found if we match this overload, so unless that parameter
    // has a default, we indicate that the actual arguments don't
    // match the function's requirements.
    template <class ParameterRequirements, class ArgPack>
    static typename ParameterRequirements::has_default
    satisfies(ParameterRequirements*, ArgPack*);

    // MPL sequence support
    typedef empty_typed_arg_list type;   // convenience
    typedef typed_arg_list_tag tag; // For dispatching to sequence intrinsics
};

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
template<class KW>
no_tag operator*(empty_typed_arg_list, KW*);
#endif

// Forward declaration for typed_arg_list::operator,
template <class KW, class Arg>
struct typed_tagged_argument;

// A tuple of tagged arguments, terminated with empty_typed_arg_list.
// Every TaggedArg is an instance of typed_tagged_argument<>.
template <class TaggedArg, class Next = empty_typed_arg_list>
struct typed_arg_list : Next
{
    typedef typed_arg_list<TaggedArg,Next> self;
    typedef typename TaggedArg::key_type key_type;

    typedef typename is_maybe<typename TaggedArg::value_type>::type holds_maybe;

    typedef typename mpl::eval_if<
        holds_maybe
      , get_reference<typename TaggedArg::value_type>
      , get_reference<TaggedArg>
    >::type reference;

    typedef typename TaggedArg::index_result_type index_result_type;

    typedef typename mpl::if_<
        holds_maybe
      , reference
      , typename TaggedArg::value_type
    >::type value_type;

    TaggedArg arg;      // Stores the argument

    // Store the arguments in successive nodes of this list
    template< // class A0, class A1, ...
        BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_ARITY, class A)
    >
    typed_arg_list( // A0& a0, A1& a1, ...
        BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PARAMETER_MAX_ARITY, A, & a)
    )
      : Next( // a1, a2, ...
            BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PARAMETER_MAX_ARITY, a)
          , void_reference()
        )
      , arg(a0)
    {}
    
    // Read the arguments from a typed_arg_list
    template<typename CTaggedArg, typename CNext>
    typed_arg_list(const typed_arg_list<CTaggedArg, CNext> &args)
      : Next(args)
      , arg(args[keyword_base<key_type>()])
    {}

    // Create a new list by prepending arg to a copy of tail.  Used
    // when incrementally building this structure with the comma
    // operator.
    typed_arg_list(TaggedArg arg, Next const& tail)
      : Next(tail)
      , arg(arg)
    {}

    // A metafunction class that, given a keyword and a default
    // type, returns the appropriate result type for a keyword
    // lookup given that default
    struct binding
    {
        template <class KW, class Default, class Reference>
        struct apply
        {
          typedef typename mpl::eval_if<
                boost::is_same<KW, key_type>
              , mpl::if_<Reference, reference, value_type>
              , mpl::apply_wrap3<typename Next::binding, KW, Default, Reference>
          >::type type;
        };
    };
    
#  define BOOST_PARAMETER_CALL_HAS_KEY(next, key) next::has_key((key*)0)  

    BOOST_MPL_ASSERT_MSG(
        sizeof(BOOST_PARAMETER_CALL_HAS_KEY(Next,key_type)) == sizeof(no_tag)
      , duplicate_keyword, (key_type)
    );

# undef BOOST_PARAMETER_CALL_HAS_KEY
    //
    // Begin implementation of indexing operators for looking up
    // specific arguments by name
    //

    // Helpers that handle the case when TaggedArg is 
    // empty<T>.
    template <class D>
    reference get_default(D const&, mpl::false_) const
    {
        return arg.value;
    }

    template <class D>
    reference get_default(D const& d, mpl::true_) const
    {
        return arg.value ? arg.value.get() : arg.value.construct(d.value);
    }

    index_result_type operator[](keyword_base<key_type> const&) const
    {
        BOOST_MPL_ASSERT_NOT((holds_maybe));
        return arg.value;
    }


    template <class Default>
    reference operator[](default_<key_type, Default> const& d) const
    {
        return get_default(d, holds_maybe());
    }

    template <class Default>
    reference operator[](lazy_default<key_type, Default>) const
    {
        return arg.value;
    }

    // Builds an overload set including operator[]s defined in base
    // classes.
    using Next::operator[];

    //
    // End of indexing support
    //


    //
    // For parameter_requirements matching this node's key_type,
    // return a bool constant wrapper indicating whether the
    // requirements are satisfied by TaggedArg.  Used only for
    // compile-time computation and never really called, so a
    // declaration is enough.
    //
    template <class HasDefault, class Predicate, class ArgPack>
    static typename mpl::apply_wrap2<
        typename mpl::lambda<Predicate, lambda_tag>::type
      , value_type, ArgPack
    >::type
    satisfies(
        parameter_requirements<key_type,Predicate,HasDefault>*
      , ArgPack*
    );

    // Builds an overload set including satisfies functions defined
    // in base classes.
    using Next::satisfies;

    // Comma operator to compose argument list without using parameters<>.
    // Useful for argument lists with undetermined length.
    template <class KW, class Arg>
    typed_arg_list<typed_tagged_argument<KW, Arg>, self> 
    operator,(typed_tagged_argument<KW, Arg> x) const
    {
        return typed_arg_list<typed_tagged_argument<KW, Arg>, self>(x, *this);
    }

    // MPL sequence support
    typedef self type;             // Convenience for users
    typedef Next tail_type;        // For the benefit of iterators
    typedef typed_arg_list_tag tag; // For dispatching to sequence intrinsics
};

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)  // ETI workaround
template <> struct typed_arg_list<int,int> {};
#endif 

// MPL sequence support
template <class ArgumentPack>
struct typed_arg_list_iterator
{
    typedef mpl::forward_iterator_tag category;

    // The incremented iterator
    typedef typed_arg_list_iterator<typename ArgumentPack::tail_type> next;
    
    // dereferencing yields the key type
    typedef typename ArgumentPack::key_type type;
};

template <>
struct typed_arg_list_iterator<empty_typed_arg_list> {};

}} // namespace parameter::aux

// MPL sequence support
namespace mpl
{
  template <>
  struct begin_impl<parameter::aux::typed_arg_list_tag>
  {
      template <class S>
      struct apply
      {
          typedef parameter::aux::typed_arg_list_iterator<S> type;
      };
  };

  template <>
  struct end_impl<parameter::aux::typed_arg_list_tag>
  {
      template <class>
      struct apply
      {
          typedef parameter::aux::typed_arg_list_iterator<parameter::aux::empty_typed_arg_list> type;
      };
  };
}

} // namespace boost

#endif // ARG_LIST_050329_HPP

