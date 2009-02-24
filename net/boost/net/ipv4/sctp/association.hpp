//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_IPV4_SCTP_ASSOCIATION_H
#define BOOST_NET_IPV4_SCTP_ASSOCIATION_H

#include <boost/net/types.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace sctp
{
class address_params;
class one2many;
class one2many_listener;

namespace detail
{ class association_options; }

class association
{
private:
	friend class address_params;
	friend class one2many;
	friend class one2many_listener;
	friend class detail::association_options;

	sctp_assoc_type	assoc_id_;

	association( sctp_assoc_type assoc_id)
	: assoc_id_( assoc_id)
	{}

	operator sctp_assoc_type const () const
	{ return assoc_id_; }
};
}}}}

#endif // BOOST_NET_IPV4_SCTP_ASSOCIATION_H
