/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Järvi
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_PP_IS_ITERATING)
#if !defined(BOOST_FUSION_TUPLE_HOLDER_IPP)
#define BOOST_FUSION_TUPLE_HOLDER_IPP

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>

#define BOOST_FUSION_TUPLE_DEFAULT_INIT_ELEM(z, n, data)                        \
    BOOST_PP_CAT(m, n)()

#define BOOST_FUSION_TUPLE_MEMBER_INIT(z, n, data)                              \
    BOOST_PP_CAT(m, n)(BOOST_PP_CAT(_, n))

#define BOOST_FUSION_TUPLE_MEMBER_COPY(z, n, data)                              \
    BOOST_PP_CAT(m, n)(t.BOOST_PP_CAT(m, n))

#define BOOST_FUSION_TUPLE_MEMBER_ASSIGN(z, n, data)                            \
    BOOST_PP_CAT(m, n) = t.BOOST_PP_CAT(m, n);

#define BOOST_FUSION_TUPLE_MEMBER_EQUAL(z, n, data)                             \
    BOOST_PP_CAT(m, n) == t.BOOST_PP_CAT(m, n) &&

#define BOOST_FUSION_TUPLE_MEMBER_NOT_EQUAL(z, n, data)                         \
    BOOST_PP_CAT(m, n) != t.BOOST_PP_CAT(m, n) ||

#define BOOST_FUSION_TUPLE_MEMBER_LESS_THAN(z, n, data)                         \
    BOOST_PP_CAT(m, n) < t.BOOST_PP_CAT(m, n) ||

#define BOOST_FUSION_TUPLE_MEMBER_GREATER_THAN(z, n, data)                      \
    BOOST_PP_CAT(m, n) > t.BOOST_PP_CAT(m, n) ||

