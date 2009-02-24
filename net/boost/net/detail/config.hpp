#ifndef ____SRC_BOOST_NET_DETAIL_CONFIG_HPP
#define ____SRC_BOOST_NET_DETAIL_CONFIG_HPP 1
 
/* ../src/boost/net/detail/config.hpp. Generated automatically at end of configure. */
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

/* Define to 1 if /dev/poll is supported */
/* #undef BOOST_NET_DEVPOLL */

/* Define to 1 if epoll is supported */
#ifndef BOOST_NET_EPOLL 
#define BOOST_NET_EPOLL  1 
#endif

/* Define to 1 if <netdb.h> defines struct addrinfo */
#ifndef BOOST_NET_HAVE_ADDRINFO_STRUCT 
#define BOOST_NET_HAVE_ADDRINFO_STRUCT  1 
#endif

/* Define to 1 if you have the <arpa/inet.h> header file. */
#ifndef BOOST_NET_HAVE_ARPA_INET_H 
#define BOOST_NET_HAVE_ARPA_INET_H  1 
#endif

/* Define to 1 if you have the <ctype.h> header file. */
#ifndef BOOST_NET_HAVE_CTYPE_H 
#define BOOST_NET_HAVE_CTYPE_H  1 
#endif

/* Define to 1 if you have the <devpoll.h> header file. */
/* #undef BOOST_NET_HAVE_DEVPOLL_H */

/* Define to 1 if you have the <dlfcn.h> header file. */
#ifndef BOOST_NET_HAVE_DLFCN_H 
#define BOOST_NET_HAVE_DLFCN_H  1 
#endif

/* Define to 1 if <devpoll.h> defines struct dvpoll */
/* #undef BOOST_NET_HAVE_DVPOLL_STRUCT */

/* Define to 1 if <sys/epoll.h> defines struct epoll_event */
#ifndef BOOST_NET_HAVE_EPOLL_EVENT_STRUCT 
#define BOOST_NET_HAVE_EPOLL_EVENT_STRUCT  1 
#endif

/* Define to 1 if you have the <errno.h> header file. */
#ifndef BOOST_NET_HAVE_ERRNO_H 
#define BOOST_NET_HAVE_ERRNO_H  1 
#endif

/* Define to 1 if you have the <fcntl.h> header file. */
#ifndef BOOST_NET_HAVE_FCNTL_H 
#define BOOST_NET_HAVE_FCNTL_H  1 
#endif

/* Define to 1 if <netinet/in.h> defines struct group_req */
#ifndef BOOST_NET_HAVE_GROUP_REQ_STRUCT 
#define BOOST_NET_HAVE_GROUP_REQ_STRUCT  1 
#endif

/* Define to 1 if <netinet/in.h> defines struct group_source_req */
#ifndef BOOST_NET_HAVE_GROUP_SOURCE_REQ_STRUCT 
#define BOOST_NET_HAVE_GROUP_SOURCE_REQ_STRUCT  1 
#endif

/* Define to 1 if <netdb.h> defines struct hostent */
#ifndef BOOST_NET_HAVE_HOSTENT_STRUCT 
#define BOOST_NET_HAVE_HOSTENT_STRUCT  1 
#endif

/* Define to 1 if <netinet/in.h> defines struct in6_addr */
#ifndef BOOST_NET_HAVE_IN6_ADDR_STRUCT 
#define BOOST_NET_HAVE_IN6_ADDR_STRUCT  1 
#endif

/* Define to 1 if you have the <inttypes.h> header file. */
#ifndef BOOST_NET_HAVE_INTTYPES_H 
#define BOOST_NET_HAVE_INTTYPES_H  1 
#endif

/* Define to 1 if <netinet/in.h> defines struct in_addr */
#ifndef BOOST_NET_HAVE_IN_ADDR_STRUCT 
#define BOOST_NET_HAVE_IN_ADDR_STRUCT  1 
#endif

/* Define to 1 if <sys/uio.h> defines struct iovec */
#ifndef BOOST_NET_HAVE_IOVEC_STRUCT 
#define BOOST_NET_HAVE_IOVEC_STRUCT  1 
#endif

