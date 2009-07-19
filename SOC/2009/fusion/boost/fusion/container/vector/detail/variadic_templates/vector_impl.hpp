// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_VARIADIC_TEMPLATES_VECTOR_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_VARIADIC_TEMPLATES_VECTOR_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/support/variadic_templates/variadic_arguments_to_vector.hpp>
#include <boost/fusion/support/sequence_assign.hpp>
#include <boost/fusion/support/assign_tags.hpp>
#include <boost/fusion/support/sequence_base.hpp>

#include <boost/mpl/int.hpp>

#include <utility>

namespace boost { namespace fusion
{
    struct void_;

    namespace detail
    {
        template<int Index,typename... Elements>
        struct vector_impl;

        template<int Index>
        struct vector_impl<Index>
        {
            template<int,typename...>friend struct vector_impl;
            template<typename...>friend struct fusion::vector;

        protected:
            typedef void_ head;

            vector_impl()
            {
            }

            vector_impl(assign_directly)
            {
            }

            template<typename It>
            vector_impl(assign_by_deref,It const&)
            {
            }

            template<typename It>
            void
            assign(It const&)
            {
            }

            void at_impl();
        };

        template<int Index,typename Head,typename... Others>
        struct vector_impl<Index,Head,Others...>
          : vector_impl<Index+1,Others...>
        {
            template<int,typename...>friend struct vector_impl;
            template<typename...>friend struct fusion::vector;

        private:
            typedef vector_impl<Index+1,Others...> base;

        public:
            using base::at_impl;

        protected:
            typedef Head head;

        private:
            Head _element;

        protected:
            vector_impl():
                _element()
            {
            }

            template<typename It>
            vector_impl(assign_by_deref,It const& it)
              : base(assign_by_deref(),fusion::next(it))
              , _element(fusion::deref(it))
            {
            }

            template<typename It>
            void
            assign(It const& it)
            {
                _element=fusion::deref(it);
                static_cast<base*>(this)->assign(fusion::next(it));
            }

            template<typename OtherHead,typename... OtherElements>
            vector_impl(assign_directly,
                    BOOST_FUSION_R_ELSE_CLREF(OtherHead) other_head,
                    BOOST_FUSION_R_ELSE_CLREF(OtherElements)... other_elements)
                : base(assign_directly(),
                        BOOST_FUSION_FORWARD(OtherElements,other_elements)...)
                , _element(BOOST_FUSION_FORWARD(OtherHead,other_head))
            {
            }

            typename detail::add_lref<Head>::type
            at_impl(mpl::int_<Index>)
            {
                return _element;
            }

            typename detail::add_lref<typename add_const<Head>::type>::type
            at_impl(mpl::int_<Index>)const
            {
                return _element;
            }
        };
    }

    template<typename... Elements>struct vector
      : sequence_base<vector<Elements...> >
      , detail::vector_impl<0,Elements...>
    {
    private:
        typedef detail::vector_impl<0,Elements...> base;

    public:
        typedef vector<Elements...> this_type;
        typedef typename variadic_arguments_to_vector<Elements...>::type types;
        typedef vector_tag fusion_tag;
        typedef fusion_sequence_tag tag;
        typedef mpl::false_ is_view;
        typedef random_access_traversal_tag category;
        typedef mpl::int_<sizeof...(Elements)> size;

        vector()
        {}

#define VECTOR_CTOR(COMBINATION,_)\
        vector(vector COMBINATION vec)\
          : base(detail::assign_by_deref(),\
                fusion::begin(BOOST_FUSION_FORWARD(vector COMBINATION,vec)))\
        {\
        }

        BOOST_FUSION_ALL_CV_REF_COMBINATIONS(VECTOR_CTOR,_)

#undef VECTOR_CTOR

        template<typename SeqAssign>
        vector(BOOST_FUSION_R_ELSE_CLREF(SeqAssign) seq,
               typename enable_if<
                   is_sequence_assign<SeqAssign> >::type* =NULL)
          : base(detail::assign_by_deref(),fusion::begin(seq.get()))
        {}

        /*
        template<typename Seq>
        vector(typename enable_if_c<sizeof...(Elements)!=1,
                                BOOST_FUSION_R_ELSE_CLREF(Seq)>::type seq)
          : base(detail::assign_by_deref(),
                 fusion::begin(BOOST_FUSION_FORWARD(Seq,seq.seq)))
        {}
        */

        template<typename... OtherElements>
        vector(BOOST_FUSION_R_ELSE_CLREF(OtherElements)... other_elements)
          : base(detail::assign_directly(),
                 BOOST_FUSION_FORWARD(OtherElements,other_elements)...)
        {}

        template<typename Seq>
        vector&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
        {
            static_cast<base*>(this)->assign(
                fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)));
            return *this;
        }

        template<typename I>
        typename detail::add_lref<
            typename mpl::at<types, I>::type
        >::type
        at_impl(I)
        {
            return base::at_impl(mpl::int_<I::value>());
        }

        template<typename I>
        typename detail::add_lref<
        typename add_const<typename mpl::at<types, I>::type>::type
        >::type
        at_impl(I) const
        {
            return base::at_impl(mpl::int_<I::value>());
        }
    };

}}
#endif
