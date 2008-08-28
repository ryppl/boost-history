/*=================================---------------------------------------------
    Copyright Daniel Wallin, David Abrahams 2005.
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef TYPED_KEYWORD_050328_HPP
#define TYPED_KEYWORD_050328_HPP

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
struct typed_keyword
{
    typedef aux::typed_tagged_argument<Tag> typed_tagged_argument_type;
    
    typed_tagged_argument_type const
    operator=(typename Tag::value_type& x) const
    {
        return typed_tagged_argument_type(x);
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

 public: // Insurance against ODR violations
    
    // People will need to define these keywords in header files.  To
    // prevent ODR violations, it's important that the keyword used in
    // every instantiation of a function template is the same object.
    // We provide a reference to a common instance of each keyword
    // object and prevent construction by users.
    static typed_keyword<Tag> const instance;

    // This interface is deprecated
    static typed_keyword<Tag>& get()
    {
        return const_cast<typed_keyword<Tag>&>(instance);
    }
};

template <class Tag>
typed_keyword<Tag> const typed_keyword<Tag>::instance = {};

}} // namespace boost::parameter

#endif // KEYWORD_050328_HPP

