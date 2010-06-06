//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


/** This file contains includes for fusion sequence support for bit_mask_group.
 *  This header file is provided for organizational purposes.
 */
#ifndef BOOST_BIT_MASK_GROUP_FUSION_EXT_INCLUDE_HPP
#define BOOST_BIT_MASK_GROUP_FUSION_EXT_INCLUDE_HPP

/** Include files from boost used for implementing the overloads within
 *  the fusion extension.
 */
#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/include/tag_of_fwd.hpp>
#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/size.hpp>

/** Fusion sequence extension files. */ 
#include "details/tag_and_fusion_fwd.hpp"
#include "bit_mask_group_iterator.hpp"
#include "tag_of.hpp"
// #include "details/category_of_impl.hpp"
#include "details/is_sequence_impl.hpp"
#include "details/is_view_impl.hpp"
// #include "details/end_impl.hpp"
#include "details/begin_impl.hpp"
// #include "details/size_impl.hpp"
// #include "details/value_at_impl.hpp"
// #include "details/at_impl.hpp"


#endif
