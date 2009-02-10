//////////////////////////////////////////////////////////////////////////////
// divider.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_DIVIDER_HPP_ER_2009
#define BOOST_MATH_IFGT_DIVIDER_HPP_ER_2009
namespace boost{namespace math{namespace ifgt{

    template<typename RealType>
    struct divider{
        public:
        typedef RealType                                        value_type;
        template<typename ArgPack>
        divider(const ArgPack arg):x_(arg[tag::divide_by|(value_type)(2)]){}
        divider(const divider& that):x_(that.x_){}
        divider& operator=(const divider& that){
            if(&that!=this){
                x_ = that.x_;
            }
            return *this;
        }

        value_type operator()(value_type y){ return y/x_; }
        private:
        value_type x_;
    };

}}}
#endif
