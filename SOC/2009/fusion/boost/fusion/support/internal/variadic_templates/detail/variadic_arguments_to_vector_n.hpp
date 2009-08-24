/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

    template<BOOST_PP_ENUM_PARAMS_Z(1, BOOST_PP_ITERATION(), typename T)>
    struct variadic_arguments_to_vector<
        BOOST_PP_ENUM_PARAMS_Z(1, BOOST_PP_ITERATION(), T)
    >
    {
    public:
        typedef
            mpl::BOOST_PP_CAT(vector,BOOST_PP_ITERATION())<
                BOOST_PP_ENUM_PARAMS_Z(1, BOOST_PP_ITERATION(), T)
            >
        type;
    };
