////////////////////////////////////////////////////////////////////////////
// scalar.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SHARED_FETURES_SCALAR_HPP_ER_2009
#define BOOST_SHARED_FETURES_SCALAR_HPP_ER_2009
#include <boost/mpl/bool.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/utility/assert_is_base_of.hpp>
#include <boost/utility/dont_care.hpp>
#include <boost/shared_features/set.hpp>
#include <boost/shared_features/feature/keyword.hpp>
namespace boost{namespace shared_features{

namespace impl
{
    template<typename T>
    class scalar_base{
        public:
        typedef T                value_type;
        struct result : mpl::identity<T>{};
    };

    template<typename T,typename Id = mpl::void_>
    class scalar : scalar_base<T>
    {
        typedef scalar_base<T>                          super_t;
        typedef kwd<Id>                                 kwd;
        public:
        typedef typename super_t::value_type            value_type;


        scalar(): value_((value_type)(0)){}

        scalar(value_type value):value_(value){}

        template<typename Args>
        scalar(const Args& args)
            : //TODO pass scalar directly as an option
            value_((value_type)(0))
        {
            typedef mpl::bool_<false> f;
            set_if(args[kwd_do_initialize|f()],args);
        }

        scalar(const scalar& that) : value_(that.value_){}

        scalar&
        operator=(const scalar& that){
            if(&that!=this){
                value_ = that.value_;
            }
            return *this;
        }

        void set(value_type value){
            this->value_ = value;
        };
        void set(utility::dont_care,value_type value){
            return this->set(value);
        };

        value_type operator()()const{return value_;}
        value_type operator()(utility::dont_care)const{
            return this->operator()();
        }

        private:
        template<typename Args>
        void set_if(mpl::bool_<true>,const Args& args){
            set(args[kwd::scalar]);
        }
        void set_if(mpl::bool_<false>,utility::dont_care){}
        value_type value_;
    };

} // namespace impl
namespace feature{
    template <typename T,typename Id>
    struct scalar
      : depends_on0
    {
      typedef impl::scalar_base<T>   traits;
      typedef impl::scalar<T,Id>     impl;
    };
}


}}

#endif // SCALAR_HPP_INCLUDED
