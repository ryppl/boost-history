//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_EXT_RESULT_OF_MODULO_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_EXT_RESULT_OF_MODULO_ER_2010_HPP
#include <boost/concept/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/put/generic/base.hpp>
#include <boost/assign/v2/put/generic/new_fun.hpp>
#include <boost/assign/v2/put/generic/new_modifier.hpp>
#include <boost/assign/v2/put/generic/parameter.hpp>
#include <boost/assign/v2/put/generic/result_of_modulo.hpp>
#include <boost/assign/v2/put/modifier/ext/concept_parameter.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace result_of_modulo{

    template<typename T>
    struct ext_generic{

		template<typename FParameter>
        struct apply{

        	typedef typename T::fun_type old_fun_;
        	typedef typename T::modifier_tag old_tag;
        	typedef typename ::boost::mpl::apply2<
            	FParameter,
                old_fun_,
                old_tag
            >::type pair_;
            typedef typename pair_::fun_type fun_;
            typedef typename pair_::modifier_tag tag;

			typedef typename boost::is_same<fun_, old_fun_>::type is_same_fun;

			typedef typename ::boost::mpl::eval_if<
            	is_same_fun,
                    ::boost::mpl::apply1<
                        v2::result_of_modulo::new_modifier<T>,
                        tag
                    >,
                    ::boost::mpl::apply2<
                        v2::result_of_modulo::new_fun_modifier<T>,
                        fun_,
                        tag
                    >
            >::type type;

            typedef put_aux::modifier<tag> m_;

            template<typename V, typename F>
            static type call(V& v, F const& f, m_ const& m)
            {
                return type( v, f, m );
            }
            static fun_ make_fun(const T& t, ::boost::mpl::false_)
            {
                return fun_();
            }
            static fun_ make_fun(const T& t, ::boost::mpl::true_)
            {
                return t.fun;
            }
            template<typename P>
            static type call(const T& t, P const& p)
            {
                typedef put_concept::Parameter<m_,P> concept_;
                BOOST_CONCEPT_ASSERT(( concept_ ));
                return call(
                    t.container(),
                    make_fun( t, is_same_fun() ),
                    m_( p.arg() )
                );
            }

        };

    };

}// result_of_modulo_aux
}// v2
}// assign
}// boost

#endif
