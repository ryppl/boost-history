//
// Boost.Process
// ~~~~~~~~~~~~~
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008 Ilya Sokolov, Boris Schaeling
// Copyright (c) 2009 Boris Schaeling
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
#   include <utility>
#   include <cstddef>
#   include <stdlib.h>
#   include <unistd.h>
#   if defined(__CYGWIN__)
#       include <boost/scoped_array.hpp>
#       include <sys/cygwin.h>
#   endif
#elif defined(BOOST_WINDOWS_API)
#   include <boost/process/detail/windows_helpers.hpp>
#   include <boost/scoped_array.hpp>
#   include <boost/shared_array.hpp>
#   include <windows.h>
#else
#   error "Unsupported platform."
#endif

#include <boost/process/child.hpp>
#include <boost/process/context.hpp>
#include <boost/process/handle.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/system/system_error.hpp>
#include <boost/throw_exception.hpp>
#include <boost/assert.hpp>
#include <string>
#include <vector>

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
inline std::string find_executable_in_path(const std::string &file,
    std::string path = "")
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
        const char *envpath = getenv("PATH");
        if (!envpath)
            boost::throw_exception(boost::filesystem::filesystem_error(
                BOOST_PROCESS_SOURCE_LOCATION "file not found", file,
                boost::system::errc::make_error_code(
                boost::system::errc::no_such_file_or_directory)));
        path = envpath;
    }
    BOOST_ASSERT(!path.empty());

#if defined(__CYGWIN__)
    if (!cygwin_posix_path_list_p(path.c_str()))
    {
        int size = cygwin_win32_to_posix_path_list_buf_size(path.c_str());
        boost::scoped_array<char> cygpath(new char[size]);
        cygwin_win32_to_posix_path_list(path.c_str(), cygpath.get());
        path = cygpath.get();
    }
#endif

    std::string::size_type pos1 = 0, pos2;
    do
    {
        pos2 = path.find(':', pos1);
        std::string dir = (pos2 != std::string::npos) ?
            path.substr(pos1, pos2 - pos1) : path.substr(pos1);
        std::string f = dir +
            (boost::algorithm::ends_with(dir, "/") ? "" : "/") + file;
        if (!access(f.c_str(), X_OK))
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
        DWORD size = SearchPathA(path.empty() ? NULL : path.c_str(),
            file.c_str(), *ext, MAX_PATH, buf, &dummy);
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
        boost::throw_exception(boost::filesystem::filesystem_error(
            BOOST_PROCESS_SOURCE_LOCATION "file not found", file,
            boost::system::errc::make_error_code(
            boost::system::errc::no_such_file_or_directory)));

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
    if (exe.size() > 4 && (boost::algorithm::iends_with(exe, ".exe") ||
        boost::algorithm::iends_with(exe, ".com") ||
        boost::algorithm::iends_with(exe, ".bat"))) 
        end = exe.size() - 4;
#endif

    return exe.substr(begin, end - begin);
}

/**
 * Starts a new child process.
 *
 * Launches a new process based on the binary image specified by the
 * executable, the set of arguments passed to it and the execution context.
 *
 * \remark Blocking remarks: This function may block if the device holding the
 *         executable blocks when loading the image. This might happen if, e.g.,
 *         the binary is being loaded from a network share.
 *
 * \return A handle to the new child process.
 */
template <typename Arguments, typename Context>
inline child create_child(const std::string &executable, Arguments args,
    Context ctx)
{
#if !defined(NDEBUG)
    BOOST_ASSERT(ctx.stdin_behavior.get());
    BOOST_ASSERT(ctx.stdout_behavior.get());
    BOOST_ASSERT(ctx.stderr_behavior.get());
#endif

    std::string p_name = ctx.process_name.empty() ?
        executable_to_progname(executable) : ctx.process_name;
    args.insert(args.begin(), p_name);

#if defined(BOOST_POSIX_API)
    // Between fork() and execve() only async-signal-safe functions
    // must be called if multithreaded applications should be supported.
    // That's why the following code is executed before fork() is called.
#if defined(F_MAXFD)
    int maxdescs = fcntl(-1, F_MAXFD, 0);
    if (maxdescs == -1)
        maxdescs = sysconf(_SC_OPEN_MAX);
#else
    int maxdescs = sysconf(_SC_OPEN_MAX);
#endif
    if (maxdescs == -1)
        maxdescs = 1024;
    std::vector<bool> closeflags(maxdescs, true);
    std::pair<std::size_t, char**> argv = detail::collection_to_argv(args);
    std::pair<std::size_t, char**> envp =
        detail::environment_to_envp(ctx.env);

    pid_t pid = fork();
    if (pid == -1)
        BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("fork(2) failed");
    else if (pid == 0)
    {
        if (chdir(ctx.work_dir.c_str()) == -1)
        {
            write(STDERR_FILENO, "chdir() failed\n", 15);
            _exit(127);
        }

        handle hstdin = ctx.stdin_behavior->get_child_end();
        if (hstdin.valid())
        {
            if (dup2(hstdin.native(), STDIN_FILENO) == -1)
            {
                write(STDERR_FILENO, "dup2() failed\n", 14);
                _exit(127);
            }
            closeflags[STDIN_FILENO] = false;
        }

        handle hstdout = ctx.stdout_behavior->get_child_end();
        if (hstdout.valid())
        {
            if (dup2(hstdout.native(), STDOUT_FILENO) == -1)
            {
                write(STDERR_FILENO, "dup2() failed\n", 14);
                _exit(127);
            }
            closeflags[STDOUT_FILENO] = false;
        }

        handle hstderr = ctx.stderr_behavior->get_child_end();
        if (hstderr.valid())
        {
            if (dup2(hstderr.native(), STDERR_FILENO) == -1)
            {
                write(STDERR_FILENO, "dup2() failed\n", 14);
                _exit(127);
            }
            closeflags[STDERR_FILENO] = false;
        }

        ctx.setup(closeflags);

        for (std::size_t i = 0; i < closeflags.size(); ++i)
        {
            if (closeflags[i])
                close(i);
        }

        execve(executable.c_str(), argv.second, envp.second);

        // Actually we should delete argv and envp data. As we must not
        // call any non-async-signal-safe functions though we simply exit.
        write(STDERR_FILENO, "execve() failed\n", 16);
        _exit(127);
    }
    else
    {
        BOOST_ASSERT(pid > 0);

        ctx.stdin_behavior->get_child_end().close();
        ctx.stdout_behavior->get_child_end().close();
        ctx.stderr_behavior->get_child_end().close();

        return child(pid,
            ctx.stdin_behavior->get_parent_end(),
            ctx.stdout_behavior->get_parent_end(),
            ctx.stderr_behavior->get_parent_end());
    }
#elif defined(BOOST_WINDOWS_API)
    STARTUPINFOA startup_info;
    ZeroMemory(&startup_info, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);
    startup_info.dwFlags |= STARTF_USESTDHANDLES;
    startup_info.hStdInput = ctx.stdin_behavior->get_child_end().native();
    startup_info.hStdOutput = ctx.stdout_behavior->get_child_end().native();
    startup_info.hStdError = ctx.stderr_behavior->get_child_end().native();

    ctx.setup(startup_info);

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    boost::shared_array<char> cmdline =
        detail::collection_to_windows_cmdline(args);

    boost::scoped_array<char> exe(new char[executable.size() + 1]);
    strcpy_s(exe.get(), executable.size() + 1, executable.c_str());

    boost::scoped_array<char> workdir(new char[ctx.work_dir.size() + 1]);
    strcpy_s(workdir.get(), ctx.work_dir.size() + 1, ctx.work_dir.c_str());

    boost::shared_array<char> envstrs =
        detail::environment_to_windows_strings(ctx.env);

    if (CreateProcessA(exe.get(), cmdline.get(), NULL, NULL, TRUE, 0,
        envstrs.get(), workdir.get(), &startup_info, &pi) == 0)
        BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateProcess() failed");

    ctx.stdin_behavior->get_child_end().close();
    ctx.stdout_behavior->get_child_end().close();
    ctx.stderr_behavior->get_child_end().close();

    handle hprocess(pi.hProcess);

    if (CloseHandle(pi.hThread) == 0)
        BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CloseHandle() failed");

    return child(hprocess,
        ctx.stdin_behavior->get_parent_end(),
        ctx.stdout_behavior->get_parent_end(),
        ctx.stderr_behavior->get_parent_end());
#endif
}

/**
 * \overload
 */
inline child create_child(const std::string &executable)
{
    return create_child(executable, std::vector<std::string>(), context());
}

/**
 * \overload
 */
template <typename Arguments>
inline child create_child(const std::string &executable, Arguments args)
{
    return create_child(executable, args, context());
}

/**
 * Starts a shell-based command.
 *
 * Executes the given command through the default system shell. The
 * command is subject to pattern expansion, redirection and pipelining.
 * The shell is launched as described by the parameters in the context.
 *
 * This function behaves similarly to the system(3) system call. In a
 * POSIX system, the command is fed to /bin/sh whereas under a Windows
 * system, it is fed to cmd.exe. It is difficult to write portable
 * commands, but this function comes in handy in multiple situations.
 *
 * \remark Blocking remarks: This function may block if the device holding the
 *         executable blocks when loading the image. This might happen if, e.g.,
 *         the binary is being loaded from a network share.
 *
 * \return A handle to the new child process.
 */
template <typename Context>
inline child shell(const std::string &command, Context ctx)
{
#if defined(BOOST_POSIX_API)
    std::string executable = "/bin/sh";
    std::vector<std::string> args;
    args.push_back("-c");
    args.push_back(command);
#elif defined(BOOST_WINDOWS_API)
    char sysdir[MAX_PATH];
    UINT size = GetSystemDirectoryA(sysdir, sizeof(sysdir));
    if (!size)
        BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("GetSystemDirectory() failed");
    std::string executable = std::string(sysdir) +
        (sysdir[size - 1] != '\\' ? "\\cmd.exe" : "cmd.exe");
    std::vector<std::string> args;
    args.push_back("/c");
    args.push_back(command);
#endif
    return create_child(executable, args, ctx);
}

/**
 * \overload
 */
inline child shell(const std::string &command)
{
    return shell(command, context());
}

}
}

#endif
