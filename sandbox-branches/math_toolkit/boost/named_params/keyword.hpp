// Copyright Daniel Wallin, David Abrahams 2005. Use, modification and
// distribution is subject to the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef KEYWORD_050328_HPP
#define KEYWORD_050328_HPP

#include <boost/named_params/aux_/unwrap_cv_reference.hpp>
#include <boost/named_params/aux_/tagged_argument.hpp>
#include <boost/named_params/aux_/default.hpp>

namespace boost { namespace named_params {

// Instances of unique specializations of keyword<...> serve to
// associate arguments with parameter names.  For example:
//
//    struct rate_;           // parameter names
//    struct skew_;
//    namespace
//    {
//      keyword<rate_> rate;  // keywords
//      keyword<skew_> skew;
//    }
//
//    ...
//
//    f(rate = 1, skew = 2.4);
//
template <class Tag>
struct keyword
{
    template <class T>
    aux::tagged_argument<
        Tag
      , typename aux::unwrap_cv_reference<T>::type
    > 
    operator=(T& x) const
    {
        return aux::tagged_argument<
            Tag
          , BOOST_DEDUCED_TYPENAME aux::unwrap_cv_reference<T>::type
        >(x);
    }

    template <class Default>
    aux::default_<Tag, Default>
    operator|(Default& default_) const
    {
        return aux::default_<Tag, Default>(default_);
    }

    template <class Default>
    aux::lazy_default<Tag, Default>
    operator||(Default& default_) const
    {
        return aux::lazy_default<Tag, Default>(default_);
    }

#if !BOOST_WORKAROUND(BOOST_MSVC, == 1200)  // avoid partial ordering bugs
    template <class T>
    aux::tagged_argument<
        Tag
      , typename aux::unwrap_cv_reference<T const>::type
    > 
    operator=(T const& x) const
    {
        return aux::tagged_argument<
            Tag
          , BOOST_DEDUCED_TYPENAME aux::unwrap_cv_reference<T const>::type
        >(x);
    }

    template <class Default>
    aux::default_<Tag, const Default>
    operator|(const Default& default_) const
    {
        return aux::default_<Tag, const Default>(default_);
    }

    template <class Default>
    aux::lazy_default<Tag, Default>
    operator||(Default const& default_) const
    {
        return aux::lazy_default<Tag, Default>(default_);
    }
#endif 
};

}} // namespace boost::named_params

#endif // KEYWORD_050328_HPP

