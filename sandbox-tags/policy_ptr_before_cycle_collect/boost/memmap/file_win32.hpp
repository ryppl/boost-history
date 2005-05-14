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

#if !defined(BOOST_MMAP_WINDOWS)
#error Win32 header file has been included without BOOST_MMAP_WINDOWS defined
#endif

namespace boost {

template<typename T>
bool file<T>::close(void) throw()
{
    if (handle_ == BOOST_FS_INVALID_HANDLE)
        return true;

    bool ret = (::CloseHandle(handle_) != 0);
    if (ret)
    {
        handle_ = BOOST_FS_INVALID_HANDLE;
        filepath_.clear();

        // CloseHandle() doesn't reset the last error,
        // so set our errorcode to zero
        err_ = 0;
    }
    else
        err_ = ::GetLastError();

    return ret;
}



template<typename T>
bool file<T>::create(const std::basic_string<T> &filepath) THROWS_ALREADY_ATTACHED
{
    if (handle_ != BOOST_FS_INVALID_HANDLE)
        throw file_already_attached();

    handle_ = ::CreateFile(filepath.c_str(),
                           GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
                           0, NULL);
    err_ = ::GetLastError();
    if (this->is_open())
        filepath_ = filepath;
    return this->is_open();
}


template<typename T>
bool file<T>::open_readonly(const std::basic_string<T> &filepath) THROWS_ALREADY_ATTACHED
{
    if (handle_ != BOOST_FS_INVALID_HANDLE)
        throw file_already_attached();

    handle_ = ::CreateFile(filepath.c_str(),
                           GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                           0, NULL);
    err_ = ::GetLastError();
    if (this->is_open())
        filepath_ = filepath;
    return this->is_open();
}



template<typename T>
bool file<T>::open_readwrite(const std::basic_string<T> &filepath) THROWS_ALREADY_ATTACHED
{
    if (handle_ != BOOST_FS_INVALID_HANDLE)
        throw file_already_attached();

    handle_ = ::CreateFile(filepath.c_str(),
                           GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                           0, NULL);
    err_ = ::GetLastError();
    if (this->is_open())
        filepath_ = filepath;
    return this->is_open();
}


}   // namespace boost
