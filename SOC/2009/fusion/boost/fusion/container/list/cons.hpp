/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2005 Eric Niebler
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_CONS_HPP
#define BOOST_FUSION_CONTAINER_LIST_CONS_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/fusion/sequence/intrinsic/empty.hpp>
#endif
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assign_tags.hpp>
#include <boost/fusion/support/internal/sequence_assign.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/mpl/equal_to.hpp>
#endif
#ifdef BOOST_NO_RVALUE_REFERENCES
#   include <boost/call_traits.hpp>
#endif
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/fusion/container/list/detail/cons/cons_fwd.hpp>
#include <boost/fusion/container/list/detail/cons/at_impl.hpp>
#include <boost/fusion/container/list/detail/cons/value_at_impl.hpp>
#include <boost/fusion/container/list/detail/cons/empty_impl.hpp>

#include <boost/fusion/container/list/detail/cons/cons_iterator.hpp>
#include <boost/fusion/container/list/detail/cons/begin_impl.hpp>
#include <boost/fusion/container/list/detail/cons/end_impl.hpp>
#include <boost/fusion/container/list/detail/cons/deref_impl.hpp>
#include <boost/fusion/container/list/detail/cons/next_impl.hpp>
#include <boost/fusion/container/list/detail/cons/value_of_impl.hpp>
#include <boost/fusion/container/list/detail/cons/equal_to_impl.hpp>

namespace boost { namespace fusion
{
    struct void_;
    struct fusion_sequence_tag;

    struct nil
      : sequence_base<nil>
    {
        typedef void_ car_type;
        typedef void_ cdr_type;

        typedef mpl::int_<0> size;
        typedef cons_tag fusion_tag;
        typedef fusion_sequence_tag tag; 
        typedef mpl::false_ is_view;
        typedef forward_traversal_tag category;

        nil()
        {}

        nil(const nil&)
        {}

        template<typename SeqAssign>
        nil(BOOST_FUSION_R_ELSE_CLREF(SeqAssign))
        {
#ifdef BOOST_NO_RVALUE_REFERENCES
            BOOST_FUSION_MPL_ASSERT((
                result_of::empty<typename SeqAssign::seq_type>));
#else
            BOOST_FUSION_MPL_ASSERT((
                result_of::empty<
                    typename detail::remove_reference<SeqAssign>::type::seq_type
                >));
#endif
        }

        template<typename It>
        nil(detail::assign_by_deref,It const&)
        {}

        template<typename Seq>
        nil&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq))
        {
            BOOST_FUSION_MPL_ASSERT((result_of::empty<Seq>));
            return *this;
        }

        template<typename It>
        void
        assign(It const&)
        {}
    };

    template <typename Car, typename Cdr = nil>
    struct cons
      : sequence_base<cons<Car, Cdr> >
    {
        typedef Car car_type;
        typedef Cdr cdr_type;

        typedef mpl::int_<Cdr::size::value+1> size;
        typedef cons_tag fusion_tag;
        typedef fusion_sequence_tag tag; 
        typedef mpl::false_ is_view;
        typedef forward_traversal_tag category;

        cons()
          : car()
          , cdr()
        {}

        //cschmidt: iterators so we do not have to deal with the cv-ness
        //of cons_.car/cons_.cdr explicitly
#define BOOST_FUSION_CONS_CTOR(COMBINATION,_)\
        cons(cons COMBINATION cons_)\
          : car(fusion::front(BOOST_FUSION_FORWARD(cons COMBINATION,cons_)))\
          , cdr(detail::assign_by_deref(),\
                fusion::next(fusion::begin(\
                        BOOST_FUSION_FORWARD(cons COMBINATION,cons_))))\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_CONS_CTOR,_);

#undef BOOST_FUSION_CONS_CTOR

        //cschmidt: rvalue ref if possible, so this does not collide with
        //cons(OtherCar&&,OtherCdr&&)
        template<typename It>
        cons(detail::assign_by_deref,BOOST_FUSION_R_ELSE_CLREF(It) it)
          : car(fusion::deref(it))
          , cdr(detail::assign_by_deref(),fusion::next(it))
        {}

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   ifdef BOOST_NO_RVALUE_REFERENCES
        explicit
        cons(typename call_traits<Car>::param_type car)
          : car(car)
        {}

        cons(typename call_traits<Car>::param_type car,Cdr const& cdr)
          : car(car)
          , cdr(cdr)
        {}
#   else
        template<typename OtherCar>
        explicit
        cons(OtherCar&& car)
          : car(std::forward<OtherCar>(car))
        {}

        template<typename OtherCar,typename OtherCdr>
        cons(BOOST_FUSION_R_ELSE_CLREF(OtherCar) car,
             BOOST_FUSION_R_ELSE_CLREF(OtherCdr) cdr)
          : car(BOOST_FUSION_FORWARD(OtherCar,car))
          , cdr(BOOST_FUSION_FORWARD(OtherCdr,cdr))
        {}
#   endif
#else
#   ifdef BOOST_NO_RVALUE_REFERENCES
        template<typename... CdrArgs>
        explicit
        cons(typename call_traits<Car>::param_type car,
             CdrArgs const&... cdr_args)
          : car(car)
          , cdr(cdr_args)
        {}
#   else
        template<typename... CdrArgs>
        explicit
        cons(Car&& car,CdrArgs&&... cdr_args)
          : car(std::forward<Car>(car))
          , cdr(std::forward<CdrArgs>(cdr_args)...)
        {}
#   endif
#endif

        template<typename Seq>
        cons(
            BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
            typename disable_if<
                is_convertible<BOOST_FUSION_R_ELSE_CLREF(Seq), car_type>
            >::type* =0)
          : car(fusion::front(BOOST_FUSION_FORWARD(Seq,seq)))
          , cdr(detail::assign_by_deref(),
                  fusion::next(fusion::begin(BOOST_FUSION_FORWARD(Seq,seq))))
        {
            BOOST_FUSION_MPL_ASSERT_NOT((
                    mpl::equal_to<size,result_of::size<Seq> >));
        }

#define BOOST_FUSION_CONS_ASSIGN_CTOR(COMBINATION,_)\
        template<typename SeqRef>\
        cons(detail::sequence_assign_type<SeqRef> COMBINATION seq_assign)\
          : car(fusion::front(seq_assign.get()))\
          , cdr(detail::assign_by_deref(),\
                  fusion::next(fusion::begin(seq_assign.get())))\
        {\
            BOOST_FUSION_MPL_ASSERT_NOT((\
                mpl::equal_to<size,result_of::size<SeqRef> >));\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_CONS_ASSIGN_CTOR,_);

#undef BOOST_FUSION_CONS_ASSIGN_CTOR

        template<typename Seq>
        cons&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
        {
            BOOST_FUSION_MPL_ASSERT_NOT((result_of::empty<Seq>));

            assign(fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)));
            return *this;
        }

        template<typename Seq>
        cons&
        operator=(cons const& seq)
        {
#ifdef BOOST_NO_RVALUE_REFERENCES
            return this->operator=<cons<Car,Cdr> >(seq);
#else
            return this->operator=<cons<Car,Cdr> const&>(seq);
#endif
        }

        template<typename It>
        void
        assign(It const& it)
        {
            car=fusion::deref(it);
            cdr.assign(fusion::next(it));
        }

        car_type car;
        cdr_type cdr;
    };
}}

#endif
