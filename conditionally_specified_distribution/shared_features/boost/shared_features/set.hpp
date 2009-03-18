/////////////////////////////////////////////////////////////////////////////
// set.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
// Disclaimer :
//
// This code is a minor modification to Boost.Accumulator and serves a
// particular purpose not readily provided by the original library.
// Contact the Boost library for copyright implications.
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SHARED_FEATURES_SET_HPP_ER_2009
#define BOOST_SHARED_FEATURES_SET_HPP_ER_2009
#include <stdexcept>
#include <boost/version.hpp>
#include <boost/ref.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/protect.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>

#include <boost/parameter/parameters.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/fusion/include/any.hpp>
#include <boost/fusion/include/find_if.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/filter_view.hpp>
#include <boost/shared_features/parameter.hpp>
#include <boost/shared_features/wrapper.hpp>
#include <boost/shared_features/creation.hpp>
#include <boost/shared_features/visitor.hpp>
#include <boost/shared_features/functor/initialize.hpp>
#include <boost/shared_features/mpl_features.hpp>
#include <boost/shared_features/predicate.hpp>
namespace boost{namespace shared_features{
namespace detail{

    struct set_base{};
    template<typename T>
    struct is_set : is_base_and_derived<set_base,T> {};

    typedef parameter::parameters<
        parameter::required<tag::kwd_set>
    > params;
}


template<typename Features>
class set : detail::set_base{
    public:
    typedef set<Features> this_type;
    typedef Features asked_features_type;

    typedef typename detail::wrappers<asked_features_type>::type
                                                        mpl_features;

    typedef typename detail::meta::make_acc_list<
        mpl_features
    >::type
    features_type;

    set()
    :features_(
        detail::make_acc_list(
            mpl_features(),
            //detail::params()(*this)
           (kwd_set=(*this))
        )
    )
    {
        // Do :
        //  set();
        //  set.initialize(args);
        // Not do :
        //  set(args)
        // news://news.gmane.org:119/gonju8$2da$1@ger.gmane.org
        //
        // Also see : initialize(args)
    };

    template<typename Args>
    explicit set(Args const& args)
    :features_(
        detail::make_acc_list(
            mpl_features(),
           (kwd_set=(*this),args)
        )
    )
    {
        // To be avoided (see above)
    }

    template<typename BinaryFun>
    void visit(const BinaryFun& f){
            this->visit_impl(
                detail::make_visitor(
                    f,
                    detail::params()(
                        *this
                    )
                )
            );
    }

    template<typename BinaryFun,typename Args>
    void visit(const BinaryFun& f,const Args& args){
            this->visit_impl(
                detail::make_visitor(
                    f,
                    (
                        args,
                        (kwd_set = *this)
                    )
                )
                // TODO Warning. Weird that this cause compile error:
                //  See creation.cpp
                //detail::make_visitor(
                //    f,
                //    (
                //        (kwd_set = *this),
                //        args
                //    )
                //)
            );
    }

    template<typename Filter,typename BinaryFun>
    void visit_if(const BinaryFun& f){
            this->visit_if_impl<Filter>(
                detail::make_visitor(
                    f,
                    detail::params()(
                        *this
                    )
                )
            );
    }

    template<typename Filter,typename BinaryFun,typename Args>
    void visit_if(const BinaryFun& f,const Args& args){
            this->visit_if_impl<Filter>(
                detail::make_visitor(
                    f,
                    (
                        args,
                        (kwd_set = *this)
                    )
                )
            );
    }



    template<typename Feature>
    struct apply : fusion::result_of::value_of<
        typename fusion::result_of::find_if<
            features_type,
            matches_feature<Feature>
        >::type
    >
    {};


    template<typename Args>
    void initialize(const Args& args){
        visit(
            functor::initialize(),
            (args,kwd_do_initialize = mpl::bool_<true>())
        );
        // See the warning in set().
        // kwd_do_initialize used to distinguish default construction
        // from call to intialize
    }

    template<typename Feature>
    struct result_of_extract{
        typedef  typename apply<Feature>::type type;
    };

    template<typename Feature>
    typename apply<Feature>::type& extract()
    {
        return *fusion::find_if<matches_feature<Feature> >(
            this->features_
        );
    }

    template<typename Feature>
    typename apply<Feature>::type const &extract() const
    {
        return *fusion::find_if<matches_feature<Feature> >(
            this->features_
        );
    }

    private:
    //protected:
    template<typename UnaryFun>
    void visit_impl(const UnaryFun& f){
        fusion::for_each(this->features_,f);
    }

    template<typename Filter,typename UnaryFun>
    void visit_if_impl(const UnaryFun& f){
        typedef fusion::filter_view<features_type,Filter> view_t;
        view_t view(this->features_);
        fusion::for_each(view,f);
    }
    features_type features_;
};



}}

#endif
