/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifdef TWO_DIMENSIONAL
template<int X, int Y>
struct frame_last_it
{
    static void
    call()
    {
        test_impl<X, Y>();

        frame_last_it<X, Y+1>();
    }
};

template<int X, int Y>
struct frame_last_it<X, TWO_DIMENSIONAL>
{
    static void
    call()
    {}
};
#endif

template<int A, int B, int C>
struct frame_abc
{
    static void
    call()
    {
        static const int it=
            A*BOOST_FUSION_COMPILE_TIME_TEST_RES*
                BOOST_FUSION_COMPILE_TIME_TEST_RES+
            B*BOOST_FUSION_COMPILE_TIME_TEST_RES+
            C;
#ifdef TWO_DIMENSIONAL
        frame_last_it<it, 0>();
#else
        test_impl<it>();
#endif
    }
};


template<int A, int B>
struct frame_abc<A, B, BOOST_FUSION_COMPILE_TIME_TEST_RES>
{
    static void
    call()
    {}
};

template<int A, int B>
struct frame_ab
{
    static void
    call()
    {
        frame_abc<A, B, 0>::call();
        frame_ab<A, B+1>::call();
    }
};

template<int A>
struct frame_ab<A, BOOST_FUSION_COMPILE_TIME_TEST_RES>
{
    static void
    call()
    {}
};

template<int A>
struct frame_a
{
    static void
    call()
    {
        frame_ab<A, 0>::call();
        frame_a<A+1>::call();
    }
};

template<>
struct frame_a<BOOST_FUSION_COMPILE_TIME_TEST_RES>
{
    static void
    call()
    {}
};

int main()
{
    frame_a<0>::call();

    return result;
}
