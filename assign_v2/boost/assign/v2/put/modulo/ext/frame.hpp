//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODULO_EXT_FRAME_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODULO_EXT_FRAME_ER_2010_HPP
#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/assign/v2/put/frame/modifier.hpp>
#include <boost/assign/v2/put/modulo/modifier.hpp>
#include <boost/assign/v2/put/modulo/fun_modifier.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_parameter{

    template<typename F, typename Tag>
    struct traits{
        typedef F fun_type;
        typedef Tag modifier_tag;
        typedef traits<F, Tag> type;
    };

}// put_parameter
namespace put_concept{

    template<typename Tag, typename P>
    struct ExtParam
    {
        BOOST_CONCEPT_USAGE(ExtParam)
        {
            typedef put_aux::modifier<Tag> m_;
            m_ m( p.arg() );
        }

        private:
        static P & p;
    };

}// put_concept
namespace result_of_modulo{

    template<typename D>
    struct ext{

        template<typename FParameter>
        struct apply{

            typedef typename D::fun_type old_fun_;
            typedef typename D::modifier_tag old_tag;
            typedef typename ::boost::mpl::apply2<
                FParameter, old_fun_, old_tag
            >::type traits_;
            typedef typename traits_::fun_type fun_;
            typedef typename traits_::modifier_tag tag;

            typedef result_of_modulo::modifier<D> meta1_;
            typedef result_of_modulo::fun_modifier<D> meta2_;

            typedef boost::is_same<fun_, old_fun_> is_same_fun_;
            typedef typename ::boost::mpl::eval_if<
                is_same_fun_,
                ::boost::mpl::apply1< meta1_, tag >,
                ::boost::mpl::apply2< meta2_, fun_, tag>
            >::type type;

            typedef put_aux::modifier<tag> m_;

            template<typename C, typename F>
            static type call(C& cont, F const& f, m_ const& m)
            {
                return type( cont, f, m );
            }
            static fun_ make_fun(const D& derived, ::boost::mpl::false_)
            {
                return fun_();
            }
            static fun_ make_fun(const D& derived, ::boost::mpl::true_)
            {
                return derived.fun;
            }
            template<typename P>
            static type call(const D& derived, P const& p)
            {
                typedef put_concept::ExtParam<tag, P> concept_;
                BOOST_CONCEPT_ASSERT(( concept_ ));
                return call(
                    derived.container(),
                    make_fun( derived, is_same_fun_() ),
                    m_( p.arg() )
                );
            }

        };

    };

}// result_of_modulo
}// v2
}// assign
}// boost

#endif
