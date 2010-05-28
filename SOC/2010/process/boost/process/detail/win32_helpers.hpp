// 
// Boost.Process 
// ~~~~~~~~~~~~~ 
// 
// Copyright (c) 2006, 2007 Julio M. Merino Vidal 
// Copyright (c) 2008, 2009 Boris Schaeling 
// Copyright (c) 2010 Boris Schaeling, Felipe Tanus
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying 
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) 
// 

/** 
 * \file boost/process/detail/helper_functions.hpp 
 * 
 * Includes the declaration of helper functions for the operations in a win32 system.
 * It's for internal purposes.
 *
 */ 

#include <boost/process/detail/file_handle.hpp> 
#include <boost/process/detail/stream_detail.hpp> 
#include <boost/process/detail/pipe.hpp> 
#include <boost/process/child.hpp> 
#include <boost/scoped_ptr.hpp> 
#include <boost/shared_array.hpp> 
#include <boost/scoped_array.hpp> 
#include <boost/assert.hpp> 
#include <boost/system/system_error.hpp> 
#include <boost/throw_exception.hpp> 
#include <vector> 
#include <map> 
#include <string> 
#include <cstddef> 
#include <string.h> 
#include <windows.h> 



#ifndef BOOST_PROCESS_WIN32_HELPERS_HPP 
#define BOOST_PROCESS_WIN32_HELPERS_HPP
namespace boost{
namespace process{
namespace detail{

/** 
 * Converts an environment to a string used by CreateProcess(). 
 * 
 * Converts the environment's contents to the format used by the 
 * CreateProcess() system call. The returned char* string is 
 * allocated in dynamic memory; the caller must free it when not 
 * used any more. This is enforced by the use of a shared pointer. 
 * 
 * \return A dynamically allocated char* string that represents 
 *         the environment's content. This string is of the form 
 *         var1=value1\\0var2=value2\\0\\0. 
 */ 
inline boost::shared_array<char> environment_to_win32_strings(environment_t &env){ 
    boost::shared_array<char> envp; 

    if (env.empty()) 
    { 
        envp.reset(new char[2]); 
        ::ZeroMemory(envp.get(), 2); 
    } 
    else 
    { 
        std::string s; 
        for (environment_t::const_iterator it = env.begin(); it != env.end(); ++it) 
        { 
            s += (*it).first + "=" + (*it).second; 
            s.push_back(0); 
        } 

        envp.reset(new char[s.size() + 1]); 
#if defined(__CYGWIN__) || defined(_SCL_SECURE_NO_DEPRECATE) 
        ::memcpy(envp.get(), s.c_str(), s.size() + 1); 
#else 
        ::memcpy_s(envp.get(), s.size() + 1, s.c_str(), s.size() + 1); 
#endif 
    } 

    return envp; 
} 



/** 
 * Converts the command line to a plain string. Converts the command line's 
 * list of arguments to the format expected by the \a lpCommandLine parameter 
 * in the CreateProcess() system call. 
 * 
 * This operation is only available on Windows systems. 
 * 
 * \return A dynamically allocated string holding the command line 
 *         to be passed to the executable. It is returned in a 
 *         shared_array object to ensure its release at some point. 
 */ 
template <class Arguments> 
inline boost::shared_array<char> collection_to_win32_cmdline(const Arguments &args){ 
    typedef std::vector<std::string> arguments_t; 
    arguments_t args2; 
    typename Arguments::size_type i = 0; 
    std::size_t size = 0; 
    for (typename Arguments::const_iterator it = args.begin(); it != args.end(); ++it) 
    { 
        std::string arg = *it; 

        std::string::size_type pos = 0; 
        while ( (pos = arg.find('"', pos)) != std::string::npos) 
        { 
            arg.replace(pos, 1, "\\\""); 
            pos += 2; 
        } 

        if (arg.find(' ') != std::string::npos) 
            arg = '\"' + arg + '\"'; 

        if (i++ != args.size() - 1) 
            arg += ' '; 

        args2.push_back(arg); 
        size += arg.size() + 1; 
    } 

    boost::shared_array<char> cmdline(new char[size]); 
    cmdline.get()[0] = '\0'; 
    for (arguments_t::size_type i = 0; i < args.size(); ++i) 
#if defined(__CYGWIN__) || defined(_SCL_SECURE_NO_DEPRECATE) 
        ::strncat(cmdline.get(), args2[i].c_str(), args2[i].size()); 
#else 
        ::strcat_s(cmdline.get(), size, args2[i].c_str()); 
#endif 

    return cmdline; 
} 

/*
 * This function configures the std stream of the new process.
 * It recieves stream_detail from that stream and a STARTUPINFOA
 * That will be stored the configuration. 
 */
inline file_handle configure_win32_stream(stream_detail &sd){

        file_handle return_handle;

        switch (sd.behavior){ 
                case closed:{ 
                        return_handle = file_handle();
                        break; 
                } 
                case inherit:{                       
                        return_handle = file_handle::win32_dup_std(sd.stream_handle, true);
                        break; 
                } 

                case dummy:{
                        HANDLE h;
                        if(sd.stream_type == stdin_type){
                                h = ::CreateFileA("NUL",
                                        GENERIC_READ, 0, NULL, OPEN_EXISTING, 
                                        FILE_ATTRIBUTE_NORMAL, NULL); 

                        }
                        else{
                                h = ::CreateFileA("NUL", 
                                        GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
                                        FILE_ATTRIBUTE_NORMAL, NULL); 							
                        }

                        if (h == INVALID_HANDLE_VALUE) 
                                boost::throw_exception(boost::system::system_error(boost::system::error_code(::GetLastError(), boost::system::get_system_category()), "boost::process::detail::win32_start: CreateFile failed")); 

                        return_handle = file_handle(h); 
                		
                        break; 
                } 

                case capture:{ 
                        if(sd.stream_type == stdin_type){
                                sd.object.pipe_.rend().win32_set_inheritable(true); 
                                return_handle = sd.object.pipe_.rend(); 
                        }
                        else{
                                sd.object.pipe_.wend().win32_set_inheritable(true); 	
                                return_handle = sd.object.pipe_.wend(); 
                        }

                        break; 
                } 

                default:{ 
                        BOOST_ASSERT(false); 
                        
                } 
        }
        
        return return_handle;
        

}

 
}
}
}


#endif
