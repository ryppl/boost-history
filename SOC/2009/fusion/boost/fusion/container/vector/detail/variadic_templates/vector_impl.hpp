// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_VECTOR_VECTOR_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_VECTOR_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/support/variadic_templates/variadic_arguments_to_vector.hpp>
#include <boost/fusion/support/sequence_assign.hpp>
#include <boost/fusion/support/assign_tags.hpp>

#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

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

            template<typename Iterator>
            vector_impl(assign_by_deref,Iterator const&)
            {
            }

            template<typename Iterator>
            void
            assign(Iterator const&)
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

            template<typename Iterator>
            vector_impl(assign_by_deref,Iterator const& iterator):
                base(assign_by_deref(),fusion::next(iterator)),
                _element(fusion::deref(iterator))
            {
            }

            template<typename Iterator>
            void
            assign(Iterator const& iterator)
            {
                _element=fusion::deref(iterator);
                static_cast<base*>(this)->assign(fusion::next(iterator));
            }

#ifdef BOOST_NO_RVALUE_REFERENCES
            vector_impl(assign_directly,
                    typename detail::call_param<Head>::type head,
                    typename detail::call_param<OtherElements>::type... others)
                : base(assign_directly(),assign_directly,others...),
                , _element(head)
            {
            }
#else
            template<typename OtherHead,typename... OtherElements>
            vector_impl(assign_directly,
                    OtherHead&& other_head,
                    OtherElements&&... other_elements)
                : base(assign_directly(),
                        std::forward<OtherElements>(other_elements)...)
                , _element(std::forward<OtherHead>(other_head))
            {
            }
#endif

            typename add_reference<Head>::type
            at_impl(mpl::int_<Index>)
            {
                return _element;
            }

            typename add_reference<typename add_const<Head>::type>::type
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
        {
        }

        template<typename Vec>
        vector(BOOST_FUSION_R_ELSE_CLREF(Vec) vec,
               typename enable_if<is_convertible<
                   typename detail::remove_reference<Vec>::type*
                 , vector const volatile*> >::type* =NULL)
          : base(detail::assign_by_deref(),
                 fusion::begin(BOOST_FUSION_FORWARD(Vec,vec)))
        {
        }

        template<typename SequenceAssign>
        vector(BOOST_FUSION_R_ELSE_CLREF(SequenceAssign) seq,
               typename enable_if<
                   is_sequence_assign<SequenceAssign> >::type* =NULL)
          : base(detail::assign_by_deref(),fusion::begin(seq.get()))
        {
        }

        /*
        template<typename Sequence>
        vector(typename enable_if_c<sizeof...(Elements)!=1,
                                BOOST_FUSION_R_ELSE_CLREF(Sequence)>::type seq)
          : base(detail::assign_by_deref(),
                 fusion::begin(BOOST_FUSION_FORWARD(Sequence,seq.seq)))
        {
        }
        */

#ifdef BOOST_NO_RVALUE_REFERENCES
        vector(typename detail::call_param<Head>::type head,
               typename detail::call_param<OtherElements>::type... others)
          : base(assign_directly(),others...)
          , _element(head)
        {
        }
#else
        template<typename... OtherElements>
        vector(OtherElements&&... other_elements)
          : base(detail::assign_directly(),
                 std::forward<OtherElements>(other_elements)...)
        {
        }
#endif

        template<typename Sequence>
        vector&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Sequence) sequence)
        {
            static_cast<base*>(this)->assign(
                fusion::begin(BOOST_FUSION_FORWARD(Sequence,sequence)));
            return *this;
        }

        template<typename I>
        typename mpl::at<types, I>::type&
        at_impl(I)
        {
            return base::at_impl(mpl::int_<I::value>());
        }

        template<typename I>
        typename add_const<typename mpl::at<types, I>::type>::type&
        at_impl(I) const
        {
            return base::at_impl(mpl::int_<I::value>());
        }
    };

}}
#endif
