/*=================================---------------------------------------------
    Copyright Daniel Wallin, David Abrahams 2005.
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef KEYWORD_BASE_050328_HPP
#define KEYWORD_BASE_050328_HPP

#include <boost/parameter/aux_/unwrap_cv_reference.hpp>
#include <boost/parameter/aux_/typed_tagged_argument.hpp>
#include <boost/parameter/aux_/default.hpp>

namespace boost { namespace parameter {

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
struct keyword_base
{
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

};

}} // namespace boost::parameter

#endif // KEYWORD_BASE_050328_HPP

