/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

template<int X, int Y, int Z>
struct frame_z
{
    static void
    call()
    {
        test_impl<
            X*BOOST_FUSION_COMPILE_TIME_TEST_RES*
                BOOST_FUSION_COMPILE_TIME_TEST_RES+
            Y*BOOST_FUSION_COMPILE_TIME_TEST_RES+
            Z
        >();
    }
};


template<int X, int Y>
struct frame_z<X, Y, BOOST_FUSION_COMPILE_TIME_TEST_RES>
{
    static void
    call()
    {}
};

template<int X, int Y>
struct frame_y
{
    static void
    call()
    {
        frame_z<X, Y, 0>::call();
        frame_y<X, Y+1>::call();
    }
};

template<int X>
struct frame_y<X, BOOST_FUSION_COMPILE_TIME_TEST_RES>
{
    static void
    call()
    {}
};

template<int X>
struct frame_x
{
    static void
    call()
    {
        frame_y<X, 0>::call();
        frame_x<X+1>::call();
    }
};

template<>
struct frame_x<BOOST_FUSION_COMPILE_TIME_TEST_RES>
{
    static void
    call()
    {}
};

int main()
{
    frame_x<0>::call();

    return result;
}
