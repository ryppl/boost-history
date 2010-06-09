//
// Boost.Process
// ~~~~~~~~~~~~~
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008, 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/**
 * \file boost/process/detail/file_handle.hpp
 *
 * Includes the declaration of the file_handle class. This file is for
 * internal usage only and must not be included by the library user.
 */

#ifndef BOOST_PROCESS_DETAIL_FILE_HANDLE_HPP
#define BOOST_PROCESS_DETAIL_FILE_HANDLE_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_POSIX_API)
#   include <cerrno>
#   include <unistd.h>
#elif defined(BOOST_WINDOWS_API)
#   include <windows.h>
#else
#   error "Unsupported platform." 
#endif

#include <boost/assert.hpp>

namespace boost {
namespace process {
namespace detail {

/**
 * Simple RAII model for system file handles.
 *
 * The \a file_handle class is a simple RAII model for native system file
 * handles. This class wraps one of such handles grabbing its ownership,
 * and automaticaly closes it upon destruction. It is basically used
 * inside the library to avoid leaking open file handles, shall an
 * unexpected execution trace occur.
 *
 * A \a file_handle object can be copied but doing so invalidates the
 * source object. There can only be a single valid \a file_handle object
 * for a given system file handle. This is similar to std::auto_ptr's
 * semantics.
 *
 * This class also provides some convenience methods to issue special file
 * operations under their respective platforms.
 */
class file_handle
{
public:
#if defined(BOOST_PROCESS_DOXYGEN)
    /**
     * Opaque name for the native handle type.
     *
     * Each operating system identifies file handles using a specific type.
     * The \a handle_type type is used to transparently refer to file
     * handles regarless of the operating system in which this class is
     * used.
     *
     * If this class is used on a POSIX system, \a NativeSystemHandle is
     * an integer type while it is a \a HANDLE on a Windows system.
     */
    typedef NativeSystemHandle handle_type;
#elif defined(BOOST_POSIX_API)
    typedef int handle_type;
#elif defined(BOOST_WINDOWS_API)
    typedef HANDLE handle_type;
#endif

    /**
     * Constructs an invalid file handle.
     *
     * This constructor creates a new \a file_handle object that represents
     * an invalid file handle. An invalid file handle can be copied but
     * cannot be manipulated in any way (except checking for its validity).
     *
     * \see valid()
     */
    file_handle()
        : handle_(invalid_value())
    {
    }

    /**
     * Constructs a new file handle from a native file handle.
     *
     * This constructor creates a new \a file_handle object that takes
     * ownership of the given \a h native file handle. The user must not
     * close \a h on his own during the lifetime of the new object.
     * Ownership can be reclaimed using release().
     *
     * \pre The native file handle must be valid; a close operation must
     *      succeed on it.
     * \see release()
     */
    file_handle(handle_type h)
        : handle_(h)
    {
        // Boris: I wonder why this assert existed before (also in
        //        previous Boost.Process versions).
        // BOOST_ASSERT(handle_ != invalid_value());
    }

    /**
     * Copy constructor; invalidates the source handle.
     *
     * This copy constructor creates a new file handle from a given one.
     * Ownership of the native file handle is transferred to the new
     * object, effectively invalidating the source file handle. This
     * avoids having two live \a file_handle objects referring to the
     * same native file handle. The source file handle needs not be
     * valid in the name of simplicity.
     *
     * \post The source file handle is invalid.
     * \post The new file handle owns the source's native file handle.
     */
    file_handle(const file_handle &fh)
        : handle_(fh.handle_)
    {
        fh.handle_ = invalid_value();
    }

    /**
     * Releases resources if the handle is valid.
     *
     * If the file handle is valid, the destructor closes it.
     *
     * \see valid()
     */
    ~file_handle()
    {
        if (valid())
            close();
    }

    /**
     * Assignment operator; invalidates the source handle.
     *
     * This assignment operator transfers ownership of the RHS file
     * handle to the LHS one, effectively invalidating the source file
     * handle. This avoids having two live \a file_handle objects
     * referring to the same native file handle. The source file
     * handle needs not be valid in the name of simplicity.
     *
     * \post The RHS file handle is invalid.
     * \post The LHS file handle owns RHS' native file handle.
     * \return A reference to the LHS file handle.
     */
    file_handle &operator=(const file_handle &fh)
    {
        handle_ = fh.handle_;
        fh.handle_ = invalid_value();
        return *this;
    }

    /**
     * Checks whether the file handle is valid or not.
     *
     * Returns a boolean indicating whether the file handle is valid or
     * not. If the file handle is invalid, no other methods can be
     * executed other than the destructor.
     *
     * \return true if the file handle is valid; false otherwise.
     */
    bool valid()
    {
        return handle_ != invalid_value();
    }

    /**
     * Reclaims ownership of the native file handle.
     *
     * Explicitly reclaims ownership of the native file handle contained
     * in the \a file_handle object, returning the native file handle.
     * The caller is responsible of closing it later on.
     *
     * \pre The file handle is valid.
     * \post The file handle is invalid.
     * \return The native file handle.
     */
    handle_type release()
    {
        BOOST_ASSERT(valid());

        handle_type h = handle_;
        handle_ = invalid_value();
        return h;
    }

    /**
     * Gets the native file handle.
     *
     * Returns the native file handle for the \a file_handle object.
     * The caller can issue any operation on it except closing it.
     * If closing is required, release() shall be used.
     *
     * \pre The file handle is valid.
     * \post The file handle is valid.
     * \return The native file handle.
     */
    handle_type get()
    {
        BOOST_ASSERT(valid());

        return handle_;
    }

#if defined(BOOST_POSIX_API) || defined(BOOST_PROCESS_DOXYGEN)
    /**
     * Closes the file handle.
     *
     * Explicitly closes the file handle, which must be valid. Upon
     * exit, the handle is not valid any more.
     *
     * \pre The file handle is valid.
     * \post The file handle is invalid.
     * \post The native file handle is closed.
     */
    void close()
    {
        BOOST_ASSERT(valid());
        ::close(handle_);
        handle_ = invalid_value();
    }
#endif

#if defined(BOOST_WINDOWS_API) || defined(BOOST_PROCESS_DOXYGEN)
    /**
     * Closes the file handle.
     *
     * Explicitly closes the file handle, which must be valid. Upon
     * exit, the handle is not valid any more.
     *
     * \pre The file handle is valid.
     * \post The file handle is invalid.
     * \post The native file handle is closed.
     */
    void close()
    {
        BOOST_ASSERT(valid());
        ::CloseHandle(handle_);
        handle_ = invalid_value();
    }
#endif

private:
    /**
     * Internal handle value.
     *
     * This variable holds the native handle value for the file handle
     * hold by this object. It is interesting to note that this needs
     * to be mutable because the copy constructor and the assignment
     * operator invalidate the source object.
     */
    mutable handle_type handle_;

    /**
     * Constant function representing an invalid handle value.
     *
     * Returns the platform-specific handle value that represents an
     * invalid handle. This is a constant function rather than a regular
     * constant because, in the latter case, we cannot define it under
     * Windows due to the value being of a complex type.
     */
    static const handle_type invalid_value()
    {
#if defined(BOOST_POSIX_API)
        return -1;
#elif defined(BOOST_WINDOWS_API)
        return INVALID_HANDLE_VALUE;
#endif
    }
};

}
}
}

#endif