/* Define to 1 if socket option IPTOS_LOWCOST is supported */
#ifndef BOOST_NET_HAVE_IPTOS_LOWCOST 
#define BOOST_NET_HAVE_IPTOS_LOWCOST  1 
#endif

/* Define to 1 if socket option IP_RECVDSTADDR is supported */
/* #undef BOOST_NET_HAVE_IP_RECVDSTADDR */

/* Define to 1 if socket option IP_RECVIF is supported */
/* #undef BOOST_NET_HAVE_IP_RECVIF */

/* Define to 1 if <ys/event.h> defines struct kevent */
/* #undef BOOST_NET_HAVE_KEVENT_STRUCT */

/* Define to 1 if <sys/socket.h> defines struct linger */
#ifndef BOOST_NET_HAVE_LINGER_STRUCT 
#define BOOST_NET_HAVE_LINGER_STRUCT  1 
#endif

/* Define to 1 if you have the <memory.h> header file. */
#ifndef BOOST_NET_HAVE_MEMORY_H 
#define BOOST_NET_HAVE_MEMORY_H  1 
#endif

/* Define to 1 if <sys/stat.h> defines mode_t */
#ifndef BOOST_NET_HAVE_MODE_T 
#define BOOST_NET_HAVE_MODE_T  1 
#endif

/* Define to 1 if <sys/socket.h> defines struct msghdr */
#ifndef BOOST_NET_HAVE_MSGHDR_STRUCT 
#define BOOST_NET_HAVE_MSGHDR_STRUCT  1 
#endif

/* Define to 1 if sendto supports flag MSG_NOSIGNAL */
#ifndef BOOST_NET_HAVE_MSG_NOSIGNAL 
#define BOOST_NET_HAVE_MSG_NOSIGNAL  1 
#endif

/* Define to 1 if you have the <netconfig.h> header file. */
/* #undef BOOST_NET_HAVE_NETCONFIG_H */

/* Define to 1 if you have the <netdb.h> header file. */
#ifndef BOOST_NET_HAVE_NETDB_H 
#define BOOST_NET_HAVE_NETDB_H  1 
#endif

/* Define to 1 if you have the <netdir.h> header file. */
/* #undef BOOST_NET_HAVE_NETDIR_H */

/* Define to 1 if you have the <netinet/in.h> header file. */
#ifndef BOOST_NET_HAVE_NETINET_IN_H 
#define BOOST_NET_HAVE_NETINET_IN_H  1 
#endif

/* Define to 1 if you have the <netinet/in_systm.h> header file. */
#ifndef BOOST_NET_HAVE_NETINET_IN_SYSTM_H 
#define BOOST_NET_HAVE_NETINET_IN_SYSTM_H  1 
#endif

/* Define to 1 if you have the <netinet/ip.h> header file. */
#ifndef BOOST_NET_HAVE_NETINET_IP_H 
#define BOOST_NET_HAVE_NETINET_IP_H  1 
#endif

/* Define to 1 if you have the <netinet/sctp.h> header file. */
#ifndef BOOST_NET_HAVE_NETINET_SCTP_H 
#define BOOST_NET_HAVE_NETINET_SCTP_H  1 
#endif

/* Define to 1 if you have the <netinet/tcp.h> header file. */
#ifndef BOOST_NET_HAVE_NETINET_TCP_H 
#define BOOST_NET_HAVE_NETINET_TCP_H  1 
#endif

/* Define to 1 if you have the <net/if.h> header file. */
#ifndef BOOST_NET_HAVE_NET_IF_H 
#define BOOST_NET_HAVE_NET_IF_H  1 
#endif

/* Define to 1 if <poll.h> defines struct pollfd */
#ifndef BOOST_NET_HAVE_POLLFD_STRUCT 
#define BOOST_NET_HAVE_POLLFD_STRUCT  1 
#endif

/* Define to 1 if you have the <poll.h> header file. */
#ifndef BOOST_NET_HAVE_POLL_H 
#define BOOST_NET_HAVE_POLL_H  1 
#endif

