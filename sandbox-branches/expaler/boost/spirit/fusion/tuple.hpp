/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Järvi
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_TUPLE_HPP)
#define BOOST_FUSION_TUPLE_HPP

#if !defined(BOOST_FUSION_MAX_ARITY)
# define BOOST_FUSION_MAX_ARITY 6
#endif

#include <boost/spirit/fusion/detail/dependencies.hpp>
#include <boost/spirit/fusion/detail/helpers.hpp>
#include <boost/spirit/fusion/detail/macro_utils.hpp>

//  include the actual tuple_N implementations
#include <boost/spirit/fusion/detail/holder.ipp>

namespace boost { namespace fusion
{
    struct void_t {};

    ///////////////////////////////////////////////////////////////////////////
    //
    //  tuple forward declaration
    //
    //      Expands to:
    //
    //      template <
    //          typename T0 = void_t,
    //          typename T1 = void_t,
    //          ...
    //          typename TN = void_t,
    //          typename ID = TupleID
    //      >
    //      struct tuple
    //
    //      where N = BOOST_FUSION_MAX_ARITY
    //      and TupleID is a unique type related to the tuple's arity
    //
    ///////////////////////////////////////////////////////////////////////////
    //  include the BOOST_FUSION_TUPLE_ID
    #include <boost/spirit/fusion/identity.hpp>

    template <
        BOOST_FUSION_TEMPLATE_PARAMS_WITH_DEFAULT(
            BOOST_FUSION_MAX_ARITY, void_t)
      , typename ID = BOOST_FUSION_TUPLE_ID>
    class tuple;

    ///////////////////////////////////////////////////////////////////////////
    //
    //  tuple_element metafunction
    //
    //      Given a constant integer N and a tuple type TupleT, returns the
    //      tuple element type at slot N. (N is a zero based index). Usage:
    //
    //          tuple_element<N, TupleT>::type
    //
    ///////////////////////////////////////////////////////////////////////////
    template <unsigned N, typename TupleT>
    struct tuple_element
    {
        typedef typename
            mpl::at_c<typename TupleT::types, N>::type type;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  tuple_size metafunction
    //
    //      Get the size of TupleT. Usage:
    //
    //          tuple_size<TupleT>::value
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename TupleT>
    struct tuple_size
    {
        BOOST_STATIC_CONSTANT(unsigned, value = TupleT::size);
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  get function
    //
    //      Given a constant integer N and a tuple, returns a reference to
    //      the Nth element of the tuple. (N is a zero based index). Usage:
    //
    //          get<N>(t)
    //
    ///////////////////////////////////////////////////////////////////////////
    //  include the actual tuple element_at implementations
    #include <boost/spirit/fusion/detail/element_at.ipp>

    template <unsigned N, BOOST_FUSION_TEMPLATE_PARAMS_ALL(T)>
    inline typename tuple_detail::
        const_get_result<N, tuple<BOOST_FUSION_TEMPLATE_ARGS(T)> >::type
    get(tuple<BOOST_FUSION_TEMPLATE_ARGS(T)> const& t, mpl::int_<N>* = 0)
    {
        return tuple_detail::element_at_<N>::type::get(t);
    }

    template <unsigned N, BOOST_FUSION_TEMPLATE_PARAMS_ALL(T)>
    inline typename tuple_detail::
        get_result<N, tuple<BOOST_FUSION_TEMPLATE_ARGS(T)> >::type
    get(tuple<BOOST_FUSION_TEMPLATE_ARGS(T)>& t, mpl::int_<N>* = 0)
    {
        return tuple_detail::element_at_<N>::type::get(t);
    }

    /* Note: mpl::int_<N>* = 0 is a workaround to an MSVC 6 bug */

    ///////////////////////////////////////////////////////////////////////////
    //
    //  is_tuple metafunction
    //
    //      Given a type T, returns a value true or false if T is a
    //      tuple or not. Usage:
    //
    //          is_tuple<T>::value
    //
    ///////////////////////////////////////////////////////////////////////////
    namespace tuple_detail
    {
        struct tuple_base {};
    }

    template <typename T>
    struct is_tuple
        : mpl::bool_<is_base_and_derived<tuple_detail::tuple_base, T>::value> {};

