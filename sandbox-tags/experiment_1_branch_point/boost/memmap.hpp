// (C) Copyright Craig Henderson 2002
//               cdm.henderson@virgin.net
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  The author makes no representations
// about the suitability of this software for any purpose.  It is
// provided "as is" without express or implied warranty.
//
// Thanks to Malte Starostik for POSIX 64 bit support changes
//
//
// Synopsis:
// --------
//
//  filesize_t get_file_size(file_handle_t &handle);
//  template<typename T> class memory_mapped_file
//  template<typename T> class file


#if !defined(BOOST_MMAP_POSIX)  &&  !defined(BOOST_MMAP_WINDOWS)
#error No memory map configuration defined: BOOST_MMAP_POSIX or BOOST_MMAP_WINDOWS
#endif

#include <sys/stat.h>
#include "boost/config.hpp"
#include "boost/utility.hpp"

namespace boost {

#if defined(BOOST_MMAP_WINDOWS)
        typedef HANDLE file_handle_t;
        typedef DWORD  protection_t;
        typedef struct flags_or_security_
        {
            DWORD                 access;
            LPSECURITY_ATTRIBUTES security;
        }   flags_or_security_t;
#ifdef _USE_INT64
        typedef UINT64 filesize_t;
        typedef struct offset_
        {
            DWORD hi, lo;
        } offset_t;
#else
        typedef DWORD offset_t;
        typedef DWORD filesize_t;
#endif // _USE_INT64

        typedef struct length_
        {
            filesize_t  map;
            SIZE_T      view;
        } length_t;

        typedef struct detail_struct_
        {
            file_handle_t file_mapping_handle_;
        } detail_struct;
        typedef DWORD err_t;

        #define BOOST_FS_INVALID_HANDLE INVALID_HANDLE_VALUE
#elif defined(BOOST_MMAP_POSIX)
        typedef int   file_handle_t;
        typedef int   protection_t;
        typedef int   flags_or_security_t;
        typedef off_t length_t;
        typedef off_t filesize_t;
        typedef off_t offset_t;

        #define BOOST_FS_INVALID_HANDLE -1

        typedef struct detail_struct_
        {
            int len;
        } detail_struct;
        typedef int err_t;
#endif



    // returns the size of an open file
    filesize_t get_file_size(file_handle_t handle)
    {
#if defined(BOOST_MMAP_POSIX)
        struct stat info;
        if (::fstat(handle, &info) == -1)
            return 0;
        return info.st_size;
#elif defined(BOOST_MMAP_WINDOWS)
        union
        {
            filesize_t len;
            struct
            {
                DWORD lo;
                DWORD hi;
            } i64;
        } u;
        u.i64.lo = ::GetFileSize(handle, &u.i64.hi);
        return u.len;
#endif
    }


    typedef enum file_access_ { readonly, readwrite } file_access;


    class file_already_attached : public std::exception
    {
      public:
        virtual char const * what() const throw()
        {
            return "boost::file_already_attached";
        }
    };

    #if defined(BOOST_MSVC)
    #   define THROWS_ALREADY_ATTACHED
    #else
    #   define THROWS_ALREADY_ATTACHED throw(file_already_attached)
    #endif


    template<typename T=char>
    class file : noncopyable
    {
      private:
        err_t                err_;
        file_handle_t        handle_;
        std::basic_string<T> filepath_;

      public:
        file() throw();
        file(const std::basic_string<T> &filepath, file_access access) throw();
        ~file() throw();

        bool close(void) throw();
        bool create(const std::basic_string<T> &filepath) THROWS_ALREADY_ATTACHED;

        err_t                error(void)    const throw() { return err_;                                }
        std::basic_string<T> filepath(void) const throw() { return filepath_;                           }
        file_handle_t        handle(void)   const throw() { return handle_;                             }
        filesize_t           size(void)     const throw() { return get_file_size(handle_);              }
        bool                 is_open(void)  const throw() { return handle_ != BOOST_FS_INVALID_HANDLE;  }

        bool open_readonly(const std::basic_string<T> &filepath)  THROWS_ALREADY_ATTACHED;
        bool open_readwrite(const std::basic_string<T> &filepath) THROWS_ALREADY_ATTACHED;
    };

    template<typename T>
    inline file<T>::file() throw()
      : err_(0),
        handle_(BOOST_FS_INVALID_HANDLE)
    {
    }

    template<typename T>
    inline file<T>::file(const std::basic_string<T> &filepath, file_access access) throw()
      : err_(0),
        handle_(BOOST_FS_INVALID_HANDLE)
    {
        if (access == readonly)
            this->open_readonly(filepath);
        else if (access == readwrite)
            this->open_readwrite(filepath);
    }

    template<typename T>
    inline file<T>::~file() throw()
    {
        this->close();
    }

    


    // memory mapped file management class
    template <typename T, typename F=file<char> >
    class memory_mapped_file : noncopyable
    {
      private:
        T             *ptr_;
        err_t          err_;
        detail_struct  detail_;

      public:
        memory_mapped_file();
        memory_mapped_file(F &file, file_access access);
        memory_mapped_file(file_handle_t &handle, file_access access);
        ~memory_mapped_file();

        bool map_readonly(file_handle_t handle);
        bool map_readwrite(file_handle_t handle);

        // this is exposed publicly for completeness, but is
        // unlikely to be used by the library user
        bool map(file_handle_t       &handle,
                 protection_t        &prot,
                 flags_or_security_t &fos,
                 length_t            &len,
                 offset_t            &off);

        // release the mapping
        bool     release(void);

        // is the object mapped to a file?
        bool     is_mapped(void) const { return (ptr_ != 0); }
        err_t    error(void)     const { return err_;        }

        // data accessibility
        T       *get(void)             { return ptr_;        }
        const T *get(void)       const { return ptr_;        }
    };


    // cross platform default ctor
    template <typename T, typename F>
    inline memory_mapped_file<T, F>::memory_mapped_file()
    : err_(0),
        ptr_(0)
    {
        memset(&detail_, 0, sizeof(detail_));
    }

    template <typename T, typename F>
    inline memory_mapped_file<T, F>::memory_mapped_file(file_handle_t &handle,
                                                 file_access    access)
      : err_(0),
        ptr_(0)
    {
        memset(&detail_, 0, sizeof(detail_));
        if (access == readonly)
            this->map_readonly(handle);
        else if (access == readwrite)
            this->map_readwrite(handle);
    }

    template <typename T, typename F>
    inline memory_mapped_file<T, F>::memory_mapped_file(F &file, file_access access)
      : err_(0),
        ptr_(0)
    {
        memset(&detail_, 0, sizeof(detail_));
        if (access == readonly)
            this->map_readonly(file.handle());
        else if (access == readwrite)
            this->map_readwrite(file.handle());
    }

    template <typename T, typename F>
    inline memory_mapped_file<T, F>::~memory_mapped_file()
    {
        this->release();
    }

}   // namespace boost


/////////////////////////////////////////////////////////////////////
//
// platform specific implementations are in separate headers
//
#if defined(BOOST_MMAP_WINDOWS)
#   include "memmap/mmf_win32.hpp"   // platform specific code for Win32
#   include "memmap/file_win32.hpp"  // platform specific code for Win32
#elif defined(BOOST_MMAP_POSIX)
#   include "memmap/mmf_posix.hpp"   // platform specific code for POSIX
#   include "memmap/file_posix.hpp"  // platform specific code for POSIX
#endif