/* Define to 1 if you have the <pthread.h> header file. */
#ifndef BOOST_NET_HAVE_PTHREAD_H 
#define BOOST_NET_HAVE_PTHREAD_H  1 
#endif

/* Define to 1 if you have the <resolv.h> header file. */
#ifndef BOOST_NET_HAVE_RESOLV_H 
#define BOOST_NET_HAVE_RESOLV_H  1 
#endif

/* Define to 1 if <netinet/sctp.h> defines struct sctp_assocparams */
#ifndef BOOST_NET_HAVE_SCTP_ASSOCPARAMS_STRUCT 
#define BOOST_NET_HAVE_SCTP_ASSOCPARAMS_STRUCT  1 
#endif

/* Define to 1 if <netinet/sctp.h> defines sctp_assoc_t */
#ifndef BOOST_NET_HAVE_SCTP_ASSOC_T 
#define BOOST_NET_HAVE_SCTP_ASSOC_T  1 
#endif

/* Define to 1 if <netinet/sctp.h> defines struct sctp_event_subscribe */
#ifndef BOOST_NET_HAVE_SCTP_EVENT_SUBSCRIBE_STRUCT 
#define BOOST_NET_HAVE_SCTP_EVENT_SUBSCRIBE_STRUCT  1 
#endif

/* Define to 1 if <netinet/sctp.h> defines struct sctp_initmsg */
#ifndef BOOST_NET_HAVE_SCTP_INITMSG_STRUCT 
#define BOOST_NET_HAVE_SCTP_INITMSG_STRUCT  1 
#endif

/* Define to 1 if socket option SCTP_MAXBURST is supported */
/* #undef BOOST_NET_HAVE_SCTP_MAXBURST */

/* Define to 1 if <netinet/sctp.h> defines struct sctp_paddrinfo */
#ifndef BOOST_NET_HAVE_SCTP_PADDRINFO_STRUCT 
#define BOOST_NET_HAVE_SCTP_PADDRINFO_STRUCT  1 
#endif

/* Define to 1 if <netinet/sctp.h> defines struct sctp_paddrparams */
#ifndef BOOST_NET_HAVE_SCTP_PADDRPARAMS_STRUCT 
#define BOOST_NET_HAVE_SCTP_PADDRPARAMS_STRUCT  1 
#endif

/* Define to 1 if socket option SCTP_PRIMARY_ADDR is supported */
#ifndef BOOST_NET_HAVE_SCTP_PRIMARY_ADDR 
#define BOOST_NET_HAVE_SCTP_PRIMARY_ADDR  1 
#endif

/* Define to 1 if <netinet/sctp.h> defines struct sctp_rtoinfo */
#ifndef BOOST_NET_HAVE_SCTP_RTOINFO_STRUCT 
#define BOOST_NET_HAVE_SCTP_RTOINFO_STRUCT  1 
#endif

/* Define to 1 if <netinet/sctp.h> defines struct sctp_setadaptation */
#ifndef BOOST_NET_HAVE_SCTP_SETADAPTATION_STRUCT 
#define BOOST_NET_HAVE_SCTP_SETADAPTATION_STRUCT  1 
#endif

/* Define to 1 if <netinet/sctp.h> defines struct sctp_setpeerprim */
#ifndef BOOST_NET_HAVE_SCTP_SETPEERPRIM_STRUCT 
#define BOOST_NET_HAVE_SCTP_SETPEERPRIM_STRUCT  1 
#endif

/* Define to 1 if <netinet/sctp.h> defines struct sctp_setprim */
#ifndef BOOST_NET_HAVE_SCTP_SETPRIM_STRUCT 
#define BOOST_NET_HAVE_SCTP_SETPRIM_STRUCT  1 
#endif

/* Define to 1 if <netinet/sctp.h> defines struct sctp_sndrcvinfo */
#ifndef BOOST_NET_HAVE_SCTP_SNDRCVINFO_STRUCT 
#define BOOST_NET_HAVE_SCTP_SNDRCVINFO_STRUCT  1 
#endif

