/*=================================---------------------------------------------
    Copyright Daniel Wallin, David Abrahams 2005.
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST_PARAMETER_TYPED_TAGGED_ARGUMENT_050328_HPP
# define BOOST_PARAMETER_TYPED_TAGGED_ARGUMENT_050328_HPP

# include <boost/parameter/aux_/void.hpp>
# include <boost/parameter/aux_/typed_arg_list.hpp>
# include <boost/parameter/aux_/result_of0.hpp>
# include <boost/parameter/aux_/tagged_argument.hpp>
# include <boost/mpl/if.hpp>
# include <boost/mpl/apply_wrap.hpp>
# include <boost/mpl/and.hpp>
# include <boost/mpl/not.hpp>
# include <boost/type_traits/is_same.hpp>
# include <boost/type_traits/is_convertible.hpp>
# include <boost/type_traits/is_reference.hpp>

namespace boost { namespace parameter {

template <class Tag>
struct keyword_base;

namespace aux {

// Holds a reference to an argument of type Keyword::value_type associated with
// keyword Keyword
    
template <class Keyword, class Arg>
struct typed_tagged_argument : tagged_argument_base
{
    typedef Keyword key_type;
    typedef Arg value_type;
    typedef Arg& reference;

    typed_tagged_argument(reference x) : value(x) {}

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
              , mpl::identity<Default>
          >::type type;
        };
    };

    // Comma operator to compose argument list without using parameters<>.
    // Useful for argument lists with undetermined length.
    template <class Keyword2, class Arg2>
    typed_arg_list<
        typed_tagged_argument
      , typed_arg_list<typed_tagged_argument<Keyword2, Arg2> > 
    >
    operator,(typed_tagged_argument<Keyword2, Arg2> x) const
    {
        return typed_arg_list<
            typed_tagged_argument
          , typed_arg_list<typed_tagged_argument<Keyword2, Arg2> > 
        >(
            *this
          , typed_arg_list<typed_tagged_argument<Keyword2, Arg2> >(x, empty_typed_arg_list())
        );
    }

    // If this function is called, it means there is no argument
    // in the list that matches the supplied keyword. Just return
    // the default value.
    template <class Tag>
    typename Tag::value_type & operator[](const keyword_base<Tag>&) const
    {
        return Tag::default_value();
    }

    reference operator[](keyword_base<Keyword> const&) const
    {
        return value;
    }

/*    reference operator[](untyped_keyword<Keyword> const&) const
    {
        return value;
    }*/

    template <class Default>
    reference operator[](default_<key_type,Default> const& x) const
    {
        return value;
    }

    template <class F>
    reference operator[](lazy_default<key_type,F> const& x) const
    {
        return value;
    }

    template <class KW, class Default>
    Default& operator[](default_<KW,Default> const& x) const
    {
        return x.value;
    }

    template <class KW, class F>
    typename result_of0<F>::type operator[](lazy_default<KW,F> const& x) const
    {
        return x.compute_default();
    }

    template <class ParameterRequirements>
    static typename ParameterRequirements::has_default
    satisfies(ParameterRequirements*);

    template <class HasDefault, class Predicate>
    static typename mpl::apply1<Predicate, value_type>::type
    satisfies(
        parameter_requirements<key_type,Predicate,HasDefault>*
    );

    reference value;
# if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1310))
    // warning suppression
 private:
    void operator=(tagged_argument const&);
 public:    
# endif
    // MPL sequence support
    typedef typed_tagged_argument type;            // Convenience for users
    typedef empty_typed_arg_list tail_type;        // For the benefit of iterators
    typedef arg_list_tag tag; // For dispatching to sequence intrinsics
    
    template<typename TypedTaggedArgument, typename Next>
    operator const typed_arg_list<TypedTaggedArgument, Next> () const
    {
        return typed_arg_list<typed_tagged_argument>(*this, empty_typed_arg_list());
    }
};

}}} // namespace boost::parameter::aux

#endif // BOOST_PARAMETER_TYPED_TAGGED_ARGUMENT_050328_HPP

