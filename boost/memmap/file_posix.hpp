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

#if !defined(BOOST_MMAP_POSIX)
#error POSIX header file has been included without BOOST_MMAP_POSIX defined
#endif

#include <sys/fcntl.h>

namespace boost {

template<typename T>
bool file<T>::close(void) throw()
{
    if (handle_ == BOOST_FS_INVALID_HANDLE)
        return true;

    bool ret = (::close(handle_) != 0);
    if (ret)
    {
        err_    = 0;
        handle_ = BOOST_FS_INVALID_HANDLE;
        filepath_.clear();
    }
    else
        err_ = errno;

    return ret;
}



template<typename T>
bool file<T>::create(const std::basic_string<T> &filepath) throw(file_already_attached)
{
    if (handle_ != BOOST_FS_INVALID_HANDLE)
        throw file_already_attached();

    handle_ = ::open(filepath.c_str(), O_CREAT | O_RDWR | O_BINARY);
    err_ = errno;
    if (this->is_open())
        filepath_ = filepath;
    return this->is_open();
}


template<typename T>
bool file<T>::open_readonly(const std::basic_string<T> &filepath) throw(file_already_attached)
{
    if (handle_ != BOOST_FS_INVALID_HANDLE)
        throw file_already_attached();

    handle_ = ::open(filepath.c_str(), O_RDONLY);
    err_ = errno;
    if (this->is_open())
        filepath_ = filepath;
    return this->is_open();
}



template<typename T>
bool file<T>::open_readwrite(const std::basic_string<T> &filepath) throw(file_already_attached)
{
    if (handle_ != BOOST_FS_INVALID_HANDLE)
        throw file_already_attached();

    handle_ = ::open(filepath.c_str(), O_RDWR);
    err_ = errno;
    if (this->is_open())
        filepath_ = filepath;
    return this->is_open();
}


}   // namespace boost