/* Define to 1 if <netinet/sctp.h> defines struct sctp_status */
#ifndef BOOST_NET_HAVE_SCTP_STATUS_STRUCT 
#define BOOST_NET_HAVE_SCTP_STATUS_STRUCT  1 
#endif

/* Define to 1 if you have the <semaphore.h> header file. */
#ifndef BOOST_NET_HAVE_SEMAPHORE_H 
#define BOOST_NET_HAVE_SEMAPHORE_H  1 
#endif

/* Define to 1 if <semaphore.h> defines sem_t */
#ifndef BOOST_NET_HAVE_SEM_T 
#define BOOST_NET_HAVE_SEM_T  1 
#endif

/* Define to 1 if <netdb.h> defines struct servent */
#ifndef BOOST_NET_HAVE_SERVENT_STRUCT 
#define BOOST_NET_HAVE_SERVENT_STRUCT  1 
#endif

/* Define to 1 if <signal.h> defines struct sigaction */
#ifndef BOOST_NET_HAVE_SIGACTION_STRUCT 
#define BOOST_NET_HAVE_SIGACTION_STRUCT  1 
#endif

/* Define to 1 if you have the <signal.h> header file. */
#ifndef BOOST_NET_HAVE_SIGNAL_H 
#define BOOST_NET_HAVE_SIGNAL_H  1 
#endif

/* Define to 1 if <signal.h> defines sigset_t */
#ifndef BOOST_NET_HAVE_SIGSET_T 
#define BOOST_NET_HAVE_SIGSET_T  1 
#endif

/* Define to 1 if <netinet/in.h> defines struct sockaddr_in6 */
#ifndef BOOST_NET_HAVE_SOCKADDR_IN6_STRUCT 
#define BOOST_NET_HAVE_SOCKADDR_IN6_STRUCT  1 
#endif

/* Define to 1 if <netinet/in.h> defines struct sockaddr_in */
#ifndef BOOST_NET_HAVE_SOCKADDR_IN_STRUCT 
#define BOOST_NET_HAVE_SOCKADDR_IN_STRUCT  1 
#endif

/* Define to 1 if <netinet/in.h> defines struct sockaddr_storage */
#ifndef BOOST_NET_HAVE_SOCKADDR_STORAGE_STRUCT 
#define BOOST_NET_HAVE_SOCKADDR_STORAGE_STRUCT  1 
#endif

/* Define to 1 if <sys/socket.h> defines struct sockaddr */
#ifndef BOOST_NET_HAVE_SOCKADDR_STRUCT 
#define BOOST_NET_HAVE_SOCKADDR_STRUCT  1 
#endif

/* Define to 1 if <sys/un.h> defines struct sockaddr_un */
#ifndef BOOST_NET_HAVE_SOCKADDR_UN_STRUCT 
#define BOOST_NET_HAVE_SOCKADDR_UN_STRUCT  1 
#endif

/* Define to 1 if <sys/socket.h> defines socklen_t */
#ifndef BOOST_NET_HAVE_SOCKLEN_T 
#define BOOST_NET_HAVE_SOCKLEN_T  1 
#endif

/* Define to 1 if socket option SO_REUSEPORT is supported */
/* #undef BOOST_NET_HAVE_SO_REUSEPORT */

/* Define to 1 if socket option TCP_KEEPALIVE is supported */
/* #undef BOOST_NET_HAVE_SO_TCP_KEEPALIVE */

/* Define to 1 if socket option TCP_MAXRT is supported */
/* #undef BOOST_NET_HAVE_SO_TCP_MAXRT */

/* Define to 1 if socket option SO_USELOOPBACK is supported */
/* #undef BOOST_NET_HAVE_SO_USELOOPBACK */

/* Define to 1 if you have the <stdint.h> header file. */
#ifndef BOOST_NET_HAVE_STDINT_H 
#define BOOST_NET_HAVE_STDINT_H  1 
#endif

/* Define to 1 if you have the <stdio.h> header file. */
#ifndef BOOST_NET_HAVE_STDIO_H 
#define BOOST_NET_HAVE_STDIO_H  1 
#endif

