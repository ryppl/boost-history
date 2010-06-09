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
 * \file boost/process/operations.hpp
 *
 * Provides miscellaneous free functions.
 */

#ifndef BOOST_PROCESS_OPERATIONS_HPP
#define BOOST_PROCESS_OPERATIONS_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_POSIX_API)
#   include <boost/process/detail/posix_helpers.hpp>
#   include <stdlib.h>
#   include <unistd.h>
#   if defined(__CYGWIN__)
#       include <boost/scoped_array.hpp>
#       include <sys/cygwin.h>
#   endif
#elif defined(BOOST_WINDOWS_API)
#   include <boost/process/detail/win32_helpers.hpp>
#   include <boost/algorithm/string/predicate.hpp>
#   include <windows.h>
#else
#   error "Unsupported platform."
#endif

#include <boost/process/child.hpp>
#include <boost/process/context.hpp>
#include <boost/process/stream_behavior.hpp>
#include <boost/process/status.hpp>
#include <boost/process/detail/file_handle.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/system/system_error.hpp>
#include <boost/throw_exception.hpp>
#include <boost/scoped_array.hpp>
#include <boost/assert.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstddef>

namespace boost {
namespace process {

/**
 * Locates the executable program \a file in all the directory components
 * specified in \a path. If \a path is empty, the value of the PATH
 * environment variable is used.
 *
 * The path variable is interpreted following the same conventions used
 * to parse the PATH environment variable in the underlying platform.
 *
 * \throw boost::filesystem::filesystem_error If the file cannot be found
 *        in the path.
 */
inline std::string find_executable_in_path(const std::string &file, std::string path = "")
{
#if defined(BOOST_POSIX_API)
    BOOST_ASSERT(file.find('/') == std::string::npos);
#elif defined(BOOST_WINDOWS_API)
    BOOST_ASSERT(file.find_first_of("\\/") == std::string::npos);
#endif

    std::string result;

#if defined(BOOST_POSIX_API)
    if (path.empty())
    {
        const char *envpath = ::getenv("PATH");
        if (!envpath)
            boost::throw_exception(boost::filesystem::filesystem_error("boost::process::find_executable_in_path: retrieving PATH failed", file, boost::system::errc::make_error_code(boost::system::errc::no_such_file_or_directory)));

        path = envpath;
    }
    BOOST_ASSERT(!path.empty());

#if defined(__CYGWIN__)
    if (!::cygwin_posix_path_list_p(path.c_str()))
    {
        int size = ::cygwin_win32_to_posix_path_list_buf_size(path.c_str());
        boost::scoped_array<char> cygpath(new char[size]);
        ::cygwin_win32_to_posix_path_list(path.c_str(), cygpath.get());
        path = cygpath.get();
    }
#endif

    std::string::size_type pos1 = 0, pos2;
    do
    {
        pos2 = path.find(':', pos1);
        std::string dir = (pos2 != std::string::npos) ? path.substr(pos1, pos2 - pos1) : path.substr(pos1); 
        std::string f = dir + (boost::algorithm::ends_with(dir, "/") ? "" : "/") + file;
        if (!::access(f.c_str(), X_OK))
            result = f;
        pos1 = pos2 + 1;
    } while (pos2 != std::string::npos && result.empty());
#elif defined(BOOST_WINDOWS_API)
    const char *exts[] = { "", ".exe", ".com", ".bat", NULL };
    const char **ext = exts;
    while (*ext)
    {
        char buf[MAX_PATH];
        char *dummy;
        DWORD size = ::SearchPathA(path.empty() ? NULL : path.c_str(), file.c_str(), *ext, MAX_PATH, buf, &dummy);
        BOOST_ASSERT(size < MAX_PATH);
        if (size > 0)
        {
            result = buf;
            break;
        }
        ++ext;
    }
#endif

    if (result.empty())
        boost::throw_exception(boost::filesystem::filesystem_error(BOOST_PROCESS_SOURCE_LOCATION "file not found", file, boost::system::errc::make_error_code(boost::system::errc::no_such_file_or_directory)));

    return result;
}

/**
 * Extracts the program name from a given executable.
 *
 * \return The program name. On Windows the program name
 *         is returned without a file extension.
 */
inline std::string executable_to_progname(const std::string &exe)
{
    std::string::size_type begin = 0;
    std::string::size_type end = std::string::npos;

#if defined(BOOST_POSIX_API)
    std::string::size_type slash = exe.rfind('/');
#elif defined(BOOST_WINDOWS_API)
    std::string::size_type slash = exe.find_last_of("/\\");
#endif
    if (slash != std::string::npos)
        begin = slash + 1;

#if defined(BOOST_WINDOWS_API)
    if (exe.size() > 4 && (boost::algorithm::iends_with(exe, ".exe") || boost::algorithm::iends_with(exe, ".com") || boost::algorithm::iends_with(exe, ".bat"))) 
        end = exe.size() - 4;
#endif

    return exe.substr(begin, end - begin);
}

/**
 * Starts a new child process.
 *
 * Launches a new process based on the binary image specified by the
 * executable, the set of arguments to be passed to it and several
 * parameters that describe the execution context.
 *
 * \remark Blocking remarks: This function may block if the device
 * holding the executable blocks when loading the image. This might
 * happen if, e.g., the binary is being loaded from a network share.
 *
 * \return A handle to the new child process.
 */
template <class Arguments>
inline child create_child(const std::string &executable, Arguments args, context ctx = context())
{
    std::string p_name = ctx.process_name.empty() ? executable : ctx.process_name;
    args.insert(args.begin(), p_name);

#if defined(BOOST_POSIX_API)
    child::id_type pid = ::fork();

    if (pid == -1)
        BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("fork(2) failed"); 

    if (pid == 0) 
    { 
#if defined(F_MAXFD) 
        int maxdescs = ::fcntl(-1, F_MAXFD, 0); 
        if (maxdescs == -1) 
            maxdescs = ::sysconf(_SC_OPEN_MAX); 
#else 
        int maxdescs = ::sysconf(_SC_OPEN_MAX); 
#endif 
        if (maxdescs == -1) 
            maxdescs = 1024; 
        try 
        { 
            boost::scoped_array<bool> closeflags(new bool[maxdescs]); 
            for (int i = 0; i < maxdescs; ++i) 
                closeflags[i] = true; 

            // setup_input(infoin, closeflags.get(), maxdescs); 
            // setup_output(infoout, closeflags.get(), maxdescs); 

            int stdin_fd = ctx.stdin_behavior->get_child_end();
            if (stdin_fd != -1 && stdin_fd < maxdescs)
                closeflags[stdin_fd] = false;

            int stdout_fd = ctx.stdout_behavior->get_child_end();
            if (stdout_fd != -1 && stdout_fd < maxdescs)
                closeflags[stdout_fd] = false;

            int stderr_fd = ctx.stderr_behavior->get_child_end();
            if (stderr_fd != -1 && stderr_fd < maxdescs)
                closeflags[stderr_fd] = false;

            for (int i = 0; i < maxdescs; ++i) 
            { 
                if (closeflags[i]) 
                    ::close(i); 
            } 

            // setup(); 
        } 
        catch (const boost::system::system_error &e) 
        { 
            ::write(STDERR_FILENO, e.what(), std::strlen(e.what())); 
            ::write(STDERR_FILENO, "\n", 1); 
            std::exit(EXIT_FAILURE); 
        } 

        std::pair<std::size_t, char**> argcv = detail::collection_to_posix_argv(args);
        char **envp = detail::environment_to_envp(ctx.environment);
        ::execve(executable.c_str(), argcv.second, envp);

        boost::system::system_error e(boost::system::error_code(errno, boost::system::get_system_category()), BOOST_PROCESS_SOURCE_LOCATION "execve(2) failed"); 

        for (std::size_t i = 0; i < argcv.first; ++i) 
            delete[] argcv.second[i]; 
        delete[] argcv.second; 

        for (std::size_t i = 0; i < env.size(); ++i) 
            delete[] envp[i]; 
        delete[] envp; 

        ::write(STDERR_FILENO, e.what(), std::strlen(e.what())); 
        ::write(STDERR_FILENO, "\n", 1); 
        // TODO: Use return values which is less likely used by the
        // program which should have been started.
        std::exit(EXIT_FAILURE); 
    } 

    BOOST_ASSERT(pid > 0);

    if (ctx.stdin_behavior->get_child_end() != -1)
        ::close(ctx.stdin_behavior->get_child_end());
    if (ctx.stdout_behavior->get_child_end() != -1)
        ::close(ctx.stdout_behavior->get_child_end());
    if (ctx.stderr_behavior->get_child_end() != -1)
        ::close(ctx.stderr_behavior->get_child_end());

    return child(pid, 
        detail::file_handle(ctx.stdin_behavior->get_parent_end()), 
        detail::file_handle(ctx.stdout_behavior->get_parent_end()), 
        detail::file_handle(ctx.stderr_behavior->get_parent_end())); 
#elif defined(BOOST_WINDOWS_API)
    STARTUPINFOA startup_info;
    ::ZeroMemory(&startup_info, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);
    startup_info.dwFlags |= STARTF_USESTDHANDLES;
    startup_info.hStdInput = ctx.stdin_behavior->get_child_end();
    startup_info.hStdOutput = ctx.stdout_behavior->get_child_end();
    startup_info.hStdError = ctx.stderr_behavior->get_child_end();

    PROCESS_INFORMATION pi;
    ::ZeroMemory(&pi, sizeof(pi));

    boost::shared_array<char> cmdline = detail::collection_to_win32_cmdline(args);

    boost::scoped_array<char> exe(new char[executable.size() + 1]);
    ::strcpy_s(exe.get(), executable.size() + 1, executable.c_str());

    boost::scoped_array<char> workdir(new char[ctx.work_dir.size() + 1]);
    ::strcpy_s(workdir.get(), ctx.work_dir.size() + 1, ctx.work_dir.c_str());

    boost::shared_array<char> envstrs = detail::environment_to_win32_strings(ctx.environment);

    if (::CreateProcessA(exe.get(), cmdline.get(), NULL, NULL, TRUE, 0, envstrs.get(), workdir.get(), &startup_info, &pi) == 0)
        BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateProcess() failed"); 

    if (!::CloseHandle(pi.hThread))
        BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CloseHandle() failed"); 

    return child(pi.dwProcessId, 
        detail::file_handle(ctx.stdin_behavior->get_parent_end()), 
        detail::file_handle(ctx.stdout_behavior->get_parent_end()), 
        detail::file_handle(ctx.stderr_behavior->get_parent_end()), 
        detail::file_handle(pi.hProcess)); 
#endif 
}

inline child create_child(const std::string &executable, context ctx = context())
{
    return create_child(executable, std::vector<std::string>(), ctx); 
}

}
}

#endif
