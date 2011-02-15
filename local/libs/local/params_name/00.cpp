       
namespace std {
}
typedef int ptrdiff_t;
typedef unsigned int size_t;
namespace std {
  using ::ptrdiff_t;
  using ::size_t;
}
       
namespace std {
  namespace rel_ops
  {
    template <class _Tp>
      inline bool
      operator!=(const _Tp& __x, const _Tp& __y)
      { return !(__x == __y); }
    template <class _Tp>
      inline bool
      operator>(const _Tp& __x, const _Tp& __y)
      { return __y < __x; }
    template <class _Tp>
      inline bool
      operator<=(const _Tp& __x, const _Tp& __y)
      { return !(__y < __x); }
    template <class _Tp>
      inline bool
      operator>=(const _Tp& __x, const _Tp& __y)
      { return !(__x < __y); }
  }
}
       
namespace std {
  template<typename _Tp>
    inline void
    swap(_Tp& __a, _Tp& __b)
    {
     
      _Tp __tmp = (__a);
      __a = (__b);
      __b = (__tmp);
    }
}
namespace std {
  template<class _T1, class _T2>
    struct pair
    {
      typedef _T1 first_type;
      typedef _T2 second_type;
      _T1 first;
      _T2 second;
      pair()
      : first(), second() { }
      pair(const _T1& __a, const _T2& __b)
      : first(__a), second(__b) { }
      template<class _U1, class _U2>
        pair(const pair<_U1, _U2>& __p)
 : first(__p.first),
   second(__p.second) { }
    };
  template<class _T1, class _T2>
    inline bool
    operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return __x.first == __y.first && __x.second == __y.second; }
  template<class _T1, class _T2>
    inline bool
    operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return __x.first < __y.first
      || (!(__y.first < __x.first) && __x.second < __y.second); }
  template<class _T1, class _T2>
    inline bool
    operator!=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return !(__x == __y); }
  template<class _T1, class _T2>
    inline bool
    operator>(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return __y < __x; }
  template<class _T1, class _T2>
    inline bool
    operator<=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return !(__y < __x); }
  template<class _T1, class _T2>
    inline bool
    operator>=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return !(__x < __y); }
  template<class _T1, class _T2>
    inline pair<_T1, _T2>
    make_pair(_T1 __x, _T2 __y)
    { return pair<_T1, _T2>(__x, __y); }
}
extern "C" {
extern "C" {
}
extern "C" {
}
extern "C" {
typedef signed char __int8_t ;
typedef unsigned char __uint8_t ;
typedef signed short __int16_t;
typedef unsigned short __uint16_t;
typedef __int16_t __int_least16_t;
typedef __uint16_t __uint_least16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;
typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;
typedef signed long long __int64_t;
typedef unsigned long long __uint64_t;
}
typedef void *_LOCK_T;
extern "C"
{
void __cygwin_lock_init(_LOCK_T *);
void __cygwin_lock_init_recursive(_LOCK_T *);
void __cygwin_lock_fini(_LOCK_T *);
void __cygwin_lock_lock(_LOCK_T *);
int __cygwin_lock_trylock(_LOCK_T *);
void __cygwin_lock_unlock(_LOCK_T *);
}
typedef long _off_t;
typedef short __dev_t;
typedef unsigned short __uid_t;
typedef unsigned short __gid_t;
__extension__ typedef long long _off64_t;
typedef long _fpos_t;
typedef _off64_t _fpos64_t;
typedef int _ssize_t;
typedef unsigned int wint_t;
typedef struct
{
  int __count;
  union
  {
    wint_t __wch;
    unsigned char __wchb[4];
  } __value;
} _mbstate_t;
typedef _LOCK_T _flock_t;
typedef void *_iconv_t;
typedef long int __off_t;
typedef int __pid_t;
__extension__ typedef long long int __loff_t;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long clock_t;
typedef long time_t;
struct timespec {
  time_t tv_sec;
  long tv_nsec;
};
struct itimerspec {
  struct timespec it_interval;
  struct timespec it_value;
};
typedef long daddr_t;
typedef char * caddr_t;
typedef int pid_t;
typedef _ssize_t ssize_t;
typedef unsigned short nlink_t;
typedef long fd_mask;
typedef struct _types_fd_set {
 fd_mask fds_bits[(((64)+(((sizeof (fd_mask) * 8))-1))/((sizeof (fd_mask) * 8)))];
} _types_fd_set;
typedef unsigned long clockid_t;
typedef unsigned long timer_t;
typedef unsigned long useconds_t;
typedef long suseconds_t;
extern "C"
{
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int_least8_t;
typedef short int_least16_t;
typedef int int_least32_t;
typedef long long int_least64_t;
typedef unsigned char uint_least8_t;
typedef unsigned short uint_least16_t;
typedef unsigned int uint_least32_t;
typedef unsigned long long uint_least64_t;
typedef signed char int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
typedef long long int_fast64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef unsigned long long uint_fast64_t;
typedef int intptr_t;
typedef unsigned int uintptr_t;
typedef long long intmax_t;
typedef unsigned long long uintmax_t;
typedef struct timespec timespec_t;
typedef struct timespec timestruc_t;
typedef _off64_t off_t;
typedef __loff_t loff_t;
typedef short __dev16_t;
typedef unsigned long __dev32_t;
typedef __dev32_t dev_t;
typedef long blksize_t;
typedef long __blkcnt32_t;
typedef long long __blkcnt64_t;
typedef __blkcnt64_t blkcnt_t;
typedef unsigned long fsblkcnt_t;
typedef unsigned long fsfilcnt_t;
typedef unsigned short __uid16_t;
typedef unsigned long __uid32_t;
typedef __uid32_t uid_t;
typedef unsigned short __gid16_t;
typedef unsigned long __gid32_t;
typedef __gid32_t gid_t;
typedef unsigned long __ino32_t;
typedef unsigned long long __ino64_t;
typedef __ino64_t ino_t;
typedef unsigned long id_t;
struct flock {
 short l_type;
 short l_whence;
 off_t l_start;
 off_t l_len;
 pid_t l_pid;
};
typedef long long key_t;
typedef unsigned long vm_offset_t;
typedef unsigned long vm_size_t;
typedef void *vm_object_t;
typedef unsigned char u_int8_t;
typedef __uint16_t u_int16_t;
typedef __uint32_t u_int32_t;
typedef __uint64_t u_int64_t;
typedef __int32_t register_t;
typedef char *addr_t;
typedef unsigned mode_t;
typedef struct __pthread_t {char __dummy;} *pthread_t;
typedef struct __pthread_mutex_t {char __dummy;} *pthread_mutex_t;
typedef struct __pthread_key_t {char __dummy;} *pthread_key_t;
typedef struct __pthread_attr_t {char __dummy;} *pthread_attr_t;
typedef struct __pthread_mutexattr_t {char __dummy;} *pthread_mutexattr_t;
typedef struct __pthread_condattr_t {char __dummy;} *pthread_condattr_t;
typedef struct __pthread_cond_t {char __dummy;} *pthread_cond_t;
typedef struct
{
  pthread_mutex_t mutex;
  int state;
}
pthread_once_t;
typedef struct __pthread_rwlock_t {char __dummy;} *pthread_rwlock_t;
typedef struct __pthread_rwlockattr_t {char __dummy;} *pthread_rwlockattr_t;
}
extern char **environ;
void __attribute__((__cdecl__)) _exit (int __status ) __attribute__ ((noreturn));
int __attribute__((__cdecl__)) access (const char *__path, int __amode );
unsigned __attribute__((__cdecl__)) alarm (unsigned __secs );
int __attribute__((__cdecl__)) chdir (const char *__path );
int __attribute__((__cdecl__)) chmod (const char *__path, mode_t __mode );
int __attribute__((__cdecl__)) chown (const char *__path, uid_t __owner, gid_t __group );
int __attribute__((__cdecl__)) chroot (const char *__path );
int __attribute__((__cdecl__)) close (int __fildes );
size_t __attribute__((__cdecl__)) confstr (int __name, char *__buf, size_t __len);
char * __attribute__((__cdecl__)) ctermid (char *__s );
char * __attribute__((__cdecl__)) cuserid (char *__s );
int __attribute__((__cdecl__)) daemon (int nochdir, int noclose);
int __attribute__((__cdecl__)) dup (int __fildes );
int __attribute__((__cdecl__)) dup2 (int __fildes, int __fildes2 );
int __attribute__((__cdecl__)) dup3 (int __fildes, int __fildes2, int flags);
int __attribute__((__cdecl__)) eaccess (const char *__path, int __mode);
void __attribute__((__cdecl__)) endusershell (void);
int __attribute__((__cdecl__)) euidaccess (const char *__path, int __mode);
int __attribute__((__cdecl__)) execl (const char *__path, const char *, ... );
int __attribute__((__cdecl__)) execle (const char *__path, const char *, ... );
int __attribute__((__cdecl__)) execlp (const char *__file, const char *, ... );
int __attribute__((__cdecl__)) execv (const char *__path, char * const __argv[] );
int __attribute__((__cdecl__)) execve (const char *__path, char * const __argv[], char * const __envp[] );
int __attribute__((__cdecl__)) execvp (const char *__file, char * const __argv[] );
int __attribute__((__cdecl__)) execvpe (const char *__file, char * const __argv[], char * const __envp[] );
int __attribute__((__cdecl__)) faccessat (int __dirfd, const char *__path, int __mode, int __flags);
int __attribute__((__cdecl__)) fchdir (int __fildes);
int __attribute__((__cdecl__)) fchmod (int __fildes, mode_t __mode );
int __attribute__((__cdecl__)) fchown (int __fildes, uid_t __owner, gid_t __group );
int __attribute__((__cdecl__)) fchownat (int __dirfd, const char *__path, uid_t __owner, gid_t __group, int __flags);
int __attribute__((__cdecl__)) fexecve (int __fd, char * const __argv[], char * const __envp[] );
pid_t __attribute__((__cdecl__)) fork (void );
long __attribute__((__cdecl__)) fpathconf (int __fd, int __name );
int __attribute__((__cdecl__)) fsync (int __fd);
int __attribute__((__cdecl__)) fdatasync (int __fd);
char * __attribute__((__cdecl__)) getcwd (char *__buf, size_t __size );
int __attribute__((__cdecl__)) getdomainname (char *__name, size_t __len);
gid_t __attribute__((__cdecl__)) getegid (void );
uid_t __attribute__((__cdecl__)) geteuid (void );
gid_t __attribute__((__cdecl__)) getgid (void );
int __attribute__((__cdecl__)) getgroups (int __gidsetsize, gid_t __grouplist[] );
long __attribute__((__cdecl__)) gethostid (void);
char * __attribute__((__cdecl__)) getlogin (void );
int __attribute__((__cdecl__)) getlogin_r (char *name, size_t namesize);
char * __attribute__((__cdecl__)) getpass (const char *__prompt);
int __attribute__((__cdecl__)) getpagesize (void);
int __attribute__((__cdecl__)) getpeereid (int, uid_t *, gid_t *);
pid_t __attribute__((__cdecl__)) getpgid (pid_t);
pid_t __attribute__((__cdecl__)) getpgrp (void );
pid_t __attribute__((__cdecl__)) getpid (void );
pid_t __attribute__((__cdecl__)) getppid (void );
pid_t __attribute__((__cdecl__)) getsid (pid_t);
uid_t __attribute__((__cdecl__)) getuid (void );
char * __attribute__((__cdecl__)) getusershell (void);
char * __attribute__((__cdecl__)) getwd (char *__buf );
int __attribute__((__cdecl__)) iruserok (unsigned long raddr, int superuser, const char *ruser, const char *luser);
int __attribute__((__cdecl__)) isatty (int __fildes );
int __attribute__((__cdecl__)) lchown (const char *__path, uid_t __owner, gid_t __group );
int __attribute__((__cdecl__)) link (const char *__path1, const char *__path2 );
int __attribute__((__cdecl__)) linkat (int __dirfd1, const char *__path1, int __dirfd2, const char *__path2, int __flags );
int __attribute__((__cdecl__)) nice (int __nice_value );
off_t __attribute__((__cdecl__)) lseek (int __fildes, off_t __offset, int __whence );
int __attribute__((__cdecl__)) lockf (int __fd, int __cmd, off_t __len);
long __attribute__((__cdecl__)) pathconf (const char *__path, int __name );
int __attribute__((__cdecl__)) pause (void );
int __attribute__((__cdecl__)) pthread_atfork (void (*)(void), void (*)(void), void (*)(void));
int __attribute__((__cdecl__)) pipe (int __fildes[2] );
int __attribute__((__cdecl__)) pipe2 (int __fildes[2], int flags);
ssize_t __attribute__((__cdecl__)) pread (int __fd, void *__buf, size_t __nbytes, off_t __offset);
ssize_t __attribute__((__cdecl__)) pwrite (int __fd, const void *__buf, size_t __nbytes, off_t __offset);
_ssize_t __attribute__((__cdecl__)) read (int __fd, void *__buf, size_t __nbyte );
int __attribute__((__cdecl__)) rresvport (int *__alport);
int __attribute__((__cdecl__)) revoke (char *__path);
int __attribute__((__cdecl__)) rmdir (const char *__path );
int __attribute__((__cdecl__)) ruserok (const char *rhost, int superuser, const char *ruser, const char *luser);
void * __attribute__((__cdecl__)) sbrk (ptrdiff_t __incr);
int __attribute__((__cdecl__)) setegid (gid_t __gid );
int __attribute__((__cdecl__)) seteuid (uid_t __uid );
int __attribute__((__cdecl__)) setgid (gid_t __gid );
int __attribute__((__cdecl__)) setgroups (int ngroups, const gid_t *grouplist );
int __attribute__((__cdecl__)) setpgid (pid_t __pid, pid_t __pgid );
int __attribute__((__cdecl__)) setpgrp (void );
int __attribute__((__cdecl__)) setregid (gid_t __rgid, gid_t __egid);
int __attribute__((__cdecl__)) setreuid (uid_t __ruid, uid_t __euid);
pid_t __attribute__((__cdecl__)) setsid (void );
int __attribute__((__cdecl__)) setuid (uid_t __uid );
void __attribute__((__cdecl__)) setusershell (void);
unsigned __attribute__((__cdecl__)) sleep (unsigned int __seconds );
void __attribute__((__cdecl__)) swab (const void *, void *, ssize_t);
long __attribute__((__cdecl__)) sysconf (int __name );
pid_t __attribute__((__cdecl__)) tcgetpgrp (int __fildes );
int __attribute__((__cdecl__)) tcsetpgrp (int __fildes, pid_t __pgrp_id );
char * __attribute__((__cdecl__)) ttyname (int __fildes );
int __attribute__((__cdecl__)) ttyname_r (int, char *, size_t);
int __attribute__((__cdecl__)) unlink (const char *__path );
int __attribute__((__cdecl__)) usleep (useconds_t __useconds);
int __attribute__((__cdecl__)) vhangup (void );
_ssize_t __attribute__((__cdecl__)) write (int __fd, const void *__buf, size_t __nbyte );
extern "C" {
extern int __attribute__((dllimport)) opterr;
extern int __attribute__((dllimport)) optind;
extern int __attribute__((dllimport)) optopt;
extern int __attribute__((dllimport)) optreset;
extern char __attribute__((dllimport)) *optarg;
int getopt (int, char * const *, const char *);
}
pid_t __attribute__((__cdecl__)) vfork (void );
int __attribute__((__cdecl__)) ftruncate (int __fd, off_t __length);
int __attribute__((__cdecl__)) truncate (const char *, off_t __length);
int __attribute__((__cdecl__)) getdtablesize (void);
int __attribute__((__cdecl__)) setdtablesize (int);
useconds_t __attribute__((__cdecl__)) ualarm (useconds_t __useconds, useconds_t __interval);
 int __attribute__((__cdecl__)) gethostname (char *__name, size_t __len);
char * __attribute__((__cdecl__)) mktemp (char *);
void __attribute__((__cdecl__)) sync (void);
ssize_t __attribute__((__cdecl__)) readlink (const char *__path, char *__buf, size_t __buflen);
ssize_t __attribute__((__cdecl__)) readlinkat (int __dirfd1, const char *__path, char *__buf, size_t __buflen);
int __attribute__((__cdecl__)) symlink (const char *__name1, const char *__name2);
int __attribute__((__cdecl__)) symlinkat (const char *, int, const char *);
int __attribute__((__cdecl__)) unlinkat (int, const char *, int);
}
namespace boost{
   __extension__ typedef long long long_long_type;
   __extension__ typedef unsigned long long ulong_long_type;
}
namespace mpl_ { namespace aux {} }
namespace boost { namespace mpl { using namespace mpl_;
namespace aux { using namespace mpl_::aux; }
}}
namespace mpl_ {
template< bool C_ > struct bool_;
typedef bool_<true> true_;
typedef bool_<false> false_;
}
namespace boost { namespace mpl { using ::mpl_::bool_; } }
namespace boost { namespace mpl { using ::mpl_::true_; } }
namespace boost { namespace mpl { using ::mpl_::false_; } }
namespace mpl_ {
struct integral_c_tag { static const int value = 0; };
}
namespace boost { namespace mpl { using ::mpl_::integral_c_tag; } }
namespace mpl_ {
template< bool C_ > struct bool_
{
    static const bool value = C_;
    typedef integral_c_tag tag;
    typedef bool_ type;
    typedef bool value_type;
    operator bool() const { return this->value; }
};
template< bool C_ >
bool const bool_<C_>::value;
}
namespace boost { namespace mpl { namespace aux {
template< typename T > struct nested_type_wknd
    : T::type
{
};
}}}
namespace mpl_ {
struct void_;
}
namespace boost { namespace mpl { using ::mpl_::void_; } }
namespace mpl_ {
struct na
{
    typedef na type;
    enum { value = 0 };
};
}
namespace boost { namespace mpl { using ::mpl_::na; } }
namespace boost { namespace mpl {
template< typename T >
struct is_na
    : false_
{
};
template<>
struct is_na<na>
    : true_
{
};
template< typename T >
struct is_not_na
    : true_
{
};
template<>
struct is_not_na<na>
    : false_
{
};
template< typename T, typename U > struct if_na
{
    typedef T type;
};
template< typename U > struct if_na<na,U>
{
    typedef U type;
};
}}
namespace mpl_ {
template< int N > struct int_;
}
namespace boost { namespace mpl { using ::mpl_::int_; } }
namespace mpl_ {
template< int N >
struct int_
{
    static const int value = N;
    typedef int_ type;
    typedef int value_type;
    typedef integral_c_tag tag;
    typedef mpl_::int_< static_cast<int>((value + 1)) > next;
    typedef mpl_::int_< static_cast<int>((value - 1)) > prior;
    operator int() const { return static_cast<int>(this->value); }
};
template< int N >
int const mpl_::int_< N >::value;
}
namespace boost { namespace mpl { namespace aux {
template< typename F > struct template_arity;
}}}
namespace boost { namespace mpl {
template<
      typename T = na
    , typename Tag = void_
    , typename Arity = int_< aux::template_arity<T>::value >
    >
struct lambda;
}}
namespace boost { namespace mpl {
namespace aux {
template< long C_ >
struct not_impl
    : bool_<!C_>
{
};
}
template<
      typename T = na
    >
struct not_
    : aux::not_impl<
          ::boost::mpl::aux::nested_type_wknd<T>::value
        >
{
   
};
template<> struct not_< na > { template< typename T1 , typename T2 =na , typename T3 =na , typename T4 =na , typename T5 =na > struct apply : not_< T1 > { }; }; template< typename Tag > struct lambda< not_< na > , Tag , int_<-1> > { typedef false_ is_le; typedef not_< na > result_; typedef not_< na > type; }; namespace aux { template< typename T1 > struct template_arity< not_< T1 > > : int_<1> { }; template<> struct template_arity< not_< na > > : int_<-1> { }; }
}}
namespace boost { namespace mpl { namespace aux {
template< typename T > struct value_type_wknd
{
    typedef typename T::value_type type;
};
}}}
namespace boost { namespace mpl { namespace aux {
typedef char (&no_tag)[1];
typedef char (&yes_tag)[2];
template< bool C_ > struct yes_no_tag
{
    typedef no_tag type;
};
template<> struct yes_no_tag<true>
{
    typedef yes_tag type;
};
template< long n > struct weighted_tag
{
    typedef char (&type)[n];
};
}}}
       
