// Pinhole EditorTypeFinder.hpp file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DETAIL_EDITORTYPEFINDER
#define BOOST_DETAIL_EDITORTYPEFINDER

#include <boost/mpl/void.hpp>
#include "BoolEditor.hpp"
#include "DoubleEditor.hpp"
#include "FloatEditor.hpp"
#include "IntegerEditor.hpp"
#include "StringEditor.hpp"

namespace boost { namespace pinhole { namespace detail
{

    template<typename Value_Type>
    struct EditorTypeFinder
    {
        typedef boost::mpl::void_ type;
    };

    template<>
    struct EditorTypeFinder<bool>
    {
        typedef BoolEditor type;
    };

    template<>
    struct EditorTypeFinder<double>
    {
        typedef DoubleEditor type;
    };

    template<>
    struct EditorTypeFinder<float>
    {
        typedef FloatEditor type;
    };

    template<>
    struct EditorTypeFinder<int>
    {
        typedef IntegerEditor type;
    };

    template<>
    struct EditorTypeFinder<time_t>
    {
        typedef IntegerEditor type;
    };

    template<>
    struct EditorTypeFinder<std::string>
    {
        typedef StringEditor type;
    };

    template<>
    struct EditorTypeFinder<std::wstring>
    {
        typedef WStringEditor type;
    };
}}}

#endif // include guard