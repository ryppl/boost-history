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

#if !defined(BOOST_MMAP_POSIX)
#error Posix header file has been included without BOOST_MMAP_POSIX defined
#endif

#include <sys/mman.h>

namespace boost {

template <typename T, typename F>
bool memory_mapped_file<T, F>::release(void)
{
    return ::munmap(reinterpret_cast<char *>(ptr_), detail_.len);
}


template <typename T, typename F>
bool memory_mapped_file<T, F>::map_readonly(file_handle_t handle)
{
    detail_.len = get_file_size(handle);

    flags_or_security_t fos  = MAP_FILE;
    protection_t        prot = PROT_READ;
    length_t            len  = detail_.len;
    offset_t            off  = 0;
    return this->map(handle, prot, fos, len, off);
}


template <typename T, typename F>
bool memory_mapped_file<T, F>::map_readwrite(file_handle_t handle)
{
    detail_.len = get_file_size(handle);

    flags_or_security_t fos  = MAP_PRIVATE | MAP_FILE;
    protection_t        prot = PROT_WRITE;
    length_t            len  = detail_.len;
    offset_t            off  = 0;
    return this->map(handle, prot, fos, len, off);
}



template <typename T, typename F>
bool memory_mapped_file<T, F>::map(file_handle_t    &handle,
                                protection_t        &prot,
                                flags_or_security_t &fos,
                                length_t            &len,
                                offset_t            &off)
{
    if (ptr_ != 0)
        return false;

    void *ptr = ::mmap(0, len, prot, fos, handle, off);
    if (ptr == MAP_FAILED)  // defined in sys/mman.h
    {
        err_ = errno;
        return false;
    }

    ptr_ = reinterpret_cast<T *>(ptr);
    return (ptr_ != 0);
}

}   // namespace boost
