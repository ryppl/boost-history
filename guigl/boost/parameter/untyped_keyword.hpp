/*=================================---------------------------------------------
    Copyright Daniel Wallin, David Abrahams 2005.
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef UNTYPED_KEYWORD_050328_HPP
#define UNTYPED_KEYWORD_050328_HPP

#include <boost/parameter/keyword_base.hpp>

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
struct untyped_keyword : public keyword_base<Tag>
{    
    template<typename T>
    aux::typed_tagged_argument<Tag, T> const
    operator=(T& x) const
    {
        return aux::typed_tagged_argument<Tag, T>(x);
    }

    template<typename T>
    aux::typed_tagged_argument<Tag, const T> const
    operator=(const T& x) const
    {
        return aux::typed_tagged_argument<Tag, const T>(x);
    }

 public: // Insurance against ODR violations
    
    // People will need to define these keywords in header files.  To
    // prevent ODR violations, it's important that the keyword used in
    // every instantiation of a function template is the same object.
    // We provide a reference to a common instance of each keyword
    // object and prevent construction by users.
    static untyped_keyword<Tag> const instance;

    // This interface is deprecated
    static untyped_keyword<Tag>& get()
    {
        return const_cast<typed_keyword<Tag>&>(instance);
    }
};

template <class Tag>
untyped_keyword<Tag> const untyped_keyword<Tag>::instance = untyped_keyword<Tag>();

}} // namespace boost::parameter

#endif // UNTYPED_KEYWORD_050328_HPP

