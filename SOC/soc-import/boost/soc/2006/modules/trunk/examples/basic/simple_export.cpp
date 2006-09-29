// simple_export.cpp
#include "simple_export.h"
#include "simple.h"

typedef   signed   char   __int8_t ;

 typedef   unsigned   char   __uint8_t ;

 typedef      short   __int16_t ;

 typedef      unsigned   short   __uint16_t ;

 typedef   int   __int32_t ;

 typedef   unsigned   int   __uint32_t ;

 typedef   long   long   __int64_t ;

 typedef   unsigned   long   long      __uint64_t ;

 typedef   long   __darwin_intptr_t ;

 typedef   unsigned   int   __darwin_natural_t ;

 typedef   int   __darwin_ct_rune_t ;
     
 typedef   union   { 
 char   __mbstate8 [ 128 ] ;

 long   long      _mbstateL ;
  
 }
  __mbstate_t ;

 typedef   __mbstate_t   __darwin_mbstate_t ;
     
 typedef   int      __darwin_ptrdiff_t ;
     
 typedef   long   unsigned   int   __darwin_size_t ;
     
 typedef   __builtin_va_list      __darwin_va_list ;
     
 typedef   int   __darwin_wchar_t ;
     
 typedef   __darwin_wchar_t      __darwin_rune_t ;
     
 typedef   int   __darwin_wint_t ;
     
 typedef   unsigned   long   __darwin_clock_t ;
     
 typedef   __uint32_t   __darwin_socklen_t ;
     
 typedef   long   __darwin_ssize_t ;
     
 typedef   long   __darwin_time_t ;
     
 struct   mcontext ;

 struct   mcontext64 ;

 struct   __darwin_pthread_handler_rec 
 { 
 void   (   * __routine ) ( void   *   ) ;
     
 void   * __arg ;
  
 struct   __darwin_pthread_handler_rec   * __next ;

 }
;

 struct   _opaque_pthread_attr_t   {   long   __sig ;
  char   __opaque [ 36 ] ;
  }
;

 struct   _opaque_pthread_cond_t   {   long   __sig ;
  char   __opaque [ 24 ] ;
  }
;

 struct   _opaque_pthread_condattr_t   {   long   __sig ;
  char   __opaque [ 4 ] ;
  }
;

 struct   _opaque_pthread_mutex_t   {   long   __sig ;
  char   __opaque [ 40 ] ;
  }
;

 struct   _opaque_pthread_mutexattr_t   {   long   __sig ;
  char   __opaque [ 8 ] ;
  }
;

 struct   _opaque_pthread_once_t   {   long   __sig ;
  char   __opaque [ 4 ] ;
  }
;

 struct   _opaque_pthread_rwlock_t   {   long   __sig ;
  char   __opaque [ 124 ] ;
  }
;

 struct   _opaque_pthread_rwlockattr_t   {   long   __sig ;
  char   __opaque [ 12 ] ;
  }
;

 struct   _opaque_pthread_t   {   long   __sig ;
  struct   __darwin_pthread_handler_rec   * __cleanup_stack ;
  char   __opaque [ 596 ] ;
  }