#define BOOST_FUSION_TUPLE_MEMBER_LESS_THAN_EQUAL(z, n, data)                   \
    BOOST_PP_CAT(m, n) <= t.BOOST_PP_CAT(m, n)                                  \
      && (!(t.BOOST_PP_CAT(m, n) <= BOOST_PP_CAT(m, n)) || (

#define BOOST_FUSION_TUPLE_MEMBER_GREATER_THAN_EQUAL(z, n, data)                \
    BOOST_PP_CAT(m, n) >= t.BOOST_PP_CAT(m, n)                                  \
      && (!(t.BOOST_PP_CAT(m, n) >= BOOST_PP_CAT(m, n)) || (

#define BOOST_FUSION_TUPLE_DOUBLE_PARENTHESIS(z, n, data) ))

#define BOOST_FUSION_TUPLE_MEMBER_DATA(z, n, data)                              \
    BOOST_PP_CAT(T, n) BOOST_PP_CAT(m, n);

#define BOOST_FUSION_ITERATOR_DATA_TYPES(z, n, data)                            \
    typename BOOST_PP_CAT(T, n) =                                               \
        typename next_iterator<BOOST_PP_CAT(T, BOOST_PP_DEC(n))>::type

#define BOOST_FUSION_ITERATOR_MEMBER_TYPES(z, n, data)                          \
    BOOST_PP_CAT(m, n)( boost::fusion::next(BOOST_PP_CAT(m, BOOST_PP_DEC(n))) )
    // *** fully qualified boost::fusion::next 
    // to avoid conflict with mpl::next ***

#if defined(BOOST_MSVC) // $$$ VC7.1 bug $$$
# define BOOST_FUSION_TUPLE_MEMBER_COPY_FROM_ITERS(z, n, data)                  \
    BOOST_PP_CAT(m, n)(                                                         \
        static_cast<BOOST_PP_CAT(T, n)>(*(iters.BOOST_PP_CAT(m, n))))
#else
# define BOOST_FUSION_TUPLE_MEMBER_COPY_FROM_ITERS(z, n, data)                  \
    BOOST_PP_CAT(m, n)(*(iters.BOOST_PP_CAT(m, n)))
#endif

#define BOOST_FUSION_TUPLE_UNUSED_TEMPLATE_ARG(z, n, data)                      \
    typename BOOST_PP_CAT(Unused, n) = void_t

namespace boost { namespace fusion
{
    // forward declarations

    template <typename DerivedT>
    struct next_iterator;

    template <typename DerivedT>
    struct deref_iterator;

    struct void_t;

  namespace tuple_detail
  {
    struct iterator_data_tag {};

    ///////////////////////////////////////////////////////////////////////////
    //
    //  tuple_holder_0 (arity-0 tuple holder)
    //
    ///////////////////////////////////////////////////////////////////////////
    template <unsigned N>
    struct tuple_holder {};

    struct tuple_holder_0
    {
        template <
            BOOST_FUSION_TEMPLATE_PARAMS_WITH_DEFAULT(
                BOOST_FUSION_MAX_ARITY, void_t)
        >
        struct iterator_data;

        template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(T)>
        struct data
        {
            typedef mpl::vector0<> types;

            data() {}

            template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(IterT)>
            data(iterator_data<BOOST_PP_ENUM_PARAMS(
                    BOOST_FUSION_MAX_ARITY, IterT)> const&
                  , iterator_data_tag)
                {}

            // $$$ How shall empty tuples behave with comparison? $$$
            bool operator==(data const& t) const
            {
                return true;
            }

            bool operator!=(data const& t) const
            {
                return false;
            }

            bool operator<(data const& t) const
            {
                return false;
            }

            bool operator>(data const& t) const
            {
                return false;
            }

            bool operator<=(data const& t) const
            {
                return false;
    	    }

            bool operator>=(data const& t) const
            {
                return false;
    	    }
        };

        template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(T)>
        struct iterator_data
        {
            iterator_data(T0 const&) {}
        };
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The following Boost::PP code generates expansions for:
    //
    //  struct tuple_holder_N
    //  {
    //      template <typename T0, typename T1, ... typename TM>
    //      struct data
    //      {
    //          data() : m0(), m1(), ..., mN() {}
    //
    //          /*** See note 1, below ***/
    //          data(
    //              typename tuple_detail::call_param<T0>::type _0,
    //              typename tuple_detail::call_param<T1>::type _1,
    //              ...
    //              typename tuple_detail::call_param<TN>::type _N)
    //              : m0(_0), m1(_1), mN(_N) {}
    //
    //          template <typename TB0, typename TB1... typename TBN>
    //          data(data<TB0, TB1...TBN> const& t)
    //              : m0(t.m0), m1(t.m1)... mN(t.mN) {}
    //
    //          template <typename Iter0, typename Iter1, ... typename IterM>
    //          data(iterator_data<IterT0, Iter1...IterT5> const& iters
    //              , iterator_data_tag)
    //              : m0(*(iters.m0)), m1(*(iters.m1))... mN(*(iters.mN)) {}
    //
    //          template <typename TupleT>
    //          data& operator=(TupleT const& t)
    //          { m0 = t.m0; m1 = t.m1;... mN = t.mN; }
    //
    //          template <typename TupleT>
    //          bool operator==(TupleT const& t) const
    //          { m0 == t.m0 && m1 == t.m1 && ... mN == t.mN && true; }
    //
    //          template <typename TupleT>
    //          bool operator!=(TupleT const& t) const
    //          { m0 != t.m0 || m1 != t.m1 || ... mN != t.mN || false; }
    //
    //          template <typename TupleT>
    //          bool operator<(TupleT const& t) const
    //          { m0 < t.m0 || m1 < t.m1 || ... mN < t.mN || false; }
    //
    //          template <typename TupleT>
    //          bool operator>(TupleT const& t) const
    //          { m0 > t.m0 || m1 > t.m1 || ... mN > t.mN || false; }
    //
    //          template <typename TupleT>
    //          bool operator<=(TupleT const& t) const
    //          { m0 <= t.m0 && (!(t.m0 <= m0) || (m1 <= t.m1 && (!(t.m1 <= m1)
    //                                         || (... mN <= t.mN )))) ... )); }
    //
    //          template <typename TupleT>
    //          bool operator>=(TupleT const& t) const
    //          { m0 >= t.m0 && (!(t.m0 >= m0) || (m1 >= t.m1 && (!(t.m1 >= m1)
    //                                         || (... mN >= t.mN )))) ... )); }
    //
    //          T0 m0;
    //          T1 m1;
    //          ...
    //          TN mN;
    //      };
    //
    //      template <
    //            typename T0
    //          , typename T1 = typename next_iterator<T0)>::type // note 2
    //          , ... typename TM = typename next_iterator<TP)>::type
    //        >
    //      struct iterator_data
    //      {
    //          iterator_data(T0 const& iter)
    //              : m0(iter)
    //              , m1(next(m0))
    //              , m2(next(m1))
    //              ...
    //              , mN(next(mP))
    //          {}
    //
    //          T0 m0; T1 m1; ...; TN mN;
    //      };
    //  };
    //
    //  where N runs from 1 to BOOST_FUSION_MAX_ARITY
    //  M is BOOST_FUSION_MAX_ARITY
    //  and P is N-1
    //
    //  *** note 1 ***
    //  ***
    //  *** For tuple_holder_1, there is a special case constructor to
    //  *** accomodate constructing from iterator_data objects since these
    //  *** are constructed with 2 arguments for API consistency. An UnusedT
    //  *** argument is introduced:
    //
    //      template <typename UnusedT>
    //      data(typename tuple_detail::call_param<T0>::type _0, UnusedT)
    //          : m0(_0) {}
    //
    //  TX defaults to TX = typename next_iterator<TX+1)>::type IFF
    //  X < N, otherwise TX defaults to TX = void_t.
    //
    ///////////////////////////////////////////////////////////////////////////
#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (3, (1, BOOST_FUSION_MAX_ARITY,                                             \
        "boost/spirit/fusion/detail/holder.ipp"))
#include BOOST_PP_ITERATE()

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The following Boost::PP code generate expansions for:
    //
    //      template <typename T0, typename T1... typename TN>
    //      struct tuples
    //      {
    //          typedef mpl::vectorM<
    //              tuple_detail::tuple_holder_0,
    //              tuple_detail::tuple_holder_1,
    //              tuple_detail::tuple_holder_2,
    //              ...
    //              tuple_detail::tuple_holder_M
    //          type;
    //      };
    //
    //  where N = BOOST_FUSION_MAX_ARITY and M = N + 1
    //
    ///////////////////////////////////////////////////////////////////////////

#define BOOST_FUSION_TUPLE_TUPLE_N_PARAM(z, n, data)   \
    BOOST_PP_CAT(tuple_detail::tuple_holder_, n)

    struct tuple_holders
    {
        typedef
            BOOST_PP_CAT(
                mpl::vector,
                BOOST_PP_INC(BOOST_FUSION_MAX_ARITY))
                <BOOST_PP_ENUM(
                    BOOST_PP_INC(BOOST_FUSION_MAX_ARITY),
                    BOOST_FUSION_TUPLE_TUPLE_N_PARAM, _) >
        type;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  iterator_data utilities
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename HolderT, typename MaybeIterT>
    struct get_iterator_data_from_holder
    {
        typedef typename HolderT::template iterator_data<MaybeIterT> type;
    };

    template <typename N, typename MaybeIterT, typename T0>
    struct get_iterator_data
    {
        typedef typename mpl::at<
            typename tuple_holders::type
          , N
        >::type tuple_holder_t;

        typedef typename
            mpl::if_<
                is_convertible<MaybeIterT, T0>
              , mpl::identity<T0>
              , get_iterator_data_from_holder<tuple_holder_t, MaybeIterT>
            >::type
        result_type;

        typedef typename result_type::type type;
    };

    template <typename MaybeIterT, typename T0>
    struct get_iterator_data_tag
    {
        typedef typename
            mpl::if_<
                is_convertible<MaybeIterT, T0>
              , int
              , iterator_data_tag
            >::type
        type;
    };

#undef BOOST_FUSION_TUPLE_TUPLE_N_PARAM

}}} // namespace boost::fusion::tuple_detail

    ///////////////////////////////////////////////////////////////////////////
    //
    //  Our BOOST_FUSION_TUPLE_HOLDER_TYPE
    //
    //      Given N, the number of tuple elements (tuple::size or
    //      tuple::size_type), select tuple_holder_N, its nested template
    //      data<T0..TN> will be the actual data holder for our tuple
    //      (see tuple.hpp's tuple::data member)
    //
    ///////////////////////////////////////////////////////////////////////////
#if defined __BORLANDC__ && __BORLANDC__ <= 0x561

//  Again, Borland chokes if the template parameter is a static constant
//  passed to mpl::at_c (i.e. tuple::size). On the other hand, MSVC
//  chokes when we are using the tuple's size_type passed to mpl::at.

    #define BOOST_FUSION_TUPLE_HOLDER_TYPE                                      \
        typedef typename mpl::at<                                               \
            typename tuple_detail::tuple_holders::type                          \
          , size_type                                                           \
        >::type

#else

    #define BOOST_FUSION_TUPLE_HOLDER_TYPE                                      \
        typedef typename mpl::at_c<                                             \
            typename tuple_detail::tuple_holders::type                          \
          , size                                                                \
        >::type

    #endif

#undef BOOST_FUSION_TUPLE_DEFAULT_INIT_ELEM
#undef BOOST_FUSION_TUPLE_MEMBER_INIT
#undef BOOST_FUSION_TUPLE_MEMBER_COPY
#undef BOOST_FUSION_TUPLE_MEMBER_ASSIGN
#undef BOOST_FUSION_TUPLE_MEMBER_EQUAL
#undef BOOST_FUSION_TUPLE_MEMBER_NOT_EQUAL
#undef BOOST_FUSION_TUPLE_MEMBER_LESS_THAN
#undef BOOST_FUSION_TUPLE_MEMBER_GREATER_THAN
#undef BOOST_FUSION_TUPLE_MEMBER_LESS_THAN_EQUAL
#undef BOOST_FUSION_TUPLE_MEMBER_GREATER_THAN_EQUAL
#undef BOOST_FUSION_TUPLE_DOUBLE_PARENTHESIS
#undef BOOST_FUSION_TUPLE_MEMBER_DATA
#undef BOOST_FUSION_ITERATOR_DATA_TYPES
#undef BOOST_FUSION_ITERATOR_MEMBER_TYPES
#undef BOOST_FUSION_TUPLE_MEMBER_COPY_FROM_ITERS
#undef BOOST_FUSION_TUPLE_UNUSED_TEMPLATE_ARG
#endif // defined(BOOST_FUSION_TUPLE_HOLDER_IPP)

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    struct BOOST_PP_CAT(tuple_holder_, N)
    {
        template <
            typename T0
        #if (N > 1)
          , BOOST_PP_ENUM_SHIFTED(N, BOOST_FUSION_ITERATOR_DATA_TYPES, _)
        #endif

        #if (N < BOOST_FUSION_MAX_ARITY)
          , BOOST_PP_ENUM(BOOST_PP_SUB(BOOST_FUSION_MAX_ARITY, N)
            , BOOST_FUSION_TUPLE_UNUSED_TEMPLATE_ARG, _)
        #endif
        >
        struct iterator_data;

        template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(T)>
        struct data
        {
            typedef mpl::BOOST_PP_CAT(vector, N)<
                BOOST_PP_ENUM_PARAMS(N, T)
            > types;

            data() : BOOST_PP_ENUM(N, BOOST_FUSION_TUPLE_DEFAULT_INIT_ELEM, _) {}
        #if (N == 1)
            template <typename UnusedT>
            data(BOOST_FUSION_CALL_PARAMS(N), UnusedT)
                : BOOST_PP_ENUM(N, BOOST_FUSION_TUPLE_MEMBER_INIT, _) {}
        #else
            data(BOOST_FUSION_CALL_PARAMS(N))
                : BOOST_PP_ENUM(N, BOOST_FUSION_TUPLE_MEMBER_INIT, _) {}
        #endif

            template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(IterT)>
            data(iterator_data<BOOST_PP_ENUM_PARAMS(
                    BOOST_FUSION_MAX_ARITY, IterT)> const& iters
                  , iterator_data_tag)
                : BOOST_PP_ENUM(N, BOOST_FUSION_TUPLE_MEMBER_COPY_FROM_ITERS, _) {}

            template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(TB)>
            data(
                data<BOOST_PP_ENUM_PARAMS(
                    BOOST_FUSION_MAX_ARITY, TB)> const& t)
                : BOOST_PP_ENUM(N, BOOST_FUSION_TUPLE_MEMBER_COPY, _) {}

            template <typename TupleT>
            data&
            operator=(TupleT const& t)
            {
                BOOST_PP_REPEAT(N, BOOST_FUSION_TUPLE_MEMBER_ASSIGN, _)
                return *this;
            }

            template <typename TupleT>
            bool operator==(TupleT const& t) const
            {
                return BOOST_PP_REPEAT(
                    N, BOOST_FUSION_TUPLE_MEMBER_EQUAL, _) true;
            }

            template <typename TupleT>
            bool operator!=(TupleT const& t) const
            {
                return BOOST_PP_REPEAT(
                    N, BOOST_FUSION_TUPLE_MEMBER_NOT_EQUAL, _) false;
            }

            template <typename TupleT>
            bool operator<(TupleT const& t) const
            {
                return BOOST_PP_REPEAT(
                    N, BOOST_FUSION_TUPLE_MEMBER_LESS_THAN, _) false;
            }

            template <typename TupleT>
            bool operator>(TupleT const& t) const
            {
                return BOOST_PP_REPEAT(
                    N, BOOST_FUSION_TUPLE_MEMBER_GREATER_THAN, _) false;
            }

            template <typename TupleT>
            bool operator<=(TupleT const& t) const
            {
                return
                    BOOST_PP_REPEAT(
                        BOOST_PP_DEC(N)
                      , BOOST_FUSION_TUPLE_MEMBER_LESS_THAN_EQUAL, _)
    	            BOOST_PP_CAT(m, BOOST_PP_DEC(N))
    	                <= t.BOOST_PP_CAT(m, BOOST_PP_DEC(N))
    	            BOOST_PP_REPEAT(
    	                BOOST_PP_DEC(N)
    	              , BOOST_FUSION_TUPLE_DOUBLE_PARENTHESIS, _);
    	    }

            template <typename TupleT>
            bool operator>=(TupleT const& t) const
            {
                return
                    BOOST_PP_REPEAT(
                        BOOST_PP_DEC(N)
                      , BOOST_FUSION_TUPLE_MEMBER_GREATER_THAN_EQUAL, _)
    	            BOOST_PP_CAT(m, BOOST_PP_DEC(N))
    	                >= t.BOOST_PP_CAT(m, BOOST_PP_DEC(N))
    	            BOOST_PP_REPEAT(
    	                BOOST_PP_DEC(N)
    	              , BOOST_FUSION_TUPLE_DOUBLE_PARENTHESIS, _);
    	    }

            BOOST_PP_REPEAT(N, BOOST_FUSION_TUPLE_MEMBER_DATA, _)
        };

        template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(T)>
        struct iterator_data
        {
            iterator_data(T0 const& iter)
                : m0(iter) BOOST_PP_COMMA_IF(BOOST_PP_DEC(N))
                    BOOST_PP_ENUM_SHIFTED(N
                        , BOOST_FUSION_ITERATOR_MEMBER_TYPES, _)
            {}

            BOOST_PP_REPEAT(N, BOOST_FUSION_TUPLE_MEMBER_DATA, _)
        };
    };

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)
