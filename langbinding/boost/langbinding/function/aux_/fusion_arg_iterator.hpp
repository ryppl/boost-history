// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FUSION_VOID_PTR_ITERATOR_040923_HPP
#define FUSION_VOID_PTR_ITERATOR_040923_HPP

#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/support/iterator_base.hpp>

#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/equal_to.hpp>

#include <boost/langbinding/converter/converter_function.hpp>

namespace boost { namespace langbinding { namespace function { namespace aux {

// Define a sequence sized by an MPL Integral Constant
template <typename Arity>
struct fusion_arg_sequence {
    typedef Arity size;
    fusion_arg_sequence(converter::from_xxx_data* cvtrs) : converters(cvtrs) {}

    converter::from_xxx_data* converters;
};

struct fusion_arg_sequence_tag;

template <typename Pos>
struct fusion_arg_iterator 
    : fusion::iterator_base<fusion_arg_iterator<Pos> >
{
    typedef Pos index;
    // This could easily be random_access_traversal_tag, but all we need is a 
    // forward sequence:
    typedef fusion::forward_traversal_tag category;
    fusion_arg_iterator(converter::from_xxx_data* p_) : p(p_) {}

    // Pointer to corresponding data element
    converter::from_xxx_data* p;
};

struct fusion_arg_iterator_tag;
    
}}}} // namespace boost::langbinding::function::aux

// Specialize all the necessary fusion metafunctions/metaclasses to define a 
// fusion Sequence 

namespace boost { namespace fusion { 

namespace traits {

    template<typename Arity>
    struct tag_of<langbinding::function::aux::fusion_arg_sequence<Arity> >
    {
        typedef langbinding::function::aux::fusion_arg_sequence_tag type;
    };

    template<typename Pos>
    struct tag_of<langbinding::function::aux::fusion_arg_iterator<Pos> >
    {
        typedef langbinding::function::aux::fusion_arg_iterator_tag type;
    };

} // namespace traits

namespace extension {

    // Define the iterator's operations first
   
    template <typename Tag>
    struct next_impl;

    template <>
    struct next_impl<langbinding::function::aux::fusion_arg_iterator_tag>
    {
        template <typename Iterator>
        struct apply
        {
            typedef typename Iterator::index index;
            typedef langbinding::function::aux::fusion_arg_iterator<
                typename mpl::next<index>::type
            > type;

            static type call(Iterator const & i)
            {
                return type(i.p);
            };
        };
    };

    template <typename Tag>
    struct deref_impl;

    template <>
    struct deref_impl<langbinding::function::aux::fusion_arg_iterator_tag>
    {
        template <typename Iterator>
        struct apply
        {
            typedef langbinding::converter::from_xxx_data & type;

            static type call(Iterator const & i)
            {
                return i.p[Iterator::index::value];
            }
        };
    };

    template<typename Tag>
    struct equal_to_impl;

    template<>
    struct equal_to_impl<langbinding::function::aux::fusion_arg_iterator_tag>
    {
        template<typename It1, typename It2>
        struct apply
            : mpl::equal_to<
            typename It1::index,
            typename It2::index>
        {};
    };

    // Now define the sequence's intrinsics

    template<typename Tag>
    struct is_sequence_impl;
    
    template<>
    struct is_sequence_impl<langbinding::function::aux::fusion_arg_sequence_tag>
    {
        template<typename Sequence>
        struct apply : mpl::true_ {};
    };

    template<typename Tag>
    struct is_view_impl;
    
    template<>
    struct is_view_impl<langbinding::function::aux::fusion_arg_sequence_tag>
    {
        template<typename Sequence>
        struct apply : mpl::false_ {};
    };

    template<typename Tag>
    struct size_impl;

    template<>
    struct size_impl<langbinding::function::aux::fusion_arg_sequence_tag>
    {
        template<typename Sequence>
        struct apply
            : Sequence::size
        {};
    };

    template<typename Tag>
    struct begin_impl;

    template<>
    struct begin_impl<langbinding::function::aux::fusion_arg_sequence_tag>
    {
        template<typename Sequence>
        struct apply
        {
            typedef langbinding::function::aux::fusion_arg_iterator<
                mpl::int_<0>::type
            > type;

            static type call(Sequence & seq)
            {
                return type(seq.converters);
            }
        };
    };

    template<typename Tag>
    struct end_impl;

    template<>
    struct end_impl<langbinding::function::aux::fusion_arg_sequence_tag>
    {
        template<typename Sequence>
        struct apply
        {
            typedef langbinding::function::aux::fusion_arg_iterator<
                typename Sequence::size::type
            > type;

            static type call(Sequence & seq)
            {
                return type(seq.converters);
            }
        };
    };

} // namespace extension

}} // namespace boost::fusion


#endif // FUSION_VOID_PTR_ITERATOR_040923_HPP

