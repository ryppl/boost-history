/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <boost/fusion/sequence/container/vector/vector.hpp>
#include <boost/fusion/sequence/container/vector/vector_iterator.hpp>

#define FUSION_SEQUENCE vector
#define FUSION_TRAVERSAL_TAG random_access_traversal_tag
#include "./iterator.hpp"

int
main()
{
    test();
    return boost::report_errors();
}


