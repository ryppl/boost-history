///////////////////////////////////////////////////////////////////////////////
//  constant.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_GENERATOR_CONSTANT_HPP_ER_2009
#define BOOST_GENERATOR_CONSTANT_HPP_ER_2009
#include <boost/math/tools/precision.hpp>

namespace boost{
namespace generator{

    template<typename T>
    class constant{
        public:
        typedef T result_type;
        constant():x_(T()){}
        constant(T x): x_(x){}

        T operator()()const{
            return x_;
        }

        private:
        T x_;
    };

    //Useful as "no censoring"
    template<typename T>
    class max_value : public constant<T>{
        typedef constant<T> super_t;
        public:
        max_value(): super_t(math::tools::max_value<T>()){}

        private:
        max_value(T x);
    };


}
}
#endif // CONSTANT_HPP_INCLUDED
