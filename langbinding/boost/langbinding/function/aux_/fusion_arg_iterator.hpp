// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FUSION_VOID_PTR_ITERATOR_040923_HPP
#define FUSION_VOID_PTR_ITERATOR_040923_HPP

#include <boost/spirit/fusion/iterator/detail/iterator_base.hpp>

namespace boost { namespace langbinding { namespace function { namespace aux {

struct fusion_arg_iterator;
    
struct fusion_arg_iterator 
    : fusion::iterator_base<fusion_arg_iterator>
{
    typedef fusion_arg_iterator tag;

    fusion_arg_iterator(converter::from_xxx_data* p_) : p(p_) {}

    converter::from_xxx_data* p;
};

}}}} // namespace boost::langbinding::function::aux

namespace boost { namespace fusion { namespace meta {
   
    template <typename Tag>
    struct next_impl;

    template <>
    struct next_impl<langbinding::function::aux::fusion_arg_iterator>
    {
        template <typename Iterator>
        struct apply
        {
            typedef langbinding::function::aux::fusion_arg_iterator type;

            static type call(Iterator i)
            {
                return langbinding::function::aux::fusion_arg_iterator(
                    i.p + 1
                );
            };
        };
    };

    template <typename Tag>
    struct deref_impl;

    template <>
    struct deref_impl<langbinding::function::aux::fusion_arg_iterator>
    {
        template <typename Iterator>
        struct apply
        {
            typedef langbinding::converter::from_xxx_data& type;

            static type call(Iterator i)
            {
                return *i.p;
            }
        };
    };

}}} // namespace boost::fusion::meta

#endif // FUSION_VOID_PTR_ITERATOR_040923_HPP

