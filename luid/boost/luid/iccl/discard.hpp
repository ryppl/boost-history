/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_ICCL_DISCARD_HPP_
#define BOOST_LUID_ICCL_DISCARD_HPP_

namespace boost
{
namespace luid
{
namespace iccl
{

template <typename Inner>
struct discard
{
    typedef Inner inner_type;
    typedef typename inner_type::config config;
    typedef typename inner_type::node_type node_type;
    typedef typename inner_type::header_type header_type;
    typedef typename inner_type::type type;
};

}
}
}
#endif // BOOST_LUID_ICCL_DISCARD_HPP_