;

 typedef      __int64_t      __darwin_blkcnt_t ;
     
 typedef      __int32_t      __darwin_blksize_t ;
     
 typedef   __int32_t      __darwin_dev_t ;
  
 typedef   unsigned   int      __darwin_fsblkcnt_t ;
     
 typedef   unsigned   int      __darwin_fsfilcnt_t ;
     
 typedef   __uint32_t      __darwin_gid_t ;
  
 typedef   __uint32_t      __darwin_id_t ;
  
 typedef   __uint32_t      __darwin_ino_t ;
  
 typedef   __darwin_natural_t   __darwin_mach_port_name_t ;
  
 typedef   __darwin_mach_port_name_t   __darwin_mach_port_t ;
  
 typedef   struct   mcontext   * __darwin_mcontext_t ;
     
 typedef   struct   mcontext64   * __darwin_mcontext64_t ;
  
 typedef   __uint16_t      __darwin_mode_t ;
     
 typedef   __int64_t      __darwin_off_t ;
  
 typedef   __int32_t      __darwin_pid_t ;
  
 typedef   struct   _opaque_pthread_attr_t 
 __darwin_pthread_attr_t ;
  
 typedef   struct   _opaque_pthread_cond_t 
 __darwin_pthread_cond_t ;
  
 typedef   struct   _opaque_pthread_condattr_t 
 __darwin_pthread_condattr_t ;
  
 typedef   unsigned   long      __darwin_pthread_key_t ;
     
 typedef   struct   _opaque_pthread_mutex_t 
 __darwin_pthread_mutex_t ;
  
 typedef   struct   _opaque_pthread_mutexattr_t 
 __darwin_pthread_mutexattr_t ;
  
 typedef   struct   _opaque_pthread_once_t 
 __darwin_pthread_once_t ;
  
 typedef   struct   _opaque_pthread_rwlock_t 
 __darwin_pthread_rwlock_t ;
  
 typedef   struct   _opaque_pthread_rwlockattr_t 
 __darwin_pthread_rwlockattr_t ;
  
 typedef   struct   _opaque_pthread_t 
 * __darwin_pthread_t ;
     
 typedef   __uint32_t      __darwin_sigset_t ;
     
 typedef   __int32_t      __darwin_suseconds_t ;
     
 typedef   __uint32_t      __darwin_uid_t ;
  
 typedef   __uint32_t      __darwin_useconds_t ;
     
 typedef      unsigned   char      __darwin_uuid_t [ 16 ] ;

 struct      sigaltstack 
 { 
 void      * ss_sp ;
  
 __darwin_size_t   ss_size ;
     
 int      ss_flags ;
  
 }
;

 typedef   struct   sigaltstack   __darwin_stack_t ;
     
 struct   ucontext 
 { 
 int   uc_onstack ;

 __darwin_sigset_t      uc_sigmask ;
     
 __darwin_stack_t   uc_stack ;
     
 struct   ucontext      * uc_link ;
  
 __darwin_size_t      uc_mcsize ;
  
 __darwin_mcontext_t      uc_mcontext ;
     
 }
;

 typedef   struct   ucontext   __darwin_ucontext_t ;
     
 struct   ucontext64   { 
 int   uc_onstack ;

 __darwin_sigset_t      uc_sigmask ;
     
 __darwin_stack_t   uc_stack ;
     
 struct   ucontext64   * uc_link ;
  
 __darwin_size_t      uc_mcsize ;
  
 __darwin_mcontext64_t   uc_mcontext64 ;
     
 }
;

 typedef   struct   ucontext64   __darwin_ucontext64_t ;
  
 typedef      int   __darwin_nl_item ;

 typedef      int   __darwin_wctrans_t ;

 typedef      unsigned   long      __darwin_wctype_t ;

 typedef   __darwin_va_list      va_list ;

 typedef      __darwin_size_t   size_t ;

 typedef   __darwin_off_t   fpos_t ;

 struct   __sbuf   { 
 unsigned   char      * _base ;

 int   _size ;

 }