/* Define to 1 if you have the <stdlib.h> header file. */
#ifndef BOOST_NET_HAVE_STDLIB_H 
#define BOOST_NET_HAVE_STDLIB_H  1 
#endif

/* Define to 1 if you have the <strings.h> header file. */
#ifndef BOOST_NET_HAVE_STRINGS_H 
#define BOOST_NET_HAVE_STRINGS_H  1 
#endif

/* Define to 1 if you have the <string.h> header file. */
#ifndef BOOST_NET_HAVE_STRING_H 
#define BOOST_NET_HAVE_STRING_H  1 
#endif

/* Define to 1 if you have the <stropts.h> header file. */
#ifndef BOOST_NET_HAVE_STROPTS_H 
#define BOOST_NET_HAVE_STROPTS_H  1 
#endif

/* Define to 1 if you have the <sys/devpoll.h> header file. */
/* #undef BOOST_NET_HAVE_SYS_DEVPOLL_H */

/* Define to 1 if you have the <sys/epoll.h> header file. */
#ifndef BOOST_NET_HAVE_SYS_EPOLL_H 
#define BOOST_NET_HAVE_SYS_EPOLL_H  1 
#endif

/* Define to 1 if you have the <sys/event.h> header file. */
/* #undef BOOST_NET_HAVE_SYS_EVENT_H */

/* Define to 1 if you have the <sys/filio.h> header file. */
/* #undef BOOST_NET_HAVE_SYS_FILIO_H */

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#ifndef BOOST_NET_HAVE_SYS_IOCTL_H 
#define BOOST_NET_HAVE_SYS_IOCTL_H  1 
#endif

/* Define to 1 if you have the <sys/select.h> header file. */
#ifndef BOOST_NET_HAVE_SYS_SELECT_H 
#define BOOST_NET_HAVE_SYS_SELECT_H  1 
#endif

/* Define to 1 if you have the <sys/socket.h> header file. */
#ifndef BOOST_NET_HAVE_SYS_SOCKET_H 
#define BOOST_NET_HAVE_SYS_SOCKET_H  1 
#endif

/* Define to 1 if you have the <sys/sockio.h> header file. */
/* #undef BOOST_NET_HAVE_SYS_SOCKIO_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#ifndef BOOST_NET_HAVE_SYS_STAT_H 
#define BOOST_NET_HAVE_SYS_STAT_H  1 
#endif

/* Define to 1 if you have the <sys/sysctl.h> header file. */
#ifndef BOOST_NET_HAVE_SYS_SYSCTL_H 
#define BOOST_NET_HAVE_SYS_SYSCTL_H  1 
#endif

/* Define to 1 if you have the <sys/time.h> header file. */
#ifndef BOOST_NET_HAVE_SYS_TIME_H 
#define BOOST_NET_HAVE_SYS_TIME_H  1 
#endif

/* Define to 1 if you have the <sys/types.h> header file. */
#ifndef BOOST_NET_HAVE_SYS_TYPES_H 
#define BOOST_NET_HAVE_SYS_TYPES_H  1 
#endif

/* Define to 1 if you have the <sys/uio.h> header file. */
#ifndef BOOST_NET_HAVE_SYS_UIO_H 
#define BOOST_NET_HAVE_SYS_UIO_H  1 
#endif

/* Define to 1 if you have the <sys/un.h> header file. */
#ifndef BOOST_NET_HAVE_SYS_UN_H 
#define BOOST_NET_HAVE_SYS_UN_H  1 
#endif

/* Define to 1 if you have the <sys/utsname.h> header file. */
#ifndef BOOST_NET_HAVE_SYS_UTSNAME_H 
#define BOOST_NET_HAVE_SYS_UTSNAME_H  1 
#endif

/* Define to 1 if you have the <sys/wait.h> header file. */
#ifndef BOOST_NET_HAVE_SYS_WAIT_H 
#define BOOST_NET_HAVE_SYS_WAIT_H  1 
#endif

