////////////////////////////////////////////////////////////////////////////
// container.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SHARED_FEATURES_CONTAINER_HPP_ER_2009
#define BOOST_SHARED_FEATURES_CONTAINER_HPP_ER_2009
#include <boost/range.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/utility/dont_care.hpp>
#include <boost/shared_features/set.hpp>
#include <boost/shared_features/feature/keyword.hpp>

namespace boost{namespace shared_features{

namespace impl
{

    template<typename Container,typename Id>
    class container_base{

        protected:
        typedef Container                                   elements_t;

        // used for returning a range (subset)
        typedef iterator_range<const elements_t>         iterator_range;

        typedef typename range_size<elements_t>::type       size_type;
        typedef typename range_iterator<elements_t>::type   iterator;

        public:
        typedef typename range_value<elements_t>::type      value_type;

        struct result : mpl::identity<const elements_t&>{};
        struct result_of_at : mpl::identity<value_type>{};
        struct result_of_range : mpl::identity<iterator_range>{};
        struct identifier : mpl::identity<Id>{};
    };

    template<
        typename Container,
        typename Id
    >
    class container : public container_base<Container,Id>{
        typedef container_base<Container,Id>                super_t;
        typedef kwd<Id>                                     kwd;
        typedef container<Container,Id>                     this_type;
        public:

        container(){}

        template<typename Args>
        container(const Args& args){
            typedef mpl::bool_<false> f;
            set_if(
                args[shared_features::kwd_do_initialize|f()],
                args
            );
        }

        container(const container& that){
            set(that());
        }
        container&
        operator=(const container& that){
            if(&that!=this){
                set(that());
            }
            return (*this);
        }

        //modify
        template<typename R>
        void set(const R& r){
            (this->elements_).clear();
            std::copy(
                begin(r),
                end(r),
                back_inserter(elements_)
            );
        };
        template<typename R>
        void set(utility::dont_care,const R& r){
            return this->set(r);
        }

        void set_at(
            typename super_t::size_type pos,
            typename super_t::value_type x
        ){
            typedef typename
                range_iterator<typename super_t::elements_t>::type it_t;
            it_t i = begin(elements_);
            std::advance(i,pos);
            *i = x;
        };
        void set_at(
            utility::dont_care,
            typename super_t::size_type pos,
            typename super_t::value_type x
        ){
            return this->set_at(pos,x);
        }

        template<typename R>
        void replace(typename super_t::size_type position, const R& r){
            BOOST_ASSERT(position+size(r)-(this->size())>=0);
            typedef typename
                range_iterator<typename super_t::elements_t>::type it_t;
            it_t i = begin(elements_);
            std::advance(i,position);
            copy(
                begin(r),
                end(r),
                i
            );
        }
        template<typename R>
        void replace(
            utility::dont_care,
            typename super_t::size_type position,
            const R& r
        ){
            return this->replace(position,r);
        }

        //query
        typename super_t::size_type
        size()const{
            return size(this->elements);
        }
        typename super_t::size_type
        size(utility::dont_care)const{ return size(); }

        typename super_t::result::type
        operator()()const{ return this->elements_; }

        typename super_t::result::type
        operator()(utility::dont_care)const{ return this->operator()(); }

        typename super_t::result_of_at::type
        at(typename super_t::size_type pos)const{
            typedef typename
                range_iterator<
                    typename add_const<typename super_t::elements_t>::type
                >::type const_it_t;
            const_it_t i = begin(this->operator()());
            std::advance(i,pos);
            return (*i);
        }
        typename super_t::result_of_at::type
        at(utility::dont_care,typename super_t::size_type pos)const{
            return this->at(pos);
        }

        typename super_t::result_of_range::type
        range(
            typename super_t::size_type position,
            typename super_t::size_type size
        ){
            return make_iterator_range(elements_,position,position+size);
        }


        typename super_t::result_of_range::type
        range(
            utility::dont_care,
            typename super_t::size_type position,
            typename super_t::size_type size
        ){
            return this->range(position,size) ;
        }

        private:
        template<typename Args>
        void set_if(mpl::bool_<true>,const Args& args){
            set(args[kwd::container]);
        }
        void set_if(mpl::bool_<false>,utility::dont_care){}
        typename super_t::elements_t elements_;
    };

} // namespace impl
namespace feature{
    // C is not a feature but physical container
    template <typename C,typename Id = mpl::void_>
    struct container
      : shared_features::depends_on0
    {
      typedef impl::container_base<C,Id> traits;
      typedef impl::container<C,Id>      impl;
    };
}


}}

#endif
