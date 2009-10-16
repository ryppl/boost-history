//////////////////////////////////////////////////////////////////////////////
// detail::fusion::functor::identity_f.hpp                                  //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_FUSION_MAP_IDENTITY_F_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_FUSION_MAP_IDENTITY_F_HPP_ER_2009
#include <boost/utility/result_of.hpp>

#include <boost/fusion/include/pair.hpp>


#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/map_fwd.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/include/at_key.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace fusion{
namespace map{

    template<typename K1,typename K2,typename F>
    class identity_f{
    
        public:

        struct identity
        {
            typedef K1 key_;
            template<typename X>
            struct apply{
                typedef boost::fusion::pair<K1,X> pair_;
            };
        };
        
        struct function
        {
            typedef K2 key_;

            template<typename X>
            struct apply{
                typedef typename boost::result_of<F(const X&)>::type cr_data2_;
                typedef typename remove_cv<
                    typename remove_reference<
                        cr_data2_
                    >::type
                >::type data2_;
                typedef boost::fusion::pair<K2,data2_> pair_;
            };
        };

        identity_f()
            {}
        identity_f(const F& f)
            :f_(f){}
        identity_f(const identity_f& that)
            :f_(that.f_){}

        identity_f& operator=(const identity_f& that)
        {
            if(&that!=this)
            {
                //static_cast<F&>(*this) = static_cast<const F&>(that);
                this->f_ = that.f_;
            }
            return *this;
        }

        template<typename S>    
        struct result{};

        template<typename F1,typename X>
        struct result<F1(const X&)>
        {
            typedef typename identity::template apply<X> app1_;
            typedef typename function::template apply<X> app2_;
            typedef typename app1_::pair_ p1_;
            typedef typename app2_::pair_ p2_;
            typedef boost::fusion::map<p1_,p2_> type;
        };

        template<typename S>
        struct sig : result<S>{};

        template<typename X>
        typename result<identity_f(const X&)>::type
        operator()(const X& x)const
        {
            typedef typename result<identity_f(const X&)>::type type;
            return type(
                boost::fusion::make_pair<K1>(x),
                boost::fusion::make_pair<K2>(
                    this->f_(x)
                )
            );
        }
    
        private:
        F f_;
    };

}// map
}// fusion
}// detail
}// statistics
}// boost

#endif
