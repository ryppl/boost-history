// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_VARIADIC_TEMPLATES_ZIP_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_VARIADIC_TEMPLATES_ZIP_HPP

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/container/generation/vector_tie.hpp>
#include <boost/fusion/view/zip_view.hpp>
#include <boost/fusion/support/ref.hpp>

namespace boost { namespace fusion {

    namespace result_of
    {
        template<typename... Seqs>
        struct zip
        {
            typedef
                zip_view<
                    vector<typename detail::add_lref<Seqs>::type...>
                >
            type;
        };
    }

    template<typename... Seqs>
    inline typename result_of::zip<BOOST_FUSION_R_ELSE_CLREF(Seqs)...>::type
    zip(BOOST_FUSION_R_ELSE_CLREF(Seqs)... seqs)
    {
        return typename
            result_of::zip<BOOST_FUSION_R_ELSE_CLREF(Seqs)...>::type(
                    vector_tie(seqs...));
    }

}}

#endif
