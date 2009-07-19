/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

//TODO!!!
#define FUSION_MEMBER_DEFAULT_INIT(z, n, _)     m##n(T##n())
#define FUSION_MEMBER_INIT(z, n, _)             m##n(_##n)
#define FUSION_COPY_INIT(z, n, _)               m##n(other.m##n)
#define FUSION_MEMBER_DECL(z, n, _)             T##n m##n;

#define FUSION_MEMBER_ASSIGN(z, n, _)                                           \
    this->BOOST_PP_CAT(m, n) = vec.BOOST_PP_CAT(m, n);

#define FUSION_DEREF_MEMBER_ASSIGN(z, n, _)                                     \
    this->BOOST_PP_CAT(m, n) = *BOOST_PP_CAT(i, n);

#define FUSION_AT_IMPL(z, n, _)                                                 \
    typename add_reference<T##n>::type                                          \
        at_impl(mpl::int_<n>) { return this->m##n; }                            \
    typename add_reference<typename add_const<T##n>::type>::type                \
        at_impl(mpl::int_<n>) const { return this->m##n; }

#define FUSION_ITER_DECL_VAR(z, n, _)                                           \
    typedef typename result_of::next<                                           \
        BOOST_PP_CAT(I, BOOST_PP_DEC(n))>::type BOOST_PP_CAT(I, n);             \
    BOOST_PP_CAT(I, n) BOOST_PP_CAT(i, n)                                       \
        = fusion::next(BOOST_PP_CAT(i, BOOST_PP_DEC(n)));

#define BOOST_FUSION_DEREF_I(Z,N,_) fusion::deref(*BOOST_PP_CAT(i,N))

#define N BOOST_PP_ITERATION()

#if N
    template <BOOST_PP_ENUM_PARAMS(N, typename T)>
    struct BOOST_PP_CAT(vector_data, N)
    {
        BOOST_PP_CAT(vector_data, N)()
            : BOOST_PP_ENUM(N, FUSION_MEMBER_DEFAULT_INIT, _) {}

        BOOST_PP_CAT(vector_data, N)
            (BOOST_PP_ENUM_BINARY_PARAMS(N, const T,& _))
          : BOOST_PP_ENUM(N, FUSION_MEMBER_INIT, _)
        {}

        BOOST_PP_CAT(vector_data, N)(
            BOOST_PP_CAT(vector_data, N) const& other)
            : BOOST_PP_ENUM(N, FUSION_COPY_INIT, _) {}

        BOOST_PP_CAT(vector_data, N)&
        operator=(BOOST_PP_CAT(vector_data, N) const& vec)
        {
            BOOST_PP_REPEAT(N, FUSION_MEMBER_ASSIGN, _)
            return *this;
        }

        template <typename Sequence>
        static BOOST_PP_CAT(vector_data, N)
        init_from_sequence(Sequence const& seq)
        {
            typedef typename result_of::begin<Sequence const>::type I0;
            I0 i0 = fusion::begin(seq);
            BOOST_PP_REPEAT_FROM_TO(1, N, FUSION_ITER_DECL_VAR, _)
            return BOOST_PP_CAT(vector_data, N) (BOOST_PP_ENUM(N, BOOST_FUSION_DEREF_I,_));
        }

        BOOST_PP_REPEAT(N, FUSION_MEMBER_DECL, _)
    };
#endif

#if !N
    struct void_;

    template <class T0=void_>
    struct vector0;

    template<>
    struct vector0<void_>
      : sequence_base<vector0<void_> >
#else
    template <BOOST_PP_ENUM_PARAMS(N, typename T)>
    struct BOOST_PP_CAT(vector, N)
      : sequence_base<BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N,T)> >
      , BOOST_PP_CAT(vector_data, N)<BOOST_PP_ENUM_PARAMS(N, T)>
#endif
    {
        typedef BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N, T)> this_type;
#if N
        typedef BOOST_PP_CAT(vector_data, N)<BOOST_PP_ENUM_PARAMS(N, T)> base_type;
#endif
        typedef mpl::BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N, T)> types;
        typedef vector_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::false_ is_view;
        typedef random_access_traversal_tag category;
        typedef mpl::int_<N> size;

        BOOST_PP_CAT(vector, N)()
        {}

#if N
#if (N == 1)
        explicit
#endif
        BOOST_PP_CAT(vector, N)(BOOST_PP_ENUM_BINARY_PARAMS(N, const T,& _))
          : base_type(BOOST_PP_ENUM_PARAMS(N, _))
        {}
#endif

#if N
        template <BOOST_PP_ENUM_PARAMS(N, typename U)>
#endif
        BOOST_PP_CAT(vector, N)(
            BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N, U)> const& vec)
#if N
          : base_type(BOOST_PP_ENUM_PARAMS(N, vec.m))
#endif
        {}

        template<typename Seq>
        BOOST_PP_CAT(vector, N)(detail::sequence_assign_type<Seq> const& seq)
#if N
          : base_type(base_type::init_from_sequence(seq.get()))
#endif
        {}

#if N
        template <BOOST_PP_ENUM_PARAMS(N, typename U)>
#endif
        BOOST_PP_CAT(vector, N)&
        operator=(BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N, U)> const& vec)
        {
            BOOST_PP_REPEAT(N, FUSION_MEMBER_ASSIGN, _)
            return *this;
        }

        template <typename Sequence>
        this_type&
        operator=(Sequence const& seq)
        {
            typedef typename result_of::begin<Sequence const>::type I0;
            I0 i0 = fusion::begin(seq);
            BOOST_PP_REPEAT_FROM_TO(1, N, FUSION_ITER_DECL_VAR, _)
            BOOST_PP_REPEAT(N, FUSION_DEREF_MEMBER_ASSIGN, _)
            return *this;
        }

        BOOST_PP_REPEAT(N, FUSION_AT_IMPL, _)

        template<typename I>
        typename detail::add_lref<typename mpl::at<types, I>::type>::type
        at_impl(I i)
        {
            return this->at_impl(mpl::int_<I::value>());
        }

        template<typename I>
        typename detail::add_lref<typename add_const<typename mpl::at<types, I>::type>::type>::type
        at_impl(I i) const
        {
            return this->at_impl(mpl::int_<I::value>());
        }
    };

#undef N
#undef FUSION_MEMBER_DEFAULT_INIT
#undef FUSION_MEMBER_INIT
#undef FUSION_COPY_INIT
#undef FUSION_MEMBER_DECL
#undef FUSION_MEMBER_ASSIGN
#undef FUSION_DEREF_MEMBER_ASSIGN
#undef FUSION_AT_IMPL
#undef FUSION_ITER_DECL_VAR
#undef BOOST_FUSION_DEREF_I