/* Define to 1 if <sys/time.h> defines struct timespec */
#ifndef BOOST_NET_HAVE_TIMESPEC_STRUCT 
#define BOOST_NET_HAVE_TIMESPEC_STRUCT  1 
#endif

/* Define to 1 if <sys/time.h> defines struct timeval */
#ifndef BOOST_NET_HAVE_TIMEVAL_STRUCT 
#define BOOST_NET_HAVE_TIMEVAL_STRUCT  1 
#endif

/* Define to 1 if you have the <time.h> header file. */
#ifndef BOOST_NET_HAVE_TIME_H 
#define BOOST_NET_HAVE_TIME_H  1 
#endif

/* Define to 1 if <sys/types.h> defines uint16_t */
#ifndef BOOST_NET_HAVE_UINT16_T 
#define BOOST_NET_HAVE_UINT16_T  1 
#endif

/* Define to 1 if <sys/types.h> defines uint32_t */
#ifndef BOOST_NET_HAVE_UINT32_T 
#define BOOST_NET_HAVE_UINT32_T  1 
#endif

/* Define to 1 if you have the <unistd.h> header file. */
#ifndef BOOST_NET_HAVE_UNISTD_H 
#define BOOST_NET_HAVE_UNISTD_H  1 
#endif

/* Define to 1 if <sys/types.h> defines u_char */
#ifndef BOOST_NET_HAVE_U_CHAR 
#define BOOST_NET_HAVE_U_CHAR  1 
#endif

/* Define to 1 if IPv4 is supported */
/* #undef BOOST_NET_IPV4 */

/* Define to 1 if IPv6 is supported */
/* #undef BOOST_NET_IPV6 */

/* Define to 1 if kqueue is supported */
/* #undef BOOST_NET_KQUEUE */

/* Define to 1 if Multicasting is supported */
#ifndef BOOST_NET_MCAST 
#define BOOST_NET_MCAST  1 
#endif

/* Define to 1 if mkfifo is supported */
#ifndef BOOST_NET_MKFIFO 
#define BOOST_NET_MKFIFO  1 
#endif

/* Name of package */
#ifndef BOOST_NET_PACKAGE 
#define BOOST_NET_PACKAGE  "net" 
#endif

/* Define to the address where bug reports for this package should be sent. */
#ifndef BOOST_NET_PACKAGE_BUGREPORT 
#define BOOST_NET_PACKAGE_BUGREPORT  "" 
#endif

/* Define to the full name of this package. */
#ifndef BOOST_NET_PACKAGE_NAME 
#define BOOST_NET_PACKAGE_NAME  "" 
#endif

/* Define to the full name and version of this package. */
#ifndef BOOST_NET_PACKAGE_STRING 
#define BOOST_NET_PACKAGE_STRING  "" 
#endif

/* Define to the one symbol short name of this package. */
#ifndef BOOST_NET_PACKAGE_TARNAME 
#define BOOST_NET_PACKAGE_TARNAME  "" 
#endif

/* Define to the version of this package. */
#ifndef BOOST_NET_PACKAGE_VERSION 
#define BOOST_NET_PACKAGE_VERSION  "" 
#endif

/* Define to 1 if SCTP is supported */
#ifndef BOOST_NET_SCTP 
#define BOOST_NET_SCTP  1 
#endif

/* Define to 1 if sockaddr_in.sin_len is supported */
/* #undef BOOST_NET_SIN4_LEN */

/* Define to 1 if sockaddr_in6.sin6_len is supported */
/* #undef BOOST_NET_SIN6_LEN */

/* Define to 1 if you have the ANSI C header files. */
#ifndef BOOST_NET_STDC_HEADERS 
#define BOOST_NET_STDC_HEADERS  1 
#endif

/* Define to 1 if UNIXDOMAIN is supported */
#ifndef BOOST_NET_UNIXDOMAIN 
#define BOOST_NET_UNIXDOMAIN  1 
#endif

/* Version number of package */
#ifndef BOOST_NET_VERSION 
#define BOOST_NET_VERSION  "0.1" 
#endif
 
/* ____SRC_BOOST_NET_DETAIL_CONFIG_HPP */
#endif
