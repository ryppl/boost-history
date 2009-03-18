///////////////////////////////////////////////////////////////////////////////
//  as_random_distribution.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_GENERATOR_AS_RANDOM_DISTRIBUTION_HPP_ER_2009
#define BOOST_GENERATOR_AS_RANDOM_DISTRIBUTION_HPP_ER_2009
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/eval_if.hpp>
namespace boost{

    BOOST_MPL_HAS_XXX_TRAIT_DEF(input_type);



    // Maps Generator (or RandomDistribution) to RandomDistribution
	template<
        typename G,
        typename InputType = unsigned long
    >
	class as_random_distribution : G{
	    typedef G super_t;
        typedef has_input_type<super_t> has_it;
        template<typename T>
        struct input{
            typedef typename T::input_type type;
        };
	public:
		typedef typename mpl::eval_if<
                has_it,
                input<super_t>,
                mpl::identity<InputType>
        >::type                                       input_type;
		typedef typename super_t::result_type		    result_type;

        as_random_distribution(const super_t& g)
        :super_t(g){};

        template<typename Args>
        as_random_distribution(const Args& args)
        :super_t(args){};

//        //Overload needed, for example, ref<Clock>(Clock&)
//        template<typename Args>
//        as_random_distribution(Args& args)
//        :super_t(args){};

		as_random_distribution(const as_random_distribution& that)
		:super_t(static_cast<const super_t&>(that)){};

		as_random_distribution&
		operator=(const as_random_distribution& that){
		    if(&that!=this){
                super_t::operator=(that);
            }
		    return *this;
		}

        template<typename U>
        result_type
        operator()(U& u){
            return impl(typename has_it::type(),u);
        }


    private:

    template <typename U >
    result_type
    impl(mpl::bool_<false>,U& u) {
        return super_t::operator()();
    }

    template <typename U>
    result_type
    impl(mpl::bool_<true>,U& u) {
        return super_t::operator()(u);
    }
};

}//boost
#endif