    ///////////////////////////////////////////////////////////////////////////
    //
    //  tuple
    //
    //      Holds heterogeneously typed values that can be accessed
    //      individually by its zero based index. The maximum number of
    //      elements is limited by the user #definable constant
    //      BOOST_FUSION_MAX_ARITY, which defaults to 6.
    //
    //      Usage examples:
    //
    //          tuple<int, char>    t2(1, 'x'); //  constructor
    //          tuple<long, short>  t2(1);      //  short is default constructed
    //
    //          t1 = t2;    //  assignment, tuples with same number of elements
    //                      //  may be assigned as long as each corresponding
    //                      //  element are assignable, i.e long <- int and
    //                      //  short <- char. This is also true for copy
    //                      //  construction if the tuple elements are copy
    //                      //  constructible.
    //
    //          t1 == t2;   //  comparison, tuples with same number of elements
    //                      //  may be assigned as long as each corresponding
    //                      //  element are comparable, This is also true for
    //                      //  !=, <, <=, > and >=.
    //
    //          std::pair<long, short>  p(1, 1.0);
    //          tuple<long, short>  t3(p);          //  copy from a std::pair
    //          t3 = p;                             //  assign from a std::pair
    //
    //      *** There is a special constructor that accepts a special
    //      *** half-compile-time-half-run-time iterator. See iterator.hpp
    //      *** for details and usage.
    //
    //      The actual interface, without the preprocessor gunk looks
    //      like this:
    //
    //      << where N = BOOST_FUSION_MAX_ARITY >>
    //
    //      template <typename T0, typename T1, ... typename TN, typename ID>
    //      class tuple
    //      {
    //          typedef implementation-defined types;
    //          typedef implementation-defined begin;
    //          typedef implementation-defined end;
    //          typedef implementation-defined size_type;
    //          static unsigned const size = size_type::value;
    //
    //          tuple()
    //              : data() {}
    //
    //          explicit tuple(
    //              typename tuple_detail::call_param<T0>::type _0);
    //
    //          tuple(
    //              typename tuple_detail::call_param<T0>::type _0,
    //              typename tuple_detail::call_param<T1>::type _1);
    //
    //          ...
    //
    //          tuple(
    //              typename tuple_detail::call_param<T0>::type _0,
    //              typename tuple_detail::call_param<T1>::type _1,
    //              ...
    //              typename tuple_detail::call_param<TN>::type _N);
    //
    //          template <typename XT>
    //          explicit tuple(XT const& x);
    //
    //          template <typename TB0, typename TB1... typename TBN>
    //          tuple(tuple<TB0, TB1... TBN, ID> const& t);
    //
    //          template <typename FirstT, typename SecondT>
    //          tuple(std::pair<FirstT, SecondT> const& t);
    //
    //          template <typename TB0, typename TB1... typename TBN>
    //          tuple& operator=(tuple<TB0, TB1... TBN, ID> const& t);
    //
    //          template <typename FirstT, typename SecondT>
    //          tuple&
    //          operator=(std::pair<FirstT, SecondT> const& t);
    //
    //          implementation-defined data
    //      };
    //
    ///////////////////////////////////////////////////////////////////////////
    template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(T), typename ID>
    class tuple : tuple_detail::tuple_base
    {
        typedef BOOST_PP_CAT(mpl::vector, BOOST_FUSION_MAX_ARITY)<
            BOOST_FUSION_TEMPLATE_ARGS(T)> all_types_;

    public:

        typedef typename
            mpl::count_if<
                all_types_
              , mpl::not_<is_same<mpl::_, void_t> >
            >::type
        size_type;

        BOOST_STATIC_CONSTANT(unsigned, size = size_type::value);
        BOOST_FUSION_TUPLE_HOLDER_TYPE tuple_holder_type;

        typedef typename tuple_holder_type::
            template data<BOOST_FUSION_TEMPLATE_ARGS(T)>
        data_type;

        typedef typename data_type::types types;
        typedef typename mpl::begin<types>::type begin;
        typedef typename mpl::end<types>::type end;

        tuple() : data() {} // arity zero (default constructor)

        explicit tuple(typename tuple_detail::call_param<T0>::type _0)
            : data(_0, int()) {}

        tuple(
            typename tuple_detail::call_param<T0>::type _0
          , typename tuple_detail::call_param<T1>::type _1)
            : data(_0, _1) {}

        //  Bring in the rest of the constructors
        #include <boost/spirit/fusion/detail/constructor.ipp>

        template <typename XT>
        explicit tuple(XT const& x)
            : data(
                typename tuple_detail::
                    get_iterator_data<size_type, XT, T0>::type(x)
              , typename tuple_detail::
                    get_iterator_data_tag<XT, T0>::type()) {}

        template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(TB)>
        tuple(BOOST_FUSION_TUPLE(TB) const& t)
            : data(t.data) {}

        template <typename FirstT, typename SecondT>
        tuple(std::pair<FirstT, SecondT> const& t)
            : data(t.first, t.second) { BOOST_STATIC_ASSERT(size == 2); }

        template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(TB)>
        tuple& operator=(BOOST_FUSION_TUPLE(TB) const& t)
        { data = t.data; return *this; }

        template <typename FirstT, typename SecondT>
        tuple&
        operator=(std::pair<FirstT, SecondT> const& t)
        {
            BOOST_STATIC_ASSERT(size == 2);
            tuple_detail::element_at_<0>::type::get(*this) = t.first;
            tuple_detail::element_at_<1>::type::get(*this) = t.second;
            return *this;
        }

        data_type data;
    };

}} // end of namespace boost::fusion

#undef BOOST_FUSION_TUPLE_MEMBERS_TYPE
#endif // defined(BOOST_FUSION_TUPLE_HPP)
