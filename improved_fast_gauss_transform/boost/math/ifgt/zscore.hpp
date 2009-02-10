//////////////////////////////////////////////////////////////////////////////
// ifgt/zscore.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_ZSCORE_HPP_ER_2009
#define BOOST_MATH_ZSCORE_HPP_ER_2009
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <boost/range.hpp>
#include <boost/assert.hpp>
#include <boost/bind.hpp>
namespace boost{namespace math{

    /// Maps x,y,h to (x-y)/h
    template<typename R0>
    class zscore{
        typedef typename range_iterator<R0>::type     iter_type;
     public:
        typedef typename iter_type::value_type   value_type;
        zscore(const R0& center_):center(center_){}
        zscore(const zscore& that):center(that.zscore){}

        template<typename R1,typename RealType>
        void operator()(const R1& source,RealType bandwidth,R0& out){
            BOOST_ASSERT(size(source)==size(center));

            std::transform(
                begin(source),
                end(source),
                begin(center),
                begin(out),
                bind(
                    std::divides<value_type>(),
                    bind(
                        std::minus<value_type>(),
                        _1,
                        _2
                    ),
                    (value_type)(bandwidth)
                )
            );
        }
      private:
        zscore& operator=(const zscore& that);
        zscore();
        const R0&    center;
    };

    template<typename R0>
    zscore<R0> make_zscore(const R0& center){return zscore<R0>(center);}
}}

#endif
