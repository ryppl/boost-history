/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <boost/fusion/sequence/container/deque/deque.hpp>

#define FUSION_SEQUENCE deque
#define FUSION_TRAVERSAL_TAG bidirectional_traversal_tag
#include "./iterator.hpp"

int
main()
{
    test();
    return boost::report_errors();
}