;

 struct   __sFILEX ;

 typedef      struct   __sFILE   { 
 unsigned   char   * _p ;
     
 int      _r ;
  
 int      _w ;
  
 short      _flags ;
  
 short      _file ;
  
 struct      __sbuf   _bf ;
     
 int      _lbfsize ;
     
 void      * _cookie ;
     
 int      (   * _close ) ( void   *   ) ;

 int      (   * _read )   ( void   *   ,   char   *   ,   int ) ;

 fpos_t      (   * _seek )   ( void   *   ,   fpos_t ,   int ) ;

 int      (   * _write ) ( void   *   ,   const   char   *   ,   int ) ;

 struct      __sbuf   _ub ;
     
 struct   __sFILEX   * _extra ;
  
 int      _ur ;
  
 unsigned   char   _ubuf [ 3 ] ;
     
 unsigned   char   _nbuf [ 1 ] ;
     
 struct      __sbuf   _lb ;
     
 int      _blksize ;
     
 fpos_t      _offset ;
     
 }
  FILE ;

 extern   "C"   { 
 extern   FILE   __sF [ ] ;

 }

 extern   "C"   { 
 void   clearerr ( FILE   *   ) ;

 int   fclose ( FILE   *   ) ;

 int   feof ( FILE   *   ) ;

 int   ferror ( FILE   *   ) ;

 int   fflush ( FILE   *   ) ;

 int   fgetc ( FILE   *   ) ;

 int   fgetpos ( FILE   *   ,   fpos_t   *   ) ;

 char      * fgets ( char   *   ,   int ,   FILE   *   ) ;

 FILE      * fopen ( const   char   *   ,   const   char   *   ) ;

 int   fprintf ( FILE   *   ,   const   char   *   ,   ...   )   __asm ( "_"   "fprintf"   "$LDBLStub" ) ;

 int   fputc ( int ,   FILE   *   ) ;

 int   fputs ( const   char   *   ,   FILE   *   ) ;

 size_t   fread ( void   *   ,   size_t ,   size_t ,   FILE   *   ) ;

 FILE      * freopen ( const   char   *   ,   const   char   *   , 
 FILE   *   )   ;

 int   fscanf ( FILE   *   ,   const   char   *   ,   ...   )   __asm ( "_"   "fscanf"   "$LDBLStub" ) ;

 int   fseek ( FILE   *   ,   long ,   int ) ;

 int   fsetpos ( FILE   *   ,   const   fpos_t   *   ) ;

 long   ftell ( FILE   *   ) ;

 size_t   fwrite ( const   void   *   ,   size_t ,   size_t ,   FILE   *   )   ;

 int   getc ( FILE   *   ) ;

 int   getchar ( void ) ;

 char      * gets ( char   *   ) ;

 extern   const   int   sys_nerr ;
  
 extern   const   char   * const   sys_errlist [ ] ;

 void   perror ( const   char   *   ) ;

 int   printf ( const   char   *   ,   ...   )   __asm ( "_"   "printf"   "$LDBLStub" ) ;

 int   putc ( int ,   FILE   *   ) ;

 int   putchar ( int ) ;

 int   puts ( const   char   *   ) ;

 int   remove ( const   char   *   ) ;

 int   rename   ( const   char   *   ,   const   char   *   ) ;

 void   rewind ( FILE   *   ) ;

 int   scanf ( const   char   *   ,   ...   )   __asm ( "_"   "scanf"   "$LDBLStub" ) ;

 void   setbuf ( FILE   *   ,   char   *   ) ;

 int   setvbuf ( FILE   *   ,   char   *   ,   int ,   size_t ) ;

 int   sprintf ( char   *   ,   const   char   *   ,   ...   )   __asm ( "_"   "sprintf"   "$LDBLStub" ) ;

 int   sscanf ( const   char   *   ,   const   char   *   ,   ...   )   __asm ( "_"   "sscanf"   "$LDBLStub" ) ;

 FILE      * tmpfile ( void ) ;

 char      * tmpnam ( char   *   ) ;

 int   ungetc ( int ,   FILE   *   ) ;

 int   vfprintf ( FILE   *   ,   const   char   *   ,   va_list )   __asm ( "_"   "vfprintf"   "$LDBLStub" ) ;

 int   vprintf ( const   char   *   ,   va_list )   __asm ( "_"   "vprintf"   "$LDBLStub" ) ;

 int   vsprintf ( char   *   ,   const   char   *   ,   va_list )   __asm ( "_"   "vsprintf"   "$LDBLStub" ) ;

 int   asprintf ( char   *   *   ,   const   char   *   ,   ...   )   __asm ( "_"   "asprintf"   "$LDBLStub" ) ;

 int   vasprintf ( char   *   *   ,   const   char   *   ,   va_list )   __asm ( "_"   "vasprintf"   "$LDBLStub" ) ;

 }

 extern   "C"   { 
 char      * ctermid ( char   *   ) ;

 char      * ctermid_r ( char   *   ) ;

 FILE      * fdopen ( int ,   const   char   *   ) ;

 char      * fgetln ( FILE   *   ,   size_t   *   ) ;

 int   fileno ( FILE   *   ) ;

 void   flockfile ( FILE   *   ) ;

 const   char   
 * fmtcheck ( const   char   *   ,   const   char   *   ) ;

 int   fpurge ( FILE   *   ) ;

 int   fseeko ( FILE   *   ,   fpos_t ,   int ) ;

 fpos_t   ftello ( FILE   *   ) ;

 int   ftrylockfile ( FILE   *   ) ;

 void   funlockfile ( FILE   *   ) ;

 int   getc_unlocked ( FILE   *   ) ;

 int   getchar_unlocked ( void ) ;

 int   getw ( FILE   *   ) ;

 int   pclose ( FILE   *   ) ;

 FILE      * popen ( const   char   *   ,   const   char   *   ) ;

 int   putc_unlocked ( int ,   FILE   *   ) ;

 int   putchar_unlocked ( int ) ;

 int   putw ( int ,   FILE   *   ) ;

 void   setbuffer ( FILE   *   ,   char   *   ,   int ) ;

 int   setlinebuf ( FILE   *   ) ;

 int   snprintf ( char   *   ,   size_t ,   const   char   *   ,   ...   )   __asm ( "_"   "snprintf"   "$LDBLStub" ) ;

 char      * tempnam ( const   char   *   ,   const   char   *   ) ;

 int   vfscanf ( FILE   *   ,   const   char   *   ,   va_list )   __asm ( "_"   "vfscanf"   "$LDBLStub" ) ;

 int   vscanf ( const   char   *   ,   va_list )   __asm ( "_"   "vscanf"   "$LDBLStub" ) ;

 int   vsnprintf ( char   *   ,   size_t ,   const   char   *   ,   va_list )   __asm ( "_"   "vsnprintf"   "$LDBLStub" ) ;

 int   vsscanf ( const   char   *   ,   const   char   *   ,   va_list )   __asm ( "_"   "vsscanf"   "$LDBLStub" ) ;

 FILE      * zopen ( const   char   *   ,   const   char   *   ,   int ) ;

 }

 extern   "C"   { 
 FILE      * funopen ( const   void   *   , 
 int   (   *   ) ( void   *   ,   char   *   ,   int ) , 
 int   (   *   ) ( void   *   ,   const   char   *   ,   int ) , 
 fpos_t   (   *   ) ( void   *   ,   fpos_t ,   int ) , 
 int   (   *   ) ( void   *   ) ) ;

 }

 extern   "C"   { 
 int      __srget ( FILE   *   ) ;

 int      __svfscanf ( FILE   *   ,   const   char   *   ,   va_list )   __asm ( "_"   "__svfscanf"   "$LDBLStub" ) ;

 int      __swbuf ( int ,   FILE   *   ) ;

 }

 static   inline   int   __sputc ( int   _c ,   FILE   * _p )   { 
 if   (   -- _p -> _w   >=   0   ||   ( _p -> _w   >=   _p -> _lbfsize   &&   ( char ) _c   !=   '\n' ) ) 
 return   (   * _p -> _p ++   =   _c ) ;

 else 
 return   ( __swbuf ( _c ,   _p ) ) ;

 }

 
 
// module Foo
namespace Foo {

 
 int   main   ( int   args ,   char   *   *   argv )   { 
 printf   ( "blah\n" ) ;

 return   0 ;

 }

 
// private section
namespace _private_0 {

 void   foo   ( )   { 
 printf   ( "you can't see me.\n" ) ;

 return ;

 }

}
using namespace _private_0;

 }
;

  