namespace mpl_ {
struct failed {};
template< bool C > struct assert { typedef void* type; };
template<> struct assert<false> { typedef assert type; };
template< bool C >
int assertion_failed( typename assert<C>::type );
template< bool C >
struct assertion
{
    static int failed( assert<false> );
};
template<>
struct assertion<true>
{
    static int failed( void* );
};
struct assert_
{
    template< typename T1, typename T2 = na, typename T3 = na, typename T4 = na > struct types {};
    static assert_ const arg;
    enum relations { equal = 1, not_equal, greater, greater_equal, less, less_equal };
};
boost::mpl::aux::weighted_tag<1>::type operator==( assert_, assert_ );
boost::mpl::aux::weighted_tag<2>::type operator!=( assert_, assert_ );
boost::mpl::aux::weighted_tag<3>::type operator>( assert_, assert_ );
boost::mpl::aux::weighted_tag<4>::type operator>=( assert_, assert_ );
boost::mpl::aux::weighted_tag<5>::type operator<( assert_, assert_ );
boost::mpl::aux::weighted_tag<6>::type operator<=( assert_, assert_ );
template< assert_::relations r, long x, long y > struct assert_relation {};
template< bool > struct assert_arg_pred_impl { typedef int type; };
template<> struct assert_arg_pred_impl<true> { typedef void* type; };
template< typename P > struct assert_arg_pred
{
    typedef typename P::type p_type;
    typedef typename assert_arg_pred_impl< p_type::value >::type type;
};
template< typename P > struct assert_arg_pred_not
{
    typedef typename P::type p_type;
    enum { p = !p_type::value };
    typedef typename assert_arg_pred_impl<p>::type type;
};
template< typename Pred >
failed ************ (Pred::************
      assert_arg( void (*)(Pred), typename assert_arg_pred<Pred>::type )
    );
template< typename Pred >
failed ************ (boost::mpl::not_<Pred>::************
      assert_not_arg( void (*)(Pred), typename assert_arg_pred_not<Pred>::type )
    );
template< typename Pred >
assert<false>
assert_arg( void (*)(Pred), typename assert_arg_pred_not<Pred>::type );
template< typename Pred >
assert<false>
assert_not_arg( void (*)(Pred), typename assert_arg_pred<Pred>::type );
}
CODE(( ((BOOST_LOCAL_AUX_PP_NIL, BOOST_LOCAL_AUX_PP_NIL BOOST_PP_EMPTY)) ((int x, BOOST_PP_EMPTY)) (( double y , 1.23 BOOST_PP_EMPTY )) , (BOOST_LOCAL_AUX_PP_NIL) ( c) (& d) , 1 , (BOOST_LOCAL_AUX_PP_NIL) ( a) (& b) , 0 , BOOST_PP_EMPTY ))
CODE(( ((BOOST_LOCAL_AUX_PP_NIL, BOOST_LOCAL_AUX_PP_NIL BOOST_PP_EMPTY)) ((int x, BOOST_PP_EMPTY)) (( double y , 1.23 BOOST_PP_EMPTY )) , (BOOST_LOCAL_AUX_PP_NIL) ( c) (& d) , 1 , (BOOST_LOCAL_AUX_PP_NIL) ( a) (& b) , 0 , BOOST_PP_EMPTY ))
CODE(( ( (BOOST_LOCAL_AUX_PP_NIL, BOOST_LOCAL_AUX_PP_NIL BOOST_PP_EMPTY) ) , (BOOST_LOCAL_AUX_PP_NIL) , 0 , (BOOST_LOCAL_AUX_PP_NIL) , 0 , BOOST_PP_EMPTY ))
CODE(( ( (BOOST_LOCAL_AUX_PP_NIL, BOOST_LOCAL_AUX_PP_NIL BOOST_PP_EMPTY) ) , (BOOST_LOCAL_AUX_PP_NIL) , 0 , (BOOST_LOCAL_AUX_PP_NIL) , 0 , BOOST_PP_EMPTY ))
CODE(( ( (BOOST_LOCAL_AUX_PP_NIL, BOOST_LOCAL_AUX_PP_NIL BOOST_PP_EMPTY) ) , (BOOST_LOCAL_AUX_PP_NIL) , 0 , (BOOST_LOCAL_AUX_PP_NIL) , 0 , BOOST_PP_EMPTY ))
struct ERROR_default_value_cannot_be_specified_as_the_first_element; typedef struct ERROR_default_value_cannot_be_specified_as_the_first_element24 : boost::mpl::assert_ { static boost::mpl::failed ************ (ERROR_default_value_cannot_be_specified_as_the_first_element::************ assert_arg()) () { return 0; } } mpl_assert_arg24; enum { mpl_assertion_in_line_24 = sizeof( boost::mpl::assertion_failed<(0)>( mpl_assert_arg24::assert_arg() ) ) }
struct ERROR_default_value_cannot_be_specified_as_the_first_element; typedef struct ERROR_default_value_cannot_be_specified_as_the_first_element26 : boost::mpl::assert_ { static boost::mpl::failed ************ (ERROR_default_value_cannot_be_specified_as_the_first_element::************ assert_arg()) () { return 0; } } mpl_assert_arg26; enum { mpl_assertion_in_line_26 = sizeof( boost::mpl::assertion_failed<(0)>( mpl_assert_arg26::assert_arg() ) ) }
struct ERROR_default_value_at_element_6_must_follow_an_unbound_parameter; typedef struct ERROR_default_value_at_element_6_must_follow_an_unbound_parameter29 : boost::mpl::assert_ { static boost::mpl::failed ************ (ERROR_default_value_at_element_6_must_follow_an_unbound_parameter::************ assert_arg()) () { return 0; } } mpl_assert_arg29; enum { mpl_assertion_in_line_29 = sizeof( boost::mpl::assertion_failed<(0)>( mpl_assert_arg29::assert_arg() ) ) }
struct ERROR_default_value_at_element_6_must_follow_an_unbound_parameter; typedef struct ERROR_default_value_at_element_6_must_follow_an_unbound_parameter31 : boost::mpl::assert_ { static boost::mpl::failed ************ (ERROR_default_value_at_element_6_must_follow_an_unbound_parameter::************ assert_arg()) () { return 0; } } mpl_assert_arg31; enum { mpl_assertion_in_line_31 = sizeof( boost::mpl::assertion_failed<(0)>( mpl_assert_arg31::assert_arg() ) ) }
struct ERROR_object_this_cannot_be_bound_multiple_times; typedef struct ERROR_object_this_cannot_be_bound_multiple_times34 : boost::mpl::assert_ { static boost::mpl::failed ************ (ERROR_object_this_cannot_be_bound_multiple_times::************ assert_arg()) () { return 0; } } mpl_assert_arg34; enum { mpl_assertion_in_line_34 = sizeof( boost::mpl::assertion_failed<(0)>( mpl_assert_arg34::assert_arg() ) ) }
struct ERROR_object_this_cannot_be_bound_multiple_times; typedef struct ERROR_object_this_cannot_be_bound_multiple_times36 : boost::mpl::assert_ { static boost::mpl::failed ************ (ERROR_object_this_cannot_be_bound_multiple_times::************ assert_arg()) () { return 0; } } mpl_assert_arg36; enum { mpl_assertion_in_line_36 = sizeof( boost::mpl::assertion_failed<(0)>( mpl_assert_arg36::assert_arg() ) ) }
