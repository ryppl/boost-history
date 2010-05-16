// 
// Boost.Process 
// ~~~~~~~~~~~~~ 
// 
// Copyright (c) 2006, 2007 Julio M. Merino Vidal 
// Copyright (c) 2008, 2009 Boris Schaeling 
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
		#include <boost/process/detail/posix_helpers.hpp> 
		#include <stdlib.h> 
		#include <unistd.h> 
		#if defined(__CYGWIN__) 
				#include <boost/scoped_array.hpp> 
				#include <sys/cygwin.h> 
				#endif 
#elif defined(BOOST_WINDOWS_API) 
		#include <boost/process/detail/win32_helpers.hpp> 
		#include <boost/algorithm/string/predicate.hpp> 
		#include <windows.h> 		 
#else 
#  error "Unsupported platform." 
#endif 

/*
#include <boost/process/child.hpp> 
#include <boost/process/stream_behavior.hpp> 
#include <boost/process/status.hpp> 
#include <boost/process/detail/file_handle.hpp> 
#include <boost/process/detail/pipe.hpp> 
#include <boost/process/detail/stream_info.hpp> 
*/
#include <boost/process/child.hpp> 
#include <boost/process/context.hpp> 
#include <boost/process/stream_behavior.hpp> 
#include <boost/process/status.hpp> 
#include <boost/process/detail/file_handle.hpp> 
#include <boost/process/detail/stream_detail.hpp>


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
inline std::string find_executable_in_path(const std::string &file, std::string path = ""){
        #if defined(BOOST_POSIX_API) 
                BOOST_ASSERT(file.find('/') == std::string::npos); 
        #elif defined(BOOST_WINDOWS_API) 
                BOOST_ASSERT(file.find_first_of("\\/") == std::string::npos); 
        #endif 

        std::string result; 

        #if defined(BOOST_POSIX_API) 
                if (path.empty()){ 
                        const char *envpath = ::getenv("PATH"); 
                if (!envpath) 
                    boost::throw_exception(boost::filesystem::filesystem_error("boost::process::find_executable_in_path: retrieving PATH failed", file, boost::system::errc::make_error_code(boost::system::errc::no_such_file_or_directory))); 

                path = envpath; 

                } 
                BOOST_ASSERT(!path.empty()); 

                #if defined(__CYGWIN__) 
                        if (!::cygwin_posix_path_list_p(path.c_str())){ 
                                int size = ::cygwin_win32_to_posix_path_list_buf_size(path.c_str()); 
                                boost::scoped_array<char> cygpath(new char[size]); 
                                ::cygwin_win32_to_posix_path_list(path.c_str(), cygpath.get()); 
                                path = cygpath.get();
                        }
                #endif 

                std::string::size_type pos1 = 0, pos2; 
                do{ 
                        pos2 = path.find(':', pos1); 
                        std::string dir;
                        if(pos2 != std::string::npos)
                                dir = path.substr(pos1, pos2 - pos1);
                        else
                                path.substr(pos1);

                        std::string f = dir + 
                                (boost::algorithm::ends_with(dir, "/") ? "" : "/") + 
                                file; 

                        if (!::access(f.c_str(), X_OK)) 
                                result = f; 
                        pos1 = pos2 + 1; 
                } while (pos2 != std::string::npos && result.empty());

         #elif defined(BOOST_WINDOWS_API)
                
                const char *exts[] = { "", ".exe", ".com", ".bat", NULL }; 
                const char **ext = exts; 

                while (*ext){
                        char buf[MAX_PATH];
                        char *dummy; 

                        DWORD size = ::SearchPathA(
                                 path.empty() ? NULL : 
                                 path.c_str(), file.c_str(), *ext, MAX_PATH, buf, &dummy);

                        BOOST_ASSERT(size < MAX_PATH); 

                        if (size > 0){ 
                                result = buf; 
                                break; 
                        } 
                        ++ext; 
                } 

        #endif 
        if (result.empty()) 
                boost::throw_exception(boost::filesystem::filesystem_error("boost::process::find_executable_in_path: file not found", file, boost::system::errc::make_error_code(boost::system::errc::no_such_file_or_directory))); 

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
                if (exe.size() > 4 &&
                                (boost::algorithm::iends_with(exe, ".exe")
                                 || boost::algorithm::iends_with(exe, ".com")
                                 || boost::algorithm::iends_with(exe, ".bat")
                                 )
                    ) 
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



template<class Arguments>
inline child create_child(const std::string &executable, Arguments args, context &ctx = DEFAULT_CONTEXT){ 

        detail::file_handle fhstdin, fhstdout, fhstderr;

        detail::stream_detail stdin_stream(detail::stdin_type), 
								stdout_stream(detail::stdout_type),
								stderr_stream(detail::stderr_type);

        stdin_stream.behavior = ctx.stdin_behavior;
        stdout_stream.behavior = ctx.stdout_behavior;
        stderr_stream.behavior = ctx.stderr_behavior;

		std::string p_name = ctx.process_name.empty() ? executable : ctx.process_name;
        args.insert(args.begin(),p_name);


#if defined(BOOST_POSIX_API)

        child::id_type pid = ::fork(); 
        if (pid == -1) 
                boost::throw_exception(boost::system::system_error(boost::system::error_code(errno, boost::system::get_system_category()), "boost::process::detail::posix_start: fork(2) failed")); 

        else if (pid == 0){ 

                detail::configure_posix_stream(stdin_stream);
                detail::configure_posix_stream(stdout_stream);
                detail::configure_posix_stream(stderr_stream);

				std::pair<std::size_t, char**> argcv = detail::collection_to_posix_argv(args); 
        		char **envp = detail::environment_to_envp(ctx.environment);

                ::execve(executable.c_str(), argcv.second, envp); 

                BOOST_ASSERT(false);

        }

        BOOST_ASSERT(pid > 0); 


		//TODO: turn this in a helper
        if(ctx.stdin_behavior == capture){
                stdin_stream.object.pipe_->rend().close();
                fhstdin = stdin_stream.object.pipe_->rend().release();
                BOOST_ASSERT(fhstdin.valid()); 
        }

        if(ctx.stdout_behavior == capture){
                stdout_stream.object.pipe_->wend().close();
                fhstdout = stdout_stream.object.pipe_->wend().release();
                BOOST_ASSERT(fhstdout.valid()); 
        }

        if(ctx.stderr_behavior == capture){
                stderr_stream.object.pipe_->wend().close();
                fhstderr = stderr_stream.object.pipe_->wend().release();
                BOOST_ASSERT(fhstderr.valid()); 
        }


        return child(pid, fhstdin, fhstdout, fhstderr); 


#elif defined(BOOST_WINDOWS_API) 


		//Set up the pipes when needed for the current process.
		if (stdin_stream.behavior == capture)
				fhstdin = stdin_stream.object.pipe_->wend();
		if (stdin_stream.behavior == capture)
				fhstdin = stdin_stream.object.pipe_->wend();
		if (stdin_stream.behavior == capture)
				fhstdin = stdin_stream.object.pipe_->wend();

		//define startup info from the new child
        STARTUPINFOA start_up_info;
        ::ZeroMemory(&start_up_info, sizeof(start_up_info)); 
        start_up_info.cb = sizeof(start_up_info); 

	   	start_up_info.dwFlags |= STARTF_USESTDHANDLES; 
		
		configure_win32_stream(stdin_stream, start_up_info); 
    	configure_win32_stream(stdout_stream, start_up_info); 
		configure_win32_stream(stderr_stream, start_up_info);  

		//define basic info to start the process
    	PROCESS_INFORMATION pi; 
    	::ZeroMemory(&pi, sizeof(pi)); 

    	boost::shared_array<char> cmdline = detail::collection_to_win32_cmdline(args); 

	    boost::scoped_array<char> exe(new char[executable.size() + 1]); 
		::strcpy_s(exe.get(), executable.size() + 1, executable.c_str()); 

    	boost::scoped_array<char> workdir(new char[ctx.work_dir.size() + 1]); 
		::strcpy_s(workdir.get(), ctx.work_dir.size() + 1, ctx.work_dir.c_str()); 
		

	    boost::shared_array<char> envstrs = detail::environment_to_win32_strings(ctx.environment); 

   		if ( ! ::CreateProcessA(exe.get(), cmdline.get(), 
			NULL, NULL, TRUE, 0, envstrs.get(), workdir.get(), 
			&start_up_info, &pi)) 
        		boost::throw_exception(boost::system::system_error(boost::system::error_code(::GetLastError(), boost::system::get_system_category()), "boost::process::detail::win32_start: CreateProcess failed")); 

        if (! ::CloseHandle(pi.hThread)) 
            boost::throw_exception(boost::system::system_error(boost::system::error_code(::GetLastError(), boost::system::get_system_category()), "boost::process::launch: CloseHandle failed")); 

        return child(pi.dwProcessId, fhstdin, fhstdout, fhstderr, detail::file_handle(pi.hProcess)); 

#endif 
} 


inline child create_child(const std::string &executable, context &ctx = DEFAULT_CONTEXT){ 
        std::vector<std::string> args ;
        return create_child(executable,args,ctx);
}
}
}

#endif 
