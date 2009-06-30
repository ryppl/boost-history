/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ZIP_VIEW_ZIP_VIEW_ITERATOR_FWD_HPP
#define BOOST_FUSION_VIEW_ZIP_VIEW_ZIP_VIEW_ITERATOR_FWD_HPP

#include <boost/fusion/view/detail/strictest_traversal.hpp>

namespace boost { namespace fusion {

    template<typename IteratorSequence,
             typename Traversal =
                 typename detail::strictest_traversal<IteratorSequence>::type
            >
    struct zip_view_iterator;

}}

#endif
