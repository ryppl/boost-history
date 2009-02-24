//  Copyright (c) 2001-2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NET_TYPES_H
#define BOOST_NET_TYPES_H

#include <boost/net/detail/config.hpp>

extern "C"
{
#ifdef BOOST_NET_HAVE_DEVPOLL_H
#include <devpoll.h>
#endif

#ifdef BOOST_NET_HAVE_NETDB_H
#include <netdb.h>
#endif

#ifdef BOOST_NET_HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef BOOST_NET_HAVE_NETINET_SCTP_H
#include <netinet/sctp.h>
#endif

#ifdef BOOST_NET_HAVE_POLL_H
#include <poll.h>
#endif

#ifdef BOOST_NET_HAVE_SEMAPHORE_H
#include <semaphore.h>
#endif

#ifdef BOOST_NET_HAVE_SYS_EPOLL_H
#include <sys/epoll.h>
#endif

#ifdef BOOST_NET_HAVE_SYS_EVENT_H
#include <sys/event.h>
#endif

#ifdef BOOST_NET_HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef BOOST_NET_HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef BOOST_NET_HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifdef BOOST_NET_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef BOOST_NET_HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif

#ifdef BOOST_NET_HAVE_SYS_UN_H
#include <sys/un.h>
#endif
}

#include <csignal>

namespace boost { namespace net
{
typedef int						native_handle_type;

#ifdef BOOST_NET_HAVE_MODE_T
typedef mode_t					mode_type;
#endif

#ifdef BOOST_NET_HAVE_SCTP_ASSOC_T
typedef sctp_assoc_t	 		sctp_assoc_type;
#endif


#ifdef BOOST_NET_HAVE_SEM_T
typedef sem_t					sem_type;
#endif

#ifdef BOOST_NET_HAVE_SIGSET_T
typedef sigset_t				sigset_type;
#endif

#ifdef BOOST_NET_HAVE_SOCKLEN_T
typedef socklen_t				sockaddr_len_type;
#else
typedef unsigned int			sockaddr_len_type;
#endif

#ifdef BOOST_NET_HAVE_UINT32_T
typedef uint32_t				u_long_type;
#else
typedef unsigned int			u_long_type;
#endif

#ifdef BOOST_NET_HAVE_UINT16_T
typedef uint16_t				u_short_type;
#else
typedef unsigned short			u_short_type;
#endif

#ifdef BOOST_NET_HAVE_U_CHAR
typedef u_char					u_char_type;
#else
typedef unsigned char			u_char_type;
#endif

#ifdef BOOST_NET_HAVE_ADDRINFO_STRUCT
typedef addrinfo 				addrinfo_type;
#endif

#ifdef BOOST_NET_HAVE_DVPOLL_STRUCT
typedef dvpoll 					dvpoll_type;
#endif

#ifdef BOOST_NET_HAVE_EPOLL_EVENT_STRUCT
typedef epoll_event				epoll_event_type;
#endif

#ifdef BOOST_NET_HAVE_GROUP_REQ_STRUCT
typedef group_req				group_req_type;
#endif

#ifdef BOOST_NET_HAVE_GROUP_SOURCE_REQ_STRUCT
typedef group_source_req		group_source_req_type;
#endif

typedef sa_family_t				family_type;

#ifdef BOOST_NET_HAVE_HOSTENT_STRUCT
typedef hostent					hostent_type;
#endif

#ifdef BOOST_NET_HAVE_IN_ADDR_STRUCT
typedef in_addr					in4_addr_type;
#endif

#ifdef BOOST_NET_HAVE_IN6_ADDR_STRUCT
typedef in6_addr				in6_addr_type;
#endif

#ifdef BOOST_NET_HAVE_IP_MREQ_STRUCT
typedef ip_mreq					ip4_mreq_type;
#endif

#ifdef BOOST_NET_HAVE_IOVEC_STRUCT
typedef iovec 					iovec_type;
#endif

#ifdef BOOST_NET_HAVE_KEVENT_STRUCT
typedef kevent 					kevent_type;
#endif


#ifdef BOOST_NET_HAVE_LINGER_STRUCT
typedef linger					linger_type;
#endif

#ifdef BOOST_NET_HAVE_MSGHDR_STRUCT
typedef msghdr					msghdr_type;
#endif

#ifdef BOOST_NET_HAVE_POLLFD_STRUCT
typedef pollfd 					pollfd_type;
#endif

#ifdef BOOST_NET_HAVE_SCTP_ASSOCPARAMS_STRUCT
typedef sctp_assocparams		sctp_assocparams_type;
#endif

#ifdef BOOST_NET_HAVE_SCTP_EVENT_SUBSCRIBE_STRUCT
typedef sctp_event_subscribe	sctp_event_subscribe_type;
#endif

#ifdef BOOST_NET_HAVE_SCTP_INITMSG_STRUCT
typedef sctp_initmsg			sctp_initmsg_type;
#endif

#ifdef BOOST_NET_HAVE_SCTP_PADDRINFO_STRUCT
typedef sctp_paddrinfo			sctp_paddrinfo_type;
#endif

#ifdef BOOST_NET_HAVE_SCTP_PADDRPARAMS_STRUCT
typedef sctp_paddrparams		sctp_paddrparams_type;
#endif

#ifdef BOOST_NET_HAVE_SCTP_RTOINFO_STRUCT
typedef sctp_rtoinfo			sctp_rtoinfo_type;
#endif

#ifdef BOOST_NET_HAVE_SCTP_SETADAPTATION_STRUCT
typedef sctp_setadaptation		sctp_setadaptation_type;
#endif

#ifdef BOOST_NET_HAVE_SCTP_SETPEERPRIM_STRUCT
typedef sctp_setpeerprim		sctp_setpeerprim_type;
#endif

#ifdef BOOST_NET_HAVE_SCTP_SETPRIM_STRUCT
typedef sctp_setprim			sctp_setprim_type;
#endif

#ifdef BOOST_NET_HAVE_SCTP_SNDRCVINFO_STRUCT
typedef sctp_sndrcvinfo			sctp_sndrcvinfo_type;
#endif

#ifdef BOOST_NET_HAVE_SCTP_STATUS_STRUCT
typedef sctp_status				sctp_status_type;
#endif

#ifdef BOOST_NET_HAVE_SERVENT_STRUCT
typedef servent					servent_type;
#endif

#ifdef BOOST_NET_HAVE_SIGACTION_STRUCT
typedef struct sigaction		sigaction_type;
#endif

#ifdef BOOST_NET_HAVE_SOCKADDR_STRUCT
typedef sockaddr				sockaddr_type;
#endif

#ifdef BOOST_NET_HAVE_SOCKADDR_IN_STRUCT
typedef sockaddr_in				sockaddr_in4_type;
#endif

#ifdef BOOST_NET_HAVE_SOCKADDR_IN6_STRUCT
typedef sockaddr_in6			sockaddr_in6_type;
#endif

#ifdef BOOST_NET_HAVE_SOCKADDR_STORAGE_STRUCT
typedef sockaddr_storage		sockaddr_storage_type;
#endif

#ifdef BOOST_NET_HAVE_SOCKADDR_UN_STRUCT
typedef sockaddr_un				sockaddr_un_type;
#endif

#ifdef BOOST_NET_HAVE_TIMESPEC_STRUCT
typedef timespec				timespec_type;
#endif

#ifdef BOOST_NET_HAVE_TIMEVAL_STRUCT
typedef timeval					timeval_type;
#endif
} }

#endif // BOOST_NET_TYPES_H
