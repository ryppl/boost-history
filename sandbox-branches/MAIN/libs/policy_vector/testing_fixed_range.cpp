# 1 "testing_vector.cpp"
 


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/iostream" 1 3
 
 



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/iostream.h" 1 3
 

























#pragma interface



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/streambuf.h" 1 3
 


























#pragma interface


   



extern "C" {
# 1 "/usr/include/libio.h" 1 3
 






























# 1 "/usr/include/_G_config.h" 1 3
 





 

# 1 "/usr/include/bits/types.h" 1 3
 

















 






# 1 "/usr/include/features.h" 1 3
 




















 






























































 




















 





 



 







 
# 142 "/usr/include/features.h" 3


 









 








 



























# 208 "/usr/include/features.h" 3


































 



 


 








 




 
















 


# 1 "/usr/include/sys/cdefs.h" 1 3
 




















 




 






 





 








 



# 65 "/usr/include/sys/cdefs.h" 3


 





 




 









 







 

















 















 







 






 








 








 








 











 










 







 




 


















# 283 "/usr/include/features.h" 2 3



 








 





 

 








# 1 "/usr/include/gnu/stubs.h" 1 3
 



















# 312 "/usr/include/features.h" 2 3




# 26 "/usr/include/bits/types.h" 2 3



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


































typedef unsigned int size_t;






















 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 29 "/usr/include/bits/types.h" 2 3


 
typedef unsigned char __u_char;
typedef unsigned short __u_short;
typedef unsigned int __u_int;
typedef unsigned long __u_long;

__extension__ typedef unsigned long long int __u_quad_t;
__extension__ typedef long long int __quad_t;
# 48 "/usr/include/bits/types.h" 3

typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;

typedef __quad_t *__qaddr_t;

typedef __u_quad_t __dev_t;		 
typedef __u_int __uid_t;		 
typedef __u_int __gid_t;		 
typedef __u_long __ino_t;		 
typedef __u_int __mode_t;		 
typedef __u_int __nlink_t; 		 
typedef long int __off_t;		 
typedef __quad_t __loff_t;		 
typedef int __pid_t;			 
typedef int __ssize_t;			 
typedef __u_long __rlim_t;		 
typedef __u_quad_t __rlim64_t;		 
typedef __u_int __id_t;			 

typedef struct
  {
    int __val[2];
  } __fsid_t;				 

 
typedef int __daddr_t;			 
typedef char *__caddr_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef long int __swblk_t;		 

typedef long int __clock_t;

 
typedef int __clockid_t;

 
typedef int __timer_t;


 



typedef int __key_t;

 
typedef unsigned short int __ipc_pid_t;


 
typedef long int __blksize_t;

 

 
typedef long int __blkcnt_t;
typedef __quad_t __blkcnt64_t;

 
typedef __u_long __fsblkcnt_t;
typedef __u_quad_t __fsblkcnt64_t;

 
typedef __u_long __fsfilcnt_t;
typedef __u_quad_t __fsfilcnt64_t;

 
typedef __u_quad_t __ino64_t;

 
typedef __loff_t __off64_t;

 
typedef long int __t_scalar_t;
typedef unsigned long int __t_uscalar_t;

 
typedef int __intptr_t;

 
typedef unsigned int __socklen_t;


 

# 1 "/usr/include/bits/pthreadtypes.h" 1 3
 
 
 
 
 
 
 
 
 
 
 
 
 









# 1 "/usr/include/bits/sched.h" 1 3
 



















# 62 "/usr/include/bits/sched.h" 3





 
struct __sched_param
  {
    int __sched_priority;
  };


# 23 "/usr/include/bits/pthreadtypes.h" 2 3


typedef int __atomic_lock_t;

 
struct _pthread_fastlock
{
  long int __status;    
  __atomic_lock_t __spinlock;   

};


 
typedef struct _pthread_descr_struct *_pthread_descr;




 
typedef struct __pthread_attr_s
{
  int __detachstate;
  int __schedpolicy;
  struct __sched_param __schedparam;
  int __inheritsched;
  int __scope;
  size_t __guardsize;
  int __stackaddr_set;
  void *__stackaddr;
  size_t __stacksize;
} pthread_attr_t;


 
typedef struct
{
  struct _pthread_fastlock __c_lock;  
  _pthread_descr __c_waiting;         
} pthread_cond_t;


 
typedef struct
{
  int __dummy;
} pthread_condattr_t;

 
typedef unsigned int pthread_key_t;


 
 

typedef struct
{
  int __m_reserved;                
  int __m_count;                   
  _pthread_descr __m_owner;        
  int __m_kind;                    
  struct _pthread_fastlock __m_lock;  
} pthread_mutex_t;


 
typedef struct
{
  int __mutexkind;
} pthread_mutexattr_t;


 
typedef int pthread_once_t;


# 119 "/usr/include/bits/pthreadtypes.h" 3


# 138 "/usr/include/bits/pthreadtypes.h" 3



 
typedef unsigned long int pthread_t;


# 143 "/usr/include/bits/types.h" 2 3




# 9 "/usr/include/_G_config.h" 2 3





# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 





























 



















































typedef unsigned int  wint_t;




 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 14 "/usr/include/_G_config.h" 2 3










# 1 "/usr/include/wchar.h" 1 3
 

















 











# 46 "/usr/include/wchar.h" 3


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 190 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3














 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 48 "/usr/include/wchar.h" 2 3


# 1 "/usr/include/bits/wchar.h" 1 3
 

























# 50 "/usr/include/wchar.h" 2 3


 













 
typedef struct
{
  int __count;
  union
  {
    wint_t __wch;
    char __wchb[4];
  } __value;		 
} __mbstate_t;




 

# 682 "/usr/include/wchar.h" 3



# 24 "/usr/include/_G_config.h" 2 3


typedef struct
{
  __off_t __pos;
  __mbstate_t __state;
} _G_fpos_t;
typedef struct
{
  __off64_t __pos;
  __mbstate_t __state;
} _G_fpos64_t;








# 1 "/usr/include/gconv.h" 1 3
 

















 








# 1 "/usr/include/wchar.h" 1 3
 

















 











# 46 "/usr/include/wchar.h" 3


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 190 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3














 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 48 "/usr/include/wchar.h" 2 3




 











# 76 "/usr/include/wchar.h" 3




 

# 682 "/usr/include/wchar.h" 3



# 28 "/usr/include/gconv.h" 2 3



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 


# 269 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 31 "/usr/include/gconv.h" 2 3


 


 
enum
{
  __GCONV_OK = 0,
  __GCONV_NOCONV,
  __GCONV_NODB,
  __GCONV_NOMEM,

  __GCONV_EMPTY_INPUT,
  __GCONV_FULL_OUTPUT,
  __GCONV_ILLEGAL_INPUT,
  __GCONV_INCOMPLETE_INPUT,

  __GCONV_ILLEGAL_DESCRIPTOR,
  __GCONV_INTERNAL_ERROR
};


 
enum
{
  __GCONV_IS_LAST = 0x0001,
  __GCONV_IGNORE_ERRORS = 0x0002
};


 
struct __gconv_step;
struct __gconv_step_data;
struct __gconv_loaded_object;
struct __gconv_trans_data;


 
typedef int (*__gconv_fct) (struct __gconv_step *, struct __gconv_step_data *,
			    __const unsigned char **, __const unsigned char *,
			    unsigned char **, size_t *, int, int);

 
typedef int (*__gconv_init_fct) (struct __gconv_step *);
typedef void (*__gconv_end_fct) (struct __gconv_step *);


 
typedef int (*__gconv_trans_fct) (struct __gconv_step *,
				  struct __gconv_step_data *, void *,
				  __const unsigned char *,
				  __const unsigned char **,
				  __const unsigned char *, unsigned char **,
				  size_t *);

 
typedef int (*__gconv_trans_context_fct) (void *, __const unsigned char *,
					  __const unsigned char *,
					  unsigned char *, unsigned char *);

 
typedef int (*__gconv_trans_query_fct) (__const char *, __const char ***,
					size_t *);

 
typedef int (*__gconv_trans_init_fct) (void **, const char *);
typedef void (*__gconv_trans_end_fct) (void *);

struct __gconv_trans_data
{
   
  __gconv_trans_fct __trans_fct;
  __gconv_trans_context_fct __trans_context_fct;
  __gconv_trans_end_fct __trans_end_fct;
  void *__data;
  struct __gconv_trans_data *__next;
};


 
struct __gconv_step
{
  struct __gconv_loaded_object *__shlib_handle;
  __const char *__modname;

  int __counter;

  char *__from_name;
  char *__to_name;

  __gconv_fct __fct;
  __gconv_init_fct __init_fct;
  __gconv_end_fct __end_fct;

   

  int __min_needed_from;
  int __max_needed_from;
  int __min_needed_to;
  int __max_needed_to;

   
  int __stateful;

  void *__data;		 
};

 

struct __gconv_step_data
{
  unsigned char *__outbuf;     
  unsigned char *__outbufend;  


   
  int __flags;

   

  int __invocation_counter;

   

  int __internal_use;

  __mbstate_t *__statep;
  __mbstate_t __state;	 


   
  struct __gconv_trans_data *__trans;
};


 
typedef struct __gconv_info
{
  size_t __nsteps;
  struct __gconv_step *__steps;
  __extension__ struct __gconv_step_data __data [0] ;
} *__gconv_t;


# 44 "/usr/include/_G_config.h" 2 3

typedef union
{
  struct __gconv_info __cd;
  struct
  {
    struct __gconv_info __cd;
    struct __gconv_step_data __data;
  } __combined;
} _G_iconv_t;

typedef int _G_int16_t __attribute__ ((__mode__ (__HI__)));
typedef int _G_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int _G_uint16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int _G_uint32_t __attribute__ ((__mode__ (__SI__)));




 


















 




 














# 32 "/usr/include/libio.h" 2 3

 

















 

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h" 1 3
 
































































 






typedef void *__gnuc_va_list;



 

# 122 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h" 3




















# 209 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h" 3




# 53 "/usr/include/libio.h" 2 3







# 72 "/usr/include/libio.h" 3


 

















# 103 "/usr/include/libio.h" 3











 

























 



















struct _IO_jump_t;  struct _IO_FILE;

 







typedef void _IO_lock_t;



 

struct _IO_marker {
  struct _IO_marker *_next;
  struct _IO_FILE *_sbuf;
   

   
  int _pos;
# 192 "/usr/include/libio.h" 3

};

 
enum __codecvt_result
{
  __codecvt_ok,
  __codecvt_partial,
  __codecvt_error,
  __codecvt_noconv
};

# 259 "/usr/include/libio.h" 3


struct _IO_FILE {
  int _flags;		 


   
   
  char* _IO_read_ptr;	 
  char* _IO_read_end;	 
  char* _IO_read_base;	 
  char* _IO_write_base;	 
  char* _IO_write_ptr;	 
  char* _IO_write_end;	 
  char* _IO_buf_base;	 
  char* _IO_buf_end;	 
   
  char *_IO_save_base;  
  char *_IO_backup_base;   
  char *_IO_save_end;  

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;
  int _blksize;
  __off_t   _old_offset;  


   
  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];

   

  _IO_lock_t *_lock;








  __off64_t   _offset;





  void *__pad1;
  void *__pad2;

  int _mode;
   
  char _unused2[15 * sizeof (int) - 2 * sizeof (void *)];

};





struct _IO_FILE_plus;

extern struct _IO_FILE_plus _IO_2_1_stdin_;
extern struct _IO_FILE_plus _IO_2_1_stdout_;
extern struct _IO_FILE_plus _IO_2_1_stderr_;











 

 

typedef __ssize_t __io_read_fn (void *__cookie, char *__buf, size_t __nbytes);

 





typedef __ssize_t __io_write_fn (void *__cookie, __const char *__buf,
				 size_t __n);

 





typedef int __io_seek_fn (void *__cookie, __off64_t   *__pos, int __w);

 
typedef int __io_close_fn (void *__cookie);


# 389 "/usr/include/libio.h" 3




extern "C" {


extern int __underflow (_IO_FILE *) throw () ;
extern int __uflow (_IO_FILE *) throw () ;
extern int __overflow (_IO_FILE *, int) throw () ;
extern wint_t   __wunderflow (_IO_FILE *) throw () ;
extern wint_t   __wuflow (_IO_FILE *) throw () ;
extern wint_t   __woverflow (_IO_FILE *, wint_t  ) throw () ;
























extern int _IO_getc (_IO_FILE *__fp) throw () ;
extern int _IO_putc (int __c, _IO_FILE *__fp) throw () ;
extern int _IO_feof (_IO_FILE *__fp) throw () ;
extern int _IO_ferror (_IO_FILE *__fp) throw () ;

extern int _IO_peekc_locked (_IO_FILE *__fp) throw () ;

 



extern void _IO_flockfile (_IO_FILE *) throw () ;
extern void _IO_funlockfile (_IO_FILE *) throw () ;
extern int _IO_ftrylockfile (_IO_FILE *) throw () ;
















extern int _IO_vfscanf (_IO_FILE * __restrict, const char * __restrict,
			__gnuc_va_list , int *__restrict) throw () ;
extern int _IO_vfprintf (_IO_FILE *__restrict, const char *__restrict,
			 __gnuc_va_list ) throw () ;
extern __ssize_t   _IO_padn (_IO_FILE *, int, __ssize_t  ) throw () ;
extern size_t   _IO_sgetn (_IO_FILE *, void *, size_t  ) throw () ;

extern __off64_t   _IO_seekoff (_IO_FILE *, __off64_t  , int, int) throw () ;
extern __off64_t   _IO_seekpos (_IO_FILE *, __off64_t  , int) throw () ;

extern void _IO_free_backup_area (_IO_FILE *) throw () ;

# 511 "/usr/include/libio.h" 3



}



# 36 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/streambuf.h" 2 3

}
 

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h" 1 3
 
































































 










 



 

















void va_end (__gnuc_va_list);		 


 



 












 























 
 













# 175 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h" 3


 




 

 

 

typedef __gnuc_va_list va_list;
























# 40 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/streambuf.h" 2 3

























extern "C++" {
class istream;  
class ostream; class streambuf;

 




typedef __off64_t   streamoff;
typedef __off64_t   streampos;




typedef __ssize_t   streamsize;

typedef unsigned long __fmtflags;
typedef unsigned char __iostate;

struct _ios_fields
{  
    streambuf *_strbuf;
    ostream* _tie;
    int _width;
    __fmtflags _flags;
    wchar_t   _fill;
    __iostate _state;
    __iostate _exceptions;
    int _precision;

    void *_arrays;  
};















# 124 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/streambuf.h" 3


class ios : public _ios_fields {
  ios& operator=(ios&);   
  ios (const ios&);  
  public:
    typedef __fmtflags fmtflags;
    typedef int iostate;
    typedef int openmode;
    typedef __ssize_t   streamsize;
    enum io_state {
	goodbit = 0 ,
	eofbit = 1 ,
	failbit = 2 ,
	badbit = 4  };
    enum open_mode {
	in = 1 ,
	out = 2 ,
	ate = 4 ,
	app = 8 ,
	trunc = 16 ,
	nocreate = 32 ,
	noreplace = 64 ,
	bin = 128 ,  
	binary = 128  };
    enum seek_dir { beg, cur, end};
    typedef enum seek_dir seekdir;
     
    enum { skipws= 01 ,
	   left= 02 , right= 04 , internal= 010 ,
	   dec= 020 , oct= 040 , hex= 0100 ,
	   showbase= 0200 , showpoint= 0400 ,
	   uppercase= 01000 , showpos= 02000 ,
	   scientific= 04000 , fixed= 010000 ,
	   unitbuf= 020000 , stdio= 040000 



	   };
    enum {  
	basefield=dec+oct+hex,
	floatfield = scientific+fixed,
	adjustfield = left+right+internal
    };

# 177 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/streambuf.h" 3


    ostream* tie() const { return _tie; }
    ostream* tie(ostream* val) { ostream* save=_tie; _tie=val; return save; }

     
    wchar_t   fill() const { return _fill; }
    wchar_t   fill(wchar_t   newf)
	{wchar_t   oldf = _fill; _fill = newf; return oldf;}
    fmtflags flags() const { return _flags; }
    fmtflags flags(fmtflags new_val) {
	fmtflags old_val = _flags; _flags = new_val; return old_val; }
    int precision() const { return _precision; }
    int precision(int newp) {
	unsigned short oldp = _precision; _precision = (unsigned short)newp;
	return oldp; }
    fmtflags setf(fmtflags val) {
	fmtflags oldbits = _flags;
	_flags |= val; return oldbits; }
    fmtflags setf(fmtflags val, fmtflags mask) {
	fmtflags oldbits = _flags;
	_flags = (_flags & ~mask) | (val & mask); return oldbits; }
    fmtflags unsetf(fmtflags mask) {
	fmtflags oldbits = _flags;
	_flags &= ~mask; return oldbits; }
    int width() const { return _width; }
    int width(int val) { int save = _width; _width = val; return save; }




    void _throw_failure() const { }

    void clear(iostate state = 0) {
	_state = _strbuf ? state : state|badbit;
	if (_state & _exceptions) _throw_failure(); }
    void set(iostate flag) { _state |= flag;
	if (_state & _exceptions) _throw_failure(); }
    void setstate(iostate flag) { _state |= flag;  
	if (_state & _exceptions) _throw_failure(); }
    int good() const { return _state == 0; }
    int eof() const { return _state & ios::eofbit; }
    int fail() const { return _state & (ios::badbit|ios::failbit); }
    int bad() const { return _state & ios::badbit; }
    iostate rdstate() const { return _state; }
    operator void*() const { return fail() ? (void*)0 : (void*)(-1); }
    int operator!() const { return fail(); }
    iostate exceptions() const { return _exceptions; }
    void exceptions(iostate enable) {
	_exceptions = enable;
	if (_state & _exceptions) _throw_failure(); }

    streambuf* rdbuf() const { return _strbuf; }
    streambuf* rdbuf(streambuf *_s) {
      streambuf *_old = _strbuf; _strbuf = _s; clear (); return _old; }

    static int sync_with_stdio(int on);
    static void sync_with_stdio() { sync_with_stdio(1); }
    static fmtflags bitalloc();
    static int xalloc();
    void*& pword(int);
    void* pword(int) const;
    long& iword(int);
    long iword(int) const;









     
    class Init {
    public:
      Init () { }
    };

  protected:
    inline ios(streambuf* sb = 0, ostream* tie_to = 0);
    inline virtual ~ios();
    inline void init(streambuf* sb, ostream* tie = 0);
};




typedef ios::seek_dir _seek_dir;


 
 
 
 
 

 
 
class streammarker : private _IO_marker {
    friend class streambuf;
    void set_offset(int offset) { _pos = offset; }
  public:
    streammarker(streambuf *sb);
    ~streammarker();
    int saving() { return  1; }
    int delta(streammarker&);
    int delta();
};

struct streambuf : public _IO_FILE {  
    friend class ios;
    friend class istream;
    friend class ostream;
    friend class streammarker;
    const void *&_vtable() { return *(const void**)((_IO_FILE*)this + 1); }
  protected:
    static streambuf* _list_all;  
    _IO_FILE*& xchain() { return _chain; }
    void _un_link();
    void _link_in();
    char* gptr() const
      { return _flags  & 0x100  ? _IO_save_base : _IO_read_ptr; }
    char* pptr() const { return _IO_write_ptr; }
    char* egptr() const
      { return _flags  & 0x100  ? _IO_save_end : _IO_read_end; }
    char* epptr() const { return _IO_write_end; }
    char* pbase() const { return _IO_write_base; }
    char* eback() const
      { return _flags  & 0x100  ? _IO_save_base : _IO_read_base;}
    char* base() const { return _IO_buf_base; }
    char* ebuf() const { return _IO_buf_end; }
    int blen() const { return _IO_buf_end - _IO_buf_base; }
    void xput_char(char c) { *_IO_write_ptr++ = c; }
    int xflags() { return _flags ; }
    int xflags(int f) {int fl = _flags ; _flags  = f; return fl;}
    void xsetflags(int f) { _flags  |= f; }
    void xsetflags(int f, int mask)
      { _flags  = (_flags  & ~mask) | (f & mask); }
    void gbump(int n)
      { _flags  & 0x100  ? (_IO_save_base+=n):(_IO_read_ptr+=n);}
    void pbump(int n) { _IO_write_ptr += n; }
    void setb(char* b, char* eb, int a=0);
    void setp(char* p, char* ep)
      { _IO_write_base=_IO_write_ptr=p; _IO_write_end=ep; }
    void setg(char* eb, char* g, char *eg) {
      if (_flags  & 0x100 ) _IO_free_backup_area(this); 
      _IO_read_base = eb; _IO_read_ptr = g; _IO_read_end = eg; }
    char *shortbuf() { return _shortbuf; }

    int in_backup() { return _flags & 0x100 ; }
     
    char *Gbase() { return in_backup() ? _IO_save_base : _IO_read_base; }
     
    char *eGptr() { return in_backup() ? _IO_save_end : _IO_read_end; }
     
    char *Bbase() { return in_backup() ? _IO_read_base : _IO_save_base; }
    char *Bptr() { return _IO_backup_base; }
     
    char *eBptr() { return in_backup() ? _IO_read_end : _IO_save_end; }
    char *Nbase() { return _IO_save_base; }
    char *eNptr() { return _IO_save_end; }
    int have_backup() { return _IO_save_base != __null ; }
    int have_markers() { return _markers != __null ; }
    void free_backup_area();
    void unsave_markers();  
    int put_mode() { return _flags & 0x800 ; }
    int switch_to_get_mode();
    
    streambuf(int flags=0);
  public:
    static int flush_all();
    static void flush_all_linebuffered();  
    virtual ~streambuf();
    virtual int overflow(int c = (-1) );  
    virtual int underflow();  
    virtual int uflow();  
    virtual int pbackfail(int c);
 
    virtual streamsize xsputn(const char* s, streamsize n);
    virtual streamsize xsgetn(char* s, streamsize n);
    virtual streampos seekoff(streamoff, _seek_dir, int mode=ios::in|ios::out);
    virtual streampos seekpos(streampos pos, int mode = ios::in|ios::out);

    streampos pubseekoff(streamoff o, _seek_dir d, int mode=ios::in|ios::out)
      { return _IO_seekoff (this, o, d, mode); }
    streampos pubseekpos(streampos pos, int mode = ios::in|ios::out)
      { return _IO_seekpos (this, pos, mode); }
    streampos sseekoff(streamoff, _seek_dir, int mode=ios::in|ios::out);
    streampos sseekpos(streampos pos, int mode = ios::in|ios::out);
    virtual streambuf* setbuf(char* p, int len);
    virtual int sync();
    virtual int doallocate();

    int seekmark(streammarker& mark, int delta = 0);
    int sputbackc(char c);
    int sungetc();
    int unbuffered() { return _flags & 2  ? 1 : 0; }
    int linebuffered() { return _flags & 0x200  ? 1 : 0; }
    void unbuffered(int i)
	{ if (i) _flags |= 2 ; else _flags &= ~2 ; }
    void linebuffered(int i)
	{ if (i) _flags |= 0x200 ; else _flags &= ~0x200 ; }
    int allocate() {  
	if (base() || unbuffered()) return 0;
	else return doallocate(); }
     
    void allocbuf() { if (base() == __null ) doallocbuf(); }
    void doallocbuf();
    int in_avail() { return _IO_read_end - _IO_read_ptr; }
    int out_waiting() { return _IO_write_ptr - _IO_write_base; }
    streamsize sputn(const char* s, streamsize n) { return xsputn(s, n); }
    streamsize padn(char pad, streamsize n) { return _IO_padn(this, pad, n); }
    streamsize sgetn(char* s, streamsize n) { return _IO_sgetn(this, s, n); }
    int ignore(int);
    int get_column();
    int set_column(int);
    long sgetline(char* buf, size_t   n, char delim, int putback_delim);
    int sputc(int c) { return _IO_putc(c, this); }
    int sbumpc() { return _IO_getc(this); }
    int sgetc() { return ((  this  )->_IO_read_ptr >= (  this  )->_IO_read_end && __underflow (  this  ) == (-1)  ? (-1)  : *(unsigned char *) (  this  )->_IO_read_ptr)  ; }
    int snextc() {
	if (_IO_read_ptr >= _IO_read_end && __underflow(this) == (-1) )
	  return (-1) ;
	else return _IO_read_ptr++, sgetc(); }
    void stossc() { if (_IO_read_ptr < _IO_read_end) _IO_read_ptr++; }
    int vscan(char const *fmt0, __gnuc_va_list  ap, ios* stream = __null );
    int scan(char const *fmt0 ...);
    int vform(char const *fmt0, __gnuc_va_list  ap);
    int form(char const *fmt0 ...);




    virtual streamsize sys_read(char* buf, streamsize size);
    virtual streamsize sys_write(const char*, streamsize);
    virtual streampos sys_seek(streamoff, _seek_dir);
    virtual int sys_close();
    virtual int sys_stat(void*);  

    virtual int showmanyc();
    virtual void imbue(void *);

};

 
 

class filebuf : public streambuf {
  protected:
    void init();
  public:
    static const int openprot;  
    filebuf();
    filebuf(int fd);
    filebuf(int fd, char* p, int len);



    ~filebuf();
    filebuf* attach(int fd);
    filebuf* open(const char *filename, const char *mode);
    filebuf* open(const char *filename, ios::openmode mode, int prot = 0664);
    virtual int underflow();
    virtual int overflow(int c = (-1) );
    int is_open() const { return _fileno >= 0; }
    int fd() const { return is_open() ? _fileno : (-1) ; }
    filebuf* close();
    virtual int doallocate();
    virtual streampos seekoff(streamoff, _seek_dir, int mode=ios::in|ios::out);
    virtual streambuf* setbuf(char* p, int len);
    streamsize xsputn(const char* s, streamsize n);
    streamsize xsgetn(char* s, streamsize n);
    virtual int sync();
  protected:  
 
    int is_reading() { return eback() != egptr(); }
    char* cur_ptr() { return is_reading() ?  gptr() : pptr(); }
     
    char* file_ptr() { return eGptr(); }
     
    virtual streamsize sys_read(char* buf, streamsize size);
    virtual streampos sys_seek(streamoff, _seek_dir);
    virtual streamsize sys_write(const char*, streamsize);
    virtual int sys_stat(void*);  
    virtual int sys_close();




};

inline void ios::init(streambuf* sb, ostream* tie_to) {
		_state = sb ? ios::goodbit : ios::badbit; _exceptions=0;
		_strbuf=sb; _tie = tie_to; _width=0; _fill=' ';

		_flags=ios::skipws|ios::dec;



		_precision=6; _arrays = 0; }

inline ios::ios(streambuf* sb, ostream* tie_to) { init(sb, tie_to); }

inline ios::~ios() {



     
     
    operator delete[] (_arrays);
}
}  

# 31 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/iostream.h" 2 3


extern "C++" {
class istream; class ostream;
typedef ios& (*__manip)(ios&);
typedef istream& (*__imanip)(istream&);
typedef ostream& (*__omanip)(ostream&);

extern istream& ws(istream& ins);
extern ostream& flush(ostream& outs);
extern ostream& endl(ostream& outs);
extern ostream& ends(ostream& outs);

class ostream : virtual public ios
{
     
    void do_osfx();
  public:
    ostream() { }
    ostream(streambuf* sb, ostream* tied= __null );
    int opfx() {
	if (!good()) return 0;
	else { if (_tie) _tie->flush();  ; return 1;} }
    void osfx() {  ;
		  if (flags() & (ios::unitbuf|ios::stdio))
		      do_osfx(); }
    ostream& flush();
    ostream& put(char c) { _strbuf->sputc(c); return *this; }





    ostream& write(const char *s, streamsize n);
    ostream& write(const unsigned char *s, streamsize n)
      { return write((const char*)s, n);}
    ostream& write(const signed char *s, streamsize n)
      { return write((const char*)s, n);}
    ostream& write(const void *s, streamsize n)
      { return write((const char*)s, n);}
    ostream& seekp(streampos);
    ostream& seekp(streamoff, _seek_dir);
    streampos tellp();
    ostream& form(const char *format ...);
    ostream& vform(const char *format, __gnuc_va_list  args);

    ostream& operator<<(char c);
    ostream& operator<<(unsigned char c) { return (*this) << (char)c; }
    ostream& operator<<(signed char c) { return (*this) << (char)c; }
    ostream& operator<<(const char *s);
    ostream& operator<<(const unsigned char *s)
	{ return (*this) << (const char*)s; }
    ostream& operator<<(const signed char *s)
	{ return (*this) << (const char*)s; }
    ostream& operator<<(const void *p);
    ostream& operator<<(int n);
    ostream& operator<<(unsigned int n);
    ostream& operator<<(long n);
    ostream& operator<<(unsigned long n);

    __extension__ ostream& operator<<(long long n);
    __extension__ ostream& operator<<(unsigned long long n);

    ostream& operator<<(short n) {return operator<<((int)n);}
    ostream& operator<<(unsigned short n) {return operator<<((unsigned int)n);}

    ostream& operator<<(bool b) { return operator<<((int)b); }

    ostream& operator<<(double n);
    ostream& operator<<(float n) { return operator<<((double)n); }

    ostream& operator<<(long double n);



    ostream& operator<<(__omanip func) { return (*func)(*this); }
    ostream& operator<<(__manip func) {(*func)(*this); return *this;}
    ostream& operator<<(streambuf*);



};

class istream : virtual public ios
{
     
protected:
    size_t   _gcount;

    int _skip_ws();
  public:
    istream(): _gcount (0) { }
    istream(streambuf* sb, ostream*tied= __null );
    istream& get(char* ptr, int len, char delim = '\n');
    istream& get(unsigned char* ptr, int len, char delim = '\n')
	{ return get((char*)ptr, len, delim); }
    istream& get(char& c);
    istream& get(unsigned char& c) { return get((char&)c); }
    istream& getline(char* ptr, int len, char delim = '\n');
    istream& getline(unsigned char* ptr, int len, char delim = '\n')
	{ return getline((char*)ptr, len, delim); }
    istream& get(signed char& c)  { return get((char&)c); }
    istream& get(signed char* ptr, int len, char delim = '\n')
	{ return get((char*)ptr, len, delim); }
    istream& getline(signed char* ptr, int len, char delim = '\n')
	{ return getline((char*)ptr, len, delim); }
    istream& read(char *ptr, streamsize n);
    istream& read(unsigned char *ptr, streamsize n)
      { return read((char*)ptr, n); }
    istream& read(signed char *ptr, streamsize n)
      { return read((char*)ptr, n); }
    istream& read(void *ptr, streamsize n)
      { return read((char*)ptr, n); }
    istream& get(streambuf& sb, char delim = '\n');
    istream& gets(char **s, char delim = '\n');
    int ipfx(int need = 0) {
	if (!good()) { set(ios::failbit); return 0; }
	else {
	   ;
	  if (_tie && (need == 0 || rdbuf()->in_avail() < need)) _tie->flush();
	  if (!need && (flags() & ios::skipws)) return _skip_ws();
	  else return 1;
	}
    }
    int ipfx0() {  
	if (!good()) { set(ios::failbit); return 0; }
	else {
	   ;
	  if (_tie) _tie->flush();
	  if (flags() & ios::skipws) return _skip_ws();
	  else return 1;
	}
    }
    int ipfx1() {  
	if (!good()) { set(ios::failbit); return 0; }
	else {
	   ;
	  if (_tie && rdbuf()->in_avail() == 0) _tie->flush();
	  return 1;
	}
    }
    void isfx() {  ; }
    int get() { if (!ipfx1()) return (-1) ;
		else { int ch = _strbuf->sbumpc();
		       if (ch == (-1) ) set(ios::eofbit);
		       isfx();
		       return ch;
		     } }
    int peek();
    size_t   gcount() { return _gcount; }
    istream& ignore(int n=1, int delim = (-1) );
    int sync ();
    istream& seekg(streampos);
    istream& seekg(streamoff, _seek_dir);
    streampos tellg();
    istream& putback(char ch) {
	if (good() && _strbuf->sputbackc(ch) == (-1) ) clear(ios::badbit);
	return *this;}
    istream& unget() {
	if (good() && _strbuf->sungetc() == (-1) ) clear(ios::badbit);
	return *this;}
    istream& scan(const char *format ...);
    istream& vscan(const char *format, __gnuc_va_list  args);






    istream& operator>>(char*);
    istream& operator>>(unsigned char* p) { return operator>>((char*)p); }
    istream& operator>>(signed char*p) { return operator>>((char*)p); }
    istream& operator>>(char& c);
    istream& operator>>(unsigned char& c) {return operator>>((char&)c);}
    istream& operator>>(signed char& c) {return operator>>((char&)c);}
    istream& operator>>(int&);
    istream& operator>>(long&);

    __extension__ istream& operator>>(long long&);
    __extension__ istream& operator>>(unsigned long long&);

    istream& operator>>(short&);
    istream& operator>>(unsigned int&);
    istream& operator>>(unsigned long&);
    istream& operator>>(unsigned short&);

    istream& operator>>(bool&);

    istream& operator>>(float&);
    istream& operator>>(double&);
    istream& operator>>(long double&);
    istream& operator>>( __manip func) {(*func)(*this); return *this;}
    istream& operator>>(__imanip func) { return (*func)(*this); }
    istream& operator>>(streambuf*);
};

class iostream : public istream, public ostream
{
  public:
    iostream() { }
    iostream(streambuf* sb, ostream*tied= __null );
};

class _IO_istream_withassign : public istream {
public:
  _IO_istream_withassign& operator=(istream&);
  _IO_istream_withassign& operator=(_IO_istream_withassign& rhs)
    { return operator= (static_cast<istream&> (rhs)); }
};

class _IO_ostream_withassign : public ostream {
public:
  _IO_ostream_withassign& operator=(ostream&);
  _IO_ostream_withassign& operator=(_IO_ostream_withassign& rhs)
    { return operator= (static_cast<ostream&> (rhs)); }
};

extern _IO_istream_withassign cin;
 
extern _IO_ostream_withassign cout, cerr;

extern _IO_ostream_withassign clog



;

extern istream& lock(istream& ins);
extern istream& unlock(istream& ins);
extern ostream& lock(ostream& outs);
extern ostream& unlock(ostream& outs);

struct Iostream_init { } ;   

inline ios& dec(ios& i)
{ i.setf(ios::dec, ios::dec|ios::hex|ios::oct); return i; }
inline ios& hex(ios& i)
{ i.setf(ios::hex, ios::dec|ios::hex|ios::oct); return i; }
inline ios& oct(ios& i)
{ i.setf(ios::oct, ios::dec|ios::hex|ios::oct); return i; }
}  


# 6 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/iostream" 2 3


# 4 "testing_vector.cpp" 2

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/vector" 1 3
 




























# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algobase.h" 1 3
 

























 








# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_config.h" 1 3
 




























 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
 
 
 
 


 

 
 
 
 
 
 
 








# 148 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_config.h" 3





















      








































# 234 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_config.h" 3


# 248 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_config.h" 3



































 
 
 
 


















 
 
# 316 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_config.h" 3






































 
 
 
# 36 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algobase.h" 2 3



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_relops.h" 1 3
 

























 






 

template <class _Tp>
inline bool operator!=(const _Tp& __x, const _Tp& __y) {
  return !(__x == __y);
}

template <class _Tp>
inline bool operator>(const _Tp& __x, const _Tp& __y) {
  return __y < __x;
}

template <class _Tp>
inline bool operator<=(const _Tp& __x, const _Tp& __y) {
  return !(__y < __x);
}

template <class _Tp>
inline bool operator>=(const _Tp& __x, const _Tp& __y) {
  return !(__x < __y);
}

 



 
 
 
# 39 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algobase.h" 2 3



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_pair.h" 1 3
 

























 






 

template <class _T1, class _T2>
struct pair {
  typedef _T1 first_type;
  typedef _T2 second_type;

  _T1 first;
  _T2 second;
  pair() : first(_T1()), second(_T2()) {}
  pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}


  template <class _U1, class _U2>
  pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}

};

template <class _T1, class _T2>
inline bool operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{ 
  return __x.first == __y.first && __x.second == __y.second; 
}

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{ 
  return __x.first < __y.first || 
         (!(__y.first < __x.first) && __x.second < __y.second); 
}

template <class _T1, class _T2>
inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y)
{
  return pair<_T1, _T2>(__x, __y);
}

 



 
 
 
# 42 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algobase.h" 2 3



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/type_traits.h" 1 3
 




















 
































struct __true_type {
};

struct __false_type {
};

template <class _Tp>
struct __type_traits { 
   typedef __true_type     this_dummy_member_must_be_first;
                    





    








 

   typedef __false_type    has_trivial_default_constructor;
   typedef __false_type    has_trivial_copy_constructor;
   typedef __false_type    has_trivial_assignment_operator;
   typedef __false_type    has_trivial_destructor;
   typedef __false_type    is_POD_type;
};



 
 
 



template<>  struct __type_traits<bool> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};



template<>  struct __type_traits<char> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<>  struct __type_traits<signed char> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<>  struct __type_traits<unsigned char> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};



template<>  struct __type_traits<wchar_t> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};



template<>  struct __type_traits<short> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<>  struct __type_traits<unsigned short> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<>  struct __type_traits<int> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<>  struct __type_traits<unsigned int> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<>  struct __type_traits<long> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<>  struct __type_traits<unsigned long> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};



template<>  struct __type_traits<long long> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<>  struct __type_traits<unsigned long long> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};



template<>  struct __type_traits<float> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<>  struct __type_traits<double> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<>  struct __type_traits<long double> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};



template <class _Tp>
struct __type_traits<_Tp*> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

# 295 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/type_traits.h" 3



 
 

template <class _Tp> struct _Is_integer {
  typedef __false_type _Integral;
};



template<>  struct _Is_integer<bool> {
  typedef __true_type _Integral;
};



template<>  struct _Is_integer<char> {
  typedef __true_type _Integral;
};

template<>  struct _Is_integer<signed char> {
  typedef __true_type _Integral;
};

template<>  struct _Is_integer<unsigned char> {
  typedef __true_type _Integral;
};



template<>  struct _Is_integer<wchar_t> {
  typedef __true_type _Integral;
};



template<>  struct _Is_integer<short> {
  typedef __true_type _Integral;
};

template<>  struct _Is_integer<unsigned short> {
  typedef __true_type _Integral;
};

template<>  struct _Is_integer<int> {
  typedef __true_type _Integral;
};

template<>  struct _Is_integer<unsigned int> {
  typedef __true_type _Integral;
};

template<>  struct _Is_integer<long> {
  typedef __true_type _Integral;
};

template<>  struct _Is_integer<unsigned long> {
  typedef __true_type _Integral;
};



template<>  struct _Is_integer<long long> {
  typedef __true_type _Integral;
};

template<>  struct _Is_integer<unsigned long long> {
  typedef __true_type _Integral;
};





 
 
 
# 45 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algobase.h" 2 3



# 1 "/usr/include/string.h" 1 3
 

















 








extern "C" { 

 


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 33 "/usr/include/string.h" 2 3



 
extern void *memcpy (void *__restrict __dest,
		     __const void *__restrict __src, size_t __n) throw () ;
 

extern void *memmove (void *__dest, __const void *__src, size_t __n)
     throw () ;

 



extern void *memccpy (void *__restrict __dest, __const void *__restrict __src,
		      int __c, size_t __n)
     throw () ;



 
extern void *memset (void *__s, int __c, size_t __n) throw () ;

 
extern int memcmp (__const void *__s1, __const void *__s2, size_t __n)
     throw ()   ;

 
extern void *memchr (__const void *__s, int __c, size_t __n)
      throw ()   ;

# 73 "/usr/include/string.h" 3



 
extern char *strcpy (char *__restrict __dest, __const char *__restrict __src)
     throw () ;
 
extern char *strncpy (char *__restrict __dest,
		      __const char *__restrict __src, size_t __n) throw () ;

 
extern char *strcat (char *__restrict __dest, __const char *__restrict __src)
     throw () ;
 
extern char *strncat (char *__restrict __dest, __const char *__restrict __src,
		      size_t __n) throw () ;

 
extern int strcmp (__const char *__s1, __const char *__s2)
     throw ()   ;
 
extern int strncmp (__const char *__s1, __const char *__s2, size_t __n)
     throw ()   ;

 
extern int strcoll (__const char *__s1, __const char *__s2)
     throw ()   ;
 
extern size_t strxfrm (char *__restrict __dest,
		       __const char *__restrict __src, size_t __n) throw () ;

# 116 "/usr/include/string.h" 3



 
extern char *strdup (__const char *__s) throw ()   ;


 







# 152 "/usr/include/string.h" 3


 
extern char *strchr (__const char *__s, int __c) throw ()   ;
 
extern char *strrchr (__const char *__s, int __c) throw ()   ;







 

extern size_t strcspn (__const char *__s, __const char *__reject)
     throw ()   ;
 

extern size_t strspn (__const char *__s, __const char *__accept)
     throw ()   ;
 
extern char *strpbrk (__const char *__s, __const char *__accept)
     throw ()   ;
 
extern char *strstr (__const char *__haystack, __const char *__needle)
     throw ()   ;







 
extern char *strtok (char *__restrict __s, __const char *__restrict __delim)
     throw () ;

 

extern char *__strtok_r (char *__restrict __s,
			 __const char *__restrict __delim,
			 char **__restrict __save_ptr) throw () ;

extern char *strtok_r (char *__restrict __s, __const char *__restrict __delim,
		       char **__restrict __save_ptr) throw () ;


# 214 "/usr/include/string.h" 3



 
extern size_t strlen (__const char *__s) throw ()   ;









 
extern char *strerror (int __errnum) throw () ;

 

extern char *strerror_r (int __errnum, char *__buf, size_t __buflen) throw () ;


 

extern void __bzero (void *__s, size_t __n) throw () ;


 
extern void bcopy (__const void *__src, void *__dest, size_t __n) throw () ;

 
extern void bzero (void *__s, size_t __n) throw () ;

 
extern int bcmp (__const void *__s1, __const void *__s2, size_t __n)
     throw ()   ;

 
extern char *index (__const char *__s, int __c) throw ()   ;

 
extern char *rindex (__const char *__s, int __c) throw ()   ;

 

extern int ffs (int __i) throw ()  __attribute__ ((__const__));

 









 
extern int strcasecmp (__const char *__s1, __const char *__s2)
     throw ()   ;

 
extern int strncasecmp (__const char *__s1, __const char *__s2, size_t __n)
     throw ()   ;


# 289 "/usr/include/string.h" 3



 

extern char *strsep (char **__restrict __stringp,
		     __const char *__restrict __delim) throw () ;


# 332 "/usr/include/string.h" 3




# 361 "/usr/include/string.h" 3



} 


# 48 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algobase.h" 2 3

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 1 3
 


 





 
# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/syslimits.h" 1 3
 





# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 1 3
 


 

# 114 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 3



# 1 "/usr/include/limits.h" 1 3
 

















 









 





 

# 113 "/usr/include/limits.h" 3




  




# 140 "/usr/include/limits.h" 3



 
# 1 "/usr/include/bits/posix1_lim.h" 1 3
 

















 









 

 


 


 


 


 


 


 


 



 


 


 


 


 



 


 


 


 


 


 


 


 


 


 


 


 



 


 


 


 


 



 
# 1 "/usr/include/bits/local_lim.h" 1 3
 


















 














 
# 1 "/usr/include/linux/limits.h" 1 3



















# 36 "/usr/include/bits/local_lim.h" 2 3


 




 




 





 

 


 

 


 

 


 



 


 

# 126 "/usr/include/bits/posix1_lim.h" 2 3








 







# 144 "/usr/include/limits.h" 2 3




# 1 "/usr/include/bits/posix2_lim.h" 1 3
 

















 







 


 


 


 


 



 



 


 



 




 




























 



# 148 "/usr/include/limits.h" 2 3






# 117 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 2 3




# 7 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/syslimits.h" 2 3


# 11 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 2 3





 



 



 




 





 



 












 

 




 



 








 



 













 



# 107 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 3




 









# 49 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algobase.h" 2 3

# 1 "/usr/include/stdlib.h" 1 3
 

















 







 





# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 


# 269 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 33 "/usr/include/stdlib.h" 2 3


extern "C" { 




# 91 "/usr/include/stdlib.h" 3


 
typedef struct
  {
    int quot;			 
    int rem;			 
  } div_t;

 

typedef struct
  {
    long int quot;		 
    long int rem;		 
  } ldiv_t;



# 118 "/usr/include/stdlib.h" 3



 



 





 

extern size_t __ctype_get_mb_cur_max (void) throw () ;


 
extern double atof (__const char *__nptr) throw ()   ;
 
extern int atoi (__const char *__nptr) throw ()   ;
 
extern long int atol (__const char *__nptr) throw ()   ;


 
__extension__ extern long long int atoll (__const char *__nptr)
     throw ()   ;


 
extern double strtod (__const char *__restrict __nptr,
		      char **__restrict __endptr) throw () ;










 
extern long int strtol (__const char *__restrict __nptr,
			char **__restrict __endptr, int __base) throw () ;
 
extern unsigned long int strtoul (__const char *__restrict __nptr,
				  char **__restrict __endptr, int __base)
     throw () ;


 
__extension__
extern long long int strtoq (__const char *__restrict __nptr,
			     char **__restrict __endptr, int __base) throw () ;
 
__extension__
extern unsigned long long int strtouq (__const char *__restrict __nptr,
				       char **__restrict __endptr, int __base)
     throw () ;



 

 
__extension__
extern long long int strtoll (__const char *__restrict __nptr,
			      char **__restrict __endptr, int __base) throw () ;
 
__extension__
extern unsigned long long int strtoull (__const char *__restrict __nptr,
					char **__restrict __endptr, int __base)
     throw () ;



# 244 "/usr/include/stdlib.h" 3



 


extern double __strtod_internal (__const char *__restrict __nptr,
				 char **__restrict __endptr, int __group)
     throw () ;
extern float __strtof_internal (__const char *__restrict __nptr,
				char **__restrict __endptr, int __group)
     throw () ;
extern long double __strtold_internal (__const char *__restrict __nptr,
				       char **__restrict __endptr,
				       int __group) throw () ;

extern long int __strtol_internal (__const char *__restrict __nptr,
				   char **__restrict __endptr,
				   int __base, int __group) throw () ;



extern unsigned long int __strtoul_internal (__const char *__restrict __nptr,
					     char **__restrict __endptr,
					     int __base, int __group) throw () ;




__extension__
extern long long int __strtoll_internal (__const char *__restrict __nptr,
					 char **__restrict __endptr,
					 int __base, int __group) throw () ;



__extension__
extern unsigned long long int __strtoull_internal (__const char *
						   __restrict __nptr,
						   char **__restrict __endptr,
						   int __base, int __group)
     throw () ;




# 377 "/usr/include/stdlib.h" 3




 


extern char *l64a (long int __n) throw () ;

 
extern long int a64l (__const char *__s) throw ()   ;


# 1 "/usr/include/sys/types.h" 1 3
 

















 








extern "C" { 





typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;




typedef __loff_t loff_t;



typedef __ino_t ino_t;











typedef __dev_t dev_t;




typedef __gid_t gid_t;




typedef __mode_t mode_t;




typedef __nlink_t nlink_t;




typedef __uid_t uid_t;





typedef __off_t off_t;











typedef __pid_t pid_t;




typedef __id_t id_t;




typedef __ssize_t ssize_t;





typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;





typedef __key_t key_t;









# 1 "/usr/include/time.h" 1 3
 

















 














# 51 "/usr/include/time.h" 3


# 61 "/usr/include/time.h" 3








 
typedef __time_t time_t;










 
typedef __clockid_t clockid_t;










 
typedef __timer_t timer_t;





# 112 "/usr/include/time.h" 3




# 364 "/usr/include/time.h" 3



# 132 "/usr/include/sys/types.h" 2 3


# 143 "/usr/include/sys/types.h" 3



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 146 "/usr/include/sys/types.h" 2 3



 
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;


 

# 180 "/usr/include/sys/types.h" 3


 







typedef int int8_t __attribute__ ((__mode__ (  __QI__ ))) ;
typedef int int16_t __attribute__ ((__mode__ (  __HI__ ))) ;
typedef int int32_t __attribute__ ((__mode__ (  __SI__ ))) ;
typedef int int64_t __attribute__ ((__mode__ (  __DI__ ))) ;


typedef unsigned int u_int8_t __attribute__ ((__mode__ (  __QI__ ))) ;
typedef unsigned int u_int16_t __attribute__ ((__mode__ (  __HI__ ))) ;
typedef unsigned int u_int32_t __attribute__ ((__mode__ (  __SI__ ))) ;
typedef unsigned int u_int64_t __attribute__ ((__mode__ (  __DI__ ))) ;

typedef int register_t __attribute__ ((__mode__ (__word__)));


 






 
# 1 "/usr/include/endian.h" 1 3
 






















 











 
# 1 "/usr/include/bits/endian.h" 1 3
 






# 37 "/usr/include/endian.h" 2 3


 



















# 212 "/usr/include/sys/types.h" 2 3


 
# 1 "/usr/include/sys/select.h" 1 3
 


















 






 


 
# 1 "/usr/include/bits/select.h" 1 3
 

























# 36 "/usr/include/bits/select.h" 3












# 56 "/usr/include/bits/select.h" 3

# 72 "/usr/include/bits/select.h" 3

# 31 "/usr/include/sys/select.h" 2 3


 
# 1 "/usr/include/bits/sigset.h" 1 3
 





















typedef int __sig_atomic_t;

 


typedef struct
  {
    unsigned long int __val[(1024 / (8 * sizeof (unsigned long int))) ];
  } __sigset_t;




 





# 125 "/usr/include/bits/sigset.h" 3

# 34 "/usr/include/sys/select.h" 2 3




typedef __sigset_t sigset_t;


 


# 1 "/usr/include/time.h" 1 3
 

















 














# 51 "/usr/include/time.h" 3


# 61 "/usr/include/time.h" 3



# 72 "/usr/include/time.h" 3



# 84 "/usr/include/time.h" 3



# 96 "/usr/include/time.h" 3








 

struct timespec
  {
    __time_t tv_sec;		 
    long int tv_nsec;		 
  };





# 364 "/usr/include/time.h" 3



# 44 "/usr/include/sys/select.h" 2 3


# 1 "/usr/include/bits/time.h" 1 3
 


















 



# 57 "/usr/include/bits/time.h" 3








 

struct timeval
  {
    __time_t tv_sec;		 
    __suseconds_t tv_usec;	 
  };


# 46 "/usr/include/sys/select.h" 2 3



typedef __suseconds_t suseconds_t;




 
typedef long int __fd_mask;

 




 
typedef struct
  {
     





    __fd_mask __fds_bits[1024  / (8 * sizeof (__fd_mask)) ];


  } fd_set;

 



 
typedef __fd_mask fd_mask;

 




 






extern "C" { 

 




extern int select (int __nfds, fd_set *__restrict __readfds,
		   fd_set *__restrict __writefds,
		   fd_set *__restrict __exceptfds,
		   struct timeval *__restrict __timeout) throw () ;

# 116 "/usr/include/sys/select.h" 3


} 


# 215 "/usr/include/sys/types.h" 2 3


 
# 1 "/usr/include/sys/sysmacros.h" 1 3
 





















 








# 47 "/usr/include/sys/sysmacros.h" 3



# 218 "/usr/include/sys/types.h" 2 3









 


typedef __blkcnt_t blkcnt_t;	  



typedef __fsblkcnt_t fsblkcnt_t;  



typedef __fsfilcnt_t fsfilcnt_t;  


# 254 "/usr/include/sys/types.h" 3








} 


# 390 "/usr/include/stdlib.h" 2 3


 



 
extern long int random (void) throw () ;

 
extern void srandom (unsigned int __seed) throw () ;

 



extern char *initstate (unsigned int __seed, char *__statebuf,
			size_t __statelen) throw () ;

 

extern char *setstate (char *__statebuf) throw () ;



 



struct random_data
  {
    int32_t *fptr;		 
    int32_t *rptr;		 
    int32_t *state;		 
    int rand_type;		 
    int rand_deg;		 
    int rand_sep;		 
    int32_t *end_ptr;		 
  };

extern int random_r (struct random_data *__restrict __buf,
		     int32_t *__restrict __result) throw () ;

extern int srandom_r (unsigned int __seed, struct random_data *__buf) throw () ;

extern int initstate_r (unsigned int __seed, char *__restrict __statebuf,
			size_t __statelen,
			struct random_data *__restrict __buf) throw () ;

extern int setstate_r (char *__restrict __statebuf,
		       struct random_data *__restrict __buf) throw () ;




 
extern int rand (void) throw () ;
 
extern void srand (unsigned int __seed) throw () ;


 
extern int rand_r (unsigned int *__seed) throw () ;




 

 
extern double drand48 (void) throw () ;
extern double erand48 (unsigned short int __xsubi[3]) throw () ;

 
extern long int lrand48 (void) throw () ;
extern long int nrand48 (unsigned short int __xsubi[3]) throw () ;

 
extern long int mrand48 (void) throw () ;
extern long int jrand48 (unsigned short int __xsubi[3]) throw () ;

 
extern void srand48 (long int __seedval) throw () ;
extern unsigned short int *seed48 (unsigned short int __seed16v[3]) throw () ;
extern void lcong48 (unsigned short int __param[7]) throw () ;


 


struct drand48_data
  {
    unsigned short int __x[3];	 
    unsigned short int __old_x[3];  
    unsigned short int __c;	 
    unsigned short int __init;	 
    unsigned long long int __a;	 
  };

 
extern int drand48_r (struct drand48_data *__restrict __buffer,
		      double *__restrict __result) throw () ;
extern int erand48_r (unsigned short int __xsubi[3],
		      struct drand48_data *__restrict __buffer,
		      double *__restrict __result) throw () ;

 
extern int lrand48_r (struct drand48_data *__restrict __buffer,
		      long int *__restrict __result) throw () ;
extern int nrand48_r (unsigned short int __xsubi[3],
		      struct drand48_data *__restrict __buffer,
		      long int *__restrict __result) throw () ;

 
extern int mrand48_r (struct drand48_data *__restrict __buffer,
		      long int *__restrict __result) throw () ;
extern int jrand48_r (unsigned short int __xsubi[3],
		      struct drand48_data *__restrict __buffer,
		      long int *__restrict __result) throw () ;

 
extern int srand48_r (long int __seedval, struct drand48_data *__buffer)
     throw () ;

extern int seed48_r (unsigned short int __seed16v[3],
		     struct drand48_data *__buffer) throw () ;

extern int lcong48_r (unsigned short int __param[7],
		      struct drand48_data *__buffer) throw () ;







 
extern void *malloc (size_t __size) throw ()   ;
 
extern void *calloc (size_t __nmemb, size_t __size)
     throw ()   ;



 

extern void *realloc (void *__ptr, size_t __size) throw ()   ;
 
extern void free (void *__ptr) throw () ;


 
extern void cfree (void *__ptr) throw () ;



# 1 "/usr/include/alloca.h" 1 3
 























# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 25 "/usr/include/alloca.h" 2 3


extern "C" { 

 


 
extern void *alloca (size_t __size) throw () ;





} 


# 546 "/usr/include/stdlib.h" 2 3




 
extern void *valloc (size_t __size) throw ()   ;








 
extern void abort (void) throw ()  __attribute__ ((__noreturn__));


 
extern int atexit (void (*__func) (void)) throw () ;


 

extern int on_exit (void (*__func) (int __status, void *__arg), void *__arg)
     throw () ;


 


extern void exit (int __status) throw ()  __attribute__ ((__noreturn__));








 
extern char *getenv (__const char *__name) throw () ;

 

extern char *__secure_getenv (__const char *__name) throw () ;


 
 

extern int putenv (char *__string) throw () ;



 

extern int setenv (__const char *__name, __const char *__value, int __replace)
     throw () ;

 
extern int unsetenv (__const char *__name) throw () ;



 


extern int clearenv (void) throw () ;




 



extern char *mktemp (char *__template) throw () ;

 





extern int mkstemp (char *__template) throw () ;













 




extern char *mkdtemp (char *__template) throw () ;



 
extern int system (__const char *__command) throw () ;










 





extern char *realpath (__const char *__restrict __name,
		       char *__restrict __resolved) throw () ;



 


typedef int (*__compar_fn_t) (__const void *, __const void *);






 

extern void *bsearch (__const void *__key, __const void *__base,
		      size_t __nmemb, size_t __size, __compar_fn_t __compar);

 

extern void qsort (void *__base, size_t __nmemb, size_t __size,
		   __compar_fn_t __compar);


 
extern int abs (int __x) throw ()  __attribute__ ((__const__));
extern long int labs (long int __x) throw ()  __attribute__ ((__const__));






 

 
extern div_t div (int __numer, int __denom)
     throw ()  __attribute__ ((__const__));
extern ldiv_t ldiv (long int __numer, long int __denom)
     throw ()  __attribute__ ((__const__));








 


 


extern char *ecvt (double __value, int __ndigit, int *__restrict __decpt,
		   int *__restrict __sign) throw () ;

 


extern char *fcvt (double __value, int __ndigit, int *__restrict __decpt,
		   int *__restrict __sign) throw () ;

 


extern char *gcvt (double __value, int __ndigit, char *__buf) throw () ;



 
extern char *qecvt (long double __value, int __ndigit,
		    int *__restrict __decpt, int *__restrict __sign) throw () ;
extern char *qfcvt (long double __value, int __ndigit,
		    int *__restrict __decpt, int *__restrict __sign) throw () ;
extern char *qgcvt (long double __value, int __ndigit, char *__buf) throw () ;


 

extern int ecvt_r (double __value, int __ndigit, int *__restrict __decpt,
		   int *__restrict __sign, char *__restrict __buf,
		   size_t __len) throw () ;
extern int fcvt_r (double __value, int __ndigit, int *__restrict __decpt,
		   int *__restrict __sign, char *__restrict __buf,
		   size_t __len) throw () ;

extern int qecvt_r (long double __value, int __ndigit,
		    int *__restrict __decpt, int *__restrict __sign,
		    char *__restrict __buf, size_t __len) throw () ;
extern int qfcvt_r (long double __value, int __ndigit,
		    int *__restrict __decpt, int *__restrict __sign,
		    char *__restrict __buf, size_t __len) throw () ;




 

extern int mblen (__const char *__s, size_t __n) throw () ;
 

extern int mbtowc (wchar_t *__restrict __pwc,
		   __const char *__restrict __s, size_t __n) throw () ;
 

extern int wctomb (char *__s, wchar_t __wchar) throw () ;


 
extern size_t mbstowcs (wchar_t *__restrict  __pwcs,
			__const char *__restrict __s, size_t __n) throw () ;
 
extern size_t wcstombs (char *__restrict __s,
			__const wchar_t *__restrict __pwcs, size_t __n)
     throw () ;



 



extern int rpmatch (__const char *__response) throw () ;



# 811 "/usr/include/stdlib.h" 3









 






# 842 "/usr/include/stdlib.h" 3


# 852 "/usr/include/stdlib.h" 3



 


extern int getloadavg (double __loadavg[], int __nelem) throw () ;





} 


# 50 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algobase.h" 2 3

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 







 

 




 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 





















typedef int ptrdiff_t;









 




 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 


# 269 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3
















 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 













 







# 51 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algobase.h" 2 3

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/new.h" 1 3
 




# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/new" 1 3
 
 




#pragma interface "new"
# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3
# 342 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3

# 8 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/new" 2 3

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/exception" 1 3
 
 




#pragma interface "exception"

extern "C++" {

namespace std {

class exception {
public:
  exception () { }
  virtual ~exception () { }
  virtual const char* what () const;
};

class bad_exception : public exception {
public:
  bad_exception () { }
  virtual ~bad_exception () { }
};

typedef void (*terminate_handler) ();
typedef void (*unexpected_handler) ();

terminate_handler set_terminate (terminate_handler);
void terminate () __attribute__ ((__noreturn__));
unexpected_handler set_unexpected (unexpected_handler);
void unexpected () __attribute__ ((__noreturn__));
bool uncaught_exception ();

}  

}  


# 9 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/new" 2 3


extern "C++" {

namespace std {

  class bad_alloc : public exception {
  public:
    virtual const char* what() const throw() { return "bad_alloc"; }
  };

  struct nothrow_t {};
  extern const nothrow_t nothrow;
  typedef void (*new_handler)();
  new_handler set_new_handler (new_handler);

}  

 
void *operator new (size_t) throw (std::bad_alloc);
void *operator new[] (size_t) throw (std::bad_alloc);
void operator delete (void *) throw();
void operator delete[] (void *) throw();
void *operator new (size_t, const std::nothrow_t&) throw();
void *operator new[] (size_t, const std::nothrow_t&) throw();
void operator delete (void *, const std::nothrow_t&) throw();
void operator delete[] (void *, const std::nothrow_t&) throw();

 
inline void *operator new(size_t, void *place) throw() { return place; }
inline void *operator new[](size_t, void *place) throw() { return place; }
}  


# 6 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/new.h" 2 3


using std::new_handler;
using std::set_new_handler;


# 52 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algobase.h" 2 3




# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_iterator.h" 1 3
 

























 






 

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

 
 
 
 

template <class _Tp, class _Distance> struct input_iterator {
  typedef input_iterator_tag iterator_category;
  typedef _Tp                value_type;
  typedef _Distance          difference_type;
  typedef _Tp*               pointer;
  typedef _Tp&               reference;
};

struct output_iterator {
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;
};

template <class _Tp, class _Distance> struct forward_iterator {
  typedef forward_iterator_tag iterator_category;
  typedef _Tp                  value_type;
  typedef _Distance            difference_type;
  typedef _Tp*                 pointer;
  typedef _Tp&                 reference;
};


template <class _Tp, class _Distance> struct bidirectional_iterator {
  typedef bidirectional_iterator_tag iterator_category;
  typedef _Tp                        value_type;
  typedef _Distance                  difference_type;
  typedef _Tp*                       pointer;
  typedef _Tp&                       reference;
};

template <class _Tp, class _Distance> struct random_access_iterator {
  typedef random_access_iterator_tag iterator_category;
  typedef _Tp                        value_type;
  typedef _Distance                  difference_type;
  typedef _Tp*                       pointer;
  typedef _Tp&                       reference;
};

# 98 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_iterator.h" 3




template <class _Iterator>
struct iterator_traits {
  typedef typename _Iterator::iterator_category iterator_category;
  typedef typename _Iterator::value_type        value_type;
  typedef typename _Iterator::difference_type   difference_type;
  typedef typename _Iterator::pointer           pointer;
  typedef typename _Iterator::reference         reference;
};

template <class _Tp>
struct iterator_traits<_Tp*> {
  typedef random_access_iterator_tag iterator_category;
  typedef _Tp                         value_type;
  typedef ptrdiff_t                   difference_type;
  typedef _Tp*                        pointer;
  typedef _Tp&                        reference;
};

template <class _Tp>
struct iterator_traits<const _Tp*> {
  typedef random_access_iterator_tag iterator_category;
  typedef _Tp                         value_type;
  typedef ptrdiff_t                   difference_type;
  typedef const _Tp*                  pointer;
  typedef const _Tp&                  reference;
};

 
 
 
 

 

template <class _Iter>
inline typename iterator_traits<_Iter>::iterator_category
__iterator_category(const _Iter&)
{
  typedef typename iterator_traits<_Iter>::iterator_category _Category;
  return _Category();
}

template <class _Iter>
inline typename iterator_traits<_Iter>::difference_type*
__distance_type(const _Iter&)
{
  return static_cast<typename iterator_traits<_Iter>::difference_type*>(0);
}

template <class _Iter>
inline typename iterator_traits<_Iter>::value_type*
__value_type(const _Iter&)
{
  return static_cast<typename iterator_traits<_Iter>::value_type*>(0);
}

template <class _Iter>
inline typename iterator_traits<_Iter>::iterator_category
iterator_category(const _Iter& __i) { return __iterator_category(__i); }


template <class _Iter>
inline typename iterator_traits<_Iter>::difference_type*
distance_type(const _Iter& __i) { return __distance_type(__i); }

template <class _Iter>
inline typename iterator_traits<_Iter>::value_type*
value_type(const _Iter& __i) { return __value_type(__i); }





# 259 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_iterator.h" 3


template <class _InputIterator, class _Distance>
inline void __distance(_InputIterator __first, _InputIterator __last,
                       _Distance& __n, input_iterator_tag)
{
  while (__first != __last) { ++__first; ++__n; }
}

template <class _RandomAccessIterator, class _Distance>
inline void __distance(_RandomAccessIterator __first, 
                       _RandomAccessIterator __last, 
                       _Distance& __n, random_access_iterator_tag)
{
  __n += __last - __first;
}

template <class _InputIterator, class _Distance>
inline void distance(_InputIterator __first, 
                     _InputIterator __last, _Distance& __n)
{
  __distance(__first, __last, __n, iterator_category(__first));
}



template <class _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type
__distance(_InputIterator __first, _InputIterator __last, input_iterator_tag)
{
  typename iterator_traits<_InputIterator>::difference_type __n = 0;
  while (__first != __last) {
    ++__first; ++__n;
  }
  return __n;
}

template <class _RandomAccessIterator>
inline typename iterator_traits<_RandomAccessIterator>::difference_type
__distance(_RandomAccessIterator __first, _RandomAccessIterator __last,
           random_access_iterator_tag) {
  return __last - __first;
}

template <class _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type
distance(_InputIterator __first, _InputIterator __last) {
  typedef typename iterator_traits<_InputIterator>::iterator_category 
    _Category;
  return __distance(__first, __last, _Category());
}



template <class _InputIter, class _Distance>
inline void __advance(_InputIter& __i, _Distance __n, input_iterator_tag) {
  while (__n--) ++__i;
}





template <class _BidirectionalIterator, class _Distance>
inline void __advance(_BidirectionalIterator& __i, _Distance __n, 
                      bidirectional_iterator_tag) {
  if (__n >= 0)
    while (__n--) ++__i;
  else
    while (__n++) --__i;
}





template <class _RandomAccessIterator, class _Distance>
inline void __advance(_RandomAccessIterator& __i, _Distance __n, 
                      random_access_iterator_tag) {
  __i += __n;
}

template <class _InputIterator, class _Distance>
inline void advance(_InputIterator& __i, _Distance __n) {
  __advance(__i, __n, iterator_category(__i));
}

template <class _Container>
class back_insert_iterator {
protected:
  _Container* container;
public:
  typedef _Container          container_type;
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  explicit back_insert_iterator(_Container& __x) : container(&__x) {}
  back_insert_iterator<_Container>&
  operator=(const typename _Container::value_type& __value) { 
    container->push_back(__value);
    return *this;
  }
  back_insert_iterator<_Container>& operator*() { return *this; }
  back_insert_iterator<_Container>& operator++() { return *this; }
  back_insert_iterator<_Container>& operator++(int) { return *this; }
};

# 378 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_iterator.h" 3


template <class _Container>
inline back_insert_iterator<_Container> back_inserter(_Container& __x) {
  return back_insert_iterator<_Container>(__x);
}

template <class _Container>
class front_insert_iterator {
protected:
  _Container* container;
public:
  typedef _Container          container_type;
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  explicit front_insert_iterator(_Container& __x) : container(&__x) {}
  front_insert_iterator<_Container>&
  operator=(const typename _Container::value_type& __value) { 
    container->push_front(__value);
    return *this;
  }
  front_insert_iterator<_Container>& operator*() { return *this; }
  front_insert_iterator<_Container>& operator++() { return *this; }
  front_insert_iterator<_Container>& operator++(int) { return *this; }
};

# 417 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_iterator.h" 3


template <class _Container>
inline front_insert_iterator<_Container> front_inserter(_Container& __x) {
  return front_insert_iterator<_Container>(__x);
}

template <class _Container>
class insert_iterator {
protected:
  _Container* container;
  typename _Container::iterator iter;
public:
  typedef _Container          container_type;
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  insert_iterator(_Container& __x, typename _Container::iterator __i) 
    : container(&__x), iter(__i) {}
  insert_iterator<_Container>&
  operator=(const typename _Container::value_type& __value) { 
    iter = container->insert(iter, __value);
    ++iter;
    return *this;
  }
  insert_iterator<_Container>& operator*() { return *this; }
  insert_iterator<_Container>& operator++() { return *this; }
  insert_iterator<_Container>& operator++(int) { return *this; }
};

# 459 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_iterator.h" 3


template <class _Container, class _Iterator>
inline 
insert_iterator<_Container> inserter(_Container& __x, _Iterator __i)
{
  typedef typename _Container::iterator __iter;
  return insert_iterator<_Container>(__x, __iter(__i));
}


template <class _BidirectionalIterator, class _Tp, class _Reference = _Tp&, 
          class _Distance = ptrdiff_t> 




class reverse_bidirectional_iterator {
  typedef reverse_bidirectional_iterator<_BidirectionalIterator, _Tp, 
                                         _Reference, _Distance>  _Self;
protected:
  _BidirectionalIterator current;
public:
  typedef bidirectional_iterator_tag iterator_category;
  typedef _Tp                        value_type;
  typedef _Distance                  difference_type;
  typedef _Tp*                       pointer;
  typedef _Reference                 reference;

  reverse_bidirectional_iterator() {}
  explicit reverse_bidirectional_iterator(_BidirectionalIterator __x)
    : current(__x) {}
  _BidirectionalIterator base() const { return current; }
  _Reference operator*() const {
    _BidirectionalIterator __tmp = current;
    return *--__tmp;
  }

  pointer operator->() const { return &(operator*()); }

  _Self& operator++() {
    --current;
    return *this;
  }
  _Self operator++(int) {
    _Self __tmp = *this;
    --current;
    return __tmp;
  }
  _Self& operator--() {
    ++current;
    return *this;
  }
  _Self operator--(int) {
    _Self __tmp = *this;
    ++current;
    return __tmp;
  }
};

# 550 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_iterator.h" 3


template <class _BiIter, class _Tp, class _Ref,
          class _Distance>
inline bool operator==(
    const reverse_bidirectional_iterator<_BiIter, _Tp, _Ref, _Distance>& __x, 
    const reverse_bidirectional_iterator<_BiIter, _Tp, _Ref, _Distance>& __y)
{
  return __x.base() == __y.base();
}



 
 
 
 
 

template <class _Iterator>
class reverse_iterator 
{
protected:
  _Iterator current;
public:
  typedef typename iterator_traits<_Iterator>::iterator_category
          iterator_category;
  typedef typename iterator_traits<_Iterator>::value_type
          value_type;
  typedef typename iterator_traits<_Iterator>::difference_type
          difference_type;
  typedef typename iterator_traits<_Iterator>::pointer
          pointer;
  typedef typename iterator_traits<_Iterator>::reference
          reference;

  typedef _Iterator iterator_type;
  typedef reverse_iterator<_Iterator> _Self;

public:
  reverse_iterator() {}
  explicit reverse_iterator(iterator_type __x) : current(__x) {}

  reverse_iterator(const _Self& __x) : current(__x.current) {}

  template <class _Iter>
  reverse_iterator(const reverse_iterator<_Iter>& __x)
    : current(__x.base()) {}

    
  iterator_type base() const { return current; }
  reference operator*() const {
    _Iterator __tmp = current;
    return *--__tmp;
  }

  pointer operator->() const { return &(operator*()); }


  _Self& operator++() {
    --current;
    return *this;
  }
  _Self operator++(int) {
    _Self __tmp = *this;
    --current;
    return __tmp;
  }
  _Self& operator--() {
    ++current;
    return *this;
  }
  _Self operator--(int) {
    _Self __tmp = *this;
    ++current;
    return __tmp;
  }

  _Self operator+(difference_type __n) const {
    return _Self(current - __n);
  }
  _Self& operator+=(difference_type __n) {
    current -= __n;
    return *this;
  }
  _Self operator-(difference_type __n) const {
    return _Self(current + __n);
  }
  _Self& operator-=(difference_type __n) {
    current += __n;
    return *this;
  }
  reference operator[](difference_type __n) const { return *(*this + __n); }  
}; 
 
template <class _Iterator>
inline bool operator==(const reverse_iterator<_Iterator>& __x, 
                       const reverse_iterator<_Iterator>& __y) {
  return __x.base() == __y.base();
}

template <class _Iterator>
inline bool operator<(const reverse_iterator<_Iterator>& __x, 
                      const reverse_iterator<_Iterator>& __y) {
  return __y.base() < __x.base();
}

template <class _Iterator>
inline typename reverse_iterator<_Iterator>::difference_type
operator-(const reverse_iterator<_Iterator>& __x, 
          const reverse_iterator<_Iterator>& __y) {
  return __y.base() - __x.base();
}

template <class _Iterator>
inline reverse_iterator<_Iterator> 
operator+(typename reverse_iterator<_Iterator>::difference_type __n,
          const reverse_iterator<_Iterator>& __x) {
  return reverse_iterator<_Iterator>(__x.base() - __n);
}

# 805 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_iterator.h" 3


 
 

template <class _Tp, class _Dist = ptrdiff_t> 
class istream_iterator {
  friend bool operator== <>  (const istream_iterator&,
                                               const istream_iterator&);
protected:
  istream* _M_stream;
  _Tp _M_value;
  bool _M_end_marker;
  void _M_read() {
    _M_end_marker = (*_M_stream) ? true : false;
    if (_M_end_marker) *_M_stream >> _M_value;
    _M_end_marker = (*_M_stream) ? true : false;
  }
public:
  typedef input_iterator_tag  iterator_category;
  typedef _Tp                 value_type;
  typedef _Dist               difference_type;
  typedef const _Tp*          pointer;
  typedef const _Tp&          reference;

  istream_iterator() : _M_stream(&cin), _M_end_marker(false) {}
  istream_iterator(istream& __s) : _M_stream(&__s) { _M_read(); }
  reference operator*() const { return _M_value; }

  pointer operator->() const { return &(operator*()); }

  istream_iterator<_Tp, _Dist>& operator++() { 
    _M_read(); 
    return *this;
  }
  istream_iterator<_Tp, _Dist> operator++(int)  {
    istream_iterator<_Tp, _Dist> __tmp = *this;
    _M_read();
    return __tmp;
  }
};

# 864 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_iterator.h" 3


template <class _Tp, class _Distance>
inline bool operator==(const istream_iterator<_Tp, _Distance>& __x,
                       const istream_iterator<_Tp, _Distance>& __y) {
  return (__x._M_stream == __y._M_stream &&
          __x._M_end_marker == __y._M_end_marker) ||
         __x._M_end_marker == false && __y._M_end_marker == false;
}

template <class _Tp>
class ostream_iterator {
protected:
  ostream* _M_stream;
  const char* _M_string;
public:
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  ostream_iterator(ostream& __s) : _M_stream(&__s), _M_string(0) {}
  ostream_iterator(ostream& __s, const char* __c) 
    : _M_stream(&__s), _M_string(__c)  {}
  ostream_iterator<_Tp>& operator=(const _Tp& __value) { 
    *_M_stream << __value;
    if (_M_string) *_M_stream << _M_string;
    return *this;
  }
  ostream_iterator<_Tp>& operator*() { return *this; }
  ostream_iterator<_Tp>& operator++() { return *this; } 
  ostream_iterator<_Tp>& operator++(int) { return *this; } 
};

# 907 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_iterator.h" 3


 



 
 
 
# 56 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algobase.h" 2 3



 

 

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*) {
  _Tp __tmp = *__a;
  *__a = *__b;
  *__b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b) {
  __iter_swap(__a, __b, __value_type( __a ) );
}

template <class _Tp>
inline void swap(_Tp& __a, _Tp& __b) {
  _Tp __tmp = __a;
  __a = __b;
  __b = __tmp;
}

 
 






template <class _Tp>
inline const _Tp& min(const _Tp& __a, const _Tp& __b) {
  return __b < __a ? __b : __a;
}

template <class _Tp>
inline const _Tp& max(const _Tp& __a, const _Tp& __b) {
  return  __a < __b ? __b : __a;
}



template <class _Tp, class _Compare>
inline const _Tp& min(const _Tp& __a, const _Tp& __b, _Compare __comp) {
  return __comp(__b, __a) ? __b : __a;
}

template <class _Tp, class _Compare>
inline const _Tp& max(const _Tp& __a, const _Tp& __b, _Compare __comp) {
  return __comp(__a, __b) ? __b : __a;
}

 
 

 
 
 
 
 
 
 

template <class _InputIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result,
                          input_iterator_tag, _Distance*)
{
  for ( ; __first != __last; ++__result, ++__first)
    *__result = *__first;
  return __result;
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter
__copy(_RandomAccessIter __first, _RandomAccessIter __last,
       _OutputIter __result, random_access_iterator_tag, _Distance*)
{
  for (_Distance __n = __last - __first; __n > 0; --__n) {
    *__result = *__first;
    ++__first;
    ++__result;
  }
  return __result;
}

template <class _Tp>
inline _Tp*
__copy_trivial(const _Tp* __first, const _Tp* __last, _Tp* __result) {
  memmove(__result, __first, sizeof(_Tp) * (__last - __first));
  return __result + (__last - __first);
}



template <class _InputIter, class _OutputIter, class _BoolType>
struct __copy_dispatch {
  static _OutputIter copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result) {
    typedef typename iterator_traits<_InputIter>::iterator_category _Category;
    typedef typename iterator_traits<_InputIter>::difference_type _Distance;
    return __copy(__first, __last, __result, _Category(), (_Distance*) 0);
  }
};

template <class _Tp>
struct __copy_dispatch<_Tp*, _Tp*, __true_type>
{
  static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    return __copy_trivial(__first, __last, __result);
  }
};

template <class _Tp>
struct __copy_dispatch<const _Tp*, _Tp*, __true_type>
{
  static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    return __copy_trivial(__first, __last, __result);
  }
};

template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last,
                        _OutputIter __result) {
  typedef typename iterator_traits<_InputIter>::value_type _Tp;
  typedef typename __type_traits<_Tp>::has_trivial_assignment_operator
          _Trivial;
  return __copy_dispatch<_InputIter, _OutputIter, _Trivial>
    ::copy(__first, __last, __result);
}

# 213 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algobase.h" 3


 
 

template <class _BidirectionalIter1, class _BidirectionalIter2, 
          class _Distance>
inline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first, 
                                           _BidirectionalIter1 __last, 
                                           _BidirectionalIter2 __result,
                                           bidirectional_iterator_tag,
                                           _Distance*)
{
  while (__first != __last)
    *--__result = *--__last;
  return __result;
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, 
                                          _RandomAccessIter __last, 
                                          _BidirectionalIter __result,
                                          random_access_iterator_tag,
                                          _Distance*)
{
  for (_Distance __n = __last - __first; __n > 0; --__n)
    *--__result = *--__last;
  return __result;
}



 
 
 
 

template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _BoolType>
struct __copy_backward_dispatch
{
  typedef typename iterator_traits<_BidirectionalIter1>::iterator_category 
          _Cat;
  typedef typename iterator_traits<_BidirectionalIter1>::difference_type
          _Distance;

  static _BidirectionalIter2 copy(_BidirectionalIter1 __first, 
                                  _BidirectionalIter1 __last, 
                                  _BidirectionalIter2 __result) {
    return __copy_backward(__first, __last, __result, _Cat(), (_Distance*) 0);
  }
};

template <class _Tp>
struct __copy_backward_dispatch<_Tp*, _Tp*, __true_type>
{
  static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    const ptrdiff_t _Num = __last - __first;
    memmove(__result - _Num, __first, sizeof(_Tp) * _Num);
    return __result - _Num;
  }
};

template <class _Tp>
struct __copy_backward_dispatch<const _Tp*, _Tp*, __true_type>
{
  static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    return  __copy_backward_dispatch<_Tp*, _Tp*, __true_type>
      ::copy(__first, __last, __result);
  }
};

template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result) {
  typedef typename __type_traits<typename iterator_traits<_BI2>::value_type>
                        ::has_trivial_assignment_operator
          _Trivial;
  return __copy_backward_dispatch<_BI1, _BI2, _Trivial>
              ::copy(__first, __last, __result);
}

# 303 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algobase.h" 3


 
 

template <class _InputIter, class _Size, class _OutputIter>
pair<_InputIter, _OutputIter> __copy_n(_InputIter __first, _Size __count,
                                       _OutputIter __result,
                                       input_iterator_tag) {
  for ( ; __count > 0; --__count) {
    *__result = *__first;
    ++__first;
    ++__result;
  }
  return pair<_InputIter, _OutputIter>(__first, __result);
}

template <class _RAIter, class _Size, class _OutputIter>
inline pair<_RAIter, _OutputIter>
__copy_n(_RAIter __first, _Size __count,
         _OutputIter __result,
         random_access_iterator_tag) {
  _RAIter __last = __first + __count;
  return pair<_RAIter, _OutputIter>(__last, copy(__first, __last, __result));
}

template <class _InputIter, class _Size, class _OutputIter>
inline pair<_InputIter, _OutputIter>
__copy_n(_InputIter __first, _Size __count, _OutputIter __result) {
  return __copy_n(__first, __count, __result,
                  __iterator_category( __first ) );
}

template <class _InputIter, class _Size, class _OutputIter>
inline pair<_InputIter, _OutputIter>
copy_n(_InputIter __first, _Size __count, _OutputIter __result) {
  return __copy_n(__first, __count, __result);
}

 
 


template <class _ForwardIter, class _Tp>
void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __value) {
  for ( ; __first != __last; ++__first)
    *__first = __value;
}

template <class _OutputIter, class _Size, class _Tp>
_OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value) {
  for ( ; __n > 0; --__n, ++__first)
    *__first = __value;
  return __first;
}

 
 

template <class _InputIter1, class _InputIter2>
pair<_InputIter1, _InputIter2> mismatch(_InputIter1 __first1,
                                        _InputIter1 __last1,
                                        _InputIter2 __first2) {
  while (__first1 != __last1 && *__first1 == *__first2) {
    ++__first1;
    ++__first2;
  }
  return pair<_InputIter1, _InputIter2>(__first1, __first2);
}

template <class _InputIter1, class _InputIter2, class _BinaryPredicate>
pair<_InputIter1, _InputIter2> mismatch(_InputIter1 __first1,
                                        _InputIter1 __last1,
                                        _InputIter2 __first2,
                                        _BinaryPredicate __binary_pred) {
  while (__first1 != __last1 && __binary_pred(*__first1, *__first2)) {
    ++__first1;
    ++__first2;
  }
  return pair<_InputIter1, _InputIter2>(__first1, __first2);
}

template <class _InputIter1, class _InputIter2>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2) {
  for ( ; __first1 != __last1; ++__first1, ++__first2)
    if (*__first1 != *__first2)
      return false;
  return true;
}

template <class _InputIter1, class _InputIter2, class _BinaryPredicate>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2, _BinaryPredicate __binary_pred) {
  for ( ; __first1 != __last1; ++__first1, ++__first2)
    if (!__binary_pred(*__first1, *__first2))
      return false;
  return true;
}

 
 
 

template <class _InputIter1, class _InputIter2>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2) {
  for ( ; __first1 != __last1 && __first2 != __last2
        ; ++__first1, ++__first2) {
    if (*__first1 < *__first2)
      return true;
    if (*__first2 < *__first1)
      return false;
  }
  return __first1 == __last1 && __first2 != __last2;
}

template <class _InputIter1, class _InputIter2, class _Compare>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2,
                             _Compare __comp) {
  for ( ; __first1 != __last1 && __first2 != __last2
        ; ++__first1, ++__first2) {
    if (__comp(*__first1, *__first2))
      return true;
    if (__comp(*__first2, *__first1))
      return false;
  }
  return __first1 == __last1 && __first2 != __last2;
}

inline bool 
lexicographical_compare(const unsigned char* __first1,
                        const unsigned char* __last1,
                        const unsigned char* __first2,
                        const unsigned char* __last2)
{
  const size_t __len1 = __last1 - __first1;
  const size_t __len2 = __last2 - __first2;
  const int __result = memcmp(__first1, __first2, min(__len1, __len2));
  return __result != 0 ? __result < 0 : __len1 < __len2;
}

inline bool lexicographical_compare(const char* __first1, const char* __last1,
                                    const char* __first2, const char* __last2)
{

  return lexicographical_compare((const signed char*) __first1,
                                 (const signed char*) __last1,
                                 (const signed char*) __first2,
                                 (const signed char*) __last2);






}

template <class _InputIter1, class _InputIter2>
int __lexicographical_compare_3way(_InputIter1 __first1, _InputIter1 __last1,
                                   _InputIter2 __first2, _InputIter2 __last2)
{
  while (__first1 != __last1 && __first2 != __last2) {
    if (*__first1 < *__first2)
      return -1;
    if (*__first2 < *__first1)
      return 1;
    ++__first1;
    ++__first2;
  }
  if (__first2 == __last2) {
    return !(__first1 == __last1);
  }
  else {
    return -1;
  }
}

inline int
__lexicographical_compare_3way(const unsigned char* __first1,
                               const unsigned char* __last1,
                               const unsigned char* __first2,
                               const unsigned char* __last2)
{
  const ptrdiff_t __len1 = __last1 - __first1;
  const ptrdiff_t __len2 = __last2 - __first2;
  const int __result = memcmp(__first1, __first2, min(__len1, __len2));
  return __result != 0 ? __result 
                       : (__len1 == __len2 ? 0 : (__len1 < __len2 ? -1 : 1));
}

inline int 
__lexicographical_compare_3way(const char* __first1, const char* __last1,
                               const char* __first2, const char* __last2)
{

  return __lexicographical_compare_3way(
                                (const signed char*) __first1,
                                (const signed char*) __last1,
                                (const signed char*) __first2,
                                (const signed char*) __last2);






}

template <class _InputIter1, class _InputIter2>
int lexicographical_compare_3way(_InputIter1 __first1, _InputIter1 __last1,
                                 _InputIter2 __first2, _InputIter2 __last2)
{
  return __lexicographical_compare_3way(__first1, __last1, __first2, __last2);
}

 



 
 
 
# 30 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/vector" 2 3

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_alloc.h" 1 3
 












 



















 
 
 
 
 
 













# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3
# 342 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3

# 53 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_alloc.h" 2 3



# 1 "/usr/include/assert.h" 1 3
 

















 



# 33 "/usr/include/assert.h" 3











 




# 63 "/usr/include/assert.h" 3


extern "C" { 

 
extern void __assert_fail (__const char *__assertion, __const char *__file,
			   unsigned int __line, __const char *__function)
     throw ()  __attribute__ ((__noreturn__));

 
extern void __assert_perror_fail (int __errnum, __const char *__file,
				  unsigned int __line,
				  __const char *__function)
     throw ()  __attribute__ ((__noreturn__));


 

extern void __assert (const char *__assertion, const char *__file, int __line)
     throw ()  __attribute__ ((__noreturn__));


} 













 















# 56 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_alloc.h" 2 3












     
     
     
# 1 "/usr/include/pthread.h" 1 3
 
 
 
 
 
 
 
 
 
 
 
 
 






# 1 "/usr/include/sched.h" 1 3
 























 



# 1 "/usr/include/time.h" 1 3
 

















 














# 51 "/usr/include/time.h" 3


# 61 "/usr/include/time.h" 3



# 72 "/usr/include/time.h" 3



# 84 "/usr/include/time.h" 3



# 96 "/usr/include/time.h" 3




# 112 "/usr/include/time.h" 3




# 364 "/usr/include/time.h" 3



# 29 "/usr/include/sched.h" 2 3


 
# 1 "/usr/include/bits/sched.h" 1 3
 


























 





 











 
struct sched_param
  {
    int __sched_priority;
  };

extern "C" { 

 

extern int clone (int (*__fn) (void *__arg), void *__child_stack,
		  int __flags, void *__arg) throw () ;


} 



# 73 "/usr/include/bits/sched.h" 3

# 32 "/usr/include/sched.h" 2 3

 



extern "C" { 

 
extern int sched_setparam (__pid_t __pid, __const struct sched_param *__param)
     throw () ;

 
extern int sched_getparam (__pid_t __pid, struct sched_param *__param) throw () ;

 
extern int sched_setscheduler (__pid_t __pid, int __policy,
			       __const struct sched_param *__param) throw () ;

 
extern int sched_getscheduler (__pid_t __pid) throw () ;

 
extern int sched_yield (void) throw () ;

 
extern int sched_get_priority_max (int __algorithm) throw () ;

 
extern int sched_get_priority_min (int __algorithm) throw () ;

 
extern int sched_rr_get_interval (__pid_t __pid, struct timespec *__t) throw () ;

} 


# 20 "/usr/include/pthread.h" 2 3

# 1 "/usr/include/time.h" 1 3
 

















 










extern "C" { 




 


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 


# 126 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 




 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 


# 269 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3
















 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 













 







# 38 "/usr/include/time.h" 2 3


 

# 1 "/usr/include/bits/time.h" 1 3
 


















 







 


 





 


extern long int __sysconf (int);




 

 

 


 






# 73 "/usr/include/bits/time.h" 3

# 42 "/usr/include/time.h" 2 3


 













 
typedef __clock_t clock_t;




# 72 "/usr/include/time.h" 3



# 84 "/usr/include/time.h" 3



# 96 "/usr/include/time.h" 3




# 112 "/usr/include/time.h" 3





 
struct tm
{
  int tm_sec;			 
  int tm_min;			 
  int tm_hour;			 
  int tm_mday;			 
  int tm_mon;			 
  int tm_year;			 
  int tm_wday;			 
  int tm_yday;			 
  int tm_isdst;			 


  long int tm_gmtoff;		 
  __const char *tm_zone;	 




};



 
struct itimerspec
  {
    struct timespec it_interval;
    struct timespec it_value;
  };

 
struct sigevent;











 

extern clock_t clock (void) throw () ;

 
extern time_t time (time_t *__timer) throw () ;

 
extern double difftime (time_t __time1, time_t __time0)
     throw ()  __attribute__ ((__const__));

 
extern time_t mktime (struct tm *__tp) throw () ;


 


extern size_t strftime (char *__restrict __s, size_t __maxsize,
			__const char *__restrict __format,
			__const struct tm *__restrict __tp) throw () ;










 

extern struct tm *gmtime (__const time_t *__timer) throw () ;

 

extern struct tm *localtime (__const time_t *__timer) throw () ;


 

extern struct tm *gmtime_r (__const time_t *__restrict __timer,
			    struct tm *__restrict __tp) throw () ;

 

extern struct tm *localtime_r (__const time_t *__restrict __timer,
			       struct tm *__restrict __tp) throw () ;


 

extern char *asctime (__const struct tm *__tp) throw () ;

 
extern char *ctime (__const time_t *__timer) throw () ;


 

 

extern char *asctime_r (__const struct tm *__restrict __tp,
			char *__restrict __buf) throw () ;

 
extern char *ctime_r (__const time_t *__restrict __timer,
		      char *__restrict __buf) throw () ;



 
extern char *__tzname[2];	 
extern int __daylight;		 
extern long int __timezone;	 



 
extern char *tzname[2];

 

extern void tzset (void) throw () ;



extern int daylight;
extern long int timezone;



 

extern int stime (__const time_t *__when) throw () ;



 






 


 
extern time_t timegm (struct tm *__tp) throw () ;

 
extern time_t timelocal (struct tm *__tp) throw () ;

 
extern int dysize (int __year) throw ()   __attribute__ ((__const__));




 
extern int nanosleep (__const struct timespec *__requested_time,
		      struct timespec *__remaining) throw () ;


 
extern int clock_getres (clockid_t __clock_id, struct timespec *__res) throw () ;

 
extern int clock_gettime (clockid_t __clock_id, struct timespec *__tp) throw () ;

 
extern int clock_settime (clockid_t __clock_id, __const struct timespec *__tp)
     throw () ;

# 305 "/usr/include/time.h" 3



 
extern int timer_create (clockid_t __clock_id,
			 struct sigevent *__restrict __evp,
			 timer_t *__restrict __timerid) throw () ;

 
extern int timer_delete (timer_t __timerid) throw () ;

 
extern int timer_settime (timer_t __timerid, int __flags,
			  __const struct itimerspec *__restrict __value,
			  struct itimerspec *__restrict __ovalue) throw () ;

 
extern int timer_gettime (timer_t __timerid, struct itimerspec *__value)
     throw () ;

 
extern int timer_getoverrun (timer_t __timerid) throw () ;



# 349 "/usr/include/time.h" 3


# 359 "/usr/include/time.h" 3



} 




# 21 "/usr/include/pthread.h" 2 3



# 1 "/usr/include/signal.h" 1 3
 

















 











extern "C" { 

# 1 "/usr/include/bits/sigset.h" 1 3
 


















# 33 "/usr/include/bits/sigset.h" 3



 





# 125 "/usr/include/bits/sigset.h" 3

# 33 "/usr/include/signal.h" 2 3


 

















# 369 "/usr/include/signal.h" 3


} 


# 24 "/usr/include/pthread.h" 2 3

# 1 "/usr/include/bits/pthreadtypes.h" 1 3
 
 
 
 
 
 
 
 
 
 
 
 
 





# 144 "/usr/include/bits/pthreadtypes.h" 3

# 25 "/usr/include/pthread.h" 2 3

# 1 "/usr/include/bits/initspin.h" 1 3
 


















 




 


# 26 "/usr/include/pthread.h" 2 3



extern "C" { 

 

























 

enum
{
  PTHREAD_CREATE_JOINABLE,

  PTHREAD_CREATE_DETACHED

};

enum
{
  PTHREAD_INHERIT_SCHED,

  PTHREAD_EXPLICIT_SCHED

};

enum
{
  PTHREAD_SCOPE_SYSTEM,

  PTHREAD_SCOPE_PROCESS

};

enum
{
  PTHREAD_MUTEX_TIMED_NP,
  PTHREAD_MUTEX_RECURSIVE_NP,
  PTHREAD_MUTEX_ERRORCHECK_NP,
  PTHREAD_MUTEX_ADAPTIVE_NP











};

enum
{
  PTHREAD_PROCESS_PRIVATE,

  PTHREAD_PROCESS_SHARED

};

# 118 "/usr/include/pthread.h" 3




 






 

struct _pthread_cleanup_buffer
{
  void (*__routine) (void *);		   
  void *__arg;				   
  int __canceltype;			   
  struct _pthread_cleanup_buffer *__prev;  
};

 

enum
{
  PTHREAD_CANCEL_ENABLE,

  PTHREAD_CANCEL_DISABLE

};
enum
{
  PTHREAD_CANCEL_DEFERRED,

  PTHREAD_CANCEL_ASYNCHRONOUS

};



 

 


extern int pthread_create (pthread_t *__restrict __thread,
			   __const pthread_attr_t *__restrict __attr,
			   void *(*__start_routine) (void *),
			   void *__restrict __arg) throw () ;

 
extern pthread_t pthread_self (void) throw () ;

 
extern int pthread_equal (pthread_t __thread1, pthread_t __thread2) throw () ;

 
extern void pthread_exit (void *__retval)
     throw ()  __attribute__ ((__noreturn__));

 


extern int pthread_join (pthread_t __th, void **__thread_return) throw () ;

 



extern int pthread_detach (pthread_t __th) throw () ;


 

 


extern int pthread_attr_init (pthread_attr_t *__attr) throw () ;

 
extern int pthread_attr_destroy (pthread_attr_t *__attr) throw () ;

 
extern int pthread_attr_setdetachstate (pthread_attr_t *__attr,
					int __detachstate) throw () ;

 
extern int pthread_attr_getdetachstate (__const pthread_attr_t *__attr,
					int *__detachstate) throw () ;

 
extern int pthread_attr_setschedparam (pthread_attr_t *__restrict __attr,
				       __const struct sched_param *__restrict
				       __param) throw () ;

 
extern int pthread_attr_getschedparam (__const pthread_attr_t *__restrict
				       __attr,
				       struct sched_param *__restrict __param)
     throw () ;

 
extern int pthread_attr_setschedpolicy (pthread_attr_t *__attr, int __policy)
     throw () ;

 
extern int pthread_attr_getschedpolicy (__const pthread_attr_t *__restrict
					__attr, int *__restrict __policy)
     throw () ;

 
extern int pthread_attr_setinheritsched (pthread_attr_t *__attr,
					 int __inherit) throw () ;

 
extern int pthread_attr_getinheritsched (__const pthread_attr_t *__restrict
					 __attr, int *__restrict __inherit)
     throw () ;

 
extern int pthread_attr_setscope (pthread_attr_t *__attr, int __scope)
     throw () ;

 
extern int pthread_attr_getscope (__const pthread_attr_t *__restrict __attr,
				  int *__restrict __scope) throw () ;

# 254 "/usr/include/pthread.h" 3


 



extern int pthread_attr_setstackaddr (pthread_attr_t *__attr,
				      void *__stackaddr) throw () ;

 
extern int pthread_attr_getstackaddr (__const pthread_attr_t *__restrict
				      __attr, void **__restrict __stackaddr)
     throw () ;

# 279 "/usr/include/pthread.h" 3


 


extern int pthread_attr_setstacksize (pthread_attr_t *__attr,
				      size_t __stacksize) throw () ;

 
extern int pthread_attr_getstacksize (__const pthread_attr_t *__restrict
				      __attr, size_t *__restrict __stacksize)
     throw () ;






 

 

extern int pthread_setschedparam (pthread_t __target_thread, int __policy,
				  __const struct sched_param *__param)
     throw () ;

 
extern int pthread_getschedparam (pthread_t __target_thread,
				  int *__restrict __policy,
				  struct sched_param *__restrict __param)
     throw () ;

















 

 

extern int pthread_mutex_init (pthread_mutex_t *__restrict __mutex,
			       __const pthread_mutexattr_t *__restrict
			       __mutex_attr) throw () ;

 
extern int pthread_mutex_destroy (pthread_mutex_t *__mutex) throw () ;

 
extern int pthread_mutex_trylock (pthread_mutex_t *__mutex) throw () ;

 
extern int pthread_mutex_lock (pthread_mutex_t *__mutex) throw () ;








 
extern int pthread_mutex_unlock (pthread_mutex_t *__mutex) throw () ;


 

 

extern int pthread_mutexattr_init (pthread_mutexattr_t *__attr) throw () ;

 
extern int pthread_mutexattr_destroy (pthread_mutexattr_t *__attr) throw () ;

 
extern int pthread_mutexattr_getpshared (__const pthread_mutexattr_t *
					 __restrict __attr,
					 int *__restrict __pshared) throw () ;

 
extern int pthread_mutexattr_setpshared (pthread_mutexattr_t *__attr,
					 int __pshared) throw () ;

# 383 "/usr/include/pthread.h" 3



 

 

extern int pthread_cond_init (pthread_cond_t *__restrict __cond,
			      __const pthread_condattr_t *__restrict
			      __cond_attr) throw () ;

 
extern int pthread_cond_destroy (pthread_cond_t *__cond) throw () ;

 
extern int pthread_cond_signal (pthread_cond_t *__cond) throw () ;

 
extern int pthread_cond_broadcast (pthread_cond_t *__cond) throw () ;

 

extern int pthread_cond_wait (pthread_cond_t *__restrict __cond,
			      pthread_mutex_t *__restrict __mutex) throw () ;

 



extern int pthread_cond_timedwait (pthread_cond_t *__restrict __cond,
				   pthread_mutex_t *__restrict __mutex,
				   __const struct timespec *__restrict
				   __abstime) throw () ;

 

 
extern int pthread_condattr_init (pthread_condattr_t *__attr) throw () ;

 
extern int pthread_condattr_destroy (pthread_condattr_t *__attr) throw () ;

 
extern int pthread_condattr_getpshared (__const pthread_condattr_t *
					__restrict __attr,
					int *__restrict __pshared) throw () ;

 
extern int pthread_condattr_setpshared (pthread_condattr_t *__attr,
					int __pshared) throw () ;


# 501 "/usr/include/pthread.h" 3


# 545 "/usr/include/pthread.h" 3



 

 





extern int pthread_key_create (pthread_key_t *__key,
			       void (*__destr_function) (void *)) throw () ;

 
extern int pthread_key_delete (pthread_key_t __key) throw () ;

 
extern int pthread_setspecific (pthread_key_t __key,
				__const void *__pointer) throw () ;

 
extern void *pthread_getspecific (pthread_key_t __key) throw () ;


 

 



extern int pthread_once (pthread_once_t *__once_control,
			 void (*__init_routine) (void)) throw () ;


 

 

extern int pthread_setcancelstate (int __state, int *__oldstate) throw () ;

 

extern int pthread_setcanceltype (int __type, int *__oldtype) throw () ;

 
extern int pthread_cancel (pthread_t __thread) throw () ;

 


extern void pthread_testcancel (void) throw () ;


 










extern void _pthread_cleanup_push (struct _pthread_cleanup_buffer *__buffer,
				   void (*__routine) (void *),
				   void *__arg) throw () ;

 





extern void _pthread_cleanup_pop (struct _pthread_cleanup_buffer *__buffer,
				  int __execute) throw () ;

 


# 644 "/usr/include/pthread.h" 3










 
# 1 "/usr/include/bits/sigthread.h" 1 3
 

























 

 

extern int pthread_sigmask (int __how,
			    __const __sigset_t *__restrict __newmask,
			    __sigset_t *__restrict __oldmask)throw () ;

 
extern int pthread_kill (pthread_t __thread, int __signo) throw () ;


# 655 "/usr/include/pthread.h" 2 3



 

 










extern int pthread_atfork (void (*__prepare) (void),
			   void (*__parent) (void),
			   void (*__child) (void)) throw () ;

 


extern void pthread_kill_other_threads_np (void) throw () ;

} 


# 71 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_alloc.h" 2 3








# 87 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_alloc.h" 3

# 97 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_alloc.h" 3

# 115 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_alloc.h" 3









 





 
 









template <int __inst>
class __malloc_alloc_template {

private:

  static void* _S_oom_malloc(size_t);
  static void* _S_oom_realloc(void*, size_t);


  static void (* __malloc_alloc_oom_handler)();


public:

  static void* allocate(size_t __n)
  {
    void* __result = malloc(__n);
    if (0 == __result) __result = _S_oom_malloc(__n);
    return __result;
  }

  static void deallocate(void* __p, size_t  )
  {
    free(__p);
  }

  static void* reallocate(void* __p, size_t  , size_t __new_sz)
  {
    void* __result = realloc(__p, __new_sz);
    if (0 == __result) __result = _S_oom_realloc(__p, __new_sz);
    return __result;
  }

  static void (* __set_malloc_handler(void (*__f)()))()
  {
    void (* __old)() = __malloc_alloc_oom_handler;
    __malloc_alloc_oom_handler = __f;
    return(__old);
  }

};

 


template <int __inst>
void (* __malloc_alloc_template<__inst>::__malloc_alloc_oom_handler)() = 0;


template <int __inst>
void*
__malloc_alloc_template<__inst>::_S_oom_malloc(size_t __n)
{
    void (* __my_malloc_handler)();
    void* __result;

    for (;;) {
        __my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == __my_malloc_handler) { throw bad_alloc() ; }
        (*__my_malloc_handler)();
        __result = malloc(__n);
        if (__result) return(__result);
    }
}

template <int __inst>
void* __malloc_alloc_template<__inst>::_S_oom_realloc(void* __p, size_t __n)
{
    void (* __my_malloc_handler)();
    void* __result;

    for (;;) {
        __my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == __my_malloc_handler) { throw bad_alloc() ; }
        (*__my_malloc_handler)();
        __result = realloc(__p, __n);
        if (__result) return(__result);
    }
}

typedef __malloc_alloc_template<0> malloc_alloc;

template<class _Tp, class _Alloc>
class simple_alloc {

public:
    static _Tp* allocate(size_t __n)
      { return 0 == __n ? 0 : (_Tp*) _Alloc::allocate(__n * sizeof (_Tp)); }
    static _Tp* allocate(void)
      { return (_Tp*) _Alloc::allocate(sizeof (_Tp)); }
    static void deallocate(_Tp* __p, size_t __n)
      { if (0 != __n) _Alloc::deallocate(__p, __n * sizeof (_Tp)); }
    static void deallocate(_Tp* __p)
      { _Alloc::deallocate(__p, sizeof (_Tp)); }
};

 
 
 
 
 
template <class _Alloc>
class debug_alloc {

private:

  enum {_S_extra = 8};   
                         
                         

public:

  static void* allocate(size_t __n)
  {
    char* __result = (char*)_Alloc::allocate(__n + _S_extra);
    *(size_t*)__result = __n;
    return __result + _S_extra;
  }

  static void deallocate(void* __p, size_t __n)
  {
    char* __real_p = (char*)__p - _S_extra;
    (static_cast<void>  (( *(size_t*)__real_p == __n ) ? 0 :	(__assert_fail ("*(size_t*)__real_p == __n" , "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_alloc.h", 263, __PRETTY_FUNCTION__ ), 0))) ;
    _Alloc::deallocate(__real_p, __n + _S_extra);
  }

  static void* reallocate(void* __p, size_t __old_sz, size_t __new_sz)
  {
    char* __real_p = (char*)__p - _S_extra;
    (static_cast<void>  (( *(size_t*)__real_p == __old_sz ) ? 0 :	(__assert_fail ("*(size_t*)__real_p == __old_sz" , "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_alloc.h", 270, __PRETTY_FUNCTION__ ), 0))) ;
    char* __result = (char*)
      _Alloc::reallocate(__real_p, __old_sz + _S_extra, __new_sz + _S_extra);
    *(size_t*)__result = __new_sz;
    return __result + _S_extra;
  }

};










 
 
 
 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 







template <bool threads, int inst>
class __default_alloc_template {

private:
   
   

    enum {_ALIGN = 8};
    enum {_MAX_BYTES = 128};
    enum {_NFREELISTS = _MAX_BYTES/_ALIGN};

  static size_t
  _S_round_up(size_t __bytes)
    { return (((__bytes) + _ALIGN-1) & ~(_ALIGN - 1)); }

private :
  union _Obj {
        union _Obj* _M_free_list_link;
        char _M_client_data[1];     
  };
private:




    static _Obj* volatile  _S_free_list[_NFREELISTS];

  static  size_t _S_freelist_index(size_t __bytes) {
        return (((__bytes) + _ALIGN-1)/_ALIGN - 1);
  }

   
  static void* _S_refill(size_t __n);
   
   
  static char* _S_chunk_alloc(size_t __size, int& __nobjs);

   
  static char* _S_start_free;
  static char* _S_end_free;
  static size_t _S_heap_size;








    static pthread_mutex_t _S_node_allocator_lock;






# 389 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_alloc.h" 3


    class _Lock {
        public:
            _Lock() { if (threads) pthread_mutex_lock(&_S_node_allocator_lock) ; }
            ~_Lock() { if (threads) pthread_mutex_unlock(&_S_node_allocator_lock) ; }
    };
    friend class _Lock;

public:

   
  static void* allocate(size_t __n)
  {
    _Obj* volatile * __my_free_list;
    _Obj*   __result;

    if (__n > (size_t) _MAX_BYTES) {
        return(malloc_alloc::allocate(__n));
    }
    __my_free_list = _S_free_list + _S_freelist_index(__n);
     
     
     

         
        _Lock __lock_instance;

    __result = *__my_free_list;
    if (__result == 0) {
        void* __r = _S_refill(_S_round_up(__n));
        return __r;
    }
    *__my_free_list = __result -> _M_free_list_link;
    return (__result);
  };

   
  static void deallocate(void* __p, size_t __n)
  {
    _Obj* __q = (_Obj*)__p;
    _Obj* volatile * __my_free_list;

    if (__n > (size_t) _MAX_BYTES) {
        malloc_alloc::deallocate(__p, __n);
        return;
    }
    __my_free_list = _S_free_list + _S_freelist_index(__n);
     

         
        _Lock __lock_instance;

    __q -> _M_free_list_link = *__my_free_list;
    *__my_free_list = __q;
     
  }

  static void* reallocate(void* __p, size_t __old_sz, size_t __new_sz);

} ;

typedef __default_alloc_template< true , 0> alloc;
typedef __default_alloc_template<false, 0> single_client_alloc;



 
 
 
 
template <bool __threads, int __inst>
char*
__default_alloc_template<__threads, __inst>::_S_chunk_alloc(size_t __size,
                                                            int& __nobjs)
{
    char* __result;
    size_t __total_bytes = __size * __nobjs;
    size_t __bytes_left = _S_end_free - _S_start_free;

    if (__bytes_left >= __total_bytes) {
        __result = _S_start_free;
        _S_start_free += __total_bytes;
        return(__result);
    } else if (__bytes_left >= __size) {
        __nobjs = (int)(__bytes_left/__size);
        __total_bytes = __size * __nobjs;
        __result = _S_start_free;
        _S_start_free += __total_bytes;
        return(__result);
    } else {
        size_t __bytes_to_get =
	  2 * __total_bytes + _S_round_up(_S_heap_size >> 4);
         
        if (__bytes_left > 0) {
            _Obj* volatile * __my_free_list =
                        _S_free_list + _S_freelist_index(__bytes_left);

            ((_Obj*)_S_start_free) -> _M_free_list_link = *__my_free_list;
            *__my_free_list = (_Obj*)_S_start_free;
        }
        _S_start_free = (char*)malloc(__bytes_to_get);
        if (0 == _S_start_free) {
            size_t __i;
            _Obj* volatile * __my_free_list;
	    _Obj* __p;
             
             
             
            for (__i = __size; __i <= _MAX_BYTES; __i += _ALIGN) {
                __my_free_list = _S_free_list + _S_freelist_index(__i);
                __p = *__my_free_list;
                if (0 != __p) {
                    *__my_free_list = __p -> _M_free_list_link;
                    _S_start_free = (char*)__p;
                    _S_end_free = _S_start_free + __i;
                    return(_S_chunk_alloc(__size, __nobjs));
                     
                     
                }
            }
	    _S_end_free = 0;	 
            _S_start_free = (char*)malloc_alloc::allocate(__bytes_to_get);
             
             
             
        }
        _S_heap_size += __bytes_to_get;
        _S_end_free = _S_start_free + __bytes_to_get;
        return(_S_chunk_alloc(__size, __nobjs));
    }
}


 
 
 
template <bool __threads, int __inst>
void*
__default_alloc_template<__threads, __inst>::_S_refill(size_t __n)
{
    int __nobjs = 20;
    char* __chunk = _S_chunk_alloc(__n, __nobjs);
    _Obj* volatile * __my_free_list;
    _Obj* __result;
    _Obj* __current_obj;
    _Obj* __next_obj;
    int __i;

    if (1 == __nobjs) return(__chunk);
    __my_free_list = _S_free_list + _S_freelist_index(__n);

     
      __result = (_Obj*)__chunk;
      *__my_free_list = __next_obj = (_Obj*)(__chunk + __n);
      for (__i = 1; ; __i++) {
        __current_obj = __next_obj;
        __next_obj = (_Obj*)((char*)__next_obj + __n);
        if (__nobjs - 1 == __i) {
            __current_obj -> _M_free_list_link = 0;
            break;
        } else {
            __current_obj -> _M_free_list_link = __next_obj;
        }
      }
    return(__result);
}

template <bool threads, int inst>
void*
__default_alloc_template<threads, inst>::reallocate(void* __p,
                                                    size_t __old_sz,
                                                    size_t __new_sz)
{
    void* __result;
    size_t __copy_sz;

    if (__old_sz > (size_t) _MAX_BYTES && __new_sz > (size_t) _MAX_BYTES) {
        return(realloc(__p, __new_sz));
    }
    if (_S_round_up(__old_sz) == _S_round_up(__new_sz)) return(__p);
    __result = allocate(__new_sz);
    __copy_sz = __new_sz > __old_sz? __old_sz : __new_sz;
    memcpy(__result, __p, __copy_sz);
    deallocate(__p, __old_sz);
    return(__result);
}


    template <bool __threads, int __inst>
    pthread_mutex_t
    __default_alloc_template<__threads, __inst>::_S_node_allocator_lock
        = {0, 0, 0, PTHREAD_MUTEX_TIMED_NP, { 0, 0 } } ;









# 602 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_alloc.h" 3


# 689 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_alloc.h" 3


template <bool __threads, int __inst>
char* __default_alloc_template<__threads, __inst>::_S_start_free = 0;

template <bool __threads, int __inst>
char* __default_alloc_template<__threads, __inst>::_S_end_free = 0;

template <bool __threads, int __inst>
size_t __default_alloc_template<__threads, __inst>::_S_heap_size = 0;

template <bool __threads, int __inst>
__default_alloc_template<__threads, __inst>::_Obj* volatile 
__default_alloc_template<__threads, __inst> ::_S_free_list[
    _NFREELISTS
] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
 
 
 










 
 
 
 
 
 
 



template <class _Tp>
class allocator {
  typedef alloc _Alloc;           
public:
  typedef size_t     size_type;
  typedef ptrdiff_t  difference_type;
  typedef _Tp*       pointer;
  typedef const _Tp* const_pointer;
  typedef _Tp&       reference;
  typedef const _Tp& const_reference;
  typedef _Tp        value_type;

  template <class _Tp1> struct rebind {
    typedef allocator<_Tp1> other;
  };

  allocator() throw()  {}
  allocator(const allocator&) throw()  {}
  template <class _Tp1> allocator(const allocator<_Tp1>&) throw()  {}
  ~allocator() throw()  {}

  pointer address(reference __x) const { return &__x; }
  const_pointer address(const_reference __x) const { return &__x; }

   
   
  _Tp* allocate(size_type __n, const void* = 0) {
    return __n != 0 ? static_cast<_Tp*>(_Alloc::allocate(__n * sizeof(_Tp)))
                    : 0;
  }

   
  void deallocate(pointer __p, size_type __n)
    { _Alloc::deallocate(__p, __n * sizeof(_Tp)); }

  size_type max_size() const throw() 
    { return size_t(-1) / sizeof(_Tp); }

  void construct(pointer __p, const _Tp& __val) { new(__p) _Tp(__val); }
  void destroy(pointer __p) { __p->~_Tp(); }
};

template<>
class allocator<void> {
  typedef size_t      size_type;
  typedef ptrdiff_t   difference_type;
  typedef void*       pointer;
  typedef const void* const_pointer;
  typedef void        value_type;

  template <class _Tp1> struct rebind {
    typedef allocator<_Tp1> other;
  };
};


template <class _T1, class _T2>
inline bool operator==(const allocator<_T1>&, const allocator<_T2>&)
{
  return true;
}

template <class _T1, class _T2>
inline bool operator!=(const allocator<_T1>&, const allocator<_T2>&)
{
  return false;
}

 
 
 
 
 
 

template <class _Tp, class _Alloc>
struct __allocator {
  _Alloc __underlying_alloc;

  typedef size_t    size_type;
  typedef ptrdiff_t difference_type;
  typedef _Tp*       pointer;
  typedef const _Tp* const_pointer;
  typedef _Tp&       reference;
  typedef const _Tp& const_reference;
  typedef _Tp        value_type;

  template <class _Tp1> struct rebind {
    typedef __allocator<_Tp1, _Alloc> other;
  };

  __allocator() throw()  {}
  __allocator(const __allocator& __a) throw() 
    : __underlying_alloc(__a.__underlying_alloc) {}
  template <class _Tp1>
  __allocator(const __allocator<_Tp1, _Alloc>& __a) throw() 
    : __underlying_alloc(__a.__underlying_alloc) {}
  ~__allocator() throw()  {}

  pointer address(reference __x) const { return &__x; }
  const_pointer address(const_reference __x) const { return &__x; }

   
  _Tp* allocate(size_type __n, const void* = 0) {
    return __n != 0
        ? static_cast<_Tp*>(__underlying_alloc.allocate(__n * sizeof(_Tp)))
        : 0;
  }

   
  void deallocate(pointer __p, size_type __n)
    { __underlying_alloc.deallocate(__p, __n * sizeof(_Tp)); }

  size_type max_size() const throw() 
    { return size_t(-1) / sizeof(_Tp); }

  void construct(pointer __p, const _Tp& __val) { new(__p) _Tp(__val); }
  void destroy(pointer __p) { __p->~_Tp(); }
};

template <class _Alloc>
class __allocator<void, _Alloc> {
  typedef size_t      size_type;
  typedef ptrdiff_t   difference_type;
  typedef void*       pointer;
  typedef const void* const_pointer;
  typedef void        value_type;

  template <class _Tp1> struct rebind {
    typedef __allocator<_Tp1, _Alloc> other;
  };
};

template <class _Tp, class _Alloc>
inline bool operator==(const __allocator<_Tp, _Alloc>& __a1,
                       const __allocator<_Tp, _Alloc>& __a2)
{
  return __a1.__underlying_alloc == __a2.__underlying_alloc;
}


template <class _Tp, class _Alloc>
inline bool operator!=(const __allocator<_Tp, _Alloc>& __a1,
                       const __allocator<_Tp, _Alloc>& __a2)
{
  return __a1.__underlying_alloc != __a2.__underlying_alloc;
}


 
 
 

template <int inst>
inline bool operator==(const __malloc_alloc_template<inst>&,
                       const __malloc_alloc_template<inst>&)
{
  return true;
}


template <int __inst>
inline bool operator!=(const __malloc_alloc_template<__inst>&,
                       const __malloc_alloc_template<__inst>&)
{
  return false;
}



template <bool __threads, int __inst>
inline bool operator==(const __default_alloc_template<__threads, __inst>&,
                       const __default_alloc_template<__threads, __inst>&)
{
  return true;
}


template <bool __threads, int __inst>
inline bool operator!=(const __default_alloc_template<__threads, __inst>&,
                       const __default_alloc_template<__threads, __inst>&)
{
  return false;
}



template <class _Alloc>
inline bool operator==(const debug_alloc<_Alloc>&,
                       const debug_alloc<_Alloc>&) {
  return true;
}


template <class _Alloc>
inline bool operator!=(const debug_alloc<_Alloc>&,
                       const debug_alloc<_Alloc>&) {
  return false;
}


 
 
 
 
 
 

 
 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 

 

template <class _Tp, class _Allocator>
struct _Alloc_traits
{
  static const bool _S_instanceless = false;
  typedef typename _Allocator::   rebind<_Tp>::other
          allocator_type;
};

template <class _Tp, class _Allocator>
const bool _Alloc_traits<_Tp, _Allocator>::_S_instanceless;

 

template <class _Tp, class _Tp1>
struct _Alloc_traits<_Tp, allocator<_Tp1> >
{
  static const bool _S_instanceless = true;
  typedef simple_alloc<_Tp, alloc> _Alloc_type;
  typedef allocator<_Tp> allocator_type;
};

 

template <class _Tp, int __inst>
struct _Alloc_traits<_Tp, __malloc_alloc_template<__inst> >
{
  static const bool _S_instanceless = true;
  typedef simple_alloc<_Tp, __malloc_alloc_template<__inst> > _Alloc_type;
  typedef __allocator<_Tp, __malloc_alloc_template<__inst> > allocator_type;
};


template <class _Tp, bool __threads, int __inst>
struct _Alloc_traits<_Tp, __default_alloc_template<__threads, __inst> >
{
  static const bool _S_instanceless = true;
  typedef simple_alloc<_Tp, __default_alloc_template<__threads, __inst> >
          _Alloc_type;
  typedef __allocator<_Tp, __default_alloc_template<__threads, __inst> >
          allocator_type;
};


template <class _Tp, class _Alloc>
struct _Alloc_traits<_Tp, debug_alloc<_Alloc> >
{
  static const bool _S_instanceless = true;
  typedef simple_alloc<_Tp, debug_alloc<_Alloc> > _Alloc_type;
  typedef __allocator<_Tp, debug_alloc<_Alloc> > allocator_type;
};

 
 

template <class _Tp, class _Tp1, int __inst>
struct _Alloc_traits<_Tp,
                     __allocator<_Tp1, __malloc_alloc_template<__inst> > >
{
  static const bool _S_instanceless = true;
  typedef simple_alloc<_Tp, __malloc_alloc_template<__inst> > _Alloc_type;
  typedef __allocator<_Tp, __malloc_alloc_template<__inst> > allocator_type;
};


template <class _Tp, class _Tp1, bool __thr, int __inst>
struct _Alloc_traits<_Tp,
                      __allocator<_Tp1,
                                  __default_alloc_template<__thr, __inst> > >
{
  static const bool _S_instanceless = true;
  typedef simple_alloc<_Tp, __default_alloc_template<__thr,__inst> >
          _Alloc_type;
  typedef __allocator<_Tp, __default_alloc_template<__thr,__inst> >
          allocator_type;
};


template <class _Tp, class _Tp1, class _Alloc>
struct _Alloc_traits<_Tp, __allocator<_Tp1, debug_alloc<_Alloc> > >
{
  static const bool _S_instanceless = true;
  typedef simple_alloc<_Tp, debug_alloc<_Alloc> > _Alloc_type;
  typedef __allocator<_Tp, debug_alloc<_Alloc> > allocator_type;
};








 





 
 
 
# 31 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/vector" 2 3

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_construct.h" 1 3
 

























 








 

 
 

template <class _Tp>
inline void destroy(_Tp* __pointer) {
  __pointer->~_Tp();
}

template <class _T1, class _T2>
inline void construct(_T1* __p, const _T2& __value) {
  new (__p) _T1(__value);
}

template <class _T1>
inline void construct(_T1* __p) {
  new (__p) _T1();
}

template <class _ForwardIterator>
inline void
__destroy_aux(_ForwardIterator __first, _ForwardIterator __last, __false_type)
{
  for ( ; __first != __last; ++__first)
    destroy(&*__first);
}

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, __true_type) {}

template <class _ForwardIterator, class _Tp>
inline void
__destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
  typedef typename __type_traits<_Tp>::has_trivial_destructor
          _Trivial_destructor;
  __destroy_aux(__first, __last, _Trivial_destructor());
}

template <class _ForwardIterator>
inline void destroy(_ForwardIterator __first, _ForwardIterator __last) {
  __destroy(__first, __last, __value_type( __first ) );
}

inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}

 



 
 
 
# 32 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/vector" 2 3

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_uninitialized.h" 1 3
 

























 






 

 

 
 
template <class _InputIter, class _ForwardIter>
inline _ForwardIter 
__uninitialized_copy_aux(_InputIter __first, _InputIter __last,
                         _ForwardIter __result,
                         __true_type)
{
  return copy(__first, __last, __result);
}

template <class _InputIter, class _ForwardIter>
_ForwardIter 
__uninitialized_copy_aux(_InputIter __first, _InputIter __last,
                         _ForwardIter __result,
                         __false_type)
{
  _ForwardIter __cur = __result;
  try  {
    for ( ; __first != __last; ++__first, ++__cur)
      construct(&*__cur, *__first);
    return __cur;
  }
  catch(...) {  destroy(__result, __cur) ; throw; } ;
}


template <class _InputIter, class _ForwardIter, class _Tp>
inline _ForwardIter
__uninitialized_copy(_InputIter __first, _InputIter __last,
                     _ForwardIter __result, _Tp*)
{
  typedef typename __type_traits<_Tp>::is_POD_type _Is_POD;
  return __uninitialized_copy_aux(__first, __last, __result, _Is_POD());
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter
  uninitialized_copy(_InputIter __first, _InputIter __last,
                     _ForwardIter __result)
{
  return __uninitialized_copy(__first, __last, __result,
                              __value_type( __result ) );
}

inline char* uninitialized_copy(const char* __first, const char* __last,
                                char* __result) {
  memmove(__result, __first, __last - __first);
  return __result + (__last - __first);
}

inline wchar_t* 
uninitialized_copy(const wchar_t* __first, const wchar_t* __last,
                   wchar_t* __result)
{
  memmove(__result, __first, sizeof(wchar_t) * (__last - __first));
  return __result + (__last - __first);
}

 

template <class _InputIter, class _Size, class _ForwardIter>
pair<_InputIter, _ForwardIter>
__uninitialized_copy_n(_InputIter __first, _Size __count,
                       _ForwardIter __result,
                       input_iterator_tag)
{
  _ForwardIter __cur = __result;
  try  {
    for ( ; __count > 0 ; --__count, ++__first, ++__cur) 
      construct(&*__cur, *__first);
    return pair<_InputIter, _ForwardIter>(__first, __cur);
  }
  catch(...) {  destroy(__result, __cur) ; throw; } ;
}

template <class _RandomAccessIter, class _Size, class _ForwardIter>
inline pair<_RandomAccessIter, _ForwardIter>
__uninitialized_copy_n(_RandomAccessIter __first, _Size __count,
                       _ForwardIter __result,
                       random_access_iterator_tag) {
  _RandomAccessIter __last = __first + __count;
  return pair<_RandomAccessIter, _ForwardIter>(
                 __last,
                 uninitialized_copy(__first, __last, __result));
}

template <class _InputIter, class _Size, class _ForwardIter>
inline pair<_InputIter, _ForwardIter>
__uninitialized_copy_n(_InputIter __first, _Size __count,
                     _ForwardIter __result) {
  return __uninitialized_copy_n(__first, __count, __result,
                                __iterator_category( __first ) );
}

template <class _InputIter, class _Size, class _ForwardIter>
inline pair<_InputIter, _ForwardIter>
uninitialized_copy_n(_InputIter __first, _Size __count,
                     _ForwardIter __result) {
  return __uninitialized_copy_n(__first, __count, __result,
                                __iterator_category( __first ) );
}

 
 
template <class _ForwardIter, class _Tp>
inline void
__uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last, 
                         const _Tp& __x, __true_type)
{
  fill(__first, __last, __x);
}

template <class _ForwardIter, class _Tp>
void
__uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last, 
                         const _Tp& __x, __false_type)
{
  _ForwardIter __cur = __first;
  try  {
    for ( ; __cur != __last; ++__cur)
      construct(&*__cur, __x);
  }
  catch(...) {  destroy(__first, __cur) ; throw; } ;
}

template <class _ForwardIter, class _Tp, class _Tp1>
inline void __uninitialized_fill(_ForwardIter __first, 
                                 _ForwardIter __last, const _Tp& __x, _Tp1*)
{
  typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
  __uninitialized_fill_aux(__first, __last, __x, _Is_POD());
                   
}

template <class _ForwardIter, class _Tp>
inline void uninitialized_fill(_ForwardIter __first,
                               _ForwardIter __last, 
                               const _Tp& __x)
{
  __uninitialized_fill(__first, __last, __x, __value_type( __first ) );
}

 
 
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter
__uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
                           const _Tp& __x, __true_type)
{
  return fill_n(__first, __n, __x);
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter
__uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
                           const _Tp& __x, __false_type)
{
  _ForwardIter __cur = __first;
  try  {
    for ( ; __n > 0; --__n, ++__cur)
      construct(&*__cur, __x);
    return __cur;
  }
  catch(...) {  destroy(__first, __cur) ; throw; } ;
}

template <class _ForwardIter, class _Size, class _Tp, class _Tp1>
inline _ForwardIter 
__uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, _Tp1*)
{
  typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
  return __uninitialized_fill_n_aux(__first, __n, __x, _Is_POD());
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter 
uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
  return __uninitialized_fill_n(__first, __n, __x, __value_type( __first ) );
}

 
 

 
 
 
 

template <class _InputIter1, class _InputIter2, class _ForwardIter>
inline _ForwardIter
__uninitialized_copy_copy(_InputIter1 __first1, _InputIter1 __last1,
                          _InputIter2 __first2, _InputIter2 __last2,
                          _ForwardIter __result)
{
  _ForwardIter __mid = uninitialized_copy(__first1, __last1, __result);
  try  {
    return uninitialized_copy(__first2, __last2, __mid);
  }
  catch(...) {  destroy(__result, __mid) ; throw; } ;
}

 
 
 
template <class _ForwardIter, class _Tp, class _InputIter>
inline _ForwardIter 
__uninitialized_fill_copy(_ForwardIter __result, _ForwardIter __mid,
                          const _Tp& __x,
                          _InputIter __first, _InputIter __last)
{
  uninitialized_fill(__result, __mid, __x);
  try  {
    return uninitialized_copy(__first, __last, __mid);
  }
  catch(...) {  destroy(__result, __mid) ; throw; } ;
}

 
 
 
template <class _InputIter, class _ForwardIter, class _Tp>
inline void
__uninitialized_copy_fill(_InputIter __first1, _InputIter __last1,
                          _ForwardIter __first2, _ForwardIter __last2,
                          const _Tp& __x)
{
  _ForwardIter __mid2 = uninitialized_copy(__first1, __last1, __first2);
  try  {
    uninitialized_fill(__mid2, __last2, __x);
  }
  catch(...) {  destroy(__first2, __mid2) ; throw; } ;
}

 



 
 
 
# 33 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/vector" 2 3

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_vector.h" 1 3
 

























 






  






 
 
 
 
 



 
template <class _Tp, class _Allocator, bool _IsStatic>
class _Vector_alloc_base {
public:
  typedef typename _Alloc_traits<_Tp, _Allocator>::allocator_type
          allocator_type;
  allocator_type get_allocator() const { return _M_data_allocator; }

  _Vector_alloc_base(const allocator_type& __a)
    : _M_data_allocator(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0) 
  {}
  
protected:
  allocator_type _M_data_allocator;
  _Tp* _M_start;
  _Tp* _M_finish;
  _Tp* _M_end_of_storage;

  _Tp* _M_allocate(size_t __n)
    { return _M_data_allocator.allocate(__n); }
  void _M_deallocate(_Tp* __p, size_t __n)
    { if (__p) _M_data_allocator.deallocate(__p, __n); }
};

 
 
template <class _Tp, class _Allocator>
class _Vector_alloc_base<_Tp, _Allocator, true> {
public:
  typedef typename _Alloc_traits<_Tp, _Allocator>::allocator_type
          allocator_type;
  allocator_type get_allocator() const { return allocator_type(); }

  _Vector_alloc_base(const allocator_type&)
    : _M_start(0), _M_finish(0), _M_end_of_storage(0) 
  {}
  
protected:
  _Tp* _M_start;
  _Tp* _M_finish;
  _Tp* _M_end_of_storage;

  typedef typename _Alloc_traits<_Tp, _Allocator>::_Alloc_type _Alloc_type;
  _Tp* _M_allocate(size_t __n)
    { return _Alloc_type::allocate(__n); }
  void _M_deallocate(_Tp* __p, size_t __n)
    { _Alloc_type::deallocate(__p, __n);}
};

template <class _Tp, class _Alloc>
struct _Vector_base
  : public _Vector_alloc_base<_Tp, _Alloc,
                              _Alloc_traits<_Tp, _Alloc>::_S_instanceless>
{
  typedef _Vector_alloc_base<_Tp, _Alloc, 
                             _Alloc_traits<_Tp, _Alloc>::_S_instanceless>
          _Base;
  typedef typename _Base::allocator_type allocator_type;

  _Vector_base(const allocator_type& __a) : _Base(__a) {}
  _Vector_base(size_t __n, const allocator_type& __a) : _Base(__a) {
    _M_start = _M_allocate(__n);
    _M_finish = _M_start;
    _M_end_of_storage = _M_start + __n;
  }

  ~_Vector_base() { _M_deallocate(_M_start, _M_end_of_storage - _M_start); }
};    

# 150 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_vector.h" 3


template <class _Tp, class _Alloc = allocator< _Tp >  >
class vector : protected _Vector_base<_Tp, _Alloc> 
{
private:
  typedef _Vector_base<_Tp, _Alloc> _Base;
public:
  typedef _Tp value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type* iterator;
  typedef const value_type* const_iterator;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef typename _Base::allocator_type allocator_type;
  allocator_type get_allocator() const { return _Base::get_allocator(); }


  typedef reverse_iterator<const_iterator> const_reverse_iterator;
  typedef reverse_iterator<iterator> reverse_iterator;







protected:

  using _Base::_M_allocate;
  using _Base::_M_deallocate;
  using _Base::_M_start;
  using _Base::_M_finish;
  using _Base::_M_end_of_storage;


protected:
  void _M_insert_aux(iterator __position, const _Tp& __x);
  void _M_insert_aux(iterator __position);

public:
  iterator begin() { return _M_start; }
  const_iterator begin() const { return _M_start; }
  iterator end() { return _M_finish; }
  const_iterator end() const { return _M_finish; }

  reverse_iterator rbegin()
    { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const
    { return const_reverse_iterator(end()); }
  reverse_iterator rend()
    { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const
    { return const_reverse_iterator(begin()); }

  size_type size() const
    { return size_type(end() - begin()); }
  size_type max_size() const
    { return size_type(-1) / sizeof(_Tp); }
  size_type capacity() const
    { return size_type(_M_end_of_storage - begin()); }
  bool empty() const
    { return begin() == end(); }

  reference operator[](size_type __n) { return *(begin() + __n); }
  const_reference operator[](size_type __n) const { return *(begin() + __n); }

  explicit vector(const allocator_type& __a = allocator_type())
    : _Base(__a) {}

  vector(size_type __n, const _Tp& __value,
         const allocator_type& __a = allocator_type()) 
    : _Base(__n, __a)
    { _M_finish = uninitialized_fill_n(_M_start, __n, __value); }

  explicit vector(size_type __n)
    : _Base(__n, allocator_type())
    { _M_finish = uninitialized_fill_n(_M_start, __n, _Tp()); }

  vector(const vector<_Tp, _Alloc>& __x) 
    : _Base(__x.size(), __x.get_allocator())
    { _M_finish = uninitialized_copy(__x.begin(), __x.end(), _M_start); }


   
  template <class _InputIterator>
  vector(_InputIterator __first, _InputIterator __last,
         const allocator_type& __a = allocator_type()) : _Base(__a) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_initialize_aux(__first, __last, _Integral());
  }

  template <class _Integer>
  void _M_initialize_aux(_Integer __n, _Integer __value, __true_type) {
    _M_start = _M_allocate(__n);
    _M_end_of_storage = _M_start + __n; 
    _M_finish = uninitialized_fill_n(_M_start, __n, __value);
  }

  template <class _InputIterator>
  void _M_initialize_aux(_InputIterator __first, _InputIterator __last,
                         __false_type) {
    _M_range_initialize(__first, __last, __iterator_category( __first ) );
  }








  ~vector() { destroy(_M_start, _M_finish); }

  vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
  void reserve(size_type __n) {
    if (capacity() < __n) {
      const size_type __old_size = size();
      iterator __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
      destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __tmp;
      _M_finish = __tmp + __old_size;
      _M_end_of_storage = _M_start + __n;
    }
  }

   
   
   
   

  void assign(size_type __n, const _Tp& __val);


  
  template <class _InputIterator>
  void assign(_InputIterator __first, _InputIterator __last) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_assign_dispatch(__first, __last, _Integral());
  }

  template <class _Integer>
  void _M_assign_dispatch(_Integer __n, _Integer __val, __true_type)
    { assign((size_type) __n, (_Tp) __val); }

  template <class _InputIter>
  void _M_assign_dispatch(_InputIter __first, _InputIter __last, __false_type)
    { _M_assign_aux(__first, __last, __iterator_category( __first ) ); }

  template <class _InputIterator>
  void _M_assign_aux(_InputIterator __first, _InputIterator __last,
                     input_iterator_tag);

  template <class _ForwardIterator>
  void _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last,
                     forward_iterator_tag); 



  reference front() { return *begin(); }
  const_reference front() const { return *begin(); }
  reference back() { return *(end() - 1); }
  const_reference back() const { return *(end() - 1); }

  void push_back(const _Tp& __x) {
    if (_M_finish != _M_end_of_storage) {
      construct(_M_finish, __x);
      ++_M_finish;
    }
    else
      _M_insert_aux(end(), __x);
  }
  void push_back() {
    if (_M_finish != _M_end_of_storage) {
      construct(_M_finish);
      ++_M_finish;
    }
    else
      _M_insert_aux(end());
  }
  void swap(vector<_Tp, _Alloc>& __x) {
     ::swap(_M_start, __x._M_start);
     ::swap(_M_finish, __x._M_finish);
     ::swap(_M_end_of_storage, __x._M_end_of_storage);
  }

  iterator insert(iterator __position, const _Tp& __x) {
    size_type __n = __position - begin();
    if (_M_finish != _M_end_of_storage && __position == end()) {
      construct(_M_finish, __x);
      ++_M_finish;
    }
    else
      _M_insert_aux(__position, __x);
    return begin() + __n;
  }
  iterator insert(iterator __position) {
    size_type __n = __position - begin();
    if (_M_finish != _M_end_of_storage && __position == end()) {
      construct(_M_finish);
      ++_M_finish;
    }
    else
      _M_insert_aux(__position);
    return begin() + __n;
  }

   
  template <class _InputIterator>
  void insert(iterator __pos, _InputIterator __first, _InputIterator __last) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_insert_dispatch(__pos, __first, __last, _Integral());
  }

  template <class _Integer>
  void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val,
                          __true_type) {
    insert(__pos, (size_type) __n, (_Tp) __val);
  }

  template <class _InputIterator>
  void _M_insert_dispatch(iterator __pos,
                          _InputIterator __first, _InputIterator __last,
                          __false_type) {
    _M_range_insert(__pos, __first, __last, __iterator_category( __first ) );
  }





  void insert (iterator __pos, size_type __n, const _Tp& __x);

  void pop_back() {
    --_M_finish;
    destroy(_M_finish);
  }
  iterator erase(iterator __position) {
    if (__position + 1 != end())
      copy(__position + 1, _M_finish, __position);
    --_M_finish;
    destroy(_M_finish);
    return __position;
  }
  iterator erase(iterator __first, iterator __last) {
    iterator __i = copy(__last, _M_finish, __first);
    destroy(__i, _M_finish);
    _M_finish = _M_finish - (__last - __first);
    return __first;
  }

  void resize(size_type __new_size, const _Tp& __x) {
    if (__new_size < size()) 
      erase(begin() + __new_size, end());
    else
      insert(end(), __new_size - size(), __x);
  }
  void resize(size_type __new_size) { resize(__new_size, _Tp()); }
  void clear() { erase(begin(), end()); }

protected:


  template <class _ForwardIterator>
  iterator _M_allocate_and_copy(size_type __n, _ForwardIterator __first, 
                                               _ForwardIterator __last)
{
    iterator __result = _M_allocate(__n);
    try  {
      uninitialized_copy(__first, __last, __result);
      return __result;
    }
    catch(...) {  _M_deallocate(__result, __n) ; throw; } ;
  }
# 440 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_vector.h" 3




  template <class _InputIterator>
  void _M_range_initialize(_InputIterator __first,  
                           _InputIterator __last, input_iterator_tag)
  {
    for ( ; __first != __last; ++__first)
      push_back(*__first);
  }

   
  template <class _ForwardIterator>
  void _M_range_initialize(_ForwardIterator __first,
                           _ForwardIterator __last, forward_iterator_tag)
  {
    size_type __n = 0;
    distance(__first, __last, __n);
    _M_start = _M_allocate(__n);
    _M_end_of_storage = _M_start + __n;
    _M_finish = uninitialized_copy(__first, __last, _M_start);
  }

  template <class _InputIterator>
  void _M_range_insert(iterator __pos,
                       _InputIterator __first, _InputIterator __last,
                       input_iterator_tag);

  template <class _ForwardIterator>
  void _M_range_insert(iterator __pos,
                       _ForwardIterator __first, _ForwardIterator __last,
                       forward_iterator_tag);


};

template <class _Tp, class _Alloc>
inline bool 
operator==(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
{
  return __x.size() == __y.size() &&
         equal(__x.begin(), __x.end(), __y.begin());
}

template <class _Tp, class _Alloc>
inline bool 
operator<(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
{
  return lexicographical_compare(__x.begin(), __x.end(), 
                                 __y.begin(), __y.end());
}



template <class _Tp, class _Alloc>
inline void swap(vector<_Tp, _Alloc>& __x, vector<_Tp, _Alloc>& __y)
{
  __x.swap(__y);
}



template <class _Tp, class _Alloc>
vector<_Tp,_Alloc>& 
vector<_Tp,_Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
  if (&__x != this) {
    const size_type __xlen = __x.size();
    if (__xlen > capacity()) {
      iterator __tmp = _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
      destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __tmp;
      _M_end_of_storage = _M_start + __xlen;
    }
    else if (size() >= __xlen) {
      iterator __i = copy(__x.begin(), __x.end(), begin());
      destroy(__i, _M_finish);
    }
    else {
      copy(__x.begin(), __x.begin() + size(), _M_start);
      uninitialized_copy(__x.begin() + size(), __x.end(), _M_finish);
    }
    _M_finish = _M_start + __xlen;
  }
  return *this;
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::assign(size_t __n, const value_type& __val) {
  if (__n > capacity()) {
    vector<_Tp, _Alloc> __tmp(__n, __val, get_allocator());
    __tmp.swap(*this);
  }
  else if (__n > size()) {
    fill(begin(), end(), __val);
    _M_finish = uninitialized_fill_n(_M_finish, __n - size(), __val);
  }
  else
    erase(fill_n(begin(), __n, __val), end());
}



template <class _Tp, class _Alloc> template <class _InputIter>
void vector<_Tp, _Alloc>::_M_assign_aux(_InputIter __first, _InputIter __last,
                                        input_iterator_tag) {
  iterator __cur = begin();
  for ( ; __first != __last && __cur != end(); ++__cur, ++__first)
    *__cur = *__first;
  if (__first == __last)
    erase(__cur, end());
  else
    insert(end(), __first, __last);
}

template <class _Tp, class _Alloc> template <class _ForwardIter>
void
vector<_Tp, _Alloc>::_M_assign_aux(_ForwardIter __first, _ForwardIter __last,
                                   forward_iterator_tag) {
  size_type __len = 0;
  distance(__first, __last, __len);

  if (__len > capacity()) {
    iterator __tmp = _M_allocate_and_copy(__len, __first, __last);
    destroy(_M_start, _M_finish);
    _M_deallocate(_M_start, _M_end_of_storage - _M_start);
    _M_start = __tmp;
    _M_end_of_storage = _M_finish = _M_start + __len;
  }
  else if (size() >= __len) {
    iterator __new_finish = copy(__first, __last, _M_start);
    destroy(__new_finish, _M_finish);
    _M_finish = __new_finish;
  }
  else {
    _ForwardIter __mid = __first;
    advance(__mid, size());
    copy(__first, __mid, _M_start);
    _M_finish = uninitialized_copy(__mid, __last, _M_finish);
  }
}



template <class _Tp, class _Alloc>
void 
vector<_Tp, _Alloc>::_M_insert_aux(iterator __position, const _Tp& __x)
{
  if (_M_finish != _M_end_of_storage) {
    construct(_M_finish, *(_M_finish - 1));
    ++_M_finish;
    _Tp __x_copy = __x;
    copy_backward(__position, _M_finish - 2, _M_finish - 1);
    *__position = __x_copy;
  }
  else {
    const size_type __old_size = size();
    const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
    iterator __new_start = _M_allocate(__len);
    iterator __new_finish = __new_start;
    try  {
      __new_finish = uninitialized_copy(_M_start, __position, __new_start);
      construct(__new_finish, __x);
      ++__new_finish;
      __new_finish = uninitialized_copy(__position, _M_finish, __new_finish);
    }
    catch(...) {  (destroy(__new_start,__new_finish), 
                  _M_deallocate(__new_start,__len)) ; throw; } ;
    destroy(begin(), end());
    _M_deallocate(_M_start, _M_end_of_storage - _M_start);
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage = __new_start + __len;
  }
}

template <class _Tp, class _Alloc>
void 
vector<_Tp, _Alloc>::_M_insert_aux(iterator __position)
{
  if (_M_finish != _M_end_of_storage) {
    construct(_M_finish, *(_M_finish - 1));
    ++_M_finish;
    copy_backward(__position, _M_finish - 2, _M_finish - 1);
    *__position = _Tp();
  }
  else {
    const size_type __old_size = size();
    const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
    iterator __new_start = _M_allocate(__len);
    iterator __new_finish = __new_start;
    try  {
      __new_finish = uninitialized_copy(_M_start, __position, __new_start);
      construct(__new_finish);
      ++__new_finish;
      __new_finish = uninitialized_copy(__position, _M_finish, __new_finish);
    }
    catch(...) {  (destroy(__new_start,__new_finish), 
                  _M_deallocate(__new_start,__len)) ; throw; } ;
    destroy(begin(), end());
    _M_deallocate(_M_start, _M_end_of_storage - _M_start);
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage = __new_start + __len;
  }
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::insert(iterator __position, size_type __n, 
                                 const _Tp& __x)
{
  if (__n != 0) {
    if (size_type(_M_end_of_storage - _M_finish) >= __n) {
      _Tp __x_copy = __x;
      const size_type __elems_after = _M_finish - __position;
      iterator __old_finish = _M_finish;
      if (__elems_after > __n) {
        uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
        _M_finish += __n;
        copy_backward(__position, __old_finish - __n, __old_finish);
        fill(__position, __position + __n, __x_copy);
      }
      else {
        uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
        _M_finish += __n - __elems_after;
        uninitialized_copy(__position, __old_finish, _M_finish);
        _M_finish += __elems_after;
        fill(__position, __old_finish, __x_copy);
      }
    }
    else {
      const size_type __old_size = size();        
      const size_type __len = __old_size + max(__old_size, __n);
      iterator __new_start = _M_allocate(__len);
      iterator __new_finish = __new_start;
      try  {
        __new_finish = uninitialized_copy(_M_start, __position, __new_start);
        __new_finish = uninitialized_fill_n(__new_finish, __n, __x);
        __new_finish
          = uninitialized_copy(__position, _M_finish, __new_finish);
      }
      catch(...) {  (destroy(__new_start,__new_finish), 
                    _M_deallocate(__new_start,__len)) ; throw; } ;
      destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __new_start;
      _M_finish = __new_finish;
      _M_end_of_storage = __new_start + __len;
    }
  }
}



template <class _Tp, class _Alloc> template <class _InputIterator>
void 
vector<_Tp, _Alloc>::_M_range_insert(iterator __pos, 
                                     _InputIterator __first, 
                                     _InputIterator __last,
                                     input_iterator_tag)
{
  for ( ; __first != __last; ++__first) {
    __pos = insert(__pos, *__first);
    ++__pos;
  }
}

template <class _Tp, class _Alloc> template <class _ForwardIterator>
void 
vector<_Tp, _Alloc>::_M_range_insert(iterator __position,
                                     _ForwardIterator __first,
                                     _ForwardIterator __last,
                                     forward_iterator_tag)
{
  if (__first != __last) {
    size_type __n = 0;
    distance(__first, __last, __n);
    if (size_type(_M_end_of_storage - _M_finish) >= __n) {
      const size_type __elems_after = _M_finish - __position;
      iterator __old_finish = _M_finish;
      if (__elems_after > __n) {
        uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
        _M_finish += __n;
        copy_backward(__position, __old_finish - __n, __old_finish);
        copy(__first, __last, __position);
      }
      else {
        _ForwardIterator __mid = __first;
        advance(__mid, __elems_after);
        uninitialized_copy(__mid, __last, _M_finish);
        _M_finish += __n - __elems_after;
        uninitialized_copy(__position, __old_finish, _M_finish);
        _M_finish += __elems_after;
        copy(__first, __mid, __position);
      }
    }
    else {
      const size_type __old_size = size();
      const size_type __len = __old_size + max(__old_size, __n);
      iterator __new_start = _M_allocate(__len);
      iterator __new_finish = __new_start;
      try  {
        __new_finish = uninitialized_copy(_M_start, __position, __new_start);
        __new_finish = uninitialized_copy(__first, __last, __new_finish);
        __new_finish
          = uninitialized_copy(__position, _M_finish, __new_finish);
      }
      catch(...) {  (destroy(__new_start,__new_finish), 
                    _M_deallocate(__new_start,__len)) ; throw; } ;
      destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __new_start;
      _M_finish = __new_finish;
      _M_end_of_storage = __new_start + __len;
    }
  }
}

# 810 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_vector.h" 3







  



 
 
 
# 34 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/vector" 2 3

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_bvector.h" 1 3
 

























 






  

static const int __WORD_BIT = int(8 *sizeof(unsigned int));






struct _Bit_reference {
  unsigned int* _M_p;
  unsigned int _M_mask;
  _Bit_reference(unsigned int* __x, unsigned int __y) 
    : _M_p(__x), _M_mask(__y) {}

public:
  _Bit_reference() : _M_p(0), _M_mask(0) {}
  operator bool() const { return !(!(*_M_p & _M_mask)); }
  _Bit_reference& operator=(bool __x)
  {
    if (__x)  *_M_p |= _M_mask;
    else      *_M_p &= ~_M_mask;
    return *this;
  }
  _Bit_reference& operator=(const _Bit_reference& __x) 
    { return *this = bool(__x); }
  bool operator==(const _Bit_reference& __x) const
    { return bool(*this) == bool(__x); }
  bool operator<(const _Bit_reference& __x) const {
    return !bool(*this) && bool(__x);
  }
  void flip() { *_M_p ^= _M_mask; }
};

inline void swap(_Bit_reference __x, _Bit_reference __y)
{
  bool __tmp = __x;
  __x = __y;
  __y = __tmp;
}

struct _Bit_iterator : public random_access_iterator<bool, ptrdiff_t> {
  typedef _Bit_reference  reference;
  typedef _Bit_reference* pointer;
  typedef _Bit_iterator   iterator;

  unsigned int* _M_p;
  unsigned int _M_offset;
  void bump_up() {
    if (_M_offset++ == __WORD_BIT - 1) {
      _M_offset = 0;
      ++_M_p;
    }
  }
  void bump_down() {
    if (_M_offset-- == 0) {
      _M_offset = __WORD_BIT - 1;
      --_M_p;
    }
  }

  _Bit_iterator() : _M_p(0), _M_offset(0) {}
  _Bit_iterator(unsigned int* __x, unsigned int __y) 
    : _M_p(__x), _M_offset(__y) {}
  reference operator*() const { return reference(_M_p, 1U << _M_offset); }
  iterator& operator++() {
    bump_up();
    return *this;
  }
  iterator operator++(int) {
    iterator __tmp = *this;
    bump_up();
    return __tmp;
  }
  iterator& operator--() {
    bump_down();
    return *this;
  }
  iterator operator--(int) {
    iterator __tmp = *this;
    bump_down();
    return __tmp;
  }
  iterator& operator+=(difference_type __i) {
    difference_type __n = __i + _M_offset;
    _M_p += __n / __WORD_BIT;
    __n = __n % __WORD_BIT;
    if (__n < 0) {
      _M_offset = (unsigned int) __n + __WORD_BIT;
      --_M_p;
    } else
      _M_offset = (unsigned int) __n;
    return *this;
  }
  iterator& operator-=(difference_type __i) {
    *this += -__i;
    return *this;
  }
  iterator operator+(difference_type __i) const {
    iterator __tmp = *this;
    return __tmp += __i;
  }
  iterator operator-(difference_type __i) const {
    iterator __tmp = *this;
    return __tmp -= __i;
  }
  difference_type operator-(iterator __x) const {
    return __WORD_BIT * (_M_p - __x._M_p) + _M_offset - __x._M_offset;
  }
  reference operator[](difference_type __i) { return *(*this + __i); }
  bool operator==(const iterator& __x) const {
    return _M_p == __x._M_p && _M_offset == __x._M_offset;
  }
  bool operator!=(const iterator& __x) const {
    return _M_p != __x._M_p || _M_offset != __x._M_offset;
  }
  bool operator<(iterator __x) const {
    return _M_p < __x._M_p || (_M_p == __x._M_p && _M_offset < __x._M_offset);
  }
};

struct _Bit_const_iterator
  : public random_access_iterator<bool, ptrdiff_t>
{
  typedef bool                 reference;
  typedef bool                 const_reference;
  typedef const bool*          pointer;
  typedef _Bit_const_iterator  const_iterator;

  unsigned int* _M_p;
  unsigned int _M_offset;
  void bump_up() {
    if (_M_offset++ == __WORD_BIT - 1) {
      _M_offset = 0;
      ++_M_p;
    }
  }
  void bump_down() {
    if (_M_offset-- == 0) {
      _M_offset = __WORD_BIT - 1;
      --_M_p;
    }
  }

  _Bit_const_iterator() : _M_p(0), _M_offset(0) {}
  _Bit_const_iterator(unsigned int* __x, unsigned int __y) 
    : _M_p(__x), _M_offset(__y) {}
  _Bit_const_iterator(const _Bit_iterator& __x) 
    : _M_p(__x._M_p), _M_offset(__x._M_offset) {}
  const_reference operator*() const {
    return _Bit_reference(_M_p, 1U << _M_offset);
  }
  const_iterator& operator++() {
    bump_up();
    return *this;
  }
  const_iterator operator++(int) {
    const_iterator __tmp = *this;
    bump_up();
    return __tmp;
  }
  const_iterator& operator--() {
    bump_down();
    return *this;
  }
  const_iterator operator--(int) {
    const_iterator __tmp = *this;
    bump_down();
    return __tmp;
  }
  const_iterator& operator+=(difference_type __i) {
    difference_type __n = __i + _M_offset;
    _M_p += __n / __WORD_BIT;
    __n = __n % __WORD_BIT;
    if (__n < 0) {
      _M_offset = (unsigned int) __n + __WORD_BIT;
      --_M_p;
    } else
      _M_offset = (unsigned int) __n;
    return *this;
  }
  const_iterator& operator-=(difference_type __i) {
    *this += -__i;
    return *this;
  }
  const_iterator operator+(difference_type __i) const {
    const_iterator __tmp = *this;
    return __tmp += __i;
  }
  const_iterator operator-(difference_type __i) const {
    const_iterator __tmp = *this;
    return __tmp -= __i;
  }
  difference_type operator-(const_iterator __x) const {
    return __WORD_BIT * (_M_p - __x._M_p) + _M_offset - __x._M_offset;
  }
  const_reference operator[](difference_type __i) { 
    return *(*this + __i); 
  }
  bool operator==(const const_iterator& __x) const {
    return _M_p == __x._M_p && _M_offset == __x._M_offset;
  }
  bool operator!=(const const_iterator& __x) const {
    return _M_p != __x._M_p || _M_offset != __x._M_offset;
  }
  bool operator<(const_iterator __x) const {
    return _M_p < __x._M_p || (_M_p == __x._M_p && _M_offset < __x._M_offset);
  }
};

 
 



 
template <class _Allocator, bool __is_static>
class _Bvector_alloc_base {
public:
  typedef typename _Alloc_traits<bool, _Allocator>::allocator_type
          allocator_type;
  allocator_type get_allocator() const { return _M_data_allocator; }

  _Bvector_alloc_base(const allocator_type& __a)
    : _M_data_allocator(__a), _M_start(), _M_finish(), _M_end_of_storage(0) {}

protected:
  unsigned int* _M_bit_alloc(size_t __n) 
    { return _M_data_allocator.allocate((__n + __WORD_BIT - 1)/__WORD_BIT); }
  void _M_deallocate() {
    if (_M_start._M_p)
      _M_data_allocator.deallocate(_M_start._M_p, 
                                   _M_end_of_storage - _M_start._M_p);
  }  

  typename _Alloc_traits<unsigned int, _Allocator>::allocator_type 
          _M_data_allocator;
  _Bit_iterator _M_start;
  _Bit_iterator _M_finish;
  unsigned int* _M_end_of_storage;
};

 
template <class _Allocator>
class _Bvector_alloc_base<_Allocator, true> {
public:
  typedef typename _Alloc_traits<bool, _Allocator>::allocator_type
          allocator_type;
  allocator_type get_allocator() const { return allocator_type(); }

  _Bvector_alloc_base(const allocator_type&)
    : _M_start(), _M_finish(), _M_end_of_storage(0) {}

protected:
  typedef typename _Alloc_traits<unsigned int, _Allocator>::_Alloc_type
          _Alloc_type;
          
  unsigned int* _M_bit_alloc(size_t __n) 
    { return _Alloc_type::allocate((__n + __WORD_BIT - 1)/__WORD_BIT); }
  void _M_deallocate() {
    if (_M_start._M_p)
      _Alloc_type::deallocate(_M_start._M_p,
                              _M_end_of_storage - _M_start._M_p);
  }  

  _Bit_iterator _M_start;
  _Bit_iterator _M_finish;
  unsigned int* _M_end_of_storage;
};  

template <class _Alloc>
class _Bvector_base
  : public _Bvector_alloc_base<_Alloc,
                               _Alloc_traits<bool, _Alloc>::_S_instanceless>
{
  typedef _Bvector_alloc_base<_Alloc,
                              _Alloc_traits<bool, _Alloc>::_S_instanceless>
          _Base;
public:
  typedef typename _Base::allocator_type allocator_type;

  _Bvector_base(const allocator_type& __a) : _Base(__a) {}
  ~_Bvector_base() { _Base::_M_deallocate(); }
};

# 348 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_bvector.h" 3


 
 
 
 










        

        
template<class _Alloc> class vector<bool,_Alloc>
  : public _Bvector_base<_Alloc>




{

  typedef _Bvector_base<_Alloc> _Base;



public:
  typedef bool value_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type; 
  typedef _Bit_reference reference;
  typedef bool const_reference;
  typedef _Bit_reference* pointer;
  typedef const bool* const_pointer;

  typedef _Bit_iterator                iterator;
  typedef _Bit_const_iterator          const_iterator;


  typedef reverse_iterator<const_iterator> const_reverse_iterator;
  typedef reverse_iterator<iterator> reverse_iterator;







  typedef typename _Base::allocator_type allocator_type;
  allocator_type get_allocator() const { return _Base::get_allocator(); }

protected:








protected:
  void _M_initialize(size_type __n) {
    unsigned int* __q = _M_bit_alloc(__n);
    _M_end_of_storage = __q + (__n + __WORD_BIT - 1)/__WORD_BIT;
    _M_start = iterator(__q, 0);
    _M_finish = _M_start + difference_type(__n);
  }
  void _M_insert_aux(iterator __position, bool __x) {
    if (_M_finish._M_p != _M_end_of_storage) {
      copy_backward(__position, _M_finish, _M_finish + 1);
      *__position = __x;
      ++_M_finish;
    }
    else {
      size_type __len = size() ? 2 * size() : __WORD_BIT;
      unsigned int* __q = _M_bit_alloc(__len);
      iterator __i = copy(begin(), __position, iterator(__q, 0));
      *__i++ = __x;
      _M_finish = copy(__position, end(), __i);
      _M_deallocate();
      _M_end_of_storage = __q + (__len + __WORD_BIT - 1)/__WORD_BIT;
      _M_start = iterator(__q, 0);
    }
  }


  template <class _InputIterator>
  void _M_initialize_range(_InputIterator __first, _InputIterator __last,
                           input_iterator_tag) {
    _M_start = iterator();
    _M_finish = iterator();
    _M_end_of_storage = 0;
    for ( ; __first != __last; ++__first) 
      push_back(*__first);
  }

  template <class _ForwardIterator>
  void _M_initialize_range(_ForwardIterator __first, _ForwardIterator __last,
                           forward_iterator_tag) {
    size_type __n = 0;
    distance(__first, __last, __n);
    _M_initialize(__n);
    copy(__first, __last, _M_start);
  }

  template <class _InputIterator>
  void _M_insert_range(iterator __pos,
                       _InputIterator __first, _InputIterator __last,
                       input_iterator_tag) {
    for ( ; __first != __last; ++__first) {
      __pos = insert(__pos, *__first);
      ++__pos;
    }
  }

  template <class _ForwardIterator>
  void _M_insert_range(iterator __position,
                       _ForwardIterator __first, _ForwardIterator __last,
                       forward_iterator_tag) {
    if (__first != __last) {
      size_type __n = 0;
      distance(__first, __last, __n);
      if (capacity() - size() >= __n) {
        copy_backward(__position, end(), _M_finish + difference_type(__n));
        copy(__first, __last, __position);
        _M_finish += difference_type(__n);
      }
      else {
        size_type __len = size() + max(size(), __n);
        unsigned int* __q = _M_bit_alloc(__len);
        iterator __i = copy(begin(), __position, iterator(__q, 0));
        __i = copy(__first, __last, __i);
        _M_finish = copy(__position, end(), __i);
        _M_deallocate();
        _M_end_of_storage = __q + (__len + __WORD_BIT - 1)/__WORD_BIT;
        _M_start = iterator(__q, 0);
      }
    }
  }      



public:
  iterator begin() { return _M_start; }
  const_iterator begin() const { return _M_start; }
  iterator end() { return _M_finish; }
  const_iterator end() const { return _M_finish; }

  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { 
    return const_reverse_iterator(end()); 
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { 
    return const_reverse_iterator(begin()); 
  }

  size_type size() const { return size_type(end() - begin()); }
  size_type max_size() const { return size_type(-1); }
  size_type capacity() const {
    return size_type(const_iterator(_M_end_of_storage, 0) - begin());
  }
  bool empty() const { return begin() == end(); }
  reference operator[](size_type __n) {
    return *(begin() + difference_type(__n));
  }
  const_reference operator[](size_type __n) const {
    return *(begin() + difference_type(__n));
  }

  explicit vector (const allocator_type& __a = allocator_type())
    : _Base(__a) {}

  vector (size_type __n, bool __value,
            const allocator_type& __a = allocator_type())
    : _Base(__a)
  {
    _M_initialize(__n);
    fill(_M_start._M_p, _M_end_of_storage, __value ? ~0 : 0);
  }

  explicit vector (size_type __n)
    : _Base(allocator_type())
  {
    _M_initialize(__n);
    fill(_M_start._M_p, _M_end_of_storage, 0);
  }

  vector (const vector & __x) : _Base(__x.get_allocator()) {
    _M_initialize(__x.size());
    copy(__x.begin(), __x.end(), _M_start);
  }


   
  template <class _InputIterator>
  vector (_InputIterator __first, _InputIterator __last,
            const allocator_type& __a = allocator_type())
    : _Base(__a)
  {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_initialize_dispatch(__first, __last, _Integral());
  }
    
  template <class _Integer>
  void _M_initialize_dispatch(_Integer __n, _Integer __x, __true_type) {
    _M_initialize(__n);
    fill(_M_start._M_p, _M_end_of_storage, __x ? ~0 : 0);
  }
    
  template <class _InputIterator>
  void _M_initialize_dispatch(_InputIterator __first, _InputIterator __last,
                              __false_type) {
    _M_initialize_range(__first, __last, __iterator_category( __first ) );
  }
# 587 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_bvector.h" 3


  ~vector () { }

  vector & operator=(const vector & __x) {
    if (&__x == this) return *this;
    if (__x.size() > capacity()) {
      _M_deallocate();
      _M_initialize(__x.size());
    }
    copy(__x.begin(), __x.end(), begin());
    _M_finish = begin() + difference_type(__x.size());
    return *this;
  }

   
   
   
   

  void assign(size_t __n, bool __x) {
    if (__n > size()) {
      fill(_M_start._M_p, _M_end_of_storage, __x ? ~0 : 0);
      insert(end(), __n - size(), __x);
    }
    else {
      erase(begin() + __n, end());
      fill(_M_start._M_p, _M_end_of_storage, __x ? ~0 : 0);
    }
  }



  template <class _InputIterator>
  void assign(_InputIterator __first, _InputIterator __last) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_assign_dispatch(__first, __last, _Integral());
  }

  template <class _Integer>
  void _M_assign_dispatch(_Integer __n, _Integer __val, __true_type)
    { assign((size_t) __n, (bool) __val); }

  template <class _InputIter>
  void _M_assign_dispatch(_InputIter __first, _InputIter __last, __false_type)
    { _M_assign_aux(__first, __last, __iterator_category( __first ) ); }

  template <class _InputIterator>
  void _M_assign_aux(_InputIterator __first, _InputIterator __last,
                     input_iterator_tag) {
    iterator __cur = begin();
    for ( ; __first != __last && __cur != end(); ++__cur, ++__first)
      *__cur = *__first;
    if (__first == __last)
      erase(__cur, end());
    else
      insert(end(), __first, __last);
  }

  template <class _ForwardIterator>
  void _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last,
                     forward_iterator_tag) {
    size_type __len = 0;
    distance(__first, __last, __len);
    if (__len < size())
      erase(copy(__first, __last, begin()), end());
    else {
      _ForwardIterator __mid = __first;
      advance(__mid, size());
      copy(__first, __mid, begin());
      insert(end(), __mid, __last);
    }
  }    



  void reserve(size_type __n) {
    if (capacity() < __n) {
      unsigned int* __q = _M_bit_alloc(__n);
      _M_finish = copy(begin(), end(), iterator(__q, 0));
      _M_deallocate();
      _M_start = iterator(__q, 0);
      _M_end_of_storage = __q + (__n + __WORD_BIT - 1)/__WORD_BIT;
    }
  }

  reference front() { return *begin(); }
  const_reference front() const { return *begin(); }
  reference back() { return *(end() - 1); }
  const_reference back() const { return *(end() - 1); }
  void push_back(bool __x) {
    if (_M_finish._M_p != _M_end_of_storage)
      *_M_finish++ = __x;
    else
      _M_insert_aux(end(), __x);
  }
  void swap(vector & __x) {
     ::swap(_M_start, __x._M_start);
     ::swap(_M_finish, __x._M_finish);
     ::swap(_M_end_of_storage, __x._M_end_of_storage);
  }
  iterator insert(iterator __position, bool __x = bool()) {
    difference_type __n = __position - begin();
    if (_M_finish._M_p != _M_end_of_storage && __position == end())
      *_M_finish++ = __x;
    else
      _M_insert_aux(__position, __x);
    return begin() + __n;
  }


   
  template <class _InputIterator>
  void insert(iterator __position,
              _InputIterator __first, _InputIterator __last) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_insert_dispatch(__position, __first, __last, _Integral());
  }

  template <class _Integer>
  void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __x,
                          __true_type) {
    insert(__pos, (size_type) __n, (bool) __x);
  }

  template <class _InputIterator>
  void _M_insert_dispatch(iterator __pos,
                          _InputIterator __first, _InputIterator __last,
                          __false_type) {
    _M_insert_range(__pos, __first, __last, __iterator_category( __first ) );
  }
# 761 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_bvector.h" 3

  
  void insert(iterator __position, size_type __n, bool __x) {
    if (__n == 0) return;
    if (capacity() - size() >= __n) {
      copy_backward(__position, end(), _M_finish + difference_type(__n));
      fill(__position, __position + difference_type(__n), __x);
      _M_finish += difference_type(__n);
    }
    else {
      size_type __len = size() + max(size(), __n);
      unsigned int* __q = _M_bit_alloc(__len);
      iterator __i = copy(begin(), __position, iterator(__q, 0));
      fill_n(__i, __n, __x);
      _M_finish = copy(__position, end(), __i + difference_type(__n));
      _M_deallocate();
      _M_end_of_storage = __q + (__len + __WORD_BIT - 1)/__WORD_BIT;
      _M_start = iterator(__q, 0);
    }
  }

  void pop_back() { --_M_finish; }
  iterator erase(iterator __position) {
    if (__position + 1 != end())
      copy(__position + 1, end(), __position);
      --_M_finish;
    return __position;
  }
  iterator erase(iterator __first, iterator __last) {
    _M_finish = copy(__last, end(), __first);
    return __first;
  }
  void resize(size_type __new_size, bool __x = bool()) {
    if (__new_size < size()) 
      erase(begin() + difference_type(__new_size), end());
    else
      insert(end(), __new_size - size(), __x);
  }
  void clear() { erase(begin(), end()); }
};



typedef vector<bool, alloc> bit_vector;

# 822 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_bvector.h" 3










  



 
 
 
# 35 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/vector" 2 3




 
 
 
# 5 "testing_vector.cpp" 2

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/memory" 1 3
 



















# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_tempbuf.h" 1 3
 

























 







 

template <class _Tp>
pair<_Tp*, ptrdiff_t> 
__get_temporary_buffer(ptrdiff_t __len, _Tp*)
{
  if (__len > ptrdiff_t(2147483647   / sizeof(_Tp)))
    __len = 2147483647   / sizeof(_Tp);

  while (__len > 0) {
    _Tp* __tmp = (_Tp*) malloc((size_t)__len * sizeof(_Tp));
    if (__tmp != 0)
      return pair<_Tp*, ptrdiff_t>(__tmp, __len);
    __len /= 2;
  }

  return pair<_Tp*, ptrdiff_t>((_Tp*)0, 0);
}



template <class _Tp>
inline pair<_Tp*, ptrdiff_t> get_temporary_buffer(ptrdiff_t __len) {
  return __get_temporary_buffer(__len, (_Tp*) 0);
}



 
 
 
 
 
template <class _Tp>
inline pair<_Tp*, ptrdiff_t> get_temporary_buffer(ptrdiff_t __len, _Tp*) {
  return __get_temporary_buffer(__len, (_Tp*) 0);
}

template <class _Tp>
void return_temporary_buffer(_Tp* __p) {
  free(__p);
}

template <class _ForwardIterator, class _Tp>
class _Temporary_buffer {
private:
  ptrdiff_t  _M_original_len;
  ptrdiff_t  _M_len;
  _Tp*       _M_buffer;

  void _M_allocate_buffer() {
    _M_original_len = _M_len;
    _M_buffer = 0;

    if (_M_len > (ptrdiff_t)(2147483647   / sizeof(_Tp)))
      _M_len = 2147483647   / sizeof(_Tp);

    while (_M_len > 0) {
      _M_buffer = (_Tp*) malloc(_M_len * sizeof(_Tp));
      if (_M_buffer)
        break;
      _M_len /= 2;
    }
  }

  void _M_initialize_buffer(const _Tp&, __true_type) {}
  void _M_initialize_buffer(const _Tp& val, __false_type) {
    uninitialized_fill_n(_M_buffer, _M_len, val);
  }

public:
  ptrdiff_t size() const { return _M_len; }
  ptrdiff_t requested_size() const { return _M_original_len; }
  _Tp* begin() { return _M_buffer; }
  _Tp* end() { return _M_buffer + _M_len; }

  _Temporary_buffer(_ForwardIterator __first, _ForwardIterator __last) {
    typedef typename __type_traits<_Tp>::has_trivial_default_constructor
            _Trivial;
    try  {
      _M_len = 0;
      distance(__first, __last, _M_len);
      _M_allocate_buffer();
      if (_M_len > 0)
        _M_initialize_buffer(*__first, _Trivial());
    }
    catch(...) {  free(_M_buffer); _M_buffer = 0; _M_len = 0 ; throw; } ;
  }
 
  ~_Temporary_buffer() {  
    destroy(_M_buffer, _M_buffer + _M_len);
    free(_M_buffer);
  }

private:
   
  _Temporary_buffer(const _Temporary_buffer&) {}
  void operator=(const _Temporary_buffer&) {}
};

 

template <class _ForwardIterator, 
          class _Tp 

                    = typename iterator_traits<_ForwardIterator>::value_type

         >
struct temporary_buffer : public _Temporary_buffer<_ForwardIterator, _Tp>
{
  temporary_buffer(_ForwardIterator __first, _ForwardIterator __last)
    : _Temporary_buffer<_ForwardIterator, _Tp>(__first, __last) {}
  ~temporary_buffer() {}
};
    
 



 
 
 
# 21 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/memory" 2 3


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_raw_storage_iter.h" 1 3
 

























 






 

template <class _ForwardIterator, class _Tp>
class raw_storage_iterator {
protected:
  _ForwardIterator _M_iter;
public:
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  explicit raw_storage_iterator(_ForwardIterator __x) : _M_iter(__x) {}
  raw_storage_iterator& operator*() { return *this; }
  raw_storage_iterator& operator=(const _Tp& __element) {
    construct(&*_M_iter, __element);
    return *this;
  }        
  raw_storage_iterator<_ForwardIterator, _Tp>& operator++() {
    ++_M_iter;
    return *this;
  }
  raw_storage_iterator<_ForwardIterator, _Tp> operator++(int) {
    raw_storage_iterator<_ForwardIterator, _Tp> __tmp = *this;
    ++_M_iter;
    return __tmp;
  }
};

# 73 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_raw_storage_iter.h" 3


 



 
 
 
# 23 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/memory" 2 3





 

template <class _Tp> class auto_ptr {
private:
  _Tp* _M_ptr;

public:
  typedef _Tp element_type;
  explicit auto_ptr(_Tp* __p = 0) throw()  : _M_ptr(__p) {}
  auto_ptr(auto_ptr& __a) throw()  : _M_ptr(__a.release()) {}
  template <class _Tp1> auto_ptr(auto_ptr<_Tp1>& __a) throw() 
    : _M_ptr(__a.release()) {}
  auto_ptr& operator=(auto_ptr& __a) throw()  {
    if (&__a != this) {
      delete _M_ptr;
      _M_ptr = __a.release();
    }
    return *this;
  }
  template <class _Tp1>
  auto_ptr& operator=(auto_ptr<_Tp1>& __a) throw()  {
    if (__a.get() != this->get()) {
      delete _M_ptr;
      _M_ptr = __a.release();
    }
    return *this;
  }
  ~auto_ptr() throw()  { delete _M_ptr; }

  _Tp& operator*() const throw()  {
    return *_M_ptr;
  }
  _Tp* operator->() const throw()  {
    return _M_ptr;
  }
  _Tp* get() const throw()  {
    return _M_ptr;
  }
  _Tp* release() throw()  {
    _Tp* __tmp = _M_ptr;
    _M_ptr = 0;
    return __tmp;
  }
  void reset(_Tp* __p = 0) throw()  {
    delete _M_ptr;
    _M_ptr = __p;
  }

   
   
   
   
  


private:
  template<class _Tp1> struct auto_ptr_ref {
    _Tp1* _M_ptr;
    auto_ptr_ref(_Tp1* __p) : _M_ptr(__p) {}
  };

public:
  auto_ptr(auto_ptr_ref<_Tp> __ref) throw() 
    : _M_ptr(__ref._M_ptr) {}
  template <class _Tp1> operator auto_ptr_ref<_Tp1>() throw()  
    { return auto_ptr_ref<_Tp>(this->release()); }
  template <class _Tp1> operator auto_ptr<_Tp1>() throw() 
    { return auto_ptr<_Tp1>(this->release()); }


};

 





 
 
 
# 6 "testing_vector.cpp" 2

# 1 "/usr/include/boost/timer.hpp" 1 3
 

 
 
 
 

 

 
 
 
 
 
 
 




# 1 "/usr/include/boost/config.hpp" 1 3
 

 
 
 
 

 

 
 
 
 
 
 




 



 

# 1 "/usr/include/boost/config/user.hpp" 1 3
 
 
 

 
 
 
 

 
 

 
 

 
 

 
 
 

 
 
 

 
 
 

 
 
 
 
 
 

 
 
 
 
 
 
 
 

 
 
 
 
 


 
 
 


# 26 "/usr/include/boost/config.hpp" 2 3



 

# 1 "/usr/include/boost/config/select_compiler_config.hpp" 1 3
 

 
 
 
 

 

 
 


 


# 77 "/usr/include/boost/config/select_compiler_config.hpp" 3

# 31 "/usr/include/boost/config.hpp" 2 3


 

# 1 "/usr/include/boost/config/compiler/gcc.hpp" 1 3
 
 
 
 

 

 





# 24 "/usr/include/boost/config/compiler/gcc.hpp" 3







 
 
 
 
 


 
 
 




 
 
 



 
 







# 35 "/usr/include/boost/config.hpp" 2 3



 

# 1 "/usr/include/boost/config/select_stdlib_config.hpp" 1 3
 

 
 
 
 

 

 

 
 
 
 
 

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/utility" 1 3
 


































 
 
 
# 18 "/usr/include/boost/config/select_stdlib_config.hpp" 2 3


# 38 "/usr/include/boost/config/select_stdlib_config.hpp" 3

 


# 59 "/usr/include/boost/config/select_stdlib_config.hpp" 3



# 40 "/usr/include/boost/config.hpp" 2 3


 

# 1 "/usr/include/boost/config/stdlib/sgi.hpp" 1 3
 
 
 
 

 

 








 
 
 




 
 
 
# 39 "/usr/include/boost/config/stdlib/sgi.hpp" 3


 
 
 
 




 
 
 




 
 
 




 
 
 
 





 
 
 



 
 
 

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/string" 1 3
 




# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/std/bastring.h" 1 3
 
 

 
 
 
 
 

 
 
 
 

 
 
 

 
 
 
 
 

 
 





#pragma interface


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/cstddef" 1 3
 
 



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3
# 342 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3

# 6 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/cstddef" 2 3


# 35 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/std/bastring.h" 2 3

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/std/straits.h" 1 3
 
 

 
 
 
 
 

 
 
 
 

 
 
 

 
 
 
 
 

 
 





 
#pragma interface "std/straits.h"




extern "C++" {
template <class charT>
struct string_char_traits {
  typedef charT char_type;  

   

  static void assign (char_type& c1, const char_type& c2)
    { c1 = c2; }
  static bool eq (const char_type& c1, const char_type& c2)
    { return (c1 == c2); }
  static bool ne (const char_type& c1, const char_type& c2)
    { return !(c1 == c2); }
  static bool lt (const char_type& c1, const char_type& c2)
    { return (c1 < c2); }
  static char_type eos () { return char_type(); }  
  static bool is_del(char_type a) { return 0; }
   
  
   

  static int compare (const char_type* s1, const char_type* s2, size_t n)
    {
      size_t i;
      for (i = 0; i < n; ++i)
	if (ne (s1[i], s2[i]))
	  return lt (s1[i], s2[i]) ? -1 : 1;

      return 0;
    }
    
  static size_t length (const char_type* s)
    {
      size_t l = 0;
      while (ne (*s++, eos ()))
	++l;
      return l;
    }

  static char_type* copy (char_type* s1, const char_type* s2, size_t n)
    {
      for (; n--; )
	assign (s1[n], s2[n]);
      return s1;
    }

  static char_type* move (char_type* s1, const char_type* s2, size_t n)
    {
      char_type a[n];
      size_t i;
      for (i = 0; i < n; ++i)
	assign (a[i], s2[i]);
      for (i = 0; i < n; ++i)
	assign (s1[i], a[i]);
      return s1;
    }

  static char_type* set (char_type* s1, const char_type& c, size_t n)
    {
      for (; n--; )
	assign (s1[n], c);
      return s1;
    }
};

class istream;
class ostream;
# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/cctype" 1 3
 
 



# 1 "/usr/include/ctype.h" 1 3
 

















 









extern "C" { 


 














enum
{
  _ISupper = (( 0 ) < 8 ? ((1 << ( 0 )) << 8) : ((1 << ( 0 )) >> 8)) ,	 
  _ISlower = (( 1 ) < 8 ? ((1 << ( 1 )) << 8) : ((1 << ( 1 )) >> 8)) ,	 
  _ISalpha = (( 2 ) < 8 ? ((1 << ( 2 )) << 8) : ((1 << ( 2 )) >> 8)) ,	 
  _ISdigit = (( 3 ) < 8 ? ((1 << ( 3 )) << 8) : ((1 << ( 3 )) >> 8)) ,	 
  _ISxdigit = (( 4 ) < 8 ? ((1 << ( 4 )) << 8) : ((1 << ( 4 )) >> 8)) ,	 
  _ISspace = (( 5 ) < 8 ? ((1 << ( 5 )) << 8) : ((1 << ( 5 )) >> 8)) ,	 
  _ISprint = (( 6 ) < 8 ? ((1 << ( 6 )) << 8) : ((1 << ( 6 )) >> 8)) ,	 
  _ISgraph = (( 7 ) < 8 ? ((1 << ( 7 )) << 8) : ((1 << ( 7 )) >> 8)) ,	 
  _ISblank = (( 8 ) < 8 ? ((1 << ( 8 )) << 8) : ((1 << ( 8 )) >> 8)) ,	 
  _IScntrl = (( 9 ) < 8 ? ((1 << ( 9 )) << 8) : ((1 << ( 9 )) >> 8)) ,	 
  _ISpunct = (( 10 ) < 8 ? ((1 << ( 10 )) << 8) : ((1 << ( 10 )) >> 8)) ,	 
  _ISalnum = (( 11 ) < 8 ? ((1 << ( 11 )) << 8) : ((1 << ( 11 )) >> 8)) 	 
};


 










extern __const unsigned short int *__ctype_b;	 
extern __const __int32_t *__ctype_tolower;  
extern __const __int32_t *__ctype_toupper;  









 



extern int  isalnum  (int) throw ()  ;
extern int  isalpha  (int) throw ()  ;
extern int  iscntrl  (int) throw ()  ;
extern int  isdigit  (int) throw ()  ;
extern int  islower  (int) throw ()  ;
extern int  isgraph  (int) throw ()  ;
extern int  isprint  (int) throw ()  ;
extern int  ispunct  (int) throw ()  ;
extern int  isspace  (int) throw ()  ;
extern int  isupper  (int) throw ()  ;
extern int  isxdigit  (int) throw ()  ;






 
extern int tolower (int __c) throw () ;

 
extern int toupper (int __c) throw () ;




 

extern int isascii (int __c) throw () ;

 

extern int toascii (int __c) throw () ;

 

extern int  _toupper  (int) throw ()  ;
extern int  _tolower  (int) throw ()  ;


 

# 148 "/usr/include/ctype.h" 3


















# 178 "/usr/include/ctype.h" 3


















# 274 "/usr/include/ctype.h" 3


} 


# 6 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/cctype" 2 3


# 105 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/std/straits.h" 2 3

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/cstring" 1 3
 
 






# 94 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/cstring" 3



# 106 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/std/straits.h" 2 3


struct string_char_traits <char> {
  typedef char char_type;

  static void assign (char_type& c1, const char_type& c2)
    { c1 = c2; }
  static bool eq (const char_type & c1, const char_type& c2)
    { return (c1 == c2); }
  static bool ne (const char_type& c1, const char_type& c2)
    { return (c1 != c2); }
  static bool lt (const char_type& c1, const char_type& c2)
    { return (c1 < c2); }
  static char_type eos () { return 0; }
  static bool is_del(char_type a) { return (__ctype_b[(int) ( ( a ) )] & (unsigned short int)   _ISspace )  ; }

  static int compare (const char_type* s1, const char_type* s2, size_t n)
    { return memcmp (s1, s2, n); }
  static size_t length (const char_type* s)
    { return strlen (s); }
  static char_type* copy (char_type* s1, const char_type* s2, size_t n)
    { return (char_type*) memcpy (s1, s2, n); }
  static char_type* move (char_type* s1, const char_type* s2, size_t n)
    { return (char_type*) memmove (s1, s2, n); }
  static char_type* set (char_type* s1, const char_type& c, size_t n)
    { return (char_type*) memset (s1, c, n); }
};

# 159 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/std/straits.h" 3

}  

# 36 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/std/bastring.h" 2 3


 
# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/alloc.h" 1 3
 






















# 40 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/alloc.h" 3




 
 
 
# 39 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/std/bastring.h" 2 3


extern "C++" {
class istream; class ostream;

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/iterator" 1 3
 






























# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3
# 342 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3

# 32 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/iterator" 2 3










 
 
 
# 44 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/std/bastring.h" 2 3




extern void __out_of_range (const char *);
extern void __length_error (const char *);














template <class charT, class traits = string_char_traits<charT>,
	  class Allocator = alloc >
class basic_string
{
private:
  struct Rep {
    size_t len, res, ref;
    bool selfish;

    charT* data () { return reinterpret_cast<charT *>(this + 1); }
    charT& operator[] (size_t s) { return data () [s]; }
    charT* grab () { if (selfish) return clone (); ++ref; return data (); }
# 108 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/std/bastring.h" 3

    void release () { if (--ref == 0) delete this; }


    inline static void * operator new (size_t, size_t);
    inline static void operator delete (void *);
    inline static Rep* create (size_t);
    charT* clone ();

    inline void copy (size_t, const charT *, size_t);
    inline void move (size_t, const charT *, size_t);
    inline void set  (size_t, const charT,   size_t);

    inline static bool excess_slop (size_t, size_t);
    inline static size_t frob_size (size_t);

  private:
    Rep &operator= (const Rep &);
  };

public:
 
  typedef	   traits		traits_type;
  typedef typename traits::char_type	value_type;
  typedef	   Allocator		allocator_type;

  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef charT& reference;
  typedef const charT& const_reference;
  typedef charT* pointer;
  typedef const charT* const_pointer;
  typedef pointer iterator;
  typedef const_pointer const_iterator;
  typedef ::reverse_iterator<iterator> reverse_iterator;
  typedef ::reverse_iterator<const_iterator> const_reverse_iterator;
  static const size_type npos = static_cast<size_type>(-1);

private:
  Rep *rep () const { return reinterpret_cast<Rep *>(dat) - 1; }
  void repup (Rep *p) { rep ()->release (); dat = p->data (); }

public:
  const charT* data () const
    { return rep ()->data(); }
  size_type length () const
    { return rep ()->len; }
  size_type size () const
    { return rep ()->len; }
  size_type capacity () const
    { return rep ()->res; }
  size_type max_size () const
    { return (npos - 1)/sizeof (charT); }		 
  bool empty () const
    { return size () == 0; }

 
  basic_string& operator= (const basic_string& str)
    {
      if (&str != this) { rep ()->release (); dat = str.rep ()->grab (); }
      return *this;
    }

  explicit basic_string (): dat (nilRep.grab ()) { }
  basic_string (const basic_string& _str): dat (_str.rep ()->grab ()) { }
  basic_string (const basic_string& _str, size_type pos, size_type n = npos)
    : dat (nilRep.grab ()) { assign (_str, pos, n); }
  basic_string (const charT* s, size_type n)
    : dat (nilRep.grab ()) { assign (s, n); }
  basic_string (const charT* s)
    : dat (nilRep.grab ()) { assign (s); }
  basic_string (size_type n, charT c)
    : dat (nilRep.grab ()) { assign (n, c); }

  template<class InputIterator>
    basic_string(InputIterator __begin, InputIterator __end)



    : dat (nilRep.grab ()) { assign (__begin, __end); }

  ~basic_string ()
    { rep ()->release (); }

  void swap (basic_string &s) { charT *d = dat; dat = s.dat; s.dat = d; }

  basic_string& append (const basic_string& _str, size_type pos = 0,
			size_type n = npos)
    { return replace (length (), 0, _str, pos, n); }
  basic_string& append (const charT* s, size_type n)
    { return replace (length (), 0, s, n); }
  basic_string& append (const charT* s)
    { return append (s, traits::length (s)); }
  basic_string& append (size_type n, charT c)
    { return replace (length (), 0, n, c); }

  template<class InputIterator>
    basic_string& append(InputIterator first, InputIterator last)



    { return replace (iend (), iend (), first, last); }

  void push_back(charT __c)
  { append(1, __c); }
  
  basic_string& assign (const basic_string& str, size_type pos = 0,
			size_type n = npos)
    { return replace (0, npos, str, pos, n); }
  basic_string& assign (const charT* s, size_type n)
    { return replace (0, npos, s, n); }
  basic_string& assign (const charT* s)
    { return assign (s, traits::length (s)); }
  basic_string& assign (size_type n, charT c)
    { return replace (0, npos, n, c); }

  template<class InputIterator>
    basic_string& assign(InputIterator first, InputIterator last)



    { return replace (ibegin (), iend (), first, last); }

  basic_string& operator= (const charT* s)
    { return assign (s); }
  basic_string& operator= (charT c)
    { return assign (1, c); }

  basic_string& operator+= (const basic_string& rhs)
    { return append (rhs); }
  basic_string& operator+= (const charT* s)
    { return append (s); }
  basic_string& operator+= (charT c)
    { return append (1, c); }

  basic_string& insert (size_type pos1, const basic_string& str,
			size_type pos2 = 0, size_type n = npos)
    { return replace (pos1, 0, str, pos2, n); }
  basic_string& insert (size_type pos, const charT* s, size_type n)
    { return replace (pos, 0, s, n); }
  basic_string& insert (size_type pos, const charT* s)
    { return insert (pos, s, traits::length (s)); }
  basic_string& insert (size_type pos, size_type n, charT c)
    { return replace (pos, 0, n, c); }
  iterator insert(iterator p, charT c)
    { size_type __o = p - ibegin ();
      insert (p - ibegin (), 1, c); selfish ();
      return ibegin () + __o; }
  iterator insert(iterator p, size_type n, charT c)
    { size_type __o = p - ibegin ();
      insert (p - ibegin (), n, c); selfish ();
      return ibegin () + __o; }

  template<class InputIterator>
    void insert(iterator p, InputIterator first, InputIterator last)



    { replace (p, p, first, last); }

  basic_string& erase (size_type pos = 0, size_type n = npos)
    { return replace (pos, n, (size_type)0, (charT)0); }
  iterator erase(iterator p)
    { size_type __o = p - begin();
      replace (__o, 1, (size_type)0, (charT)0); selfish ();
      return ibegin() + __o; }
  iterator erase(iterator f, iterator l)
    { size_type __o = f - ibegin();
      replace (__o, l-f, (size_type)0, (charT)0);selfish ();
      return ibegin() + __o; }

  basic_string& replace (size_type pos1, size_type n1, const basic_string& str,
			 size_type pos2 = 0, size_type n2 = npos);
  basic_string& replace (size_type pos, size_type n1, const charT* s,
			 size_type n2);
  basic_string& replace (size_type pos, size_type n1, const charT* s)
    { return replace (pos, n1, s, traits::length (s)); }
  basic_string& replace (size_type pos, size_type n1, size_type n2, charT c);
  basic_string& replace (size_type pos, size_type n, charT c)
    { return replace (pos, n, 1, c); }
  basic_string& replace (iterator i1, iterator i2, const basic_string& str)
    { return replace (i1 - ibegin (), i2 - i1, str); }
  basic_string& replace (iterator i1, iterator i2, const charT* s, size_type n)
    { return replace (i1 - ibegin (), i2 - i1, s, n); }
  basic_string& replace (iterator i1, iterator i2, const charT* s)
    { return replace (i1 - ibegin (), i2 - i1, s); }
  basic_string& replace (iterator i1, iterator i2, size_type n, charT c)
    { return replace (i1 - ibegin (), i2 - i1, n, c); }

  template<class InputIterator>
    basic_string& replace(iterator i1, iterator i2,
			  InputIterator j1, InputIterator j2);





private:
  static charT eos () { return traits::eos (); }
  void unique () { if (rep ()->ref > 1) alloc (length (), true); }
  void selfish () { unique (); rep ()->selfish = true; }

public:
  charT operator[] (size_type pos) const
    {
      if (pos == length ())
	return eos ();
      return data ()[pos];
    }

  reference operator[] (size_type pos)
    { selfish (); return (*rep ())[pos]; }

  reference at (size_type pos)
    {
      do { if ( pos >= length () ) __out_of_range ("pos >= length ()"); } while (0) ;
      return (*this)[pos];
    }
  const_reference at (size_type pos) const
    {
      do { if ( pos >= length () ) __out_of_range ("pos >= length ()"); } while (0) ;
      return data ()[pos];
    }

private:
  void terminate () const
    { traits::assign ((*rep ())[length ()], eos ()); }

public:
  const charT* c_str () const
    { const charT* null_str = ""; 
      if (length () == 0) return null_str; terminate (); return data (); }
  void resize (size_type n, charT c);
  void resize (size_type n)
    { resize (n, eos ()); }
  void reserve (size_type) { }

  size_type copy (charT* s, size_type n, size_type pos = 0) const;

  size_type find (const basic_string& str, size_type pos = 0) const
    { return find (str.data(), pos, str.length()); }
  size_type find (const charT* s, size_type pos, size_type n) const;
  size_type find (const charT* _s, size_type pos = 0) const
    { return find (_s, pos, traits::length (_s)); }
  size_type find (charT c, size_type pos = 0) const;

  size_type rfind (const basic_string& str, size_type pos = npos) const
    { return rfind (str.data(), pos, str.length()); }
  size_type rfind (const charT* s, size_type pos, size_type n) const;
  size_type rfind (const charT* s, size_type pos = npos) const
    { return rfind (s, pos, traits::length (s)); }
  size_type rfind (charT c, size_type pos = npos) const;

  size_type find_first_of (const basic_string& str, size_type pos = 0) const
    { return find_first_of (str.data(), pos, str.length()); }
  size_type find_first_of (const charT* s, size_type pos, size_type n) const;
  size_type find_first_of (const charT* s, size_type pos = 0) const
    { return find_first_of (s, pos, traits::length (s)); }
  size_type find_first_of (charT c, size_type pos = 0) const
    { return find (c, pos); }

  size_type find_last_of (const basic_string& str, size_type pos = npos) const
    { return find_last_of (str.data(), pos, str.length()); }
  size_type find_last_of (const charT* s, size_type pos, size_type n) const;
  size_type find_last_of (const charT* s, size_type pos = npos) const
    { return find_last_of (s, pos, traits::length (s)); }
  size_type find_last_of (charT c, size_type pos = npos) const
    { return rfind (c, pos); }

  size_type find_first_not_of (const basic_string& str, size_type pos = 0) const
    { return find_first_not_of (str.data(), pos, str.length()); }
  size_type find_first_not_of (const charT* s, size_type pos, size_type n) const;
  size_type find_first_not_of (const charT* s, size_type pos = 0) const
    { return find_first_not_of (s, pos, traits::length (s)); }
  size_type find_first_not_of (charT c, size_type pos = 0) const;

  size_type find_last_not_of (const basic_string& str, size_type pos = npos) const
    { return find_last_not_of (str.data(), pos, str.length()); }
  size_type find_last_not_of (const charT* s, size_type pos, size_type n) const;
  size_type find_last_not_of (const charT* s, size_type pos = npos) const
    { return find_last_not_of (s, pos, traits::length (s)); }
  size_type find_last_not_of (charT c, size_type pos = npos) const;

  basic_string substr (size_type pos = 0, size_type n = npos) const
    { return basic_string (*this, pos, n); }

  int compare (const basic_string& str, size_type pos = 0, size_type n = npos) const;
   
  int compare (const charT* s, size_type pos, size_type n) const;
  int compare (const charT* s, size_type pos = 0) const
    { return compare (s, pos, traits::length (s)); }

  iterator begin () { selfish (); return &(*this)[0]; }
  iterator end () { selfish (); return &(*this)[length ()]; }

private:
  iterator ibegin () const { return &(*rep ())[0]; }
  iterator iend () const { return &(*rep ())[length ()]; }

public:
  const_iterator begin () const { return ibegin (); }
  const_iterator end () const { return iend (); }

  reverse_iterator       rbegin() { return reverse_iterator (end ()); }
  const_reverse_iterator rbegin() const
    { return const_reverse_iterator (end ()); }
  reverse_iterator       rend() { return reverse_iterator (begin ()); }
  const_reverse_iterator rend() const
    { return const_reverse_iterator (begin ()); }

private:
  void alloc (size_type size, bool save);
  static size_type _find (const charT* ptr, charT c, size_type xpos, size_type len);
  inline bool check_realloc (size_type s) const;

  static Rep nilRep;
  charT *dat;
};


template <class charT, class traits, class Allocator> template <class InputIterator>
basic_string <charT, traits, Allocator>& basic_string <charT, traits, Allocator>::
replace (iterator i1, iterator i2, InputIterator j1, InputIterator j2)





{
  const size_type len = length ();
  size_type pos = i1 - ibegin ();
  size_type n1 = i2 - i1;
  size_type n2 = j2 - j1;

  do { if ( pos > len ) __out_of_range ("pos > len"); } while (0) ;
  if (n1 > len - pos)
    n1 = len - pos;
  do { if ( len - n1 > max_size () - n2 ) __length_error ("len - n1 > max_size () - n2"); } while (0) ;
  size_t newlen = len - n1 + n2;

  if (check_realloc (newlen))
    {
      Rep *p = Rep::create (newlen);
      p->copy (0, data (), pos);
      p->copy (pos + n2, data () + pos + n1, len - (pos + n1));
      for (; j1 != j2; ++j1, ++pos)
	traits::assign ((*p)[pos], *j1);
      repup (p);
    }
  else
    {
      rep ()->move (pos + n2, data () + pos + n1, len - (pos + n1));
      for (; j1 != j2; ++j1, ++pos)
	traits::assign ((*rep ())[pos], *j1);
    }
  rep ()->len = newlen;

  return *this;
}

template <class charT, class traits, class Allocator>
inline basic_string <charT, traits, Allocator>
operator+ (const basic_string <charT, traits, Allocator>& lhs,
	   const basic_string <charT, traits, Allocator>& rhs)
{
  basic_string <charT, traits, Allocator> _str (lhs);
  _str.append (rhs);
  return _str;
}

template <class charT, class traits, class Allocator>
inline basic_string <charT, traits, Allocator>
operator+ (const charT* lhs, const basic_string <charT, traits, Allocator>& rhs)
{
  basic_string <charT, traits, Allocator> _str (lhs);
  _str.append (rhs);
  return _str;
}

template <class charT, class traits, class Allocator>
inline basic_string <charT, traits, Allocator>
operator+ (charT lhs, const basic_string <charT, traits, Allocator>& rhs)
{
  basic_string <charT, traits, Allocator> _str (1, lhs);
  _str.append (rhs);
  return _str;
}

template <class charT, class traits, class Allocator>
inline basic_string <charT, traits, Allocator>
operator+ (const basic_string <charT, traits, Allocator>& lhs, const charT* rhs)
{
  basic_string <charT, traits, Allocator> _str (lhs);
  _str.append (rhs);
  return _str;
}

template <class charT, class traits, class Allocator>
inline basic_string <charT, traits, Allocator>
operator+ (const basic_string <charT, traits, Allocator>& lhs, charT rhs)
{
  basic_string <charT, traits, Allocator> str (lhs);
  str.append (1, rhs);
  return str;
}

template <class charT, class traits, class Allocator>
inline bool
operator== (const basic_string <charT, traits, Allocator>& lhs,
	    const basic_string <charT, traits, Allocator>& rhs)
{
  return (lhs.compare (rhs) == 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator== (const charT* lhs, const basic_string <charT, traits, Allocator>& rhs)
{
  return (rhs.compare (lhs) == 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator== (const basic_string <charT, traits, Allocator>& lhs, const charT* rhs)
{
  return (lhs.compare (rhs) == 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator!= (const charT* lhs, const basic_string <charT, traits, Allocator>& rhs)
{
  return (rhs.compare (lhs) != 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator!= (const basic_string <charT, traits, Allocator>& lhs, const charT* rhs)
{
  return (lhs.compare (rhs) != 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator< (const basic_string <charT, traits, Allocator>& lhs,
	    const basic_string <charT, traits, Allocator>& rhs)
{
  return (lhs.compare (rhs) < 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator< (const charT* lhs, const basic_string <charT, traits, Allocator>& rhs)
{
  return (rhs.compare (lhs) > 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator< (const basic_string <charT, traits, Allocator>& lhs, const charT* rhs)
{
  return (lhs.compare (rhs) < 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator> (const charT* lhs, const basic_string <charT, traits, Allocator>& rhs)
{
  return (rhs.compare (lhs) < 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator> (const basic_string <charT, traits, Allocator>& lhs, const charT* rhs)
{
  return (lhs.compare (rhs) > 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator<= (const charT* lhs, const basic_string <charT, traits, Allocator>& rhs)
{
  return (rhs.compare (lhs) >= 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator<= (const basic_string <charT, traits, Allocator>& lhs, const charT* rhs)
{
  return (lhs.compare (rhs) <= 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator>= (const charT* lhs, const basic_string <charT, traits, Allocator>& rhs)
{
  return (rhs.compare (lhs) <= 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator>= (const basic_string <charT, traits, Allocator>& lhs, const charT* rhs)
{
  return (lhs.compare (rhs) >= 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator!= (const basic_string <charT, traits, Allocator>& lhs,
	    const basic_string <charT, traits, Allocator>& rhs)
{
  return (lhs.compare (rhs) != 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator> (const basic_string <charT, traits, Allocator>& lhs,
	   const basic_string <charT, traits, Allocator>& rhs)
{
  return (lhs.compare (rhs) > 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator<= (const basic_string <charT, traits, Allocator>& lhs,
	    const basic_string <charT, traits, Allocator>& rhs)
{
  return (lhs.compare (rhs) <= 0);
}

template <class charT, class traits, class Allocator>
inline bool
operator>= (const basic_string <charT, traits, Allocator>& lhs,
	    const basic_string <charT, traits, Allocator>& rhs)
{
  return (lhs.compare (rhs) >= 0);
}

class istream; class ostream;
template <class charT, class traits, class Allocator> istream&
operator>> (istream&, basic_string <charT, traits, Allocator>&);
template <class charT, class traits, class Allocator> ostream&
operator<< (ostream&, const basic_string <charT, traits, Allocator>&);
template <class charT, class traits, class Allocator> istream&
getline (istream&, basic_string <charT, traits, Allocator>&, charT delim = '\n');

}  

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/std/bastring.cc" 1 3
 
 

 
 
 
 
 

 
 
 
 

 
 
 

 
 
 
 
 

 
 

extern "C++" {
template <class charT, class traits, class Allocator>
inline void * basic_string <charT, traits, Allocator>::Rep::
operator new (size_t s, size_t extra)
{
  return Allocator::allocate(s + extra * sizeof (charT));
}

template <class charT, class traits, class Allocator>
inline void basic_string <charT, traits, Allocator>::Rep::
operator delete (void * ptr)
{
  Allocator::deallocate(ptr, sizeof(Rep) +
			reinterpret_cast<Rep *>(ptr)->res *
			sizeof (charT));
}

template <class charT, class traits, class Allocator>
inline size_t basic_string <charT, traits, Allocator>::Rep::
frob_size (size_t s)
{
  size_t i = 16;
  while (i < s) i *= 2;
  return i;
}

template <class charT, class traits, class Allocator>
inline basic_string <charT, traits, Allocator>::Rep *
basic_string <charT, traits, Allocator>::Rep::
create (size_t extra)
{
  extra = frob_size (extra + 1);
  Rep *p = new (extra) Rep;
  p->res = extra;
  p->ref = 1;
  p->selfish = false;
  return p;
}

template <class charT, class traits, class Allocator>
charT * basic_string <charT, traits, Allocator>::Rep::
clone ()
{
  Rep *p = Rep::create (len);
  p->copy (0, data (), len);
  p->len = len;
  return p->data ();
}

template <class charT, class traits, class Allocator>
inline bool basic_string <charT, traits, Allocator>::Rep::
excess_slop (size_t s, size_t r)
{
  return 2 * (s <= 16 ? 16 : s) < r;
}

template <class charT, class traits, class Allocator>
inline bool basic_string <charT, traits, Allocator>::
check_realloc (basic_string::size_type s) const
{
  s += sizeof (charT);
  rep ()->selfish = false;
  return (rep ()->ref > 1
	  || s > capacity ()
	  || Rep::excess_slop (s, capacity ()));
}

template <class charT, class traits, class Allocator>
void basic_string <charT, traits, Allocator>::
alloc (basic_string::size_type __size, bool __save)
{
  if (! check_realloc (__size))
    return;

  Rep *p = Rep::create (__size);

  if (__save)
    {
      p->copy (0, data (), length ());
      p->len = length ();
    }
  else
    p->len = 0;

  repup (p);
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>&
basic_string <charT, traits, Allocator>::
replace (size_type pos1, size_type n1,
	 const basic_string& _str, size_type pos2, size_type n2)
{
  const size_t len2 = _str.length ();

  if (pos1 == 0 && n1 >= length () && pos2 == 0 && n2 >= len2)
    return operator= (_str);

  do { if ( pos2 > len2 ) __out_of_range ("pos2 > len2"); } while (0) ;

  if (n2 > len2 - pos2)
    n2 = len2 - pos2;

  return replace (pos1, n1, _str.data () + pos2, n2);
}

template <class charT, class traits, class Allocator>
inline void basic_string <charT, traits, Allocator>::Rep::
copy (size_t pos, const charT *s, size_t n)
{
  if (n)
    traits::copy (data () + pos, s, n);
}

template <class charT, class traits, class Allocator>
inline void basic_string <charT, traits, Allocator>::Rep::
move (size_t pos, const charT *s, size_t n)
{
  if (n)
    traits::move (data () + pos, s, n);
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>&
basic_string <charT, traits, Allocator>::
replace (size_type pos, size_type n1, const charT* s, size_type n2)
{
  const size_type len = length ();
  do { if ( pos > len ) __out_of_range ("pos > len"); } while (0) ;
  if (n1 > len - pos)
    n1 = len - pos;
  do { if ( len - n1 > max_size () - n2 ) __length_error ("len - n1 > max_size () - n2"); } while (0) ;
  size_t newlen = len - n1 + n2;

  if (check_realloc (newlen))
    {
      Rep *p = Rep::create (newlen);
      p->copy (0, data (), pos);
      p->copy (pos + n2, data () + pos + n1, len - (pos + n1));
      p->copy (pos, s, n2);
      repup (p);
    }
  else
    {
      rep ()->move (pos + n2, data () + pos + n1, len - (pos + n1));
      rep ()->copy (pos, s, n2);
    }
  rep ()->len = newlen;

  return *this;
}

template <class charT, class traits, class Allocator>
inline void basic_string <charT, traits, Allocator>::Rep::
set (size_t pos, const charT c, size_t n)
{
  traits::set  (data () + pos, c, n);
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>& basic_string <charT, traits, Allocator>::
replace (size_type pos, size_type n1, size_type n2, charT c)
{
  const size_t len = length ();
  do { if ( pos > len ) __out_of_range ("pos > len"); } while (0) ;
  if (n1 > len - pos)
    n1 = len - pos;
  do { if ( len - n1 > max_size () - n2 ) __length_error ("len - n1 > max_size () - n2"); } while (0) ;
  size_t newlen = len - n1 + n2;

  if (check_realloc (newlen))
    {
      Rep *p = Rep::create (newlen);
      p->copy (0, data (), pos);
      p->copy (pos + n2, data () + pos + n1, len - (pos + n1));
      p->set  (pos, c, n2);
      repup (p);
    }
  else
    {
      rep ()->move (pos + n2, data () + pos + n1, len - (pos + n1));
      rep ()->set  (pos, c, n2);
    }
  rep ()->len = newlen;

  return *this;
}

template <class charT, class traits, class Allocator>
void basic_string <charT, traits, Allocator>::
resize (size_type n, charT c)
{
  do { if ( n > max_size () ) __length_error ("n > max_size ()"); } while (0) ;

  if (n > length ())
    append (n - length (), c);
  else
    erase (n);
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>::size_type
basic_string <charT, traits, Allocator>::
copy (charT* s, size_type n, size_type pos) const
{
  do { if ( pos > length () ) __out_of_range ("pos > length ()"); } while (0) ;

  if (n > length () - pos)
    n = length () - pos;

  traits::copy (s, data () + pos, n);
  return n;
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>::size_type
basic_string <charT, traits, Allocator>::
find (const charT* s, size_type pos, size_type n) const
{
  size_t xpos = pos;
  for (; xpos + n <= length (); ++xpos)
    if (traits::eq (data () [xpos], *s)
	&& traits::compare (data () + xpos, s, n) == 0)
      return xpos;
  return npos;
}

template <class charT, class traits, class Allocator>
inline basic_string <charT, traits, Allocator>::size_type
basic_string <charT, traits, Allocator>::
_find (const charT* ptr, charT c, size_type xpos, size_type len)
{
  for (; xpos < len; ++xpos)
    if (traits::eq (ptr [xpos], c))
      return xpos;
  return npos;
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>::size_type
basic_string <charT, traits, Allocator>::
find (charT c, size_type pos) const
{
  return _find (data (), c, pos, length ());
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>::size_type
basic_string <charT, traits, Allocator>::
rfind (const charT* s, size_type pos, size_type n) const
{
  if (n > length ())
    return npos;

  size_t xpos = length () - n;
  if (xpos > pos)
    xpos = pos;

  for (++xpos; xpos-- > 0; )
    if (traits::eq (data () [xpos], *s)
	&& traits::compare (data () + xpos, s, n) == 0)
      return xpos;
  return npos;
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>::size_type
basic_string <charT, traits, Allocator>::
rfind (charT c, size_type pos) const
{
  if (1 > length ())
    return npos;

  size_t xpos = length () - 1;
  if (xpos > pos)
    xpos = pos;

  for (++xpos; xpos-- > 0; )
    if (traits::eq (data () [xpos], c))
      return xpos;
  return npos;
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>::size_type
basic_string <charT, traits, Allocator>::
find_first_of (const charT* s, size_type pos, size_type n) const
{
  size_t xpos = pos;
  for (; xpos < length (); ++xpos)
    if (_find (s, data () [xpos], 0, n) != npos)
      return xpos;
  return npos;
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>::size_type
basic_string <charT, traits, Allocator>::
find_last_of (const charT* s, size_type pos, size_type n) const
{
  if (length() == 0)
    return npos;
  size_t xpos = length () - 1;
  if (xpos > pos)
    xpos = pos;
  for (++xpos; xpos-- > 0;)
    if (_find (s, data () [xpos], 0, n) != npos)
      return xpos;
  return npos;
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>::size_type
basic_string <charT, traits, Allocator>::
find_first_not_of (const charT* s, size_type pos, size_type n) const
{
  size_t xpos = pos;
  for (; xpos < length (); ++xpos)
    if (_find (s, data () [xpos], 0, n) == npos)
      return xpos;
  return npos;
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>::size_type
basic_string <charT, traits, Allocator>::
find_first_not_of (charT c, size_type pos) const
{
  size_t xpos = pos;
  for (; xpos < length (); ++xpos)
    if (traits::ne (data () [xpos], c))
      return xpos;
  return npos;
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>::size_type
basic_string <charT, traits, Allocator>::
find_last_not_of (const charT* s, size_type pos, size_type n) const
{
  if (length() == 0)
    return npos;
  size_t xpos = length () - 1;
  if (xpos > pos)
    xpos = pos;
  for (++xpos; xpos-- > 0;)
    if (_find (s, data () [xpos], 0, n) == npos)
      return xpos;
  return npos;
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>::size_type
basic_string <charT, traits, Allocator>::
find_last_not_of (charT c, size_type pos) const
{
  if (length() == 0)
    return npos;
  size_t xpos = length () - 1;
  if (xpos > pos)
    xpos = pos;
  for (++xpos; xpos-- > 0;)
    if (traits::ne (data () [xpos], c))
      return xpos;
  return npos;
}

template <class charT, class traits, class Allocator>
int basic_string <charT, traits, Allocator>::
compare (const basic_string& _str, size_type pos, size_type n) const
{
  do { if ( pos > length () ) __out_of_range ("pos > length ()"); } while (0) ;

  size_t rlen = length () - pos;
  if (rlen > n)
    rlen = n;
  if (rlen > _str.length ())
    rlen = _str.length ();
  int r = traits::compare (data () + pos, _str.data (), rlen);
  if (r != 0)
    return r;
  if (rlen == n)
    return 0;
  return (length () - pos) - _str.length ();
}

template <class charT, class traits, class Allocator>
int basic_string <charT, traits, Allocator>::
compare (const charT* s, size_type pos, size_type n) const
{
  do { if ( pos > length () ) __out_of_range ("pos > length ()"); } while (0) ;

  size_t rlen = length () - pos;
  if (rlen > n)
    rlen = n;
  int r = traits::compare (data () + pos, s, rlen);
  if (r != 0)
    return r;
  return (length () - pos) - n;
}



template <class charT, class traits, class Allocator>
istream &
operator>> (istream &is, basic_string <charT, traits, Allocator> &s)
{
  int w = is.width (0);
  if (is.ipfx0 ())
    {
      register streambuf *sb = is.rdbuf ();
      s.resize (0);
      while (1)
	{
	  int ch = sb->sbumpc ();
	  if (ch == (-1) )
	    {
	      is.setstate (ios::eofbit);
	      break;
	    }
	  else if (traits::is_del (ch))
	    {
	      sb->sungetc ();
	      break;
	    }
	  s += static_cast<charT> (ch);
	  if (--w == 1)
	    break;
	}
    }

  is.isfx ();
  if (s.length () == 0)
    is.setstate (ios::failbit);

  return is;
}

template <class charT, class traits, class Allocator>
ostream &
operator<< (ostream &o, const basic_string <charT, traits, Allocator>& s)
{
  return o.write (s.data (), s.length ());
}

template <class charT, class traits, class Allocator>
istream&
getline (istream &is, basic_string <charT, traits, Allocator>& s, charT delim)
{
  if (is.ipfx1 ())
    {
      size_t   _count = 0;
      streambuf *sb = is.rdbuf ();
      s.resize (0);

      while (1)
	{
	  int ch = sb->sbumpc ();
	  if (ch == (-1) )
	    {
	      is.setstate (_count == 0
			   ? (ios::failbit|ios::eofbit)
			   : ios::eofbit);
	      break;
	    }

	  ++_count;

	  if (ch == delim)
	    break;

	  s += static_cast<charT> (ch);

	  if (s.length () == s.npos - 1)
	    {
	      is.setstate (ios::failbit);
	      break;
	    }
	}
    }

   
   
  is.isfx ();

  return is;
}

template <class charT, class traits, class Allocator>
basic_string <charT, traits, Allocator>::Rep
basic_string<charT, traits, Allocator>::nilRep = { 0, 0, 1, false };

template <class charT, class traits, class Allocator>
const basic_string <charT, traits, Allocator>::size_type
basic_string <charT, traits, Allocator>::npos;

}  
# 656 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/std/bastring.h" 2 3



# 6 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/string" 2 3


extern "C++" {
typedef basic_string <char> string;



}  


# 82 "/usr/include/boost/config/stdlib/sgi.hpp" 2 3







 
 
 




 
 
 
 
 
 




# 44 "/usr/include/boost/config.hpp" 2 3



 

# 1 "/usr/include/boost/config/select_platform_config.hpp" 1 3
 

 
 
 
 

 

 
 
 
 


 


# 78 "/usr/include/boost/config/select_platform_config.hpp" 3



# 49 "/usr/include/boost/config.hpp" 2 3


 

# 1 "/usr/include/boost/config/platform/linux.hpp" 1 3
 
 
 
 

 

 



 
# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/cstdlib" 1 3
 
 






#pragma interface "cstdlib"


extern "C++" {




inline long   abs(long x)		{ return x >= 0 ? x : -x; }

 

}  


# 13 "/usr/include/boost/config/platform/linux.hpp" 2 3


 
 
 
 

    
    
    





 
 
 
 









 

    







 

# 1 "/usr/include/boost/config/posix_features.hpp" 1 3
 
 
 
 

 

 


# 1 "/usr/include/unistd.h" 1 3
 

















 








extern "C" { 

 


 



 


 


 


 



 



 



 



 






 


 




 


 


 



 



 



















































































# 1 "/usr/include/bits/posix_opt.h" 1 3
 





















 


 


 


 


 


 


 


 


 


 


 


 


 



 


 


 


 


 


 



 


 


 


 


 


 


 


 


 




 


 


 


 


 


 


 


 


 


 


 


 



# 175 "/usr/include/unistd.h" 2 3


 




 





 










# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 


# 126 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 




 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 


# 269 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3
















 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 













 







# 199 "/usr/include/unistd.h" 2 3


# 236 "/usr/include/unistd.h" 3




typedef __intptr_t intptr_t;






typedef __socklen_t socklen_t;




 






 
extern int access (__const char *__name, int __type) throw () ;








 







 






 





extern __off_t lseek (int __fd, __off_t __offset, int __whence) throw () ;
# 300 "/usr/include/unistd.h" 3





 
extern int close (int __fd) throw () ;

 

extern ssize_t read (int __fd, void *__buf, size_t __nbytes) throw () ;

 
extern ssize_t write (int __fd, __const void *__buf, size_t __n) throw () ;

# 347 "/usr/include/unistd.h" 3


 



extern int pipe (int __pipedes[2]) throw () ;

 






extern unsigned int alarm (unsigned int __seconds) throw () ;

 






extern unsigned int sleep (unsigned int __seconds) throw () ;


 



extern __useconds_t ualarm (__useconds_t __value, __useconds_t __interval)
     throw () ;

 

extern int usleep (__useconds_t __useconds) throw () ;



 

extern int pause (void) throw () ;


 
extern int chown (__const char *__file, __uid_t __owner, __gid_t __group)
     throw () ;


 
extern int fchown (int __fd, __uid_t __owner, __gid_t __group) throw () ;


 

extern int lchown (__const char *__file, __uid_t __owner, __gid_t __group)
     throw () ;



 
extern int chdir (__const char *__path) throw () ;


 
extern int fchdir (int __fd) throw () ;


 






extern char *getcwd (char *__buf, size_t __size) throw () ;









 


extern char *getwd (char *__buf) throw () ;



 
extern int dup (int __fd) throw () ;

 
extern int dup2 (int __fd, int __fd2) throw () ;

 
extern char **__environ;





 

extern int execve (__const char *__path, char *__const __argv[],
		   char *__const __envp[]) throw () ;









 
extern int execv (__const char *__path, char *__const __argv[]) throw () ;

 

extern int execle (__const char *__path, __const char *__arg, ...) throw () ;

 

extern int execl (__const char *__path, __const char *__arg, ...) throw () ;

 

extern int execvp (__const char *__file, char *__const __argv[]) throw () ;

 


extern int execlp (__const char *__file, __const char *__arg, ...) throw () ;



 
extern int nice (int __inc) throw () ;



 
extern void _exit (int __status) __attribute__ ((__noreturn__));


 


# 1 "/usr/include/bits/confname.h" 1 3
 






















 
enum
  {
    _PC_LINK_MAX,

    _PC_MAX_CANON,

    _PC_MAX_INPUT,

    _PC_NAME_MAX,

    _PC_PATH_MAX,

    _PC_PIPE_BUF,

    _PC_CHOWN_RESTRICTED,

    _PC_NO_TRUNC,

    _PC_VDISABLE,

    _PC_SYNC_IO,

    _PC_ASYNC_IO,

    _PC_PRIO_IO,

    _PC_SOCK_MAXBUF,

    _PC_FILESIZEBITS,

    _PC_REC_INCR_XFER_SIZE,

    _PC_REC_MAX_XFER_SIZE,

    _PC_REC_MIN_XFER_SIZE,

    _PC_REC_XFER_ALIGN,

    _PC_ALLOC_SIZE_MIN,

    _PC_SYMLINK_MAX

  };

 
enum
  {
    _SC_ARG_MAX,

    _SC_CHILD_MAX,

    _SC_CLK_TCK,

    _SC_NGROUPS_MAX,

    _SC_OPEN_MAX,

    _SC_STREAM_MAX,

    _SC_TZNAME_MAX,

    _SC_JOB_CONTROL,

    _SC_SAVED_IDS,

    _SC_REALTIME_SIGNALS,

    _SC_PRIORITY_SCHEDULING,

    _SC_TIMERS,

    _SC_ASYNCHRONOUS_IO,

    _SC_PRIORITIZED_IO,

    _SC_SYNCHRONIZED_IO,

    _SC_FSYNC,

    _SC_MAPPED_FILES,

    _SC_MEMLOCK,

    _SC_MEMLOCK_RANGE,

    _SC_MEMORY_PROTECTION,

    _SC_MESSAGE_PASSING,

    _SC_SEMAPHORES,

    _SC_SHARED_MEMORY_OBJECTS,

    _SC_AIO_LISTIO_MAX,

    _SC_AIO_MAX,

    _SC_AIO_PRIO_DELTA_MAX,

    _SC_DELAYTIMER_MAX,

    _SC_MQ_OPEN_MAX,

    _SC_MQ_PRIO_MAX,

    _SC_VERSION,

    _SC_PAGESIZE,


    _SC_RTSIG_MAX,

    _SC_SEM_NSEMS_MAX,

    _SC_SEM_VALUE_MAX,

    _SC_SIGQUEUE_MAX,

    _SC_TIMER_MAX,


     

    _SC_BC_BASE_MAX,

    _SC_BC_DIM_MAX,

    _SC_BC_SCALE_MAX,

    _SC_BC_STRING_MAX,

    _SC_COLL_WEIGHTS_MAX,

    _SC_EQUIV_CLASS_MAX,

    _SC_EXPR_NEST_MAX,

    _SC_LINE_MAX,

    _SC_RE_DUP_MAX,

    _SC_CHARCLASS_NAME_MAX,


    _SC_2_VERSION,

    _SC_2_C_BIND,

    _SC_2_C_DEV,

    _SC_2_FORT_DEV,

    _SC_2_FORT_RUN,

    _SC_2_SW_DEV,

    _SC_2_LOCALEDEF,


    _SC_PII,

    _SC_PII_XTI,

    _SC_PII_SOCKET,

    _SC_PII_INTERNET,

    _SC_PII_OSI,

    _SC_POLL,

    _SC_SELECT,

    _SC_UIO_MAXIOV,

    _SC_IOV_MAX = _SC_UIO_MAXIOV ,

    _SC_PII_INTERNET_STREAM,

    _SC_PII_INTERNET_DGRAM,

    _SC_PII_OSI_COTS,

    _SC_PII_OSI_CLTS,

    _SC_PII_OSI_M,

    _SC_T_IOV_MAX,


     
    _SC_THREADS,

    _SC_THREAD_SAFE_FUNCTIONS,

    _SC_GETGR_R_SIZE_MAX,

    _SC_GETPW_R_SIZE_MAX,

    _SC_LOGIN_NAME_MAX,

    _SC_TTY_NAME_MAX,

    _SC_THREAD_DESTRUCTOR_ITERATIONS,

    _SC_THREAD_KEYS_MAX,

    _SC_THREAD_STACK_MIN,

    _SC_THREAD_THREADS_MAX,

    _SC_THREAD_ATTR_STACKADDR,

    _SC_THREAD_ATTR_STACKSIZE,

    _SC_THREAD_PRIORITY_SCHEDULING,

    _SC_THREAD_PRIO_INHERIT,

    _SC_THREAD_PRIO_PROTECT,

    _SC_THREAD_PROCESS_SHARED,


    _SC_NPROCESSORS_CONF,

    _SC_NPROCESSORS_ONLN,

    _SC_PHYS_PAGES,

    _SC_AVPHYS_PAGES,

    _SC_ATEXIT_MAX,

    _SC_PASS_MAX,


    _SC_XOPEN_VERSION,

    _SC_XOPEN_XCU_VERSION,

    _SC_XOPEN_UNIX,

    _SC_XOPEN_CRYPT,

    _SC_XOPEN_ENH_I18N,

    _SC_XOPEN_SHM,


    _SC_2_CHAR_TERM,

    _SC_2_C_VERSION,

    _SC_2_UPE,


    _SC_XOPEN_XPG2,

    _SC_XOPEN_XPG3,

    _SC_XOPEN_XPG4,


    _SC_CHAR_BIT,

    _SC_CHAR_MAX,

    _SC_CHAR_MIN,

    _SC_INT_MAX,

    _SC_INT_MIN,

    _SC_LONG_BIT,

    _SC_WORD_BIT,

    _SC_MB_LEN_MAX,

    _SC_NZERO,

    _SC_SSIZE_MAX,

    _SC_SCHAR_MAX,

    _SC_SCHAR_MIN,

    _SC_SHRT_MAX,

    _SC_SHRT_MIN,

    _SC_UCHAR_MAX,

    _SC_UINT_MAX,

    _SC_ULONG_MAX,

    _SC_USHRT_MAX,


    _SC_NL_ARGMAX,

    _SC_NL_LANGMAX,

    _SC_NL_MSGMAX,

    _SC_NL_NMAX,

    _SC_NL_SETMAX,

    _SC_NL_TEXTMAX,


    _SC_XBS5_ILP32_OFF32,

    _SC_XBS5_ILP32_OFFBIG,

    _SC_XBS5_LP64_OFF64,

    _SC_XBS5_LPBIG_OFFBIG,


    _SC_XOPEN_LEGACY,

    _SC_XOPEN_REALTIME,

    _SC_XOPEN_REALTIME_THREADS,


    _SC_ADVISORY_INFO,

    _SC_BARRIERS,

    _SC_BASE,

    _SC_C_LANG_SUPPORT,

    _SC_C_LANG_SUPPORT_R,

    _SC_CLOCK_SELECTION,

    _SC_CPUTIME,

    _SC_THREAD_CPUTIME,

    _SC_DEVICE_IO,

    _SC_DEVICE_SPECIFIC,

    _SC_DEVICE_SPECIFIC_R,

    _SC_FD_MGMT,

    _SC_FIFO,

    _SC_PIPE,

    _SC_FILE_ATTRIBUTES,

    _SC_FILE_LOCKING,

    _SC_FILE_SYSTEM,

    _SC_MONOTONIC_CLOCK,

    _SC_MULTI_PROCESS,

    _SC_SINGLE_PROCESS,

    _SC_NETWORKING,

    _SC_READER_WRITER_LOCKS,

    _SC_SPIN_LOCKS,

    _SC_REGEXP,

    _SC_REGEX_VERSION,

    _SC_SHELL,

    _SC_SIGNALS,

    _SC_SPAWN,

    _SC_SPORADIC_SERVER,

    _SC_THREAD_SPORADIC_SERVER,

    _SC_SYSTEM_DATABASE,

    _SC_SYSTEM_DATABASE_R,

    _SC_TIMEOUTS,

    _SC_TYPED_MEMORY_OBJECTS,

    _SC_USER_GROUPS,

    _SC_USER_GROUPS_R,

    _SC_2_PBS,

    _SC_2_PBS_ACCOUNTING,

    _SC_2_PBS_LOCATE,

    _SC_2_PBS_MESSAGE,

    _SC_2_PBS_TRACK,

    _SC_SYMLOOP_MAX,

    _SC_STREAMS,

    _SC_2_PBS_CHECKPOINT,


    _SC_V6_ILP32_OFF32,

    _SC_V6_ILP32_OFFBIG,

    _SC_V6_LP64_OFF64,

    _SC_V6_LPBIG_OFFBIG,


    _SC_HOST_NAME_MAX,

    _SC_TRACE,

    _SC_TRACE_EVENT_FILTER,

    _SC_TRACE_INHERIT,

    _SC_TRACE_LOG

  };




 
enum
  {
    _CS_PATH,			 


# 492 "/usr/include/bits/confname.h" 3


# 527 "/usr/include/bits/confname.h" 3

# 561 "/usr/include/bits/confname.h" 3


    _CS_V6_WIDTH_RESTRICTED_ENVS

  };

# 500 "/usr/include/unistd.h" 2 3


 
extern long int pathconf (__const char *__path, int __name) throw () ;

 
extern long int fpathconf (int __fd, int __name) throw () ;

 
extern long int sysconf (int __name) throw ()  __attribute__ ((__const__));


 
extern size_t confstr (int __name, char *__buf, size_t __len) throw () ;



 
extern __pid_t getpid (void) throw () ;

 
extern __pid_t getppid (void) throw () ;

 


extern __pid_t getpgrp (void) throw () ;








 
extern __pid_t __getpgid (__pid_t __pid) throw () ;





 


extern int setpgid (__pid_t __pid, __pid_t __pgid) throw () ;


 











 

extern int setpgrp (void) throw () ;

# 574 "/usr/include/unistd.h" 3



 


extern __pid_t setsid (void) throw () ;






 
extern __uid_t getuid (void) throw () ;

 
extern __uid_t geteuid (void) throw () ;

 
extern __gid_t getgid (void) throw () ;

 
extern __gid_t getegid (void) throw () ;

 


extern int getgroups (int __size, __gid_t __list[]) throw () ;






 



extern int setuid (__uid_t __uid) throw () ;


 

extern int setreuid (__uid_t __ruid, __uid_t __euid) throw () ;



 
extern int seteuid (__uid_t __uid) throw () ;


 



extern int setgid (__gid_t __gid) throw () ;


 

extern int setregid (__gid_t __rgid, __gid_t __egid) throw () ;



 
extern int setegid (__gid_t __gid) throw () ;



 


extern __pid_t fork (void) throw () ;


 



extern __pid_t vfork (void) throw () ;



 

extern char *ttyname (int __fd) throw () ;

 

extern int ttyname_r (int __fd, char *__buf, size_t __buflen) throw () ;

 

extern int isatty (int __fd) throw () ;



 

extern int ttyslot (void) throw () ;



 
extern int link (__const char *__from, __const char *__to) throw () ;


 
extern int symlink (__const char *__from, __const char *__to) throw () ;

 


extern int readlink (__const char *__restrict __path, char *__restrict __buf,
		     size_t __len) throw () ;


 
extern int unlink (__const char *__name) throw () ;

 
extern int rmdir (__const char *__path) throw () ;


 
extern __pid_t tcgetpgrp (int __fd) throw () ;

 
extern int tcsetpgrp (int __fd, __pid_t __pgrp_id) throw () ;


 
extern char *getlogin (void) throw () ;








 
extern int setlogin (__const char *__name) throw () ;




 



# 1 "/usr/include/getopt.h" 1 3
 
























 











extern "C" {


 





extern char *optarg;

 











extern int optind;

 


extern int opterr;

 

extern int optopt;

# 113 "/usr/include/getopt.h" 3



 

























 


extern int getopt (int ___argc, char *const *___argv, const char *__shortopts);




# 163 "/usr/include/getopt.h" 3

# 172 "/usr/include/getopt.h" 3



}


 



# 726 "/usr/include/unistd.h" 2 3





 


extern int gethostname (char *__name, size_t __len) throw () ;




 

extern int sethostname (__const char *__name, size_t __len) throw () ;

 

extern int sethostid (long int __id) throw () ;


 


extern int getdomainname (char *__name, size_t __len) throw () ;
extern int setdomainname (__const char *__name, size_t __len) throw () ;


 


extern int vhangup (void) throw () ;

 
extern int revoke (__const char *__file) throw () ;


 




extern int profil (unsigned short int *__sample_buffer, size_t __size,
		   size_t __offset, unsigned int __scale) throw () ;


 


extern int acct (__const char *__name) throw () ;


 
extern char *getusershell (void) throw () ;
extern void endusershell (void) throw () ;  
extern void setusershell (void) throw () ;  


 


extern int daemon (int __nochdir, int __noclose) throw () ;




 

extern int chroot (__const char *__path) throw () ;

 

extern char *getpass (__const char *__prompt) throw () ;




 
extern int fsync (int __fd) throw () ;





 
extern long int gethostid (void) throw () ;

 
extern void sync (void) throw () ;


 

extern int getpagesize (void)  throw ()  __attribute__ ((__const__));


 

extern int truncate (__const char *__file, __off_t __length) throw () ;
# 834 "/usr/include/unistd.h" 3





 

extern int ftruncate (int __fd, __off_t __length) throw () ;













 

extern int getdtablesize (void) throw () ;






 

extern int brk (void *__addr) throw () ;

 



extern void *sbrk (intptr_t __delta) throw () ;




 









extern long int syscall (long int __sysno, ...) throw () ;





 



 









extern int lockf (int __fd, int __cmd, __off_t __len) throw () ;














# 933 "/usr/include/unistd.h" 3



 

extern int fdatasync (int __fildes) throw () ;



 

# 959 "/usr/include/unistd.h" 3



 








 

 









extern int pthread_atfork (void (*__prepare) (void),
			   void (*__parent) (void),
			   void (*__child) (void)) throw () ;


} 


# 11 "/usr/include/boost/config/posix_features.hpp" 2 3


       




       




       
       
       
       
       
       




       
       





       
       
       
       




       
       
       






       
       
       
       






# 53 "/usr/include/boost/config/platform/linux.hpp" 2 3


# 79 "/usr/include/boost/config/platform/linux.hpp" 3


# 53 "/usr/include/boost/config.hpp" 2 3



 
# 1 "/usr/include/boost/config/suffix.hpp" 1 3
 

 
 
 
 

 

 
 
 
 
 
 








 
 
 
 
 
 
# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 1 3
 


 

# 114 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 3







# 30 "/usr/include/boost/config/suffix.hpp" 2 3






 
 
 





 
 
 




 
 
 





 
 
 
 





 
 
 
 





 
 
 





 
 
 
 






 
 
 
 






 
 
 




 
 
 




 
 
 




 
 
 




 
 
 
 
 
 





 
 
 




 
 
 






 
 
 
 




 
 
 
 








 
 
 
 
 
 
 
 






 

# 217 "/usr/include/boost/config/suffix.hpp" 3


 
 
 
 
 







 
 
 
 
 
 
 
 
 
 













 
 
 
 
 
 
 
 
 







 

 
 
 
 
 



 
 
 
 
 
 
 
 




 
 
 
 

















# 57 "/usr/include/boost/config.hpp" 2 3













# 21 "/usr/include/boost/timer.hpp" 2 3

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/ctime" 1 3
 
 





# 22 "/usr/include/boost/timer.hpp" 2 3

# 1 "/usr/include/boost/limits.hpp" 1 3

 
 
 
 
 
 







# 1 "/usr/include/boost/detail/limits.hpp" 1 3
 












 






 



 
















# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/climits" 1 3
 
 



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 1 3
 


 

# 114 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 3







# 6 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/climits" 2 3


# 42 "/usr/include/boost/detail/limits.hpp" 2 3

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/cfloat" 1 3
 
 



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/float.h" 1 3
 



 

    


    


    


    


    


    


    


    


    


    


    



    


    


    


    


    


    


    


    


    



    


    


    



union __convert_long_double {
  unsigned __convert_long_double_i[4];
  long double __convert_long_double_d;
};


    


    


    


    


    


    




# 6 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/cfloat" 2 3


# 43 "/usr/include/boost/detail/limits.hpp" 2 3




# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/cwchar" 1 3
 
 



# 1 "/usr/include/wchar.h" 1 3
 

















 












 




# 1 "/usr/include/stdio.h" 1 3
 


















 





# 39 "/usr/include/stdio.h" 3















 
typedef struct _IO_FILE __FILE;






# 635 "/usr/include/stdio.h" 3



# 37 "/usr/include/wchar.h" 2 3

 



 





# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 


# 126 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 




 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 


# 269 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3
















 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 













 







# 48 "/usr/include/wchar.h" 2 3




 











# 76 "/usr/include/wchar.h" 3




 



 
typedef __mbstate_t mbstate_t;


 








 





 

struct tm;


extern "C" { 

 
extern wchar_t *wcscpy (wchar_t *__restrict __dest,
			__const wchar_t *__restrict __src) throw () ;
 
extern wchar_t *wcsncpy (wchar_t *__restrict __dest,
			 __const wchar_t *__restrict __src, size_t __n)
     throw () ;

 
extern wchar_t *wcscat (wchar_t *__restrict __dest,
			__const wchar_t *__restrict __src) throw () ;
 
extern wchar_t *wcsncat (wchar_t *__restrict __dest,
			 __const wchar_t *__restrict __src, size_t __n)
     throw () ;

 
extern int wcscmp (__const wchar_t *__s1, __const wchar_t *__s2)
     throw ()   ;
 
extern int wcsncmp (__const wchar_t *__s1, __const wchar_t *__s2, size_t __n)
     throw ()   ;

# 150 "/usr/include/wchar.h" 3


 

extern int wcscoll (__const wchar_t *__s1, __const wchar_t *__s2) throw () ;
 


extern size_t wcsxfrm (wchar_t *__restrict __s1,
		       __const wchar_t *__restrict __s2, size_t __n) throw () ;

# 177 "/usr/include/wchar.h" 3


 
extern wchar_t *wcschr (__const wchar_t *__wcs, wchar_t __wc)
     throw ()   ;
 
extern wchar_t *wcsrchr (__const wchar_t *__wcs, wchar_t __wc)
     throw ()   ;








 

extern size_t wcscspn (__const wchar_t *__wcs, __const wchar_t *__reject)
     throw ()   ;
 

extern size_t wcsspn (__const wchar_t *__wcs, __const wchar_t *__accept)
     throw ()   ;
 
extern wchar_t *wcspbrk (__const wchar_t *__wcs, __const wchar_t *__accept)
     throw ()   ;
 
extern wchar_t *wcsstr (__const wchar_t *__haystack, __const wchar_t *__needle)
     throw ()   ;







 
extern wchar_t *wcstok (wchar_t *__restrict __s,
			__const wchar_t *__restrict __delim,
			wchar_t **__restrict __ptr) throw () ;

 
extern size_t wcslen (__const wchar_t *__s) throw ()   ;








 
extern wchar_t *wmemchr (__const wchar_t *__s, wchar_t __c, size_t __n)
     throw ()   ;

 
extern int wmemcmp (__const wchar_t *__restrict __s1,
		    __const wchar_t *__restrict __s2, size_t __n)
     throw ()   ;

 
extern wchar_t *wmemcpy (wchar_t *__restrict __s1,
			 __const wchar_t *__restrict __s2, size_t __n) throw () ;

 

extern wchar_t *wmemmove (wchar_t *__s1, __const wchar_t *__s2, size_t __n)
     throw () ;

 
extern wchar_t *wmemset (wchar_t *__s, wchar_t __c, size_t __n) throw () ;










 

extern wint_t btowc (int __c) throw () ;

 

extern int wctob (wint_t __c) throw () ;

 

extern int mbsinit (__const mbstate_t *__ps) throw ()   ;

 

extern size_t mbrtowc (wchar_t *__restrict __pwc,
		       __const char *__restrict __s, size_t __n,
		       mbstate_t *__p) throw () ;

 
extern size_t wcrtomb (char *__restrict __s, wchar_t __wc,
		       mbstate_t *__restrict __ps) throw () ;

 
extern size_t __mbrlen (__const char *__restrict __s, size_t __n,
			mbstate_t *__restrict __ps) throw () ;
extern size_t mbrlen (__const char *__restrict __s, size_t __n,
		      mbstate_t *__restrict __ps) throw () ;









 

extern size_t mbsrtowcs (wchar_t *__restrict __dst,
			 __const char **__restrict __src, size_t __len,
			 mbstate_t *__restrict __ps) throw () ;

 

extern size_t wcsrtombs (char *__restrict __dst,
			 __const wchar_t **__restrict __src, size_t __len,
			 mbstate_t *__restrict __ps) throw () ;


# 321 "/usr/include/wchar.h" 3



 










 

extern double wcstod (__const wchar_t *__restrict __nptr,
		      wchar_t **__restrict __endptr) throw () ;










 

extern long int wcstol (__const wchar_t *__restrict __nptr,
			wchar_t **__restrict __endptr, int __base) throw () ;

 

extern unsigned long int wcstoul (__const wchar_t *__restrict __nptr,
				  wchar_t **__restrict __endptr, int __base)
     throw () ;

# 374 "/usr/include/wchar.h" 3


# 390 "/usr/include/wchar.h" 3


# 440 "/usr/include/wchar.h" 3



 

extern double __wcstod_internal (__const wchar_t *__restrict __nptr,
				 wchar_t **__restrict __endptr, int __group)
     throw () ;
extern float __wcstof_internal (__const wchar_t *__restrict __nptr,
				wchar_t **__restrict __endptr, int __group)
     throw () ;
extern long double __wcstold_internal (__const wchar_t *__restrict __nptr,
				       wchar_t **__restrict __endptr,
				       int __group) throw () ;


extern long int __wcstol_internal (__const wchar_t *__restrict __nptr,
				   wchar_t **__restrict __endptr,
				   int __base, int __group) throw () ;



extern unsigned long int __wcstoul_internal (__const wchar_t *__restrict __npt,
					     wchar_t **__restrict __endptr,
					     int __base, int __group) throw () ;



__extension__
extern long long int __wcstoll_internal (__const wchar_t *__restrict __nptr,
					 wchar_t **__restrict __endptr,
					 int __base, int __group) throw () ;



__extension__
extern unsigned long long int __wcstoull_internal (__const wchar_t *
						   __restrict __nptr,
						   wchar_t **
						   __restrict __endptr,
						   int __base,
						   int __group) throw () ;




# 522 "/usr/include/wchar.h" 3



# 534 "/usr/include/wchar.h" 3



 
# 584 "/usr/include/wchar.h" 3


# 601 "/usr/include/wchar.h" 3



 
extern wint_t fgetwc (__FILE *__stream) throw () ;
extern wint_t getwc (__FILE *__stream) throw () ;

 
extern wint_t getwchar (void) throw () ;


 
extern wint_t fputwc (wchar_t __wc, __FILE *__stream) throw () ;
extern wint_t putwc (wchar_t __wc, __FILE *__stream) throw () ;

 
extern wint_t putwchar (wchar_t __wc) throw () ;


 

extern wchar_t *fgetws (wchar_t *__restrict __ws, int __n,
			__FILE *__restrict __stream) throw () ;

 
extern int fputws (__const wchar_t *__restrict __ws,
		   __FILE *__restrict __stream) throw () ;


 
extern wint_t ungetwc (wint_t __wc, __FILE *__stream) throw () ;


# 659 "/usr/include/wchar.h" 3



 


extern size_t wcsftime (wchar_t *__restrict __s, size_t __maxsize,
			__const wchar_t *__restrict __format,
			__const struct tm *__restrict __tp) throw () ;

 










} 




# 6 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/cwchar" 2 3


# 47 "/usr/include/boost/detail/limits.hpp" 2 3












namespace std {

enum float_round_style {
  round_indeterminate       = -1,
  round_toward_zero         =  0,
  round_to_nearest          =  1,
  round_toward_infinity     =  2,
  round_toward_neg_infinity =  3
};

enum float_denorm_style {
  denorm_indeterminate = -1,
  denorm_absent        =  0,
  denorm_present       =  1
};

 
 
 
 
 
 
 
 
 
 
 
 









 

template <class __number>
class _Numeric_limits_base {
public:
  static const  bool    is_specialized  =   false  ;

  static __number min() throw() { return __number(); }
  static __number max() throw() { return __number(); }

  static const  int    digits  =     0  ;
  static const  int    digits10  =   0  ;

  static const  bool    is_signed  =    false  ;
  static const  bool    is_integer  =   false  ;
  static const  bool    is_exact  =     false  ;

  static const  int    radix  =   0  ;

  static __number epsilon() throw()     { return __number(); }
  static __number round_error() throw() { return __number(); }

  static const  int    min_exponent  =     0  ;
  static const  int    min_exponent10  =   0  ;
  static const  int    max_exponent  =     0  ;
  static const  int    max_exponent10  =   0  ;

  static const  bool    has_infinity  =        false  ;
  static const  bool    has_quiet_NaN  =       false  ;
  static const  bool    has_signaling_NaN  =   false  ;
  static const  float_denorm_style   
                              has_denorm  =  
                              denorm_absent  ;
  static const  bool    has_denorm_loss  =     false  ;

  static __number infinity() throw()      { return __number(); }
  static __number quiet_NaN() throw()     { return __number(); }
  static __number signaling_NaN() throw() { return __number(); }
  static __number denorm_min() throw()    { return __number(); }

  static const  bool    is_iec559  =    false  ;
  static const  bool    is_bounded  =   false  ;
  static const  bool    is_modulo  =    false  ;

  static const  bool    traps  =              false  ;
  static const  bool    tinyness_before  =    false  ;
  static const  float_round_style   
                              round_style  =  
                              round_toward_zero  ;
};

 

template <class _Int,
          _Int __imin,
          _Int __imax,
          int __idigits = -1>
class _Integer_limits : public _Numeric_limits_base<_Int> 
{
public:
  static const  bool    is_specialized  =   true  ;

  static _Int min() throw() { return __imin; }
  static _Int max() throw() { return __imax; }

  static const  int   
                              digits  =  
                              (__idigits < 0) ? (int)(sizeof(_Int) * 8 )
                                                   - (__imin == 0 ? 0 : 1) 
                                              : __idigits  ;
  static const  int    digits10  =   (digits * 301) / 1000  ; 
                                 

  static const  bool    is_signed  =    __imin != 0  ;
  static const  bool    is_integer  =   true  ;
  static const  bool    is_exact  =     true  ;
  static const  int     radix  =        2  ;

  static const  bool    is_bounded  =   true  ;
  static const  bool    is_modulo  =   true  ;
};

# 189 "/usr/include/boost/detail/limits.hpp" 3


 template<class Number, unsigned int Word>
 struct float_helper{
  static Number get_word() throw() {
     
    const unsigned int _S_word[4] = { 0, 0, 0, Word };
    return *reinterpret_cast<const Number*>(
        reinterpret_cast<const char *>(&_S_word)+16-
                (sizeof(Number) == 12 ? 10 : sizeof(Number)));
  } 
};



 
template <class __number,
         int __Digits, int __Digits10,
         int __MinExp, int __MaxExp,
         int __MinExp10, int __MaxExp10,
         unsigned int __InfinityWord,
         unsigned int __QNaNWord, unsigned int __SNaNWord,
         bool __IsIEC559,
         float_round_style __RoundStyle>
class _Floating_limits : public _Numeric_limits_base<__number>
{
public:
  static const  bool    is_specialized  =   true  ;

  static const  int    digits  =     __Digits  ;
  static const  int    digits10  =   __Digits10  ;

  static const  bool    is_signed  =   true  ;

  static const  int    radix  =   2  ;

  static const  int    min_exponent  =     __MinExp  ;
  static const  int    max_exponent  =     __MaxExp  ;
  static const  int    min_exponent10  =   __MinExp10  ;
  static const  int    max_exponent10  =   __MaxExp10  ;

  static const  bool    has_infinity  =        true  ;
  static const  bool    has_quiet_NaN  =       true  ;
  static const  bool    has_signaling_NaN  =   true  ;
  static const  float_denorm_style   
                              has_denorm  =  
                              denorm_indeterminate  ;
  static const  bool    has_denorm_loss  =     false  ;

 
  static __number infinity() throw() {
    return float_helper<__number, __InfinityWord>::get_word();
  }
  static __number quiet_NaN() throw() {
    return float_helper<__number,__QNaNWord>::get_word();
  }
  static __number signaling_NaN() throw() {
    return float_helper<__number,__SNaNWord>::get_word();
  }

  static const  bool    is_iec559  =         __IsIEC559  ;
  static const  bool    is_bounded  =        true  ;
  static const  bool    traps  =             false     ;
  static const  bool    tinyness_before  =   false  ;

  static const  float_round_style    round_style  =   __RoundStyle  ;
};

 

 

template<class T> 
class numeric_limits : public _Numeric_limits_base<T> {};

 

template<>
class numeric_limits<bool>
  : public _Integer_limits<bool, false, true, 0>
{};

template<>
class numeric_limits<char>
  : public _Integer_limits<char, (-128) , 127 >
{};

template<>
class numeric_limits<signed char>
  : public _Integer_limits<signed char, (-128) , 127 >
{};

template<>
class numeric_limits<unsigned char>
  : public _Integer_limits<unsigned char, 0, 255 >
{};


template<>
class numeric_limits<wchar_t>
# 299 "/usr/include/boost/detail/limits.hpp" 3

 
  : public _Integer_limits<wchar_t, (-2147483647l - 1l)  , (2147483647l)  >

{};


template<>
class numeric_limits<short>
  : public _Integer_limits<short, (-32767-1) , 32767 >
{};

template<>
class numeric_limits<unsigned short>
  : public _Integer_limits<unsigned short, 0, 65535 >
{};

template<>
class numeric_limits<int>
  : public _Integer_limits<int, (- 2147483647  -1) , 2147483647  >
{};

template<>
class numeric_limits<unsigned int>
  : public _Integer_limits<unsigned int, 0, (2147483647   * 2U + 1) >
{};

template<>
class numeric_limits<long>
  : public _Integer_limits<long, (- 2147483647L  -1) , 2147483647L  >
{};

template<>
class numeric_limits<unsigned long>
  : public _Integer_limits<unsigned long, 0, (2147483647L   * 2UL + 1) >
{};



 
 
 











template<>
class numeric_limits<long long>
  : public _Integer_limits<long long, (- 0x7fffffffffffffffLL  - 1) , 0x7fffffffffffffffLL >
{};

template<>
class numeric_limits<unsigned long long>
  : public _Integer_limits<unsigned long long, 0, 0xffffffffffffffffLLU >
{};



 

template<> class numeric_limits<float>
  : public _Floating_limits<float, 
                            24 ,    
                            6 ,         
                            (-125) ,     
                            128 ,     
                            (-37) ,  
                            38 ,  





                            0x7f800000u,     
                            0x7f810000u,     
                            0x7fc10000u,     

                            true,            
                            round_to_nearest>
{
public:
  static float min() throw() { return 1.17549435e-38F ; }
  static float denorm_min() throw() { return 1.17549435e-38F ; }
  static float max() throw() { return 3.40282347e+38F ; }
  static float epsilon() throw() { return 1.19209290e-07F ; }
  static float round_error() throw() { return 0.5f; }  
};

template<> class numeric_limits<double>
  : public _Floating_limits<double, 
                            53 ,    
                            15 ,         
                            (-1021) ,     
                            1024 ,     
                            (-307) ,  
                            308 ,  





                            0x7ff00000u,     
                            0x7ff10000u,     
                            0x7ff90000u,     

                            true,            
                            round_to_nearest>
{
public:
  static double min() throw() { return 2.2250738585072014e-308 ; }
  static double denorm_min() throw() { return 2.2250738585072014e-308 ; }
  static double max() throw() { return 1.7976931348623157e+308 ; }
  static double epsilon() throw() { return 2.2204460492503131e-16 ; }
  static double round_error() throw() { return 0.5; }  
};

template<> class numeric_limits<long double>
  : public _Floating_limits<long double, 
                            64 ,   
                            18 ,        
                            (-16381) ,    
                            16384 ,    
                            (-4931) , 
                            4932 , 





                            0x7fff8000u,     
                            0x7fffc000u,     
                            0x7fff9000u,     

                            false,           
                            round_to_nearest>
{
public:
  static long double min() throw() { return (__extension__ ((union __convert_long_double) {__convert_long_double_i: {0x0, 0x80000000, 0x1, 0x0}}).__convert_long_double_d) ; }
  static long double denorm_min() throw() { return (__extension__ ((union __convert_long_double) {__convert_long_double_i: {0x0, 0x80000000, 0x1, 0x0}}).__convert_long_double_d) ; }
  static long double max() throw() { return (__extension__ ((union __convert_long_double) {__convert_long_double_i: {0xffffffff, 0xffffffff, 0x107ffe, 0x0}}).__convert_long_double_d) ; }
  static long double epsilon() throw() { return (__extension__ ((union __convert_long_double) {__convert_long_double_i: {0x0, 0x80000000, 0x3fc0, 0x0}}).__convert_long_double_d) ; }
  static long double round_error() throw() { return 4; }  
};

}  



 
 
 



# 15 "/usr/include/boost/limits.hpp" 2 3






# 23 "/usr/include/boost/timer.hpp" 2 3







namespace boost {

 

 

 
 
 

 
 
 
 

class timer
{
 public:
         timer() { _start_time = std::clock(); }  
 
 
 
  void   restart() { _start_time = std::clock(); }  
  double elapsed() const                   
    { return  double(std::clock() - _start_time) / 1000000l ; }

  double elapsed_max() const    
   
   
  {
    return (double(std::numeric_limits<std::clock_t>::max())
       - double(_start_time)) / double(1000000l ); 
  }

  double elapsed_min() const             
   { return double(1)/double(1000000l ); }

 private:
  std::clock_t _start_time;
};  

}  


# 7 "testing_vector.cpp" 2

# 1 "policy_vector.hpp" 1
 

























# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stdexcept" 1 3
 
 

 
 
 
 
 

 
 
 
 

 
 
 

 
 
 
 
 

 
 





#pragma interface "stdexcept"





extern "C++" {

namespace std {

class logic_error : public exception {
  string _what;
public:
  logic_error(const string& what_arg): _what (what_arg) { }
  virtual const char* what () const { return _what.c_str (); }
};

class domain_error : public logic_error {
public:
  domain_error (const string& what_arg): logic_error (what_arg) { }
};

class invalid_argument : public logic_error {
public:
  invalid_argument (const string& what_arg): logic_error (what_arg) { }
};

class length_error : public logic_error {
public:
  length_error (const string& what_arg): logic_error (what_arg) { }
};

class out_of_range : public logic_error {
public:
  out_of_range (const string& what_arg): logic_error (what_arg) { }
};

class runtime_error : public exception {
  string _what;
public:
  runtime_error(const string& what_arg): _what (what_arg) { }
  virtual const char* what () const { return _what.c_str (); }
protected:
  runtime_error(): exception () { }
};

class range_error : public runtime_error {
public:
  range_error (const string& what_arg): runtime_error (what_arg) { }
};

class overflow_error : public runtime_error {
public:
  overflow_error (const string& what_arg): runtime_error (what_arg) { }
};

class underflow_error : public runtime_error {
public:
  underflow_error (const string& what_arg): runtime_error (what_arg) { }
};

}  
 
}  


# 27 "policy_vector.hpp" 2


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/algorithm" 1 3
 
































# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algo.h" 1 3
 

























 






# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_heap.h" 1 3
 
























 






 





 

template <class _RandomAccessIterator, class _Distance, class _Tp>
void 
__push_heap(_RandomAccessIterator __first,
            _Distance __holeIndex, _Distance __topIndex, _Tp __value)
{
  _Distance __parent = (__holeIndex - 1) / 2;
  while (__holeIndex > __topIndex && *(__first + __parent) < __value) {
    *(__first + __holeIndex) = *(__first + __parent);
    __holeIndex = __parent;
    __parent = (__holeIndex - 1) / 2;
  }    
  *(__first + __holeIndex) = __value;
}

template <class _RandomAccessIterator, class _Distance, class _Tp>
inline void 
__push_heap_aux(_RandomAccessIterator __first,
                _RandomAccessIterator __last, _Distance*, _Tp*)
{
  __push_heap(__first, _Distance((__last - __first) - 1), _Distance(0), 
              _Tp(*(__last - 1)));
}

template <class _RandomAccessIterator>
inline void 
push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
  __push_heap_aux(__first, __last,
                  __distance_type( __first ) , __value_type( __first ) );
}

template <class _RandomAccessIterator, class _Distance, class _Tp, 
          class _Compare>
void
__push_heap(_RandomAccessIterator __first, _Distance __holeIndex,
            _Distance __topIndex, _Tp __value, _Compare __comp)
{
  _Distance __parent = (__holeIndex - 1) / 2;
  while (__holeIndex > __topIndex && __comp(*(__first + __parent), __value)) {
    *(__first + __holeIndex) = *(__first + __parent);
    __holeIndex = __parent;
    __parent = (__holeIndex - 1) / 2;
  }
  *(__first + __holeIndex) = __value;
}

template <class _RandomAccessIterator, class _Compare,
          class _Distance, class _Tp>
inline void 
__push_heap_aux(_RandomAccessIterator __first,
                _RandomAccessIterator __last, _Compare __comp,
                _Distance*, _Tp*) 
{
  __push_heap(__first, _Distance((__last - __first) - 1), _Distance(0), 
              _Tp(*(__last - 1)), __comp);
}

template <class _RandomAccessIterator, class _Compare>
inline void 
push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
          _Compare __comp)
{
  __push_heap_aux(__first, __last, __comp,
                  __distance_type( __first ) , __value_type( __first ) );
}

template <class _RandomAccessIterator, class _Distance, class _Tp>
void 
__adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex,
              _Distance __len, _Tp __value)
{
  _Distance __topIndex = __holeIndex;
  _Distance __secondChild = 2 * __holeIndex + 2;
  while (__secondChild < __len) {
    if (*(__first + __secondChild) < *(__first + (__secondChild - 1)))
      __secondChild--;
    *(__first + __holeIndex) = *(__first + __secondChild);
    __holeIndex = __secondChild;
    __secondChild = 2 * (__secondChild + 1);
  }
  if (__secondChild == __len) {
    *(__first + __holeIndex) = *(__first + (__secondChild - 1));
    __holeIndex = __secondChild - 1;
  }
  __push_heap(__first, __holeIndex, __topIndex, __value);
}

template <class _RandomAccessIterator, class _Tp, class _Distance>
inline void 
__pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
           _RandomAccessIterator __result, _Tp __value, _Distance*)
{
  *__result = *__first;
  __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __value);
}

template <class _RandomAccessIterator, class _Tp>
inline void 
__pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last,
               _Tp*)
{
  __pop_heap(__first, __last - 1, __last - 1, 
             _Tp(*(__last - 1)), __distance_type( __first ) );
}

template <class _RandomAccessIterator>
inline void pop_heap(_RandomAccessIterator __first, 
                     _RandomAccessIterator __last)
{
  __pop_heap_aux(__first, __last, __value_type( __first ) );
}

template <class _RandomAccessIterator, class _Distance,
          class _Tp, class _Compare>
void
__adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex,
              _Distance __len, _Tp __value, _Compare __comp)
{
  _Distance __topIndex = __holeIndex;
  _Distance __secondChild = 2 * __holeIndex + 2;
  while (__secondChild < __len) {
    if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
      __secondChild--;
    *(__first + __holeIndex) = *(__first + __secondChild);
    __holeIndex = __secondChild;
    __secondChild = 2 * (__secondChild + 1);
  }
  if (__secondChild == __len) {
    *(__first + __holeIndex) = *(__first + (__secondChild - 1));
    __holeIndex = __secondChild - 1;
  }
  __push_heap(__first, __holeIndex, __topIndex, __value, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare, 
          class _Distance>
inline void 
__pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
           _RandomAccessIterator __result, _Tp __value, _Compare __comp,
           _Distance*)
{
  *__result = *__first;
  __adjust_heap(__first, _Distance(0), _Distance(__last - __first), 
                __value, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void 
__pop_heap_aux(_RandomAccessIterator __first,
               _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
  __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp,
             __distance_type( __first ) );
}

template <class _RandomAccessIterator, class _Compare>
inline void 
pop_heap(_RandomAccessIterator __first,
         _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type( __first ) , __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Distance>
void 
__make_heap(_RandomAccessIterator __first,
            _RandomAccessIterator __last, _Tp*, _Distance*)
{
  if (__last - __first < 2) return;
  _Distance __len = __last - __first;
  _Distance __parent = (__len - 2)/2;
    
  while (true) {
    __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)));
    if (__parent == 0) return;
    __parent--;
  }
}

template <class _RandomAccessIterator>
inline void 
make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
  __make_heap(__first, __last,
              __value_type( __first ) , __distance_type( __first ) );
}

template <class _RandomAccessIterator, class _Compare,
          class _Tp, class _Distance>
void
__make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
            _Compare __comp, _Tp*, _Distance*)
{
  if (__last - __first < 2) return;
  _Distance __len = __last - __first;
  _Distance __parent = (__len - 2)/2;
    
  while (true) {
    __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)),
                  __comp);
    if (__parent == 0) return;
    __parent--;
  }
}

template <class _RandomAccessIterator, class _Compare>
inline void 
make_heap(_RandomAccessIterator __first, 
          _RandomAccessIterator __last, _Compare __comp)
{
  __make_heap(__first, __last, __comp,
              __value_type( __first ) , __distance_type( __first ) );
}

template <class _RandomAccessIterator>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
  while (__last - __first > 1)
    pop_heap(__first, __last--);
}

template <class _RandomAccessIterator, class _Compare>
void 
sort_heap(_RandomAccessIterator __first,
          _RandomAccessIterator __last, _Compare __comp)
{
  while (__last - __first > 1)
    pop_heap(__first, __last--, __comp);
}





 



 
 
 
# 34 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/stl_algo.h" 2 3


 





 

template <class _Tp>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c) {
  if (__a < __b)
    if (__b < __c)
      return __b;
    else if (__a < __c)
      return __c;
    else
      return __a;
  else if (__a < __c)
    return __a;
  else if (__b < __c)
    return __c;
  else
    return __b;
}

template <class _Tp, class _Compare>
inline const _Tp&
__median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp) {
  if (__comp(__a, __b))
    if (__comp(__b, __c))
      return __b;
    else if (__comp(__a, __c))
      return __c;
    else
      return __a;
  else if (__comp(__a, __c))
    return __a;
  else if (__comp(__b, __c))
    return __c;
  else
    return __b;
}

 
template <class _InputIter, class _Function>
_Function for_each(_InputIter __first, _InputIter __last, _Function __f) {
  for ( ; __first != __last; ++__first)
    __f(*__first);
  return __f;
}

 

template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last,
                       const _Tp& __val,
                       input_iterator_tag)
{
  while (__first != __last && *__first != __val)
    ++__first;
  return __first;
}

template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last,
                          _Predicate __pred,
                          input_iterator_tag)
{
  while (__first != __last && !__pred(*__first))
    ++__first;
  return __first;
}



template <class _RandomAccessIter, class _Tp>
_RandomAccessIter find(_RandomAccessIter __first, _RandomAccessIter __last,
                       const _Tp& __val,
                       random_access_iterator_tag)
{
  typename iterator_traits<_RandomAccessIter>::difference_type __trip_count
    = (__last - __first) >> 2;

  for ( ; __trip_count > 0 ; --__trip_count) {
    if (*__first == __val) return __first;
    ++__first;

    if (*__first == __val) return __first;
    ++__first;

    if (*__first == __val) return __first;
    ++__first;

    if (*__first == __val) return __first;
    ++__first;
  }

  switch(__last - __first) {
  case 3:
    if (*__first == __val) return __first;
    ++__first;
  case 2:
    if (*__first == __val) return __first;
    ++__first;
  case 1:
    if (*__first == __val) return __first;
    ++__first;
  case 0:
  default:
    return __last;
  }
}

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter find_if(_RandomAccessIter __first, _RandomAccessIter __last,
                          _Predicate __pred,
                          random_access_iterator_tag)
{
  typename iterator_traits<_RandomAccessIter>::difference_type __trip_count
    = (__last - __first) >> 2;

  for ( ; __trip_count > 0 ; --__trip_count) {
    if (__pred(*__first)) return __first;
    ++__first;

    if (__pred(*__first)) return __first;
    ++__first;

    if (__pred(*__first)) return __first;
    ++__first;

    if (__pred(*__first)) return __first;
    ++__first;
  }

  switch(__last - __first) {
  case 3:
    if (__pred(*__first)) return __first;
    ++__first;
  case 2:
    if (__pred(*__first)) return __first;
    ++__first;
  case 1:
    if (__pred(*__first)) return __first;
    ++__first;
  case 0:
  default:
    return __last;
  }
}



template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last,
                       const _Tp& __val)
{
  return find(__first, __last, __val, __iterator_category( __first ) );
}

template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last,
                          _Predicate __pred) {
  return find_if(__first, __last, __pred, __iterator_category( __first ) );
}

 

template <class _ForwardIter>
_ForwardIter adjacent_find(_ForwardIter __first, _ForwardIter __last) {
  if (__first == __last)
    return __last;
  _ForwardIter __next = __first;
  while(++__next != __last) {
    if (*__first == *__next)
      return __first;
    __first = __next;
  }
  return __last;
}

template <class _ForwardIter, class _BinaryPredicate>
_ForwardIter adjacent_find(_ForwardIter __first, _ForwardIter __last,
                           _BinaryPredicate __binary_pred) {
  if (__first == __last)
    return __last;
  _ForwardIter __next = __first;
  while(++__next != __last) {
    if (__binary_pred(*__first, *__next))
      return __first;
    __first = __next;
  }
  return __last;
}

 
 
 
 
 

template <class _InputIter, class _Tp, class _Size>
void count(_InputIter __first, _InputIter __last, const _Tp& __value,
           _Size& __n) {
  for ( ; __first != __last; ++__first)
    if (*__first == __value)
      ++__n;
}

template <class _InputIter, class _Predicate, class _Size>
void count_if(_InputIter __first, _InputIter __last, _Predicate __pred,
              _Size& __n) {
  for ( ; __first != __last; ++__first)
    if (__pred(*__first))
      ++__n;
}



template <class _InputIter, class _Tp>
typename iterator_traits<_InputIter>::difference_type
count(_InputIter __first, _InputIter __last, const _Tp& __value) {
  typename iterator_traits<_InputIter>::difference_type __n = 0;
  for ( ; __first != __last; ++__first)
    if (*__first == __value)
      ++__n;
  return __n;
}

template <class _InputIter, class _Predicate>
typename iterator_traits<_InputIter>::difference_type
count_if(_InputIter __first, _InputIter __last, _Predicate __pred) {
  typename iterator_traits<_InputIter>::difference_type __n = 0;
  for ( ; __first != __last; ++__first)
    if (__pred(*__first))
      ++__n;
  return __n;
}




 

template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter1 search(_ForwardIter1 __first1, _ForwardIter1 __last1,
                     _ForwardIter2 __first2, _ForwardIter2 __last2) 
{
   
  if (__first1 == __last1 || __first2 == __last2)
    return __first1;

   
  _ForwardIter2 __tmp(__first2);
  ++__tmp;
  if (__tmp == __last2)
    return find(__first1, __last1, *__first2);

   

  _ForwardIter2 __p1, __p;

  __p1 = __first2; ++__p1;

  _ForwardIter1 __current = __first1;

  while (__first1 != __last1) {
    __first1 = find(__first1, __last1, *__first2);
    if (__first1 == __last1)
      return __last1;

    __p = __p1;
    __current = __first1; 
    if (++__current == __last1)
      return __last1;

    while (*__current == *__p) {
      if (++__p == __last2)
        return __first1;
      if (++__current == __last1)
        return __last1;
    }

    ++__first1;
  }
  return __first1;
}

template <class _ForwardIter1, class _ForwardIter2, class _BinaryPred>
_ForwardIter1 search(_ForwardIter1 __first1, _ForwardIter1 __last1,
                     _ForwardIter2 __first2, _ForwardIter2 __last2,
                     _BinaryPred  __predicate) 
{
   
  if (__first1 == __last1 || __first2 == __last2)
    return __first1;

   
  _ForwardIter2 __tmp(__first2);
  ++__tmp;
  if (__tmp == __last2)
    return find(__first1, __last1, *__first2);

   

  _ForwardIter2 __p1, __p;

  __p1 = __first2; ++__p1;

  _ForwardIter1 __current = __first1;

  while (__first1 != __last1) {
    while (__first1 != __last1) {
      if (__predicate(*__first1, *__first2))
        break;
      ++__first1;
    }
    while (__first1 != __last1 && !__predicate(*__first1, *__first2))
      ++__first1;
    if (__first1 == __last1)
      return __last1;

    __p = __p1;
    __current = __first1; 
    if (++__current == __last1) return __last1;

    while (__predicate(*__current, *__p)) {
      if (++__p == __last2)
        return __first1;
      if (++__current == __last1)
        return __last1;
    }

    ++__first1;
  }
  return __first1;
}

 

template <class _ForwardIter, class _Integer, class _Tp>
_ForwardIter search_n(_ForwardIter __first, _ForwardIter __last,
                      _Integer __count, const _Tp& __val) {
  if (__count <= 0)
    return __first;
  else {
    __first = find(__first, __last, __val);
    while (__first != __last) {
      _Integer __n = __count - 1;
      _ForwardIter __i = __first;
      ++__i;
      while (__i != __last && __n != 0 && *__i == __val) {
        ++__i;
        --__n;
      }
      if (__n == 0)
        return __first;
      else
        __first = find(__i, __last, __val);
    }
    return __last;
  }
}

template <class _ForwardIter, class _Integer, class _Tp, class _BinaryPred>
_ForwardIter search_n(_ForwardIter __first, _ForwardIter __last,
                      _Integer __count, const _Tp& __val,
                      _BinaryPred __binary_pred) {
  if (__count <= 0)
    return __first;
  else {
    while (__first != __last) {
      if (__binary_pred(*__first, __val))
        break;
      ++__first;
    }
    while (__first != __last) {
      _Integer __n = __count - 1;
      _ForwardIter __i = __first;
      ++__i;
      while (__i != __last && __n != 0 && __binary_pred(*__i, __val)) {
        ++__i;
        --__n;
      }
      if (__n == 0)
        return __first;
      else {
        while (__i != __last) {
          if (__binary_pred(*__i, __val))
            break;
          ++__i;
        }
        __first = __i;
      }
    }
    return __last;
  }
} 

 

template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter2 swap_ranges(_ForwardIter1 __first1, _ForwardIter1 __last1,
                          _ForwardIter2 __first2) {
  for ( ; __first1 != __last1; ++__first1, ++__first2)
    iter_swap(__first1, __first2);
  return __first2;
}

 

template <class _InputIter, class _OutputIter, class _UnaryOperation>
_OutputIter transform(_InputIter __first, _InputIter __last,
                      _OutputIter __result, _UnaryOperation __oper) {
  for ( ; __first != __last; ++__first, ++__result)
    *__result = __oper(*__first);
  return __result;
}

template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _BinaryOperation>
_OutputIter transform(_InputIter1 __first1, _InputIter1 __last1,
                      _InputIter2 __first2, _OutputIter __result,
                      _BinaryOperation __binary_op) {
  for ( ; __first1 != __last1; ++__first1, ++__first2, ++__result)
    *__result = __binary_op(*__first1, *__first2);
  return __result;
}

 

template <class _ForwardIter, class _Tp>
void replace(_ForwardIter __first, _ForwardIter __last,
             const _Tp& __old_value, const _Tp& __new_value) {
  for ( ; __first != __last; ++__first)
    if (*__first == __old_value)
      *__first = __new_value;
}

template <class _ForwardIter, class _Predicate, class _Tp>
void replace_if(_ForwardIter __first, _ForwardIter __last,
                _Predicate __pred, const _Tp& __new_value) {
  for ( ; __first != __last; ++__first)
    if (__pred(*__first))
      *__first = __new_value;
}

template <class _InputIter, class _OutputIter, class _Tp>
_OutputIter replace_copy(_InputIter __first, _InputIter __last,
                         _OutputIter __result,
                         const _Tp& __old_value, const _Tp& __new_value) {
  for ( ; __first != __last; ++__first, ++__result)
    *__result = *__first == __old_value ? __new_value : *__first;
  return __result;
}

template <class Iterator, class _OutputIter, class _Predicate, class _Tp>
_OutputIter replace_copy_if(Iterator __first, Iterator __last,
                            _OutputIter __result,
                            _Predicate __pred, const _Tp& __new_value) {
  for ( ; __first != __last; ++__first, ++__result)
    *__result = __pred(*__first) ? __new_value : *__first;
  return __result;
}

 

template <class _ForwardIter, class _Generator>
void generate(_ForwardIter __first, _ForwardIter __last, _Generator __gen) {
  for ( ; __first != __last; ++__first)
    *__first = __gen();
}

template <class _OutputIter, class _Size, class _Generator>
_OutputIter generate_n(_OutputIter __first, _Size __n, _Generator __gen) {
  for ( ; __n > 0; --__n, ++__first)
    *__first = __gen();
  return __first;
}

 

template <class _InputIter, class _OutputIter, class _Tp>
_OutputIter remove_copy(_InputIter __first, _InputIter __last,
                        _OutputIter __result, const _Tp& __value) {
  for ( ; __first != __last; ++__first)
    if (*__first != __value) {
      *__result = *__first;
      ++__result;
    }
  return __result;
}

template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter remove_copy_if(_InputIter __first, _InputIter __last,
                           _OutputIter __result, _Predicate __pred) {
  for ( ; __first != __last; ++__first)
    if (!__pred(*__first)) {
      *__result = *__first;
      ++__result;
    }
  return __result;
}

template <class _ForwardIter, class _Tp>
_ForwardIter remove(_ForwardIter __first, _ForwardIter __last,
                    const _Tp& __value) {
  __first = find(__first, __last, __value);
  _ForwardIter __i = __first;
  return __first == __last ? __first 
                           : remove_copy(++__i, __last, __first, __value);
}

template <class _ForwardIter, class _Predicate>
_ForwardIter remove_if(_ForwardIter __first, _ForwardIter __last,
                       _Predicate __pred) {
  __first = find_if(__first, __last, __pred);
  _ForwardIter __i = __first;
  return __first == __last ? __first 
                           : remove_copy_if(++__i, __last, __first, __pred);
}

 

template <class _InputIter, class _OutputIter, class _Tp>
_OutputIter __unique_copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result, _Tp*) {
  _Tp __value = *__first;
  *__result = __value;
  while (++__first != __last)
    if (__value != *__first) {
      __value = *__first;
      *++__result = __value;
    }
  return ++__result;
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __unique_copy(_InputIter __first, _InputIter __last,
                                 _OutputIter __result, 
                                 output_iterator_tag) {
  return __unique_copy(__first, __last, __result, __value_type( __first ) );
}

template <class _InputIter, class _ForwardIter>
_ForwardIter __unique_copy(_InputIter __first, _InputIter __last,
                           _ForwardIter __result, forward_iterator_tag) {
  *__result = *__first;
  while (++__first != __last)
    if (*__result != *__first) *++__result = *__first;
  return ++__result;
}

template <class _InputIter, class _OutputIter>
inline _OutputIter unique_copy(_InputIter __first, _InputIter __last,
                               _OutputIter __result) {
  if (__first == __last) return __result;
  return __unique_copy(__first, __last, __result,
                       __iterator_category( __result ) );
}

template <class _InputIter, class _OutputIter, class _BinaryPredicate,
          class _Tp>
_OutputIter __unique_copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result,
                          _BinaryPredicate __binary_pred, _Tp*) {
  _Tp __value = *__first;
  *__result = __value;
  while (++__first != __last)
    if (!__binary_pred(__value, *__first)) {
      __value = *__first;
      *++__result = __value;
    }
  return ++__result;
}

template <class _InputIter, class _OutputIter, class _BinaryPredicate>
inline _OutputIter __unique_copy(_InputIter __first, _InputIter __last,
                                 _OutputIter __result,
                                 _BinaryPredicate __binary_pred,
                                 output_iterator_tag) {
  return __unique_copy(__first, __last, __result, __binary_pred,
                       __value_type( __first ) );
}

template <class _InputIter, class _ForwardIter, class _BinaryPredicate>
_ForwardIter __unique_copy(_InputIter __first, _InputIter __last,
                           _ForwardIter __result, 
                           _BinaryPredicate __binary_pred,
                           forward_iterator_tag) {
  *__result = *__first;
  while (++__first != __last)
    if (!__binary_pred(*__result, *__first)) *++__result = *__first;
  return ++__result;
}

template <class _InputIter, class _OutputIter, class _BinaryPredicate>
inline _OutputIter unique_copy(_InputIter __first, _InputIter __last,
                               _OutputIter __result,
                               _BinaryPredicate __binary_pred) {
  if (__first == __last) return __result;
  return __unique_copy(__first, __last, __result, __binary_pred,
                       __iterator_category( __result ) );
}

template <class _ForwardIter>
_ForwardIter unique(_ForwardIter __first, _ForwardIter __last) {
  __first = adjacent_find(__first, __last);
  return unique_copy(__first, __last, __first);
}

template <class _ForwardIter, class _BinaryPredicate>
_ForwardIter unique(_ForwardIter __first, _ForwardIter __last,
                    _BinaryPredicate __binary_pred) {
  __first = adjacent_find(__first, __last, __binary_pred);
  return unique_copy(__first, __last, __first, __binary_pred);
}

 

template <class _BidirectionalIter>
void __reverse(_BidirectionalIter __first, _BidirectionalIter __last, 
               bidirectional_iterator_tag) {
  while (true)
    if (__first == __last || __first == --__last)
      return;
    else
      iter_swap(__first++, __last);
}

template <class _RandomAccessIter>
void __reverse(_RandomAccessIter __first, _RandomAccessIter __last,
               random_access_iterator_tag) {
  while (__first < __last)
    iter_swap(__first++, --__last);
}

template <class _BidirectionalIter>
inline void reverse(_BidirectionalIter __first, _BidirectionalIter __last) {
  __reverse(__first, __last, __iterator_category( __first ) );
}

template <class _BidirectionalIter, class _OutputIter>
_OutputIter reverse_copy(_BidirectionalIter __first,
                            _BidirectionalIter __last,
                            _OutputIter __result) {
  while (__first != __last) {
    --__last;
    *__result = *__last;
    ++__result;
  }
  return __result;
}

 

template <class _EuclideanRingElement>
_EuclideanRingElement __gcd(_EuclideanRingElement __m,
                            _EuclideanRingElement __n)
{
  while (__n != 0) {
    _EuclideanRingElement __t = __m % __n;
    __m = __n;
    __n = __t;
  }
  return __m;
}

template <class _ForwardIter, class _Distance>
_ForwardIter __rotate(_ForwardIter __first,
                      _ForwardIter __middle,
                      _ForwardIter __last,
                      _Distance*,
                      forward_iterator_tag) {
  if (__first == __middle)
    return __last;
  if (__last  == __middle)
    return __first;

  _ForwardIter __first2 = __middle;
  do {
    swap(*__first++, *__first2++);
    if (__first == __middle)
      __middle = __first2;
  } while (__first2 != __last);

  _ForwardIter __new_middle = __first;

  __first2 = __middle;

  while (__first2 != __last) {
    swap (*__first++, *__first2++);
    if (__first == __middle)
      __middle = __first2;
    else if (__first2 == __last)
      __first2 = __middle;
  }

  return __new_middle;
}


template <class _BidirectionalIter, class _Distance>
_BidirectionalIter __rotate(_BidirectionalIter __first,
                            _BidirectionalIter __middle,
                            _BidirectionalIter __last,
                            _Distance*,
                            bidirectional_iterator_tag) {
  if (__first == __middle)
    return __last;
  if (__last  == __middle)
    return __first;

  __reverse(__first,  __middle, bidirectional_iterator_tag());
  __reverse(__middle, __last,   bidirectional_iterator_tag());

  while (__first != __middle && __middle != __last)
    swap (*__first++, *--__last);

  if (__first == __middle) {
    __reverse(__middle, __last,   bidirectional_iterator_tag());
    return __last;
  }
  else {
    __reverse(__first,  __middle, bidirectional_iterator_tag());
    return __first;
  }
}

template <class _RandomAccessIter, class _Distance, class _Tp>
_RandomAccessIter __rotate(_RandomAccessIter __first,
                           _RandomAccessIter __middle,
                           _RandomAccessIter __last,
                           _Distance *, _Tp *) {

  _Distance __n = __last   - __first;
  _Distance __k = __middle - __first;
  _Distance __l = __n - __k;
  _RandomAccessIter __result = __first + (__last - __middle);

  if (__k == __l) {
    swap_ranges(__first, __middle, __middle);
    return __result;
  }

  _Distance __d = __gcd(__n, __k);

  for (_Distance __i = 0; __i < __d; __i++) {
    _Tp __tmp = *__first;
    _RandomAccessIter __p = __first;

    if (__k < __l) {
      for (_Distance __j = 0; __j < __l/__d; __j++) {
        if (__p > __first + __l) {
          *__p = *(__p - __l);
          __p -= __l;
        }

        *__p = *(__p + __k);
        __p += __k;
      }
    }

    else {
      for (_Distance __j = 0; __j < __k/__d - 1; __j ++) {
        if (__p < __last - __k) {
          *__p = *(__p + __k);
          __p += __k;
        }

        *__p = * (__p - __l);
        __p -= __l;
      }
    }

    *__p = __tmp;
    ++__first;
  }

  return __result;
}

template <class _ForwardIter>
inline _ForwardIter rotate(_ForwardIter __first, _ForwardIter __middle,
                           _ForwardIter __last) {
  return __rotate(__first, __middle, __last,
                  __distance_type( __first ) ,
                  __iterator_category( __first ) );
}

template <class _ForwardIter, class _OutputIter>
_OutputIter rotate_copy(_ForwardIter __first, _ForwardIter __middle,
                            _ForwardIter __last, _OutputIter __result) {
  return copy(__first, __middle, copy(__middle, __last, __result));
}

 
 
 

template <class _Distance>
inline _Distance __random_number(_Distance __n) {



  return lrand48() % __n;

}

 

template <class _RandomAccessIter>
inline void random_shuffle(_RandomAccessIter __first,
                           _RandomAccessIter __last) {
  if (__first == __last) return;
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    iter_swap(__i, __first + __random_number((__i - __first) + 1));
}

template <class _RandomAccessIter, class _RandomNumberGenerator>
void random_shuffle(_RandomAccessIter __first, _RandomAccessIter __last,
                    _RandomNumberGenerator& __rand) {
  if (__first == __last) return;
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    iter_swap(__i, __first + __rand((__i - __first) + 1));
}

 

template <class _ForwardIter, class _OutputIter, class _Distance>
_OutputIter random_sample_n(_ForwardIter __first, _ForwardIter __last,
                            _OutputIter __out, const _Distance __n)
{
  _Distance __remaining = 0;
  distance(__first, __last, __remaining);
  _Distance __m = min(__n, __remaining);

  while (__m > 0) {
    if (__random_number(__remaining) < __m) {
      *__out = *__first;
      ++__out;
      --__m;
    }

    --__remaining;
    ++__first;
  }
  return __out;
}

template <class _ForwardIter, class _OutputIter, class _Distance,
          class _RandomNumberGenerator>
_OutputIter random_sample_n(_ForwardIter __first, _ForwardIter __last,
                            _OutputIter __out, const _Distance __n,
                            _RandomNumberGenerator& __rand)
{
  _Distance __remaining = 0;
  distance(__first, __last, __remaining);
  _Distance __m = min(__n, __remaining);

  while (__m > 0) {
    if (__rand(__remaining) < __m) {
      *__out = *__first;
      ++__out;
      --__m;
    }

    --__remaining;
    ++__first;
  }
  return __out;
}

template <class _InputIter, class _RandomAccessIter, class _Distance>
_RandomAccessIter __random_sample(_InputIter __first, _InputIter __last,
                                  _RandomAccessIter __out,
                                  const _Distance __n)
{
  _Distance __m = 0;
  _Distance __t = __n;
  for ( ; __first != __last && __m < __n; ++__m, ++__first) 
    __out[__m] = *__first;

  while (__first != __last) {
    ++__t;
    _Distance __M = __random_number(__t);
    if (__M < __n)
      __out[__M] = *__first;
    ++__first;
  }

  return __out + __m;
}

template <class _InputIter, class _RandomAccessIter,
          class _RandomNumberGenerator, class _Distance>
_RandomAccessIter __random_sample(_InputIter __first, _InputIter __last,
                                  _RandomAccessIter __out,
                                  _RandomNumberGenerator& __rand,
                                  const _Distance __n)
{
  _Distance __m = 0;
  _Distance __t = __n;
  for ( ; __first != __last && __m < __n; ++__m, ++__first)
    __out[__m] = *__first;

  while (__first != __last) {
    ++__t;
    _Distance __M = __rand(__t);
    if (__M < __n)
      __out[__M] = *__first;
    ++__first;
  }

  return __out + __m;
}

template <class _InputIter, class _RandomAccessIter>
inline _RandomAccessIter
random_sample(_InputIter __first, _InputIter __last,
              _RandomAccessIter __out_first, _RandomAccessIter __out_last) 
{
  return __random_sample(__first, __last,
                         __out_first, __out_last - __out_first);
}


template <class _InputIter, class _RandomAccessIter, 
          class _RandomNumberGenerator>
inline _RandomAccessIter
random_sample(_InputIter __first, _InputIter __last,
              _RandomAccessIter __out_first, _RandomAccessIter __out_last,
              _RandomNumberGenerator& __rand) 
{
  return __random_sample(__first, __last,
                         __out_first, __rand,
                         __out_last - __out_first);
}

 

template <class _ForwardIter, class _Predicate>
_ForwardIter __partition(_ForwardIter __first,
		         _ForwardIter __last,
			 _Predicate   __pred,
			 forward_iterator_tag) {
  if (__first == __last) return __first;

  while (__pred(*__first))
    if (++__first == __last) return __first;

  _ForwardIter __next = __first;

  while (++__next != __last)
    if (__pred(*__next)) {
      swap(*__first, *__next);
      ++__first;
    }

  return __first;
}

template <class _BidirectionalIter, class _Predicate>
_BidirectionalIter __partition(_BidirectionalIter __first,
                               _BidirectionalIter __last,
			       _Predicate __pred,
			       bidirectional_iterator_tag) {
  while (true) {
    while (true)
      if (__first == __last)
        return __first;
      else if (__pred(*__first))
        ++__first;
      else
        break;
    --__last;
    while (true)
      if (__first == __last)
        return __first;
      else if (!__pred(*__last))
        --__last;
      else
        break;
    iter_swap(__first, __last);
    ++__first;
  }
}

template <class _ForwardIter, class _Predicate>
inline _ForwardIter partition(_ForwardIter __first,
   			      _ForwardIter __last,
			      _Predicate   __pred) {
  return __partition(__first, __last, __pred, __iterator_category( __first ) );
}


template <class _ForwardIter, class _Predicate, class _Distance>
_ForwardIter __inplace_stable_partition(_ForwardIter __first,
                                        _ForwardIter __last,
                                        _Predicate __pred, _Distance __len) {
  if (__len == 1)
    return __pred(*__first) ? __last : __first;
  _ForwardIter __middle = __first;
  advance(__middle, __len / 2);
  return rotate(__inplace_stable_partition(__first, __middle, __pred, 
                                           __len / 2),
                __middle,
                __inplace_stable_partition(__middle, __last, __pred,
                                           __len - __len / 2));
}

template <class _ForwardIter, class _Pointer, class _Predicate, 
          class _Distance>
_ForwardIter __stable_partition_adaptive(_ForwardIter __first,
                                         _ForwardIter __last,
                                         _Predicate __pred, _Distance __len,
                                         _Pointer __buffer,
                                         _Distance __buffer_size) 
{
  if (__len <= __buffer_size) {
    _ForwardIter __result1 = __first;
    _Pointer __result2 = __buffer;
    for ( ; __first != __last ; ++__first)
      if (__pred(*__first)) {
        *__result1 = *__first;
        ++__result1;
      }
      else {
        *__result2 = *__first;
        ++__result2;
      }
    copy(__buffer, __result2, __result1);
    return __result1;
  }
  else {
    _ForwardIter __middle = __first;
    advance(__middle, __len / 2);
    return rotate(__stable_partition_adaptive(
                          __first, __middle, __pred,
                          __len / 2, __buffer, __buffer_size),
                    __middle,
                    __stable_partition_adaptive(
                          __middle, __last, __pred,
                          __len - __len / 2, __buffer, __buffer_size));
  }
}

template <class _ForwardIter, class _Predicate, class _Tp, class _Distance>
inline _ForwardIter
__stable_partition_aux(_ForwardIter __first, _ForwardIter __last, 
                       _Predicate __pred, _Tp*, _Distance*)
{
  _Temporary_buffer<_ForwardIter, _Tp> __buf(__first, __last);
  if (__buf.size() > 0)
    return __stable_partition_adaptive(__first, __last, __pred,
                                       _Distance(__buf.requested_size()),
                                       __buf.begin(), __buf.size());
  else
    return __inplace_stable_partition(__first, __last, __pred, 
                                      _Distance(__buf.requested_size()));
}

template <class _ForwardIter, class _Predicate>
inline _ForwardIter stable_partition(_ForwardIter __first,
                                     _ForwardIter __last, 
                                     _Predicate __pred) {
  if (__first == __last)
    return __first;
  else
    return __stable_partition_aux(__first, __last, __pred,
                                  __value_type( __first ) ,
                                  __distance_type( __first ) );
}

template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, 
                                        _RandomAccessIter __last, 
                                        _Tp __pivot) 
{
  while (true) {
    while (*__first < __pivot)
      ++__first;
    --__last;
    while (__pivot < *__last)
      --__last;
    if (!(__first < __last))
      return __first;
    iter_swap(__first, __last);
    ++__first;
  }
}    

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, 
                                        _RandomAccessIter __last, 
                                        _Tp __pivot, _Compare __comp) 
{
  while (true) {
    while (__comp(*__first, __pivot))
      ++__first;
    --__last;
    while (__comp(__pivot, *__last))
      --__last;
    if (!(__first < __last))
      return __first;
    iter_swap(__first, __last);
    ++__first;
  }
}

const int __stl_threshold = 16;

 

template <class _RandomAccessIter, class _Tp>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val) {
  _RandomAccessIter __next = __last;
  --__next;
  while (__val < *__next) {
    *__last = *__next;
    __last = __next;
    --__next;
  }
  *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, 
                               _Compare __comp) {
  _RandomAccessIter __next = __last;
  --__next;  
  while (__comp(__val, *__next)) {
    *__last = *__next;
    __last = __next;
    --__next;
  }
  *__last = __val;
}

template <class _RandomAccessIter, class _Tp>
inline void __linear_insert(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Tp*) {
  _Tp __val = *__last;
  if (__val < *__first) {
    copy_backward(__first, __last, __last + 1);
    *__first = __val;
  }
  else
    __unguarded_linear_insert(__last, __val);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __linear_insert(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Tp*, _Compare __comp) {
  _Tp __val = *__last;
  if (__comp(__val, *__first)) {
    copy_backward(__first, __last, __last + 1);
    *__first = __val;
  }
  else
    __unguarded_linear_insert(__last, __val, __comp);
}

template <class _RandomAccessIter>
void __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last) {
  if (__first == __last) return; 
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    __linear_insert(__first, __i, __value_type( __first ) );
}

template <class _RandomAccessIter, class _Compare>
void __insertion_sort(_RandomAccessIter __first,
                      _RandomAccessIter __last, _Compare __comp) {
  if (__first == __last) return;
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    __linear_insert(__first, __i, __value_type( __first ) , __comp);
}

template <class _RandomAccessIter, class _Tp>
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, 
                                    _RandomAccessIter __last, _Tp*) {
  for (_RandomAccessIter __i = __first; __i != __last; ++__i)
    __unguarded_linear_insert(__i, _Tp(*__i));
}

template <class _RandomAccessIter>
inline void __unguarded_insertion_sort(_RandomAccessIter __first, 
                                _RandomAccessIter __last) {
  __unguarded_insertion_sort_aux(__first, __last, __value_type( __first ) );
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, 
                                    _RandomAccessIter __last,
                                    _Tp*, _Compare __comp) {
  for (_RandomAccessIter __i = __first; __i != __last; ++__i)
    __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template <class _RandomAccessIter, class _Compare>
inline void __unguarded_insertion_sort(_RandomAccessIter __first, 
                                       _RandomAccessIter __last,
                                       _Compare __comp) {
  __unguarded_insertion_sort_aux(__first, __last, __value_type( __first ) ,
                                 __comp);
}

template <class _RandomAccessIter>
void __final_insertion_sort(_RandomAccessIter __first, 
                            _RandomAccessIter __last) {
  if (__last - __first > __stl_threshold) {
    __insertion_sort(__first, __first + __stl_threshold);
    __unguarded_insertion_sort(__first + __stl_threshold, __last);
  }
  else
    __insertion_sort(__first, __last);
}

template <class _RandomAccessIter, class _Compare>
void __final_insertion_sort(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Compare __comp) {
  if (__last - __first > __stl_threshold) {
    __insertion_sort(__first, __first + __stl_threshold, __comp);
    __unguarded_insertion_sort(__first + __stl_threshold, __last, __comp);
  }
  else
    __insertion_sort(__first, __last, __comp);
}

template <class _Size>
inline _Size __lg(_Size __n) {
  _Size __k;
  for (__k = 0; __n != 1; __n >>= 1) ++__k;
  return __k;
}

template <class _RandomAccessIter, class _Tp, class _Size>
void __introsort_loop(_RandomAccessIter __first,
                      _RandomAccessIter __last, _Tp*,
                      _Size __depth_limit)
{
  while (__last - __first > __stl_threshold) {
    if (__depth_limit == 0) {
      partial_sort(__first, __last, __last);
      return;
    }
    --__depth_limit;
    _RandomAccessIter __cut =
      __unguarded_partition(__first, __last,
                            _Tp(__median(*__first,
                                         *(__first + (__last - __first)/2),
                                         *(__last - 1))));
    __introsort_loop(__cut, __last, (_Tp*) 0, __depth_limit);
    __last = __cut;
  }
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first,
                      _RandomAccessIter __last, _Tp*,
                      _Size __depth_limit, _Compare __comp)
{
  while (__last - __first > __stl_threshold) {
    if (__depth_limit == 0) {
      partial_sort(__first, __last, __last, __comp);
      return;
    }
    --__depth_limit;
    _RandomAccessIter __cut =
      __unguarded_partition(__first, __last,
                            _Tp(__median(*__first,
                                         *(__first + (__last - __first)/2),
                                         *(__last - 1), __comp)),
       __comp);
    __introsort_loop(__cut, __last, (_Tp*) 0, __depth_limit, __comp);
    __last = __cut;
  }
}

template <class _RandomAccessIter>
inline void sort(_RandomAccessIter __first, _RandomAccessIter __last) {
  if (__first != __last) {
    __introsort_loop(__first, __last,
                     __value_type( __first ) ,
                     __lg(__last - __first) * 2);
    __final_insertion_sort(__first, __last);
  }
}

template <class _RandomAccessIter, class _Compare>
inline void sort(_RandomAccessIter __first, _RandomAccessIter __last,
                 _Compare __comp) {
  if (__first != __last) {
    __introsort_loop(__first, __last,
                     __value_type( __first ) ,
                     __lg(__last - __first) * 2,
                     __comp);
    __final_insertion_sort(__first, __last, __comp);
  }
}

 

template <class _RandomAccessIter>
void __inplace_stable_sort(_RandomAccessIter __first,
                           _RandomAccessIter __last) {
  if (__last - __first < 15) {
    __insertion_sort(__first, __last);
    return;
  }
  _RandomAccessIter __middle = __first + (__last - __first) / 2;
  __inplace_stable_sort(__first, __middle);
  __inplace_stable_sort(__middle, __last);
  __merge_without_buffer(__first, __middle, __last,
                         __middle - __first,
                         __last - __middle);
}

template <class _RandomAccessIter, class _Compare>
void __inplace_stable_sort(_RandomAccessIter __first,
                           _RandomAccessIter __last, _Compare __comp) {
  if (__last - __first < 15) {
    __insertion_sort(__first, __last, __comp);
    return;
  }
  _RandomAccessIter __middle = __first + (__last - __first) / 2;
  __inplace_stable_sort(__first, __middle, __comp);
  __inplace_stable_sort(__middle, __last, __comp);
  __merge_without_buffer(__first, __middle, __last,
                         __middle - __first,
                         __last - __middle,
                         __comp);
}

template <class _RandomAccessIter1, class _RandomAccessIter2,
          class _Distance>
void __merge_sort_loop(_RandomAccessIter1 __first,
                       _RandomAccessIter1 __last, 
                       _RandomAccessIter2 __result, _Distance __step_size) {
  _Distance __two_step = 2 * __step_size;

  while (__last - __first >= __two_step) {
    __result = merge(__first, __first + __step_size,
                     __first + __step_size, __first + __two_step,
                     __result);
    __first += __two_step;
  }

  __step_size = min(_Distance(__last - __first), __step_size);
  merge(__first, __first + __step_size, __first + __step_size, __last,
        __result);
}

template <class _RandomAccessIter1, class _RandomAccessIter2,
          class _Distance, class _Compare>
void __merge_sort_loop(_RandomAccessIter1 __first,
                       _RandomAccessIter1 __last, 
                       _RandomAccessIter2 __result, _Distance __step_size,
                       _Compare __comp) {
  _Distance __two_step = 2 * __step_size;

  while (__last - __first >= __two_step) {
    __result = merge(__first, __first + __step_size,
                     __first + __step_size, __first + __two_step,
                     __result,
                     __comp);
    __first += __two_step;
  }
  __step_size = min(_Distance(__last - __first), __step_size);

  merge(__first, __first + __step_size,
        __first + __step_size, __last,
        __result,
        __comp);
}

const int __stl_chunk_size = 7;
        
template <class _RandomAccessIter, class _Distance>
void __chunk_insertion_sort(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Distance __chunk_size)
{
  while (__last - __first >= __chunk_size) {
    __insertion_sort(__first, __first + __chunk_size);
    __first += __chunk_size;
  }
  __insertion_sort(__first, __last);
}

template <class _RandomAccessIter, class _Distance, class _Compare>
void __chunk_insertion_sort(_RandomAccessIter __first, 
                            _RandomAccessIter __last,
                            _Distance __chunk_size, _Compare __comp)
{
  while (__last - __first >= __chunk_size) {
    __insertion_sort(__first, __first + __chunk_size, __comp);
    __first += __chunk_size;
  }
  __insertion_sort(__first, __last, __comp);
}

template <class _RandomAccessIter, class _Pointer, class _Distance>
void __merge_sort_with_buffer(_RandomAccessIter __first, 
                              _RandomAccessIter __last,
                              _Pointer __buffer, _Distance*) {
  _Distance __len = __last - __first;
  _Pointer __buffer_last = __buffer + __len;

  _Distance __step_size = __stl_chunk_size;
  __chunk_insertion_sort(__first, __last, __step_size);

  while (__step_size < __len) {
    __merge_sort_loop(__first, __last, __buffer, __step_size);
    __step_size *= 2;
    __merge_sort_loop(__buffer, __buffer_last, __first, __step_size);
    __step_size *= 2;
  }
}

template <class _RandomAccessIter, class _Pointer, class _Distance,
          class _Compare>
void __merge_sort_with_buffer(_RandomAccessIter __first, 
                              _RandomAccessIter __last, _Pointer __buffer,
                              _Distance*, _Compare __comp) {
  _Distance __len = __last - __first;
  _Pointer __buffer_last = __buffer + __len;

  _Distance __step_size = __stl_chunk_size;
  __chunk_insertion_sort(__first, __last, __step_size, __comp);

  while (__step_size < __len) {
    __merge_sort_loop(__first, __last, __buffer, __step_size, __comp);
    __step_size *= 2;
    __merge_sort_loop(__buffer, __buffer_last, __first, __step_size, __comp);
    __step_size *= 2;
  }
}

template <class _RandomAccessIter, class _Pointer, class _Distance>
void __stable_sort_adaptive(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Pointer __buffer,
                            _Distance __buffer_size) {
  _Distance __len = (__last - __first + 1) / 2;
  _RandomAccessIter __middle = __first + __len;
  if (__len > __buffer_size) {
    __stable_sort_adaptive(__first, __middle, __buffer, __buffer_size);
    __stable_sort_adaptive(__middle, __last, __buffer, __buffer_size);
  }
  else {
    __merge_sort_with_buffer(__first, __middle, __buffer, (_Distance*)0);
    __merge_sort_with_buffer(__middle, __last, __buffer, (_Distance*)0);
  }
  __merge_adaptive(__first, __middle, __last, _Distance(__middle - __first), 
                   _Distance(__last - __middle), __buffer, __buffer_size);
}

template <class _RandomAccessIter, class _Pointer, class _Distance, 
          class _Compare>
void __stable_sort_adaptive(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Pointer __buffer,
                            _Distance __buffer_size, _Compare __comp) {
  _Distance __len = (__last - __first + 1) / 2;
  _RandomAccessIter __middle = __first + __len;
  if (__len > __buffer_size) {
    __stable_sort_adaptive(__first, __middle, __buffer, __buffer_size, 
                           __comp);
    __stable_sort_adaptive(__middle, __last, __buffer, __buffer_size, 
                           __comp);
  }
  else {
    __merge_sort_with_buffer(__first, __middle, __buffer, (_Distance*)0,
                               __comp);
    __merge_sort_with_buffer(__middle, __last, __buffer, (_Distance*)0,
                               __comp);
  }
  __merge_adaptive(__first, __middle, __last, _Distance(__middle - __first), 
                   _Distance(__last - __middle), __buffer, __buffer_size,
                   __comp);
}

template <class _RandomAccessIter, class _Tp, class _Distance>
inline void __stable_sort_aux(_RandomAccessIter __first,
                              _RandomAccessIter __last, _Tp*, _Distance*) {
  _Temporary_buffer<_RandomAccessIter, _Tp> buf(__first, __last);
  if (buf.begin() == 0)
    __inplace_stable_sort(__first, __last);
  else 
    __stable_sort_adaptive(__first, __last, buf.begin(),
                           _Distance(buf.size()));
}

template <class _RandomAccessIter, class _Tp, class _Distance, class _Compare>
inline void __stable_sort_aux(_RandomAccessIter __first,
                              _RandomAccessIter __last, _Tp*, _Distance*,
                              _Compare __comp) {
  _Temporary_buffer<_RandomAccessIter, _Tp> buf(__first, __last);
  if (buf.begin() == 0)
    __inplace_stable_sort(__first, __last, __comp);
  else 
    __stable_sort_adaptive(__first, __last, buf.begin(),
                           _Distance(buf.size()),
                           __comp);
}

template <class _RandomAccessIter>
inline void stable_sort(_RandomAccessIter __first,
                        _RandomAccessIter __last) {
  __stable_sort_aux(__first, __last,
                    __value_type( __first ) ,
                    __distance_type( __first ) );
}

template <class _RandomAccessIter, class _Compare>
inline void stable_sort(_RandomAccessIter __first,
                        _RandomAccessIter __last, _Compare __comp) {
  __stable_sort_aux(__first, __last,
                    __value_type( __first ) ,
                    __distance_type( __first ) , 
                    __comp);
}

 

template <class _RandomAccessIter, class _Tp>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle,
                    _RandomAccessIter __last, _Tp*) {
  make_heap(__first, __middle);
  for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
    if (*__i < *__first) 
      __pop_heap(__first, __middle, __i, _Tp(*__i),
                 __distance_type( __first ) );
  sort_heap(__first, __middle);
}

template <class _RandomAccessIter>
inline void partial_sort(_RandomAccessIter __first,
                         _RandomAccessIter __middle,
                         _RandomAccessIter __last) {
  __partial_sort(__first, __middle, __last, __value_type( __first ) );
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle,
                    _RandomAccessIter __last, _Tp*, _Compare __comp) {
  make_heap(__first, __middle, __comp);
  for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
    if (__comp(*__i, *__first))
      __pop_heap(__first, __middle, __i, _Tp(*__i), __comp,
                 __distance_type( __first ) );
  sort_heap(__first, __middle, __comp);
}

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first,
                         _RandomAccessIter __middle,
                         _RandomAccessIter __last, _Compare __comp) {
  __partial_sort(__first, __middle, __last, __value_type( __first ) , __comp);
}

template <class _InputIter, class _RandomAccessIter, class _Distance,
          class _Tp>
_RandomAccessIter __partial_sort_copy(_InputIter __first,
                                         _InputIter __last,
                                         _RandomAccessIter __result_first,
                                         _RandomAccessIter __result_last, 
                                         _Distance*, _Tp*) {
  if (__result_first == __result_last) return __result_last;
  _RandomAccessIter __result_real_last = __result_first;
  while(__first != __last && __result_real_last != __result_last) {
    *__result_real_last = *__first;
    ++__result_real_last;
    ++__first;
  }
  make_heap(__result_first, __result_real_last);
  while (__first != __last) {
    if (*__first < *__result_first) 
      __adjust_heap(__result_first, _Distance(0),
                    _Distance(__result_real_last - __result_first),
                    _Tp(*__first));
    ++__first;
  }
  sort_heap(__result_first, __result_real_last);
  return __result_real_last;
}

template <class _InputIter, class _RandomAccessIter>
inline _RandomAccessIter
partial_sort_copy(_InputIter __first, _InputIter __last,
                  _RandomAccessIter __result_first,
                  _RandomAccessIter __result_last) {
  return __partial_sort_copy(__first, __last, __result_first, __result_last, 
                             __distance_type( __result_first ) ,
                             __value_type( __first ) );
}

template <class _InputIter, class _RandomAccessIter, class _Compare,
          class _Distance, class _Tp>
_RandomAccessIter __partial_sort_copy(_InputIter __first,
                                         _InputIter __last,
                                         _RandomAccessIter __result_first,
                                         _RandomAccessIter __result_last,
                                         _Compare __comp, _Distance*, _Tp*) {
  if (__result_first == __result_last) return __result_last;
  _RandomAccessIter __result_real_last = __result_first;
  while(__first != __last && __result_real_last != __result_last) {
    *__result_real_last = *__first;
    ++__result_real_last;
    ++__first;
  }
  make_heap(__result_first, __result_real_last, __comp);
  while (__first != __last) {
    if (__comp(*__first, *__result_first))
      __adjust_heap(__result_first, _Distance(0),
                    _Distance(__result_real_last - __result_first),
                    _Tp(*__first),
                    __comp);
    ++__first;
  }
  sort_heap(__result_first, __result_real_last, __comp);
  return __result_real_last;
}

template <class _InputIter, class _RandomAccessIter, class _Compare>
inline _RandomAccessIter
partial_sort_copy(_InputIter __first, _InputIter __last,
                  _RandomAccessIter __result_first,
                  _RandomAccessIter __result_last, _Compare __comp) {
  return __partial_sort_copy(__first, __last, __result_first, __result_last,
                             __comp,
                             __distance_type( __result_first ) ,
                             __value_type( __first ) );
}

 

template <class _RandomAccessIter, class _Tp>
void __nth_element(_RandomAccessIter __first, _RandomAccessIter __nth,
                   _RandomAccessIter __last, _Tp*) {
  while (__last - __first > 3) {
    _RandomAccessIter __cut =
      __unguarded_partition(__first, __last,
                            _Tp(__median(*__first,
                                         *(__first + (__last - __first)/2),
                                         *(__last - 1))));
    if (__cut <= __nth)
      __first = __cut;
    else 
      __last = __cut;
  }
  __insertion_sort(__first, __last);
}

template <class _RandomAccessIter>
inline void nth_element(_RandomAccessIter __first, _RandomAccessIter __nth,
                        _RandomAccessIter __last) {
  __nth_element(__first, __nth, __last, __value_type( __first ) );
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __nth_element(_RandomAccessIter __first, _RandomAccessIter __nth,
                   _RandomAccessIter __last, _Tp*, _Compare __comp) {
  while (__last - __first > 3) {
    _RandomAccessIter __cut =
      __unguarded_partition(__first, __last,
                            _Tp(__median(*__first,
                                         *(__first + (__last - __first)/2), 
                                         *(__last - 1),
                                         __comp)),
                            __comp);
    if (__cut <= __nth)
      __first = __cut;
    else 
      __last = __cut;
  }
  __insertion_sort(__first, __last, __comp);
}

template <class _RandomAccessIter, class _Compare>
inline void nth_element(_RandomAccessIter __first, _RandomAccessIter __nth,
                 _RandomAccessIter __last, _Compare __comp) {
  __nth_element(__first, __nth, __last, __value_type( __first ) , __comp);
}


 

template <class _ForwardIter, class _Tp, class _Distance>
_ForwardIter __lower_bound(_ForwardIter __first, _ForwardIter __last,
                           const _Tp& __val, _Distance*) 
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle;

  while (__len > 0) {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    if (*__middle < __val) {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
    else
      __len = __half;
  }
  return __first;
}

template <class _ForwardIter, class _Tp>
inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last,
                                   const _Tp& __val) {
  return __lower_bound(__first, __last, __val,
                       __distance_type( __first ) );
}

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __lower_bound(_ForwardIter __first, _ForwardIter __last,
                              const _Tp& __val, _Compare __comp, _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle;

  while (__len > 0) {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    if (__comp(*__middle, __val)) {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
    else
      __len = __half;
  }
  return __first;
}

template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last,
                                const _Tp& __val, _Compare __comp) {
  return __lower_bound(__first, __last, __val, __comp,
                       __distance_type( __first ) );
}

template <class _ForwardIter, class _Tp, class _Distance>
_ForwardIter __upper_bound(_ForwardIter __first, _ForwardIter __last,
                           const _Tp& __val, _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle;

  while (__len > 0) {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    if (__val < *__middle)
      __len = __half;
    else {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
  }
  return __first;
}

template <class _ForwardIter, class _Tp>
inline _ForwardIter upper_bound(_ForwardIter __first, _ForwardIter __last,
                                const _Tp& __val) {
  return __upper_bound(__first, __last, __val,
                       __distance_type( __first ) );
}

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __upper_bound(_ForwardIter __first, _ForwardIter __last,
                           const _Tp& __val, _Compare __comp, _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle;

  while (__len > 0) {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    if (__comp(__val, *__middle))
      __len = __half;
    else {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
  }
  return __first;
}

template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter upper_bound(_ForwardIter __first, _ForwardIter __last,
                                const _Tp& __val, _Compare __comp) {
  return __upper_bound(__first, __last, __val, __comp,
                       __distance_type( __first ) );
}

template <class _ForwardIter, class _Tp, class _Distance>
pair<_ForwardIter, _ForwardIter>
__equal_range(_ForwardIter __first, _ForwardIter __last, const _Tp& __val,
              _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle, __left, __right;

  while (__len > 0) {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    if (*__middle < __val) {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
    else if (__val < *__middle)
      __len = __half;
    else {
      __left = lower_bound(__first, __middle, __val);
      advance(__first, __len);
      __right = upper_bound(++__middle, __first, __val);
      return pair<_ForwardIter, _ForwardIter>(__left, __right);
    }
  }
  return pair<_ForwardIter, _ForwardIter>(__first, __first);
}

template <class _ForwardIter, class _Tp>
inline pair<_ForwardIter, _ForwardIter>
equal_range(_ForwardIter __first, _ForwardIter __last, const _Tp& __val) {
  return __equal_range(__first, __last, __val,
                       __distance_type( __first ) );
}

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
pair<_ForwardIter, _ForwardIter>
__equal_range(_ForwardIter __first, _ForwardIter __last, const _Tp& __val,
              _Compare __comp, _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle, __left, __right;

  while (__len > 0) {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    if (__comp(*__middle, __val)) {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
    else if (__comp(__val, *__middle))
      __len = __half;
    else {
      __left = lower_bound(__first, __middle, __val, __comp);
      advance(__first, __len);
      __right = upper_bound(++__middle, __first, __val, __comp);
      return pair<_ForwardIter, _ForwardIter>(__left, __right);
    }
  }
  return pair<_ForwardIter, _ForwardIter>(__first, __first);
}           

template <class _ForwardIter, class _Tp, class _Compare>
inline pair<_ForwardIter, _ForwardIter>
equal_range(_ForwardIter __first, _ForwardIter __last, const _Tp& __val,
            _Compare __comp) {
  return __equal_range(__first, __last, __val, __comp,
                       __distance_type( __first ) );
} 

template <class _ForwardIter, class _Tp>
bool binary_search(_ForwardIter __first, _ForwardIter __last,
                   const _Tp& __val) {
  _ForwardIter __i = lower_bound(__first, __last, __val);
  return __i != __last && !(__val < *__i);
}

template <class _ForwardIter, class _Tp, class _Compare>
bool binary_search(_ForwardIter __first, _ForwardIter __last,
                   const _Tp& __val,
                   _Compare __comp) {
  _ForwardIter __i = lower_bound(__first, __last, __val, __comp);
  return __i != __last && !__comp(__val, *__i);
}

 

template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter merge(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2, _InputIter2 __last2,
                  _OutputIter __result) {
  while (__first1 != __last1 && __first2 != __last2) {
    if (*__first2 < *__first1) {
      *__result = *__first2;
      ++__first2;
    }
    else {
      *__result = *__first1;
      ++__first1;
    }
    ++__result;
  }
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}

template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _Compare>
_OutputIter merge(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2, _InputIter2 __last2,
                  _OutputIter __result, _Compare __comp) {
  while (__first1 != __last1 && __first2 != __last2) {
    if (__comp(*__first2, *__first1)) {
      *__result = *__first2;
      ++__first2;
    }
    else {
      *__result = *__first1;
      ++__first1;
    }
    ++__result;
  }
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}

 

template <class _BidirectionalIter, class _Distance>
void __merge_without_buffer(_BidirectionalIter __first,
                            _BidirectionalIter __middle,
                            _BidirectionalIter __last,
                            _Distance __len1, _Distance __len2) {
  if (__len1 == 0 || __len2 == 0)
    return;
  if (__len1 + __len2 == 2) {
    if (*__middle < *__first)
      iter_swap(__first, __middle);
    return;
  }
  _BidirectionalIter __first_cut = __first;
  _BidirectionalIter __second_cut = __middle;
  _Distance __len11 = 0;
  _Distance __len22 = 0;
  if (__len1 > __len2) {
    __len11 = __len1 / 2;
    advance(__first_cut, __len11);
    __second_cut = lower_bound(__middle, __last, *__first_cut);
    distance(__middle, __second_cut, __len22);
  }
  else {
    __len22 = __len2 / 2;
    advance(__second_cut, __len22);
    __first_cut = upper_bound(__first, __middle, *__second_cut);
    distance(__first, __first_cut, __len11);
  }
  _BidirectionalIter __new_middle
    = rotate(__first_cut, __middle, __second_cut);
  __merge_without_buffer(__first, __first_cut, __new_middle,
                         __len11, __len22);
  __merge_without_buffer(__new_middle, __second_cut, __last, __len1 - __len11,
                         __len2 - __len22);
}

template <class _BidirectionalIter, class _Distance, class _Compare>
void __merge_without_buffer(_BidirectionalIter __first,
                            _BidirectionalIter __middle,
                            _BidirectionalIter __last,
                            _Distance __len1, _Distance __len2,
                            _Compare __comp) {
  if (__len1 == 0 || __len2 == 0)
    return;
  if (__len1 + __len2 == 2) {
    if (__comp(*__middle, *__first))
      iter_swap(__first, __middle);
    return;
  }
  _BidirectionalIter __first_cut = __first;
  _BidirectionalIter __second_cut = __middle;
  _Distance __len11 = 0;
  _Distance __len22 = 0;
  if (__len1 > __len2) {
    __len11 = __len1 / 2;
    advance(__first_cut, __len11);
    __second_cut = lower_bound(__middle, __last, *__first_cut, __comp);
    distance(__middle, __second_cut, __len22);
  }
  else {
    __len22 = __len2 / 2;
    advance(__second_cut, __len22);
    __first_cut = upper_bound(__first, __middle, *__second_cut, __comp);
    distance(__first, __first_cut, __len11);
  }
  _BidirectionalIter __new_middle
    = rotate(__first_cut, __middle, __second_cut);
  __merge_without_buffer(__first, __first_cut, __new_middle, __len11, __len22,
                         __comp);
  __merge_without_buffer(__new_middle, __second_cut, __last, __len1 - __len11,
                         __len2 - __len22, __comp);
}

template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _Distance>
_BidirectionalIter1 __rotate_adaptive(_BidirectionalIter1 __first,
                                      _BidirectionalIter1 __middle,
                                      _BidirectionalIter1 __last,
                                      _Distance __len1, _Distance __len2,
                                      _BidirectionalIter2 __buffer,
                                      _Distance __buffer_size) {
  _BidirectionalIter2 __buffer_end;
  if (__len1 > __len2 && __len2 <= __buffer_size) {
    __buffer_end = copy(__middle, __last, __buffer);
    copy_backward(__first, __middle, __last);
    return copy(__buffer, __buffer_end, __first);
  }
  else if (__len1 <= __buffer_size) {
    __buffer_end = copy(__first, __middle, __buffer);
    copy(__middle, __last, __first);
    return copy_backward(__buffer, __buffer_end, __last);
  }
  else
    return rotate(__first, __middle, __last);
}

template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _BidirectionalIter3>
_BidirectionalIter3 __merge_backward(_BidirectionalIter1 __first1,
                                     _BidirectionalIter1 __last1,
                                     _BidirectionalIter2 __first2,
                                     _BidirectionalIter2 __last2,
                                     _BidirectionalIter3 __result) {
  if (__first1 == __last1)
    return copy_backward(__first2, __last2, __result);
  if (__first2 == __last2)
    return copy_backward(__first1, __last1, __result);
  --__last1;
  --__last2;
  while (true) {
    if (*__last2 < *__last1) {
      *--__result = *__last1;
      if (__first1 == __last1)
        return copy_backward(__first2, ++__last2, __result);
      --__last1;
    }
    else {
      *--__result = *__last2;
      if (__first2 == __last2)
        return copy_backward(__first1, ++__last1, __result);
      --__last2;
    }
  }
}

template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _BidirectionalIter3, class _Compare>
_BidirectionalIter3 __merge_backward(_BidirectionalIter1 __first1,
                                     _BidirectionalIter1 __last1,
                                     _BidirectionalIter2 __first2,
                                     _BidirectionalIter2 __last2,
                                     _BidirectionalIter3 __result,
                                     _Compare __comp) {
  if (__first1 == __last1)
    return copy_backward(__first2, __last2, __result);
  if (__first2 == __last2)
    return copy_backward(__first1, __last1, __result);
  --__last1;
  --__last2;
  while (true) {
    if (__comp(*__last2, *__last1)) {
      *--__result = *__last1;
      if (__first1 == __last1)
        return copy_backward(__first2, ++__last2, __result);
      --__last1;
    }
    else {
      *--__result = *__last2;
      if (__first2 == __last2)
        return copy_backward(__first1, ++__last1, __result);
      --__last2;
    }
  }
}

template <class _BidirectionalIter, class _Distance, class _Pointer>
void __merge_adaptive(_BidirectionalIter __first,
                      _BidirectionalIter __middle, 
                      _BidirectionalIter __last,
                      _Distance __len1, _Distance __len2,
                      _Pointer __buffer, _Distance __buffer_size) {
  if (__len1 <= __len2 && __len1 <= __buffer_size) {
    _Pointer __buffer_end = copy(__first, __middle, __buffer);
    merge(__buffer, __buffer_end, __middle, __last, __first);
  }
  else if (__len2 <= __buffer_size) {
    _Pointer __buffer_end = copy(__middle, __last, __buffer);
    __merge_backward(__first, __middle, __buffer, __buffer_end, __last);
  }
  else {
    _BidirectionalIter __first_cut = __first;
    _BidirectionalIter __second_cut = __middle;
    _Distance __len11 = 0;
    _Distance __len22 = 0;
    if (__len1 > __len2) {
      __len11 = __len1 / 2;
      advance(__first_cut, __len11);
      __second_cut = lower_bound(__middle, __last, *__first_cut);
      distance(__middle, __second_cut, __len22); 
    }
    else {
      __len22 = __len2 / 2;
      advance(__second_cut, __len22);
      __first_cut = upper_bound(__first, __middle, *__second_cut);
      distance(__first, __first_cut, __len11);
    }
    _BidirectionalIter __new_middle =
      __rotate_adaptive(__first_cut, __middle, __second_cut, __len1 - __len11,
                        __len22, __buffer, __buffer_size);
    __merge_adaptive(__first, __first_cut, __new_middle, __len11,
                     __len22, __buffer, __buffer_size);
    __merge_adaptive(__new_middle, __second_cut, __last, __len1 - __len11,
                     __len2 - __len22, __buffer, __buffer_size);
  }
}

template <class _BidirectionalIter, class _Distance, class _Pointer,
          class _Compare>
void __merge_adaptive(_BidirectionalIter __first, 
                      _BidirectionalIter __middle, 
                      _BidirectionalIter __last,
                      _Distance __len1, _Distance __len2,
                      _Pointer __buffer, _Distance __buffer_size,
                      _Compare __comp) {
  if (__len1 <= __len2 && __len1 <= __buffer_size) {
    _Pointer __buffer_end = copy(__first, __middle, __buffer);
    merge(__buffer, __buffer_end, __middle, __last, __first, __comp);
  }
  else if (__len2 <= __buffer_size) {
    _Pointer __buffer_end = copy(__middle, __last, __buffer);
    __merge_backward(__first, __middle, __buffer, __buffer_end, __last,
                     __comp);
  }
  else {
    _BidirectionalIter __first_cut = __first;
    _BidirectionalIter __second_cut = __middle;
    _Distance __len11 = 0;
    _Distance __len22 = 0;
    if (__len1 > __len2) {
      __len11 = __len1 / 2;
      advance(__first_cut, __len11);
      __second_cut = lower_bound(__middle, __last, *__first_cut, __comp);
      distance(__middle, __second_cut, __len22);   
    }
    else {
      __len22 = __len2 / 2;
      advance(__second_cut, __len22);
      __first_cut = upper_bound(__first, __middle, *__second_cut, __comp);
      distance(__first, __first_cut, __len11);
    }
    _BidirectionalIter __new_middle =
      __rotate_adaptive(__first_cut, __middle, __second_cut, __len1 - __len11,
                        __len22, __buffer, __buffer_size);
    __merge_adaptive(__first, __first_cut, __new_middle, __len11,
                     __len22, __buffer, __buffer_size, __comp);
    __merge_adaptive(__new_middle, __second_cut, __last, __len1 - __len11,
                     __len2 - __len22, __buffer, __buffer_size, __comp);
  }
}

template <class _BidirectionalIter, class _Tp, class _Distance>
inline void __inplace_merge_aux(_BidirectionalIter __first,
                                _BidirectionalIter __middle,
                                _BidirectionalIter __last, _Tp*, _Distance*) {
  _Distance __len1 = 0;
  distance(__first, __middle, __len1);
  _Distance __len2 = 0;
  distance(__middle, __last, __len2);

  _Temporary_buffer<_BidirectionalIter, _Tp> __buf(__first, __last);
  if (__buf.begin() == 0)
    __merge_without_buffer(__first, __middle, __last, __len1, __len2);
  else
    __merge_adaptive(__first, __middle, __last, __len1, __len2,
                     __buf.begin(), _Distance(__buf.size()));
}

template <class _BidirectionalIter, class _Tp, 
          class _Distance, class _Compare>
inline void __inplace_merge_aux(_BidirectionalIter __first,
                                _BidirectionalIter __middle,
                                _BidirectionalIter __last, _Tp*, _Distance*,
                                _Compare __comp) {
  _Distance __len1 = 0;
  distance(__first, __middle, __len1);
  _Distance __len2 = 0;
  distance(__middle, __last, __len2);

  _Temporary_buffer<_BidirectionalIter, _Tp> __buf(__first, __last);
  if (__buf.begin() == 0)
    __merge_without_buffer(__first, __middle, __last, __len1, __len2, __comp);
  else
    __merge_adaptive(__first, __middle, __last, __len1, __len2,
                     __buf.begin(), _Distance(__buf.size()),
                     __comp);
}

template <class _BidirectionalIter>
inline void inplace_merge(_BidirectionalIter __first,
                          _BidirectionalIter __middle,
                          _BidirectionalIter __last) {
  if (__first == __middle || __middle == __last)
    return;
  __inplace_merge_aux(__first, __middle, __last,
                      __value_type( __first ) , __distance_type( __first ) );
}

template <class _BidirectionalIter, class _Compare>
inline void inplace_merge(_BidirectionalIter __first,
                          _BidirectionalIter __middle,
                          _BidirectionalIter __last, _Compare __comp) {
  if (__first == __middle || __middle == __last)
    return;
  __inplace_merge_aux(__first, __middle, __last,
                      __value_type( __first ) , __distance_type( __first ) ,
                      __comp);
}

 
 
 
 

template <class _InputIter1, class _InputIter2>
bool includes(_InputIter1 __first1, _InputIter1 __last1,
              _InputIter2 __first2, _InputIter2 __last2) {
  while (__first1 != __last1 && __first2 != __last2)
    if (*__first2 < *__first1)
      return false;
    else if(*__first1 < *__first2) 
      ++__first1;
    else
      ++__first1, ++__first2;

  return __first2 == __last2;
}

template <class _InputIter1, class _InputIter2, class _Compare>
bool includes(_InputIter1 __first1, _InputIter1 __last1,
              _InputIter2 __first2, _InputIter2 __last2, _Compare __comp) {
  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first2, *__first1))
      return false;
    else if(__comp(*__first1, *__first2)) 
      ++__first1;
    else
      ++__first1, ++__first2;

  return __first2 == __last2;
}

template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter set_union(_InputIter1 __first1, _InputIter1 __last1,
                      _InputIter2 __first2, _InputIter2 __last2,
                      _OutputIter __result) {
  while (__first1 != __last1 && __first2 != __last2) {
    if (*__first1 < *__first2) {
      *__result = *__first1;
      ++__first1;
    }
    else if (*__first2 < *__first1) {
      *__result = *__first2;
      ++__first2;
    }
    else {
      *__result = *__first1;
      ++__first1;
      ++__first2;
    }
    ++__result;
  }
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}

template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _Compare>
_OutputIter set_union(_InputIter1 __first1, _InputIter1 __last1,
                      _InputIter2 __first2, _InputIter2 __last2,
                      _OutputIter __result, _Compare __comp) {
  while (__first1 != __last1 && __first2 != __last2) {
    if (__comp(*__first1, *__first2)) {
      *__result = *__first1;
      ++__first1;
    }
    else if (__comp(*__first2, *__first1)) {
      *__result = *__first2;
      ++__first2;
    }
    else {
      *__result = *__first1;
      ++__first1;
      ++__first2;
    }
    ++__result;
  }
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}

template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter set_intersection(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2,
                             _OutputIter __result) {
  while (__first1 != __last1 && __first2 != __last2) 
    if (*__first1 < *__first2) 
      ++__first1;
    else if (*__first2 < *__first1) 
      ++__first2;
    else {
      *__result = *__first1;
      ++__first1;
      ++__first2;
      ++__result;
    }
  return __result;
}

template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _Compare>
_OutputIter set_intersection(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2,
                             _OutputIter __result, _Compare __comp) {
  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first1, *__first2))
      ++__first1;
    else if (__comp(*__first2, *__first1))
      ++__first2;
    else {
      *__result = *__first1;
      ++__first1;
      ++__first2;
      ++__result;
    }
  return __result;
}

template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter set_difference(_InputIter1 __first1, _InputIter1 __last1,
                           _InputIter2 __first2, _InputIter2 __last2,
                           _OutputIter __result) {
  while (__first1 != __last1 && __first2 != __last2)
    if (*__first1 < *__first2) {
      *__result = *__first1;
      ++__first1;
      ++__result;
    }
    else if (*__first2 < *__first1)
      ++__first2;
    else {
      ++__first1;
      ++__first2;
    }
  return copy(__first1, __last1, __result);
}

template <class _InputIter1, class _InputIter2, class _OutputIter, 
          class _Compare>
_OutputIter set_difference(_InputIter1 __first1, _InputIter1 __last1,
                           _InputIter2 __first2, _InputIter2 __last2, 
                           _OutputIter __result, _Compare __comp) {
  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first1, *__first2)) {
      *__result = *__first1;
      ++__first1;
      ++__result;
    }
    else if (__comp(*__first2, *__first1))
      ++__first2;
    else {
      ++__first1;
      ++__first2;
    }
  return copy(__first1, __last1, __result);
}

template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter 
set_symmetric_difference(_InputIter1 __first1, _InputIter1 __last1,
                         _InputIter2 __first2, _InputIter2 __last2,
                         _OutputIter __result) {
  while (__first1 != __last1 && __first2 != __last2)
    if (*__first1 < *__first2) {
      *__result = *__first1;
      ++__first1;
      ++__result;
    }
    else if (*__first2 < *__first1) {
      *__result = *__first2;
      ++__first2;
      ++__result;
    }
    else {
      ++__first1;
      ++__first2;
    }
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}

template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _Compare>
_OutputIter 
set_symmetric_difference(_InputIter1 __first1, _InputIter1 __last1,
                         _InputIter2 __first2, _InputIter2 __last2,
                         _OutputIter __result,
                         _Compare __comp) {
  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first1, *__first2)) {
      *__result = *__first1;
      ++__first1;
      ++__result;
    }
    else if (__comp(*__first2, *__first1)) {
      *__result = *__first2;
      ++__first2;
      ++__result;
    }
    else {
      ++__first1;
      ++__first2;
    }
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}

 
 

template <class _ForwardIter>
_ForwardIter max_element(_ForwardIter __first, _ForwardIter __last) {
  if (__first == __last) return __first;
  _ForwardIter __result = __first;
  while (++__first != __last) 
    if (*__result < *__first)
      __result = __first;
  return __result;
}

template <class _ForwardIter, class _Compare>
_ForwardIter max_element(_ForwardIter __first, _ForwardIter __last,
                            _Compare __comp) {
  if (__first == __last) return __first;
  _ForwardIter __result = __first;
  while (++__first != __last) 
    if (__comp(*__result, *__first)) __result = __first;
  return __result;
}

template <class _ForwardIter>
_ForwardIter min_element(_ForwardIter __first, _ForwardIter __last) {
  if (__first == __last) return __first;
  _ForwardIter __result = __first;
  while (++__first != __last) 
    if (*__first < *__result)
      __result = __first;
  return __result;
}

template <class _ForwardIter, class _Compare>
_ForwardIter min_element(_ForwardIter __first, _ForwardIter __last,
                            _Compare __comp) {
  if (__first == __last) return __first;
  _ForwardIter __result = __first;
  while (++__first != __last) 
    if (__comp(*__first, *__result))
      __result = __first;
  return __result;
}

 
 

template <class _BidirectionalIter>
bool next_permutation(_BidirectionalIter __first, _BidirectionalIter __last) {
  if (__first == __last)
    return false;
  _BidirectionalIter __i = __first;
  ++__i;
  if (__i == __last)
    return false;
  __i = __last;
  --__i;

  for(;;) {
    _BidirectionalIter __ii = __i;
    --__i;
    if (*__i < *__ii) {
      _BidirectionalIter __j = __last;
      while (!(*__i < *--__j))
        {}
      iter_swap(__i, __j);
      reverse(__ii, __last);
      return true;
    }
    if (__i == __first) {
      reverse(__first, __last);
      return false;
    }
  }
}

template <class _BidirectionalIter, class _Compare>
bool next_permutation(_BidirectionalIter __first, _BidirectionalIter __last,
                      _Compare __comp) {
  if (__first == __last)
    return false;
  _BidirectionalIter __i = __first;
  ++__i;
  if (__i == __last)
    return false;
  __i = __last;
  --__i;

  for(;;) {
    _BidirectionalIter __ii = __i;
    --__i;
    if (__comp(*__i, *__ii)) {
      _BidirectionalIter __j = __last;
      while (!__comp(*__i, *--__j))
        {}
      iter_swap(__i, __j);
      reverse(__ii, __last);
      return true;
    }
    if (__i == __first) {
      reverse(__first, __last);
      return false;
    }
  }
}

template <class _BidirectionalIter>
bool prev_permutation(_BidirectionalIter __first, _BidirectionalIter __last) {
  if (__first == __last)
    return false;
  _BidirectionalIter __i = __first;
  ++__i;
  if (__i == __last)
    return false;
  __i = __last;
  --__i;

  for(;;) {
    _BidirectionalIter __ii = __i;
    --__i;
    if (*__ii < *__i) {
      _BidirectionalIter __j = __last;
      while (!(*--__j < *__i))
        {}
      iter_swap(__i, __j);
      reverse(__ii, __last);
      return true;
    }
    if (__i == __first) {
      reverse(__first, __last);
      return false;
    }
  }
}

template <class _BidirectionalIter, class _Compare>
bool prev_permutation(_BidirectionalIter __first, _BidirectionalIter __last,
                      _Compare __comp) {
  if (__first == __last)
    return false;
  _BidirectionalIter __i = __first;
  ++__i;
  if (__i == __last)
    return false;
  __i = __last;
  --__i;

  for(;;) {
    _BidirectionalIter __ii = __i;
    --__i;
    if (__comp(*__ii, *__i)) {
      _BidirectionalIter __j = __last;
      while (!__comp(*--__j, *__i))
        {}
      iter_swap(__i, __j);
      reverse(__ii, __last);
      return true;
    }
    if (__i == __first) {
      reverse(__first, __last);
      return false;
    }
  }
}

 

template <class _InputIter, class _ForwardIter>
_InputIter find_first_of(_InputIter __first1, _InputIter __last1,
                         _ForwardIter __first2, _ForwardIter __last2)
{
  for ( ; __first1 != __last1; ++__first1) 
    for (_ForwardIter __iter = __first2; __iter != __last2; ++__iter)
      if (*__first1 == *__iter)
        return __first1;
  return __last1;
}

template <class _InputIter, class _ForwardIter, class _BinaryPredicate>
_InputIter find_first_of(_InputIter __first1, _InputIter __last1,
                         _ForwardIter __first2, _ForwardIter __last2,
                         _BinaryPredicate __comp)
{
  for ( ; __first1 != __last1; ++__first1) 
    for (_ForwardIter __iter = __first2; __iter != __last2; ++__iter)
      if (__comp(*__first1, *__iter))
        return __first1;
  return __last1;
}


 
 
 
 

 
template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter1 __find_end(_ForwardIter1 __first1, _ForwardIter1 __last1,
                         _ForwardIter2 __first2, _ForwardIter2 __last2,
                         forward_iterator_tag, forward_iterator_tag)
{
  if (__first2 == __last2)
    return __last1;
  else {
    _ForwardIter1 __result = __last1;
    while (1) {
      _ForwardIter1 __new_result
        = search(__first1, __last1, __first2, __last2);
      if (__new_result == __last1)
        return __result;
      else {
        __result = __new_result;
        __first1 = __new_result;
        ++__first1;
      }
    }
  }
}

template <class _ForwardIter1, class _ForwardIter2,
          class _BinaryPredicate>
_ForwardIter1 __find_end(_ForwardIter1 __first1, _ForwardIter1 __last1,
                         _ForwardIter2 __first2, _ForwardIter2 __last2,
                         forward_iterator_tag, forward_iterator_tag,
                         _BinaryPredicate __comp)
{
  if (__first2 == __last2)
    return __last1;
  else {
    _ForwardIter1 __result = __last1;
    while (1) {
      _ForwardIter1 __new_result
        = search(__first1, __last1, __first2, __last2, __comp);
      if (__new_result == __last1)
        return __result;
      else {
        __result = __new_result;
        __first1 = __new_result;
        ++__first1;
      }
    }
  }
}

 


template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter1
__find_end(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1,
           _BidirectionalIter2 __first2, _BidirectionalIter2 __last2,
           bidirectional_iterator_tag, bidirectional_iterator_tag)
{
  typedef reverse_iterator<_BidirectionalIter1> _RevIter1;
  typedef reverse_iterator<_BidirectionalIter2> _RevIter2;

  _RevIter1 __rlast1(__first1);
  _RevIter2 __rlast2(__first2);
  _RevIter1 __rresult = search(_RevIter1(__last1), __rlast1,
                               _RevIter2(__last2), __rlast2);

  if (__rresult == __rlast1)
    return __last1;
  else {
    _BidirectionalIter1 __result = __rresult.base();
    advance(__result, -distance(__first2, __last2));
    return __result;
  }
}

template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _BinaryPredicate>
_BidirectionalIter1
__find_end(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1,
           _BidirectionalIter2 __first2, _BidirectionalIter2 __last2,
           bidirectional_iterator_tag, bidirectional_iterator_tag, 
           _BinaryPredicate __comp)
{
  typedef reverse_iterator<_BidirectionalIter1> _RevIter1;
  typedef reverse_iterator<_BidirectionalIter2> _RevIter2;

  _RevIter1 __rlast1(__first1);
  _RevIter2 __rlast2(__first2);
  _RevIter1 __rresult = search(_RevIter1(__last1), __rlast1,
                               _RevIter2(__last2), __rlast2,
                               __comp);

  if (__rresult == __rlast1)
    return __last1;
  else {
    _BidirectionalIter1 __result = __rresult.base();
    advance(__result, -distance(__first2, __last2));
    return __result;
  }
}


 

template <class _ForwardIter1, class _ForwardIter2>
inline _ForwardIter1 
find_end(_ForwardIter1 __first1, _ForwardIter1 __last1, 
         _ForwardIter2 __first2, _ForwardIter2 __last2)
{
  return __find_end(__first1, __last1, __first2, __last2,
                    __iterator_category( __first1 ) ,
                    __iterator_category( __first2 ) );
}

template <class _ForwardIter1, class _ForwardIter2, 
          class _BinaryPredicate>
inline _ForwardIter1 
find_end(_ForwardIter1 __first1, _ForwardIter1 __last1, 
         _ForwardIter2 __first2, _ForwardIter2 __last2,
         _BinaryPredicate __comp)
{
  return __find_end(__first1, __last1, __first2, __last2,
                    __iterator_category( __first1 ) ,
                    __iterator_category( __first2 ) ,
                    __comp);
}

 
 
 

template <class _RandomAccessIter, class _Distance>
bool __is_heap(_RandomAccessIter __first, _Distance __n)
{
  _Distance __parent = 0;
  for (_Distance __child = 1; __child < __n; ++__child) {
    if (__first[__parent] < __first[__child]) 
      return false;
    if ((__child & 1) == 0)
      ++__parent;
  }
  return true;
}

template <class _RandomAccessIter, class _Distance, class _StrictWeakOrdering>
bool __is_heap(_RandomAccessIter __first, _StrictWeakOrdering __comp,
               _Distance __n)
{
  _Distance __parent = 0;
  for (_Distance __child = 1; __child < __n; ++__child) {
    if (__comp(__first[__parent], __first[__child]))
      return false;
    if ((__child & 1) == 0)
      ++__parent;
  }
  return true;
}

template <class _RandomAccessIter>
inline bool is_heap(_RandomAccessIter __first, _RandomAccessIter __last)
{
  return __is_heap(__first, __last - __first);
}


template <class _RandomAccessIter, class _StrictWeakOrdering>
inline bool is_heap(_RandomAccessIter __first, _RandomAccessIter __last,
                    _StrictWeakOrdering __comp)
{
  return __is_heap(__first, __comp, __last - __first);
}

 
 
 

template <class _ForwardIter>
bool is_sorted(_ForwardIter __first, _ForwardIter __last)
{
  if (__first == __last)
    return true;

  _ForwardIter __next = __first;
  for (++__next; __next != __last; __first = __next, ++__next) {
    if (*__next < *__first)
      return false;
  }

  return true;
}

template <class _ForwardIter, class _StrictWeakOrdering>
bool is_sorted(_ForwardIter __first, _ForwardIter __last,
               _StrictWeakOrdering __comp)
{
  if (__first == __last)
    return true;

  _ForwardIter __next = __first;
  for (++__next; __next != __last; __first = __next, ++__next) {
    if (__comp(*__next, *__first))
      return false;
  }

  return true;
}





 



 
 
 
# 34 "/usr/lib/gcc-lib/i386-linux/2.95.4/../../../../include/g++-3/algorithm" 2 3




 
 
 
# 29 "policy_vector.hpp" 2



namespace boost {

 

template < class T,
           class Alloc,
           class StoragePolicy,
           template <class S> class CheckingPolicy,
           template <class T2, class V2> class IteratorPolicy
	 > 
class policy_vector;

 
 
 
 
 

template <class T, class Alloc>
class policy_vector_base {
public:
  typedef Alloc allocator_type;
  const allocator_type& get_allocator() const { return alloc; }
  policy_vector_base(const allocator_type& a) : alloc(a) {}
protected:
  T* allocate(std::size_t n) { return alloc.allocate(n); }
  void deallocate(T* ptr, std::size_t n) { if (ptr) alloc.deallocate(ptr,n); }
  void construct(T* ptr, T const& x) { alloc.construct(ptr, x); }
  void destroy(T* ptr) { alloc.destroy(ptr); }
  void destroy(T* first, T* last) {
    while (first!=last) { alloc.destroy(first); ++first; }
  }
private:
  Alloc alloc;
};

 
 
 

typedef int no_allocator_policy;

 
 
 
 
 

template < class T, class Alloc, class StoragePolicy >
struct policy_vector_storage;

 

template <std::size_t N>
struct fixed_aggregate_array {
  typedef std::size_t    size_type;
   
  static size_type size() { return N; }
  static bool empty() { return false; }
  static size_type max_size() { return N; }
  enum { static_size = N };
};

 
 
 
template < class T, class Null, std::size_t N >
struct policy_vector_storage<T, Null, fixed_aggregate_array<N> >
  : public fixed_aggregate_array<N>
{
private:
   
   
  typedef Null                                            Alloc;
  typedef fixed_aggregate_array<N>                        storage_policy;
  typedef policy_vector_storage<T, Alloc, storage_policy> self_type;
protected:
  T elems[N];
public:
   
  policy_vector_storage(const Alloc&, const storage_policy&)
    {}
  policy_vector_storage(std::size_t n, const T& value,
                        const Alloc&, const storage_policy&)
    { std::fill_n(elems, std::min(n,N), value); }
  policy_vector_storage(std::size_t, const Alloc&, const storage_policy&)
    {}
  policy_vector_storage(self_type const& rhs)
    { std::copy(rhs.elems, rhs.elems+N, elems); }
  template <class T2>
  policy_vector_storage(T2 (&rhs)[N], const Alloc&, const storage_policy&)
    { std::copy(rhs, rhs+N, elems); }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last, const Alloc&,
                        const storage_policy&)
    { std::copy(first, first+std::min(std::distance(first, last),N), elems); }
   
   
   
  void assign (const T& value) {
    std::fill_n(elems, N, value);
  }
   
  void assign (std::size_t n, const T& value) {
    std::fill_n(elems, std::min(n,N), value);
  }
   
  template <class RandomAccessIter>
  void assign(RandomAccessIter first, RandomAccessIter last) {
    std::copy(first, first+std::min(last-first, (std::ptrdiff_t)N), elems);
  }
   
  void swap (self_type& y) {
      std::swap_ranges(elems, elems+N, y.elems);
  }
};

 

template <std::size_t N>
struct fixed_allocated_array {
  typedef std::size_t    size_type;
   
  static size_type size() { return N; }
  static bool empty() { return false; }
  static size_type max_size() { return N; }
  enum { static_size = N };
};

template < class T, class Alloc, std::size_t N >
struct policy_vector_storage<T, Alloc, fixed_allocated_array<N> >
  : public policy_vector_base<T,Alloc>
  , public fixed_allocated_array<N>
{
private:
  typedef policy_vector_base<T,Alloc>                     base_type;
  typedef fixed_allocated_array<N>                        storage_policy;
  typedef policy_vector_storage<T, Alloc, storage_policy> self_type;
protected:
  T* elems;
public:
   
  policy_vector_storage(const Alloc& a, const storage_policy&)
    : base_type(a), elems(allocate(N))
    {}
  policy_vector_storage(std::size_t n, const T& value,
                        const Alloc& a, const storage_policy&)
    : base_type(a), elems(allocate(N))
    { std::uninitialized_fill_n(elems, std::min(n,N), value); }
  policy_vector_storage(std::size_t, const Alloc& a, const storage_policy&)
    : base_type(a), elems(allocate(N))
    { std::uninitialized_fill_n(elems, N, T()); }
  policy_vector_storage(const self_type& rhs)
    : base_type(rhs.get_allocator()), elems(allocate(N)) 
    { std::uninitialized_copy(rhs.elems, rhs.elems+N, elems); }
  template <class T2>
  policy_vector_storage(T2 (&rhs)[N], const Alloc& a, const storage_policy&)
    : base_type(a), elems(allocate(N))
    {
      T* end = std::uninitialized_copy(rhs, rhs+N, elems);
      std::uninitialized_fill(end, elems+N, T());
    }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last,
                        const Alloc& a, const storage_policy&)
    : base_type(a), elems(allocate(N))
    {
      if (std::distance(first, last) < N) {
        T* end = std::uninitialized_copy(first, last, elems);
        std::uninitialized_fill(end, elems+N, T());
      } else
        std::uninitialized_copy(first, first+N, elems);
    }
   
  ~policy_vector_storage()
    {
      destroy(elems, elems+N);
      deallocate(elems, N); elems = 0;
    }
   
  self_type& operator=(self_type const& rhs) {
    if (&rhs == this) return *this;
    std::copy(rhs.elems, rhs.elems+N, elems);
    return *this;
  }
   
  void assign (const T& value) {
    std::fill_n(elems, N, value);
  }
   
  void assign (std::size_t n, const T& value) {
    std::fill_n(elems, std::min(n,N), value);
  }
   
  template <class RandomAccessIter>
  void assign(RandomAccessIter first, RandomAccessIter last) {
    std::copy(first, first+std::min(last-first, (std::ptrdiff_t)N), elems);
  }
   
  void swap (self_type& y) {
    T* tmp = y.elems; y.elems = elems; elems = tmp;
  }
};

 
 
 

template <class T, std::size_t N>
struct fixed_external_static_array {
  typedef std::size_t size_type;
   
  fixed_external_static_array() : elems(0) {}
  fixed_external_static_array(T (&ptr)[N]) : elems(ptr) {}
  fixed_external_static_array(fixed_external_static_array const& sp)
    : elems(sp.elems) {}
   
  static size_type size() { return N; }
  static bool empty() { return false; }
  static size_type max_size() { return N; }
  enum { static_size = N };
protected:
  T* elems;
};

template < class T, class Null, std::size_t N>
struct policy_vector_storage<T, Null,
                             fixed_external_static_array<T, N> >
  : public fixed_external_static_array<T, N>
{
private:
   
  typedef Null                                            Alloc;
  typedef fixed_external_static_array<T, N>               storage_policy;
  typedef policy_vector_storage<T, Alloc, storage_policy> self_type;
public:
   
  policy_vector_storage(const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    {}
  policy_vector_storage(std::size_t n, const T& value,
                        const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    { std::fill_n(elems, std::min(n,N), value); }
  policy_vector_storage(std::size_t, const Alloc&, storage_policy& sp)
    : storage_policy(sp)
    { std::fill_n(elems, N, T()); }
  policy_vector_storage(const self_type& rhs)
    : storage_policy(rhs)
    {}  
  template <class T2>
  policy_vector_storage(T2 (&rhs)[N], const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    { std::copy(rhs, rhs+N, elems); }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last,
                        const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    {
      if (std::distance(first, last) < N) {
        T* end = std::copy(first, last, elems);
        std::fill(end, elems+N, T());
      } else
        std::copy(first, first+N, elems);
    }
   
  ~policy_vector_storage() {}
   
  self_type& operator=(self_type const& rhs) {
    if (&rhs == this) return *this;
    storage_policy() = rhs;
    return *this;
  }
   
  void assign (const T& value) {
    std::fill_n(elems, N, value);
  }
   
  void assign (std::size_t n, const T& value) {
    std::fill_n(elems, std::min(n,N), value);
  }
   
  template <class RandomAccessIter>
  void assign(RandomAccessIter first, RandomAccessIter last) {
    std::copy(first, first+std::min(last-first, (std::ptrdiff_t)N), elems);
  }
   
  void swap (self_type& y) {
    T* tmp = y.elems; y.elems = elems; elems = tmp;
  }
};

 
 
 

template <class Iterator, class ConstIterator>
struct fixed_external_range {
   
  typedef typename std::iterator_traits<Iterator>::difference_type size_type;
  typedef Iterator                                           iterator;
  typedef ConstIterator                                      const_iterator;
   
  fixed_external_range() : elems(0), N(0) {}
  fixed_external_range(Iterator first, Iterator last)
    : elems(first), N(last-first) {}
  fixed_external_range(fixed_external_range const& sp)
    : elems(sp.elems), N(sp.N) {}
   
  size_type size() const { return N; }
  bool empty() const { return false; }
  size_type max_size() const { return N; }
protected:
  Iterator elems;
  size_type N;
  enum { static_size = 0 };  
};

template < class T, class Null, class Iterator, class ConstIterator>
struct policy_vector_storage<T, Null,
	                     fixed_external_range<Iterator,ConstIterator> >
  : public fixed_external_range<Iterator,ConstIterator>
{
private:
   
  typedef Null                                               Alloc;
  typedef policy_vector_base<T,Alloc>                        base_type;
  typedef fixed_external_range<Iterator,ConstIterator>       storage_policy;
  typedef policy_vector_storage<T, Alloc, storage_policy >   self_type;
public:
   
  policy_vector_storage(const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    {}
  policy_vector_storage(std::size_t n, const T& value,
                        const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    { std::fill_n(elems, std::min(n,N), value); }
  policy_vector_storage(std::size_t, const storage_policy& sp)
    : storage_policy(sp)
    { std::fill_n(elems, N, T()); }
  policy_vector_storage(const self_type& rhs)
    : storage_policy(rhs)
    { std::copy(rhs.elems, rhs.elems+N, elems); }
   







  template <class Iterator2>
  policy_vector_storage(Iterator2 first, Iterator2 last,
                        const Alloc&, const storage_policy& sp)
    : storage_policy(sp)
    {
      if (std::distance(first, last) < N) {
        Iterator end = std::copy(first, last, elems);
        std::fill(end, elems+N, T());
      } else
        std::copy(first, first+N, elems);
    }
   
  ~policy_vector_storage() {}
   
  self_type& operator=(self_type const& rhs) {
    if (&rhs == this) return *this;
    storage_policy::operator=(this, rhs);
    return *this;
  }
   
  void assign (const T& value) {
    std::fill_n(elems, N, value);
  }
   
  void assign (std::size_t n, const T& value) {
    std::fill_n(elems, std::min(n,N), value);
  }
   
  template <class Iterator2>
  void assign(Iterator2 first, Iterator2 last) {
    std::copy(first, first+std::min(last-first, (std::ptrdiff_t)N), elems);
  }
   
  void swap (self_type& y) {
    Iterator tmp = y.elems; y.elems = elems; elems = tmp;
    typename storage_policy::size_type tmpN = y.N; y.N = N; N = tmpN;
  }
};

 
 
 

    
    
    
    
  
    
   template <std::size_t N>
   struct incremental_resize {
     typedef std::size_t size_type;
     bool resize_before_insert(size_type size, size_type capacity,
                               size_type n, size_type &new_capacity)
     {
       new_capacity = capacity + ((size+n-capacity+N-1)/N) * N;
       return new_capacity != capacity;
     }
     bool resize_after_erase(size_type, size_type, size_type&)
       { return false; }
   };
    
   template <std::size_t N>
   struct multiply_resize {
     typedef std::size_t size_type;
     bool resize_before_insert(size_type size, size_type capacity,
                               size_type n, size_type &new_capacity)
     {
       new_capacity = capacity ? capacity : 1;
       while (size+n > new_capacity) new_capacity *= N;
       return new_capacity != capacity;
     }
     bool resize_after_erase(size_type, size_type, size_type&)
       { return false; }
   };
    
   template <std::size_t N>
   struct multiply_divide_resize {
     typedef std::size_t size_type;
     bool resize_before_insert(size_type size, size_type capacity,
                               size_type n, size_type &new_capacity)
     {
       new_capacity = capacity ? capacity : 1;
       while (size+n > new_capacity) new_capacity *= N;
       return new_capacity != capacity;
     }
     bool resize_after_erase(size_type size, size_type capacity,
                              size_type &new_capacity)
     {
       const size_type N2 = N*N;
       new_capacity = capacity;
       while (size < new_capacity/N2) new_capacity /= N;
       return new_capacity != capacity;
     }
   };
    
    

template < class ResizePolicy = multiply_resize<2>, std::size_t initN = 0 >
struct variable_allocated_array : protected ResizePolicy {
  enum { static_init_size = initN };
};

template < class T, class Alloc,  class ResizePolicy, std::size_t initN >
struct policy_vector_storage<T, Alloc,
         variable_allocated_array<ResizePolicy, initN> >
  : public policy_vector_base<T,Alloc>
  , public variable_allocated_array<ResizePolicy, initN>
{
private:
  typedef policy_vector_base<T,Alloc>                    base_type;
  typedef variable_allocated_array<ResizePolicy, initN>  storage_policy;
  typedef policy_vector_storage<T,Alloc,storage_policy>  self_type;
  typedef std::size_t                                    size_type;
protected:
  size_type nelems, maxelems;
  T* elems;
public:
   
  policy_vector_storage(const Alloc& a, const storage_policy& sp)
    : base_type(a), storage_policy(sp), nelems(0), maxelems(initN)
    , elems(maxelems ? allocate(maxelems) : 0)
    {}
  policy_vector_storage(size_type n, const T& value,
                        const Alloc& a, const storage_policy& sp)
    : base_type(a), storage_policy(sp), nelems(n), maxelems(std::max(n,initN))
    , elems(maxelems ? allocate(maxelems) : 0)
    { std::uninitialized_fill_n(elems, nelems, value); }
  policy_vector_storage(size_type n, const Alloc& a, const storage_policy& sp)
    : base_type(a), storage_policy(sp) , nelems(n), maxelems(std::max(n,initN))
    , elems(maxelems ? allocate(maxelems) : 0)
    { std::uninitialized_fill_n(elems, nelems, T()); }
  policy_vector_storage(self_type const& rhs)
    : base_type(rhs.get_allocator()), storage_policy(rhs)
    , nelems(rhs.nelems), maxelems(std::max(rhs.maxelems,initN))
    , elems(maxelems ? allocate(maxelems) : 0)
    { std::uninitialized_copy(rhs.elems, rhs.elems+nelems, elems); }
  template <class Iterator>
  policy_vector_storage(Iterator first, Iterator last,
                        const Alloc& a, const storage_policy& sp)
    : base_type(a), storage_policy(sp)
    , nelems(std::distance(first, last)), maxelems(std::max(initN,nelems))
    , elems(maxelems ? allocate(maxelems) : 0)
    { std::uninitialized_copy(first, last, elems); }
   
  ~policy_vector_storage() {
    destroy(elems, elems+nelems); nelems = 0;
    deallocate(elems,maxelems); elems = 0;
  }
   
  policy_vector_storage& operator= (self_type const& rhs) {
    if (&rhs == this) return *this;
    destroy(elems, elems+nelems);
    if (elems) deallocate(elems, maxelems);
    nelems = rhs.nelems; maxelems = rhs.maxelems;
    elems = rhs.maxelems ? allocate(rhs.maxelems) : 0;
    std::copy( rhs.elems, rhs.elems+nelems, elems );
     
     
    return *this;
  }
   
  size_type size() const { return nelems; }
  bool empty() const { return nelems == 0; }
  size_type max_size() const { return policy_vector_base<T,Alloc>::max_size(); }
protected:
  T* allocate_and_fill_n(size_type maxn, size_type n, const T& value) {
    T* result = allocate(maxn);
    try {
      std::uninitialized_fill_n(result, n, value);
      return result;
    }
    catch(...) {
      deallocate(result, maxn);
      throw;
    }
  }
  template <class InputIter>
  T* allocate_and_copy(size_type n, InputIter first, InputIter last) {
    T* result = allocate(n);
    try {
      std::uninitialized_copy(first, last, result);
      return result;
    }
    catch(...) {
      deallocate(result, n);
      throw;
    }
  }
public:
   
  void assign(const T& value) {
    std::fill_n(elems, nelems, value);
  }
   
  void assign(size_type n, const T& value) {
    if (n>nelems) {
      size_type maxn;
      if (resize_before_insert(nelems, maxelems, n-nelems, maxn)) {
        T* new_elems = allocate_and_fill_n(maxn, n, value);
        destroy(elems, elems+nelems);
        if (elems) deallocate(elems,maxelems);
        elems = new_elems;
        maxelems = maxn;
      } else {
        T* end = std::fill_n(elems, nelems, value);
        std::uninitialized_fill_n(end, n-nelems, value);
      }
      nelems = n;
    } else {
      T* new_end = std::fill_n(elems, n, value);
      destroy(new_end, elems+nelems);
      nelems = n;
    }
  }
   
  template <class InputIter>
  void assign(InputIter first, InputIter last) {
    size_type n = std::distance(first, last);
    if (n>nelems) {
      size_type maxn;
      if (resize_before_insert(nelems, maxelems, n-nelems, maxn)) {
        T* new_elems = allocate_and_copy(maxn, first, last);
        destroy(elems, elems+nelems);
        if (elems) deallocate(elems,maxelems);
        elems = new_elems;
        maxelems = maxn;
      } else {
         
        InputIter middle = first;
	std::advance(middle, nelems);
        std::copy(first, middle, elems);
        std::uninitialized_copy(middle, last, elems+nelems);
      }
      nelems = n;
    } else {
      std::copy(first, last, elems);
      destroy(elems+n, elems+nelems);
      nelems = n;
      size_type new_capacity;
      if (resize_after_erase(nelems, maxelems, new_capacity))
        reserve(new_capacity);
    }
  }
   
  size_type capacity() const { return maxelems; }
  void reserve(size_type new_capacity) {
     
     
     
    if (new_capacity == 0) {
      destroy(elems, elems+nelems);
      if (elems) deallocate(elems,maxelems);
      elems = 0;
      maxelems = 0;
    } else if (new_capacity != maxelems && new_capacity >= nelems) {
      T* new_elems = allocate_and_copy(new_capacity, elems, elems+nelems);
      destroy(elems, elems+nelems);
      if (elems) deallocate(elems,maxelems);
      elems = new_elems;
      maxelems = new_capacity;
    }
  }
   
  void swap (self_type& y) {
     
     
    T* tmp = y.elems; y.elems = elems; elems = tmp;
    size_type ntmp = y.nelems; y.nelems = nelems; nelems = ntmp;
    size_type maxtmp = y.maxelems; y.maxelems = maxelems; maxelems = maxtmp;
  }
};

 

template <class size_type>
struct no_check {
  static void range_check (size_type, size_type) {}
};

template <class size_type>
struct exception_check {
  static void range_check (size_type i, size_type size) {
    if (i >= size)
      throw std::range_error("policy_vector: range error in checking\n");
  }
};

template <class size_type>
struct assert_check {
  static void range_check (size_type i, size_type size) {
    (static_cast<void>  (( i < size ) ? 0 :	(__assert_fail ("i < size" , "policy_vector.hpp", 675, __PRETTY_FUNCTION__ ), 0))) ;
  }
};


 

 

template <class T, class V>
class raw_pointer_iterator {
protected:
   
   
  typedef T*                     iterator;
  typedef const T*               const_iterator;
  typedef std::size_t            size_type;
  typedef std::ptrdiff_t         difference_type;
   
  static iterator
  make_iterator(V* v, size_type n) {
    return (&v->front())+n;
  }
  static const_iterator
  make_const_iterator(const V* v, size_type n) {
    return (&v->front())+n;
  }
};

 
 

# 1 "pointer_iterator_wrapper.hpp" 1
 













template <class Pointer, class V, bool Check>
struct pointer_iterator_wrapper_base
{
  pointer_iterator_wrapper_base(V*) {}
  Pointer base() const { return Pointer(); }
  Pointer check(Pointer ptr) const {}
};

template <class Pointer, class V>
struct pointer_iterator_wrapper_base<Pointer, V, true>
{
  pointer_iterator_wrapper_base(V* v) : m_v(v) {}
  Pointer base() const { return Pointer(&m_v->front()); }
  Pointer check(Pointer ptr) const {
    m_v->range_check(ptr-base(), m_v->size());
    return ptr;
  }
private:
  V* m_v; 
};

template <class V, class T, class Pointer, class Reference, bool Check>
struct pointer_iterator_wrapper : pointer_iterator_wrapper_base<Pointer,V,Check>
{
  typedef T                                      value_type;
  typedef Pointer                                pointer;
  typedef Reference                              reference;
  typedef std::ptrdiff_t                         difference_type;
  typedef std::random_access_iterator_tag        iterator_category;
protected:
  typedef pointer_iterator_wrapper_base<Pointer,V,Check>  base_type;
  typedef pointer_iterator_wrapper<V,T,Pointer,Reference,Check> self_type;
  typedef difference_type                        index_t;
public:
  pointer_iterator_wrapper() : base_type(0), m_ptr(0) {}
  pointer_iterator_wrapper(V* v, index_t n) : base_type(v), m_ptr(base()+n) {}
  pointer_iterator_wrapper(V* v, Pointer ptr) : base_type(v), m_ptr(ptr) {}
  pointer_iterator_wrapper(self_type const& w) : base_type(w), m_ptr(w.m_ptr) {}
   
  Reference operator*()  const {
    return *base_type::check(m_ptr);
  }
  Pointer   operator->() const {
    return base_type::check(m_ptr);
  }
  Reference operator[](std::size_t n) const {
    return base_type::check(m_ptr[n]);
  }
  self_type& operator++() {
    ++m_ptr; return *this;
  }
  self_type operator++(int) {
    self_type old(*this); ++m_ptr; return *old;
  }
  self_type& operator--() {
    --m_ptr; return *this;
  }
  self_type& operator--(int) {
    self_type old(*this); --m_ptr; return *old;
  }
  self_type& operator+=(difference_type n) {
    m_ptr += n; return *this;
  }
  self_type& operator-=(difference_type n) {
    m_ptr -= n; return *this;
  }

   
  self_type operator+(difference_type const& rhs) const {
    return self_type(*this) += rhs;
  }
  self_type operator-(difference_type const& rhs) const {
    return self_type(*this) -= rhs;
  }
  difference_type operator-(self_type const& rhs) const {
     
    return m_ptr - rhs.m_ptr;
  }
  bool operator==(self_type  const& rhs) const {
     
    return m_ptr == rhs.m_ptr;
  }
  bool operator!=(self_type const& rhs) const {
     
    return !(m_ptr == rhs.m_ptr);
  }
  bool operator<(self_type const& rhs) const {
     
    return m_ptr < rhs.m_ptr;
  }
  bool operator<=(self_type const& rhs) const {
     
    return !(rhs.m_ptr < m_ptr);
  }
  bool operator>(self_type const& rhs) const {
     
    return rhs.m_ptr < m_ptr;
  }
  bool operator>=(self_type const& rhs) const {
     
    return !(m_ptr < rhs.m_ptr);
  }
# 155 "pointer_iterator_wrapper.hpp"

private:
  Pointer m_ptr;
};



# 707 "policy_vector.hpp" 2


template <class T, class V>
class pointer_iterator {
protected:
   
   
  typedef pointer_iterator_wrapper<V,T,T*,T&,false>     iterator;
  typedef pointer_iterator_wrapper<const V,T,const T*,const T&,false>
                                                        const_iterator;
  typedef std::size_t                                   size_type;
  typedef std::ptrdiff_t                                difference_type;
   
  static iterator
  make_iterator(V* v, size_type n) {
    return iterator(v,n);
  }
  static const_iterator
  make_const_iterator(const V* v, size_type n) {
    return const_iterator(v,n);
  }
};

template <class T, class V>
class checked_pointer_iterator {
protected:
   
   
  typedef pointer_iterator_wrapper<V,T,T*,T&,true>      iterator;
  typedef pointer_iterator_wrapper<const V,T,const T*,const T&,true>
                                                        const_iterator;
  typedef std::size_t                                   size_type;
  typedef std::ptrdiff_t                                difference_type;
   
  static iterator
  make_iterator(V* v, size_type n) {
    return iterator(v,n);
  }
  static const_iterator
  make_const_iterator(const V* v, size_type n) {
    return const_iterator(v,n);
  }
};

 
 

# 1 "index_iterator_wrapper.hpp" 1
 













template <class V, class T, class Pointer, class Reference, bool Check>
struct index_iterator_wrapper
{
  typedef T                                      value_type;
  typedef Pointer                                pointer;
  typedef Reference                              reference;
  typedef std::ptrdiff_t                         difference_type;
  typedef std::random_access_iterator_tag        iterator_category;
protected:
  typedef index_iterator_wrapper<V,T,Pointer,Reference,Check> self_type;
  typedef std::size_t                            size_type;
public:
  index_iterator_wrapper() : _M_v(0), _M_n(0) {}
  index_iterator_wrapper(V* v, size_type n) : _M_v(v), _M_n(n) {}
  index_iterator_wrapper(self_type const& w)
    : _M_v(w._M_v), _M_n(w._M_n) {}
   
  Reference operator*()  const { check(); return *address(); }
  Pointer   operator->() const { check(); return address(); }
  Reference operator[](std::size_t n) const { check(n); return address()[n]; }
  self_type& operator++() {
    ++_M_n; return *this;
  }
  self_type operator++(int) {
    self_type old(*this); ++_M_n; return *old;
  }
  self_type& operator--() {
    --_M_n; return *this;
  }
  self_type& operator--(int) {
    self_type old(*this); --_M_n; return *old;
  }
  self_type& operator+=(difference_type n) {
    _M_n += n; return *this;
  }
  self_type& operator-=(difference_type n) {
    _M_n -= n; return *this;
  }

   
  self_type operator+(difference_type const& rhs) const {
    return self_type(*this) += rhs;
  }
  self_type operator-(difference_type const& rhs) const {
    return self_type(*this) -= rhs;
  }
  difference_type operator-(self_type const& rhs) const {
     
    return _M_n - rhs._M_n;
  }
  bool operator==(self_type  const& rhs) const {
     
    return _M_n == rhs._M_n;
  }
  bool operator!=(self_type const& rhs) const {
     
    return !(_M_n == rhs._M_n);
  }
  bool operator<(self_type const& rhs) const {
     
    return _M_n < rhs._M_n;
  }
  bool operator<=(self_type const& rhs) const {
     
    return !(rhs._M_n < _M_n);
  }
  bool operator>(self_type const& rhs) const {
     
    return rhs._M_n < _M_n;
  }
  bool operator>=(self_type const& rhs) const {
     
    return !(_M_n < rhs._M_n);
  }
# 137 "index_iterator_wrapper.hpp"

protected:
  Pointer address() const { return (&_M_v->front()) + _M_n; }
   
   
   
   
   
  void check() const {
    if (Check) _M_v->range_check(_M_n, _M_v->size());
  }
  void check(difference_type n) const {
    if (Check) _M_v->range_check(_M_n+n, _M_v->size());
  }
private:
  V* _M_v; 
  size_type _M_n;
};



# 754 "policy_vector.hpp" 2


template <class T, class V>
class index_iterator {
protected:
   
   
   
   
  typedef index_iterator_wrapper<V,T,T*,T&,false>      iterator;
  typedef index_iterator_wrapper<const V,T,const T*,const T&,false>
                                                       const_iterator;
  typedef std::size_t                                  size_type;
  typedef std::ptrdiff_t                               difference_type;
   
  static iterator
  make_iterator(V* v, size_type n) {
    return iterator(v,n);
  }
  static const_iterator
  make_const_iterator(const V* v, size_type n) {
    return const_iterator(v,n);
  }
};

template <class T, class V>
class checked_index_iterator {
protected:
   
   
  typedef index_iterator_wrapper<V,T,T*,T&,true>      iterator;
  typedef index_iterator_wrapper<const V,T,const T*,const T&,true>
                                                       const_iterator;
  typedef typename iterator::size_type                 size_type;
  typedef typename iterator::difference_type           difference_type;
   
  static iterator
  make_iterator(V* v, size_type n) {
    return iterator(v,n);
  }
  static const_iterator
  make_const_iterator(const V* v, size_type n) {
    return const_iterator(v,n);
  }
};

 
 

template <class T, class V>
class iterator_adaptor;

 
 
 
 
 
 
 
 
 

template <class T, class Alloc,
  class Iterator, class ConstIterator,
  template <class S> class CheckingPolicy,
  template <class T2, class V2> class IteratorPolicy >
class iterator_adaptor<T,
	policy_vector<T, Alloc,
	              fixed_external_range<Iterator,ConstIterator>,
	              CheckingPolicy,
		      IteratorPolicy>
        >
{
private:
  typedef fixed_external_range<Iterator,ConstIterator>       storage_policy;
  typedef policy_vector<T, Alloc,
	                fixed_external_range<Iterator,ConstIterator>,
	                CheckingPolicy,
		        IteratorPolicy>                      V;

protected:
   
  typedef pointer_iterator_wrapper<V,T,Iterator,T&,false>     iterator;
  typedef pointer_iterator_wrapper<const V,T,ConstIterator,const T&,false>
                                                             const_iterator;
  typedef typename storage_policy::size_type                 size_type;
  typedef typename iterator::difference_type                 difference_type;
   
  static iterator
  make_iterator(V* v, size_type n) {
    return iterator(v,n);
  }
  static const_iterator
  make_const_iterator(const V* v, size_type n) {
    return const_iterator(v,n);
  }
};

 

template <class T, class V>
class checked_iterator_adaptor;

template <class T, class Alloc,
  class Iterator, class ConstIterator,
  template <class S> class CheckingPolicy,
  template <class T2, class V2> class IteratorPolicy >
class checked_iterator_adaptor<T,
	policy_vector<T, Alloc,
	              fixed_external_range<Iterator,ConstIterator>,
	              CheckingPolicy,
		      IteratorPolicy>
        >
{
private:
  typedef fixed_external_range<Iterator,ConstIterator>       storage_policy;
  typedef policy_vector<T, Alloc,
	                fixed_external_range<Iterator,ConstIterator>,
	                CheckingPolicy,
		        IteratorPolicy>                      V;

protected:
   
  typedef pointer_iterator_wrapper<V,T,Iterator,T&,true>     iterator;
  typedef pointer_iterator_wrapper<const V,T,ConstIterator,const T&,true>
                                                             const_iterator;
  typedef typename storage_policy::size_type                 size_type;
  typedef typename iterator::difference_type                 difference_type;
   
  static iterator
  make_iterator(V* v, size_type n) {
    return iterator(v,n);
  }
  static const_iterator
  make_const_iterator(const V* v, size_type n) {
    return const_iterator(v,n);
  }
};

 

template <
  class T,
  class Alloc = std::allocator<T>,
  class StoragePolicy = variable_allocated_array<multiply_resize<2>, 0>,
  template <class S> class CheckingPolicy = no_check,
  template <class T2, class V2> class IteratorPolicy = raw_pointer_iterator
  > 
class policy_vector
  : public policy_vector_storage<T,Alloc,StoragePolicy>
  , public CheckingPolicy<std::size_t>
  , public IteratorPolicy<T, 
             policy_vector<T,Alloc,StoragePolicy,CheckingPolicy,IteratorPolicy>
           >
{
  typedef policy_vector_storage<T,Alloc,StoragePolicy>    storage;
  typedef policy_vector<T,Alloc,StoragePolicy,CheckingPolicy,IteratorPolicy>
                                                          self_type;
public:
  
  typedef StoragePolicy                                   storage_policy;
  typedef CheckingPolicy<std::size_t>                     checking_policy;
  typedef IteratorPolicy<T,self_type>                     iterator_policy;
   
  typedef Alloc                                           allocator_type;
  typedef T                                               value_type;
  typedef typename allocator_type::pointer                pointer;
  typedef typename allocator_type::const_pointer          const_pointer;
  typedef typename allocator_type::reference              reference;
  typedef typename allocator_type::const_reference        const_reference;
  typedef typename iterator_policy::iterator              iterator;
  typedef typename iterator_policy::const_iterator        const_iterator;
  typedef typename iterator_policy::size_type             size_type;
  typedef typename iterator_policy::difference_type       difference_type;

   
  explicit policy_vector(const allocator_type& a = allocator_type(),
                         const storage_policy& sp = storage_policy(),
                         const checking_policy& cp = checking_policy(),
                         const iterator_policy& ip = iterator_policy())
    : storage(a, sp), checking_policy(cp), iterator_policy(ip) {}
  policy_vector(size_type n, const T& value,
                         const allocator_type& a = allocator_type(),
                         const storage_policy& sp = storage_policy(),
                         const checking_policy& cp = checking_policy(),
                         const iterator_policy& ip = iterator_policy())
    : storage(n, value, a, sp), checking_policy(cp), iterator_policy(ip) {}
  explicit policy_vector(size_type n,
                         const allocator_type& a = allocator_type(),
                         const storage_policy& sp = storage_policy(),
                         const checking_policy& cp = checking_policy(),
                         const iterator_policy& ip = iterator_policy())
    : storage(n, a, sp), checking_policy(cp), iterator_policy(ip) {}
  template <class Iterator>
  policy_vector(Iterator first, Iterator last,
                         const allocator_type& a = allocator_type(),
                         const storage_policy& sp = storage_policy(),
                         const checking_policy& cp = checking_policy(),
                         const iterator_policy& ip = iterator_policy())
    : storage(first, last, a, sp), checking_policy(cp), iterator_policy(ip) {}

   
  template <class T2>
  policy_vector(T2 (&rhs)[storage_policy::static_size],
                         const allocator_type& a = allocator_type(),
                         const storage_policy& sp = storage_policy(),
                         const checking_policy& cp = checking_policy(),
                         const iterator_policy& ip = iterator_policy())
    : storage(rhs, a, sp), checking_policy(cp), iterator_policy(ip) {}

   
  policy_vector(const policy_vector &rhs)
    : storage(rhs), checking_policy(rhs), iterator_policy(rhs)
    { assign(rhs.begin(), rhs.end()); }
  template < class T2,
	     class Alloc2,
             class StoragePolicy2,
             template <class S> class CheckingPolicy2,
             template <class T3, class V3> class IteratorPolicy2 >
  explicit policy_vector(
             policy_vector<T2,Alloc2,
                           StoragePolicy2,CheckingPolicy2,IteratorPolicy2>
	     const& rhs)
    : storage(rhs), checking_policy(rhs), iterator_policy(rhs)
    { assign(rhs.begin(), rhs.end()); }

   
   
  template <class T2>
  policy_vector& operator= (T2 (&rhs)[storage_policy::static_size]) {
    assign(rhs, rhs + storage_policy::static_size);
    return *this;
  }

   
  template < class T2,
	     class Alloc2,
             class StoragePolicy2,
             template <class S> class CheckingPolicy2,
             template <class T3, class V3> class IteratorPolicy2 >
  policy_vector& operator=
    (const policy_vector<T2,Alloc2,
                         StoragePolicy2,CheckingPolicy2,IteratorPolicy2>& rhs)
  {
    storage() = rhs;
    checking_policy() = rhs;
    iterator_policy() = rhs;
    return *this;
  }

   
  iterator begin() { return make_iterator(this,0); }
  const_iterator begin() const { return make_const_iterator(this,0); }
  iterator end() { return make_iterator(this,size()); }
  const_iterator end() const { return make_const_iterator(this,size()); }

   
   

   
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
      return const_reverse_iterator(end());
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
      return const_reverse_iterator(begin());
  }

   
  reference operator[](size_type i) {
    range_check(i, size());
    return elems[i];
  }
  const_reference operator[](size_type i) const {
    range_check(i, size());
    return elems[i];
  }

   
  reference at(size_type i) {
    if (i >= size()) { throw std::range_error("policy_vector: range error in at()\n"); }
    return elems[i];
  }
  const_reference at(size_type i) const {
    if (i >= size()) { throw std::range_error("policy_vector: range error in at()\n"); }
    return elems[i];
  }

   
  reference front() { return elems[0]; }
  const_reference front() const { return elems[0]; }
  reference back() { return elems[size()-1]; }
  const_reference back() const { return elems[size()-1]; }


   
  bool operator==(const policy_vector& y) const {
    return (size() == y.size()) && std::equal(elems, elems+size(), y.elems);
  }
  bool operator!=(const policy_vector& y) const {
    return !(*this == y);
  }
  bool operator<(const policy_vector& y) const {
    return std::lexicographical_compare(elems, elems+size(),
                                        y.elems, y.elems+y.size());
  }
  bool operator>(const policy_vector& y) const {
    return y < *this;
  }
  bool operator<=(const policy_vector& y) const {
    return !(y < *this);
  }
  bool operator>=(const policy_vector& y) const {
      return !(*this < y);
  }
# 1100 "policy_vector.hpp"


   
  friend inline void swap (policy_vector& x, policy_vector& y) {
    x.swap(y);
  }

   

   
  void insert(iterator pos, const T& x) {
    size_type index = pos - begin();
    if (index != nelems) {
      size_type new_capacity;
      if (resize_before_insert(size(), capacity(), 1, new_capacity))
        reserve(new_capacity);
      T* last = elems+nelems;
      std::copy_backward( elems+index, last, last+1);
      pos = begin()+index;
    }
    ++nelems;
    *pos = x;
  }
  template <class Iterator>
  void insert(iterator pos, Iterator first, Iterator last) {
    size_type n = std::distance(first, last);
    size_type index = pos - begin();
    if (index != nelems) {
      size_type new_capacity;
      if (resize_before_insert(size(), capacity(), n, new_capacity)) {
        reserve(new_capacity);
        pos = begin()+index;
      }
      T* last = elems+nelems;
      std::copy_backward( elems+index, last, last+n);
    }
    nelems += n;
    std::copy(first, last, pos);
  }
  void insert(iterator pos, size_type n, const T& x) {
    size_type index = pos - begin();
    if (index != nelems) {
      size_type new_capacity;
      if (resize_before_insert(size(), capacity(), n, new_capacity))
        reserve(new_capacity);
      T* last = elems+nelems;
      std::copy_backward( elems+index, last, last+n);
      pos = begin()+index;
    }
    nelems += n;
    std::fill_n(pos, n, x);
  }
  
   
  iterator erase(iterator position) {
    T* pos = &*position;
    std::copy( pos+1, elems+nelems, pos);
    --nelems;
    destroy(elems+nelems);
    size_type new_capacity;
    if (resize_after_erase(size(), capacity(), new_capacity)) {
      size_type index = pos-elems;
      reserve(new_capacity);
      position = begin()+index;
    }
    return position;
  }
  iterator erase(iterator first, iterator last) {
    size_type n = last-first;
    if (n == 0) return first;
    T* start = &*first;  
    T* finish = start+n;  
    T* new_end = std::copy(finish, elems+nelems, start);
    destroy(new_end, elems+nelems);
    nelems -= n;
    size_type new_capacity;
    if (resize_after_erase(size(), capacity(), new_capacity)) {
      size_type index = start-elems;
      reserve(new_capacity);
      first = begin()+index;
    }
    return first;
  }
  
   
  void push_back(T const& x) {
    size_type new_capacity;
    if (resize_before_insert(size(), capacity(), 1, new_capacity))
      reserve(new_capacity);
    construct(elems+nelems, x);
    ++nelems;
  }
  void pop_back()  {
    --nelems;
    destroy(elems+nelems);
    size_type new_capacity;
    if (resize_after_erase(size(), capacity(), new_capacity))
      reserve(new_capacity);
  }

   

   
  void resize(size_type n, const T& x) {
    if (n > size())
      insert(end(), n-size(), x);
    else
      erase(begin()+n, end());
  }
  void resize(size_type n) {
    if (n > size())
      insert(end(), n-size(), T());
    else
      erase(begin()+n, end());
  }

   
  void clear() {
    destroy(elems, elems+nelems);
    nelems = 0;
    size_type new_capacity;
    if (resize_after_erase(size(), capacity(), new_capacity))
      reserve(new_capacity);
  }


};

}  



# 8 "testing_vector.cpp" 2


using namespace boost;

 
const std::size_t N = 100000;   
const std::size_t initN = 0;     
 

template <class T>
void ignore_unused_variable_warning(T&) {}


typedef float elem_t;




 


 
template <typename T>
void test_type() {}

template <class Array>
void test_array(const Array &a, bool checking, bool iter_checking)
{
   
  test_type<typename Array::value_type>(); 
  test_type<typename Array::reference>(); 
  test_type<typename Array::const_reference>(); 
  test_type<typename Array::iterator>(); 
  test_type<typename Array::const_iterator>(); 
  test_type<typename Array::size_type>(); 
  test_type<typename Array::difference_type>(); 

   
  typedef typename Array::value_type     value_type;
  typedef typename Array::iterator       iterator;
  typedef typename Array::const_iterator const_iterator;
  
   
   
  const_iterator first(a.begin()), last(a.end()), pos;
  boost::timer t;
  float sum = 0;
  for (pos = first; pos != last; ++pos) sum += *pos;
  std::cout << "\ttraversal:     " << t.elapsed() << "s\n";

   
   
  typedef typename Array::reverse_iterator       reverse_iterator;
  typedef typename Array::const_reverse_iterator const_reverse_iterator;
  const_reverse_iterator rfirst(a.rbegin()), rlast(a.rend()), rpos;
  boost::timer t1;
  float rsum = 0;
  for (rpos = rfirst; rpos != rlast; ++rpos) rsum += *rpos;
  std::cout << "\trtraversal:    " << t1.elapsed() << "s\n";

   
  std::size_t n = 0;
  for (pos = first; pos != last; ++pos) ++n;
  if (n == a.size())
    std::cout << "\tsize is OK\n";
  else
    std::cout << "\tsize is BROKEN\n";

   
  if (a==a)
    std::cout << "\tequality comparison is OK\n";
  else 
    std::cout << "\tequality comparison is BROKEN\n";

   
  Array b(a);
  Array c; c = a;
  if (a==b && a==c)
    std::cout << "\tcopy construction and copy assignment are OK\n";
  else
    std::cout << "\tcopy construction and copy assignment are BROKEN\n";

   
   
  b.assign(b.size(), (value_type)42);
  iterator bfirst(b.begin()), blast(b.end()), bpos;
  for (bpos=bfirst; bpos != blast; ++bpos)
      if (*bpos != 42) {
         std::cout << "\tassign value is BROKEN\n";
	 break;
      }
  if (bpos == blast)
    std::cout << "\tassign value is OK\n";

   
   
  c.assign(b.begin(), b.end());
  reverse_iterator crfirst(c.rbegin()), crlast(c.rend()), crpos;
  for (crpos=crfirst; crpos != crlast; ++crpos)
      if (*crpos != 42) {
         std::cout << "\tassign range is BROKEN\n";
	 break;
      }
  if (crpos == crlast)
    std::cout << "\tassign range is OK\n";

   
  Array d(a);
  boost::timer t2;
  b.swap(d);
  std::cout << "\tswap:          " << t2.elapsed() << "s\n";
  if (b != a)
    std::cout << "\tswap is BROKEN\n";
  else
    std::cout << "\tswap is OK\n";

   
  bool caught = false;

   
   
  try {
    value_type x = a.at(a.size());
    ignore_unused_variable_warning(x);
  }
  catch (std::range_error e) {
    std::cout << "\tat() correctly caught: " << e.what();
    caught = true;
  }
  if (!caught)
    std::cout << "\tat() did not catch any exception\n";


   
  if (!checking) return;

   
  caught = false;
  try {
    value_type x = a[a.size()];
    ignore_unused_variable_warning(x);
  }
  catch (std::range_error e) {
    std::cout << "\toperator[] correctly caught: " << e.what();
    caught = true;
  }
  if (!caught)
    std::cout << "\toperator[] did not catch exception\n";

   
  if (!iter_checking) return;

   
  caught = false;
  try {
    value_type x = *a.end();
    ignore_unused_variable_warning(x);
  }
  catch (std::range_error e) {
    std::cout << "\titerator correctly caught: " << e.what();
    caught = true;
  }
  if (!caught)
    std::cout << "\titerator did not catch exception\n";
}

template <class StoragePolicy, class Array>
void test_fixed_size_array(StoragePolicy sp, bool checking, bool iter_checking)
{






   
   
  float data[N];
  for (std::size_t i=0; i<N; ++i) data[i] = i;    
  const Array a( data, data+N, typename Array::allocator_type(), sp );

  if (!std::equal(data, data+a.size(), a.begin()))
    std::cout << "\tconversion from plain array BROKEN\n";
  else
    std::cout << "\tconversion from plain array OK\n";

   
  test_array<Array>(a, checking, iter_checking);
}

template <class Array>
void test_variable_size_array(bool checking, bool iter_checking)
{
  typedef typename Array::value_type     value_type;
  typedef typename Array::iterator       iterator;
  const float tmp[4] = { -0.8, -0.6, -0.4, -0.2 };

   
  Array a;
  if (a.size() != 0)
    std::cout << "\tdefault constructor BROKEN\n";
  else
    std::cout << "\tdefault constructor OK\n";

   
  {
    Array b(100);
    if (b.size() != 100)
      std::cout << "\tsize constructor BROKEN\n";
    Array c(tmp, tmp+4);
    if (c.size() != 4 || !std::equal(tmp, tmp+4, c.begin()))
      std::cout << "\trange constructor BROKEN\n";
    if (b.size() == 100 && c.size() == 4 && std::equal(tmp, tmp+4, c.begin()))
      std::cout << "\trange and size constructors OK\n";
  }

   
  const std::size_t N2 = 2*N;
  a.reserve(N);  
  boost::timer t;
  for (std::size_t i=0; i<N2; ++i)
    a.push_back((float)i);
  for (std::size_t i=0; i<N; ++i)
    a.pop_back();
  if (a.size() != N)
    std::cout << "\tpush/pop_back are BROKEN\n";
  else 
    std::cout << "\tpush/pop_back: " << t.elapsed() << "s\n";

   
   
  std::cout << "\tcapacity:      " << a.capacity() << "\n";;
  std::cout << "\tsize:          " << a.size() << "\n";;

   
  test_array<Array>(a, checking, iter_checking);

   
  iterator first, last, pos;
  bool insert_broken_reported = false;
  a.insert(a.begin(), -2.0);
  pos = first = a.begin()+1; last = a.end();
  try {
  for (std::size_t i=0; pos != last; ++pos, ++i)
    if (*pos != i) break;
  if (a[0] != -2.0 || pos != last) {
    std::cout << "\tinsert one element is BROKEN\n";
    insert_broken_reported = true;
  }
  }
  catch (std::range_error e) {
    std::cout << "\tcaught exception during insert one element: " << e.what();
  }
  a.insert(a.begin()+1, 3, -1.0);
  pos = first = a.begin()+4; last = a.end();
  try{
  for (std::size_t i=0; pos != last; ++pos, ++i)
    if (*pos != i) break;
  if (a[0] != -2.0 || a[1] != -1.0 || a[2] != -1.0 || a[3] != -1.0
   || pos != last) {
    std::cout << "\tinsert n copies of one element is BROKEN\n";
    insert_broken_reported = true;
  }
  }
  catch (std::range_error e) {
    std::cout << "\tcaught exception during insert n copies: " << e.what();
  }
  a.insert(a.begin()+4, tmp, tmp+4);
  pos = first = a.begin()+8; last = a.end();
  try{
  for (std::size_t i=0; pos != last; ++pos, ++i)
    if (*pos != i) break;
  if (a[0] != -2.0 || a[1] != -1.0 || a[2] != -1.0 || a[3] != -1.0
   || a[4] != tmp[0] || a[5] != tmp[1] || a[6] != tmp[2] || a[7] != tmp[3]
   || pos != last) {
    std::cout << "\tinsert range is BROKEN\n";
    insert_broken_reported = true;
  }
  }
  catch (std::range_error e) {
    std::cout << "\tcaught exception during insert range: " << e.what();
  }
  if (!insert_broken_reported)
    std::cout << "\tinsert functions are OK\n";

   
  a.erase(a.begin());
  try{
  if (a[0] != -1.0 || a[1] != -1.0 || a[2] != -1.0
   || a[3] != tmp[0] || a[4] != tmp[1] || a[5] != tmp[2] || a[6] != tmp[3])
       std::cout << "\terase position is BROKEN\n";
  else {
    pos = a.begin()+7; last = a.end();
    for (std::size_t i=0; pos != last; ++pos, ++i)
      if (*pos != i) {
        std::cout << "\terase position is BROKEN\n";
        break;
      }
  }
  }
  catch (std::range_error e) {
    std::cout << "\tcaught exception during erase position: " << e.what();
  }
  a.erase(a.begin(), a.begin()+7);
  pos = a.begin(); last = a.end();
  try{
  for (std::size_t i=0; pos != last; ++pos, ++i)
    if (*pos != i) {
       std::cout << "\terase range is BROKEN\n";
       break;
    }
  }
  catch (std::range_error e) {
    std::cout << "\tcaught exception during erase range: " << e.what();
  }
  if (pos == last)
    std::cout << "\terase functions are OK\n";

  a.resize(a.size()+4, (value_type)89);
  a.resize(a.size()-3);
  if (a.back() != 89)
    std::cout << "\tresize is BROKEN, ";
  else
    std::cout << "\tresize is OK, ";

  a.clear();
  if (a.size() != 0)
    std::cout << "clear is BROKEN\n";
  else
    std::cout << "clear is OK\n";
}

 



template <class StoragePolicy,
	  template <class S> class CheckingPolicy,
	  template <class T, class V> class IteratorPolicy>
void test_fixed_size_policy(StoragePolicy sp, bool checking, bool iter_checking)
{
  using namespace boost;
   
  typedef policy_vector<float, std::allocator<float>,
            StoragePolicy, CheckingPolicy, IteratorPolicy> Array;
   
  test_fixed_size_array<StoragePolicy,Array>(sp, checking, iter_checking);
}

template <class StoragePolicy>
void test_fixed_size( StoragePolicy sp = StoragePolicy() )
{
  using namespace boost;
  std::cout << "    a. RAW POINTER ITERATOR, NO CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    no_check, raw_pointer_iterator>(sp, false, false);
  std::cout << "    b. RAW POINTER ITERATOR, EXCEPTION CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    exception_check, raw_pointer_iterator>(sp, true, false);
  std::cout << "    c. CHECKED POINTER ITERATOR, NO CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    no_check, checked_pointer_iterator>(sp, false, false);
  std::cout << "    d. CHECKED POINTER ITERATOR, EXCEPTION CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    exception_check, checked_pointer_iterator>(sp, true, true);
  std::cout << "    e. CHECKED POINTER ITERATOR, ASSERT CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    assert_check, checked_pointer_iterator>(sp, false, false);
   
  std::cout << "    f. INDEX ITERATOR, NO CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    no_check, index_iterator>(sp, false, false);
  std::cout << "    g. INDEX ITERATOR, EXCEPTION CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    exception_check, index_iterator>(sp, true, true);
  std::cout << "    h. INDEX ITERATOR, ASSERT CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    assert_check, index_iterator>(sp, false, false);
   
}


 



template <class StoragePolicy,
	  template <class S> class CheckingPolicy,
	  template <class T, class V> class IteratorPolicy>
void test_variable_size_policy(bool checking, bool iter_checking)
{
  using namespace boost;
   
  typedef policy_vector<float, std::allocator<float>,
            StoragePolicy, CheckingPolicy, IteratorPolicy> Array;
   
  test_variable_size_array<Array>(checking, iter_checking);
}

template <class StoragePolicy>
void test_variable_size()
{
  using namespace boost;
# 438 "testing_vector.cpp"

  std::cout << "    a. CHECKED ITERATOR ADAPTOR, NO CHECKING\n";
  test_variable_size_policy<StoragePolicy,
    no_check, checked_iterator>(false, false);
  std::cout << "    b. CHECKED ITERATOR ADAPTOR, EXCEPTION CHECKING\n";
  test_variable_size_policy<StoragePolicy,
    exception_check, checked_iterator_adaptor>(true, true);
  std::cout << "    c. CHECKED ITERATOR ADAPTOR, ASSERT CHECKING\n";
  test_variable_size_policy<StoragePolicy,
    assert_check, checked_iterator_adaptor>(false, false);
   

}

 



 
typedef boost::incremental_resize<1024>      i2E10;
typedef boost::incremental_resize<16536>     i2E16;
typedef boost::multiply_resize<2>            m2;
typedef boost::multiply_divide_resize<2>     md2;
typedef boost::multiply_resize<4>            m4;
typedef boost::multiply_divide_resize<4>     md4;

 
typedef boost::fixed_aggregate_array<N>              array;
typedef boost::fixed_allocated_array<N>              fixed;
typedef boost::fixed_external_static_array<elem_t,N> external;
typedef boost::fixed_external_range<elem_t*,elem_t const*> range;
typedef boost::variable_allocated_array<i2E10,initN> vari2E10;  
typedef boost::variable_allocated_array<i2E16,initN> vari2E16;  
typedef boost::variable_allocated_array<m2,initN>    varm2;
typedef boost::variable_allocated_array<m4,initN>    varm4;
typedef boost::variable_allocated_array<md2,initN>   varmd2;
typedef boost::variable_allocated_array<md4,initN>   varmd4;

inline void separator()
{
  std::cout << "==========================================================================\n";
}

int main()
{


























  separator();
  elem_t *range_data = new elem_t[N+10];
  std::cout <<
    "2.2 TESTING FIXED EXTERNAL RANGE\n";
  test_fixed_size< range >( range(range_data+10, range_data+N+10) );
  delete[] range_data;
  separator();


# 528 "testing_vector.cpp"


# 540 "testing_vector.cpp"


# 552 "testing_vector.cpp"









  return 0;   
}

