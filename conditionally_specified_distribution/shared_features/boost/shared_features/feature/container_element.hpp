////////////////////////////////////////////////////////////////////////////
// container_element.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SHARED_FEATURES_CONTAINER_ELEMENT_HPP_ER_2009
#define BOOST_SHARED_FEATURES_CONTAINER_ELEMENT_HPP_ER_2009
#include <boost/range.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/utility/dont_care.hpp>
#include <boost/utility/remove_qualifier.hpp>
#include <boost/shared_features/set.hpp>
#include <boost/shared_features/feature/keyword.hpp>

namespace boost{
namespace shared_features{
namespace impl
{

    template<typename Container,typename Id>
    class container_element_base{
        protected:
        typedef typename Container::result::type        cont_t;
        typedef typename range_value<
            typename utility::remove_qualifier<cont_t>::type
        >::type                                         value_type;
        typedef typename range_size<
            typename utility::remove_qualifier<cont_t>::type
        >::type                                         size_type;
        public:
        struct result : mpl::identity<value_type>{};
    };

    template<
        typename Container,
        typename Id
    >
    class container_element :
        public container_element_base<typename Container::traits,Id>
    {
        typedef container_element_base<typename Container::traits,Id>
                                                            super_t;
        typedef kwd<Id>                                     kwd;
        typedef Container                                   feature_cont_t;
        public:
        container_element():position_(0){}
        template<typename Args>
        container_element(const Args& args)
        :position_(args[kwd::position|0]){
            typedef mpl::bool_<false> f;
            alert_if(args[kwd_do_initialize|f()],args[kwd::position|-1]);
        }
        container_element(const container_element& that)
        :position_(that.position_){}

        container_element&
        operator=(const container_element& that){
            if(&that!=this){
                position_ = that.position_;
            }
            return (*this);
        }

        template<typename Args>
        void set(
            const Args& args,
            typename super_t::value_type x
        ){
            return this->set_impl(
                args[kwd_set],
                args,
                x
            );
        }

        template<typename Args>
        typename super_t::result::type
        operator()(const Args& args)const{
            return get(args[kwd_set],args);
        }

        typename super_t::size_type
        position()const{ return this->position_; }

        typename super_t::size_type
        position(utility::dont_care)const{ return this->position(); }

        private:
        template<typename Set,typename Args>
        typename super_t::result::type
        get(const Set& set,const Args& args)const{
            typename super_t::size_type pos = this->position();
            return (set.template extract<feature_cont_t>()).at(args,pos);
        }
        template<typename Set,typename Args>
        void set_impl(
            Set& set,
            const Args& args,
            typename super_t::value_type x
        ){//const
            typename super_t::size_type pos = (this->position());
            (set.template extract<feature_cont_t>)().set_at(args,pos,x);
        }
        void alert_if(mpl::bool_<true>,int i)const{
            BOOST_ASSERT(i>-1);
        }
        void alert_if(mpl::bool_<false>,int i)const{
        }
        typename super_t::size_type position_;
    };
} // namespace impl
namespace feature{
    template <typename Cont,typename Id = mpl::void_>
    struct container_element
      : shared_features::depends_on<
        mpl::vector<Cont>
      >
    {
      typedef impl::container_element_base<typename Cont::traits,Id> traits;
      typedef impl::container_element<Cont,Id>      impl;
    };
}

}
}


#endif // ELEMENT_HPP_INCLUDED
