BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN
template<> struct arg<-1>
{
    static int const value = -1;
    BOOST_MPL_AUX_ARG_TYPEDEF(void_, tag)
    template<
          typename U1, typename U2, typename U3, typename U4, typename U5
        >
    struct apply
    {
        typedef U1 type;
        BOOST_MPL_ASSERT_NOT((is_na<type>));
    };
};

template<> struct arg<1>
{
    static int const value = 1;
    typedef arg<2> next;
    BOOST_MPL_AUX_ARG_TYPEDEF(void_, tag)
    template<
          typename U1, typename U2, typename U3, typename U4, typename U5
        >
    struct apply
    {
        typedef U1 type;
        BOOST_MPL_ASSERT_NOT((is_na<type>));
    };
};

template<> struct arg<2>
{
    static int const value = 2;
    typedef arg<3> next;
    BOOST_MPL_AUX_ARG_TYPEDEF(void_, tag)
    template<
          typename U1, typename U2, typename U3, typename U4, typename U5
        >
    struct apply
    {
        typedef U2 type;
        BOOST_MPL_ASSERT_NOT((is_na<type>));
    };
};

template<> struct arg<3>
{
    static int const value = 3;
    typedef arg<4> next;
    BOOST_MPL_AUX_ARG_TYPEDEF(void_, tag)
    template<
          typename U1, typename U2, typename U3, typename U4, typename U5
        >
    struct apply
    {
        typedef U3 type;
        BOOST_MPL_ASSERT_NOT((is_na<type>));
    };
};

template<> struct arg<4>
{
    static int const value = 4;
    typedef arg<5> next;
    BOOST_MPL_AUX_ARG_TYPEDEF(void_, tag)
    template<
          typename U1, typename U2, typename U3, typename U4, typename U5
        >
    struct apply
    {
        typedef U4 type;
        BOOST_MPL_ASSERT_NOT((is_na<type>));
    };
};

template<> struct arg<5>
{
    static int const value = 5;
    typedef arg<6> next;
    BOOST_MPL_AUX_ARG_TYPEDEF(void_, tag)
    template<
          typename U1, typename U2, typename U3, typename U4, typename U5
        >
    struct apply
    {
        typedef U5 type;
        BOOST_MPL_ASSERT_NOT((is_na<type>));
    };
};

BOOST_MPL_AUX_NONTYPE_ARITY_SPEC(1, int, arg)
BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE
