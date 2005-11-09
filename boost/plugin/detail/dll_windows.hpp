// Copyright Vladimir Prus 2004.
// Copyright Hartmut Kaiser 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DLL_WINDOWS_HPP_HK_2005_11_06
#define BOOST_DLL_WINDOWS_HPP_HK_2005_11_06

#include <string>
#include <stdexcept>
#include <iostream>

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <boost/plugin/config.hpp>

#include <windows.h> 

#if !defined(BOOST_WINDOWS)
#error "This file shouldn't be included directly, use the file boost/plugin/dll.hpp only."
#endif

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace plugin {

    namespace detail 
    {
        struct free_dll
        {
            free_dll(HMODULE h) : h(h) {}
            
            template<typename T>
            void operator()(T)
            {
                if (NULL != h)
                    FreeLibrary(h);
            }
            
            HMODULE h;
        };
    }
    
    class dll 
    {
    public:
        dll() {} // TODO: should remove this or make non-public
        
        dll(dll const& rhs) 
        :   dll_name(rhs.dll_name), dll_handle(NULL)
        {
            if (!dll_name.empty()) {
                dll_handle = LoadLibrary(dll_name.c_str());
                if (!dll_handle) {
                    BOOST_PLUGIN_OSSTREAM str;
                    str << "Boost.Plugin: Could not open shared library '" 
                        << dll_name << "'";
                    
                    throw std::logic_error(BOOST_PLUGIN_OSSTREAM_GETSTRING(str));
                }
            }
        }

        dll(std::string const& name) 
        :   dll_name(name), dll_handle(NULL)
        {
            dll_handle = LoadLibrary(dll_name.c_str());
            if (!dll_handle) {
                BOOST_PLUGIN_OSSTREAM str;
                str << "Boost.Plugin: Could not open shared library '" 
                    << dll_name << "'";
                
                throw std::logic_error(BOOST_PLUGIN_OSSTREAM_GETSTRING(str));
            }
        }
        
        ~dll() 
        { 
            if (NULL != dll_handle) 
                FreeLibrary(dll_handle); 
            dll_handle = NULL;
        }
        
        std::string get_name() const { return dll_name; }
        
        template<typename SymbolType>
        boost::shared_ptr<typename boost::remove_pointer<SymbolType>::type> 
        get (std::string const& symbol_name) const
        {
            BOOST_STATIC_ASSERT(boost::is_pointer<SymbolType>::value);
            typedef typename remove_pointer<SymbolType>::type PointedType;
            
        // Open the library. Yes, we do it on every access to 
        // a symbol, the LoadLibrary function increases the refcnt of the dll
        // so in the end the dll class holds one refcnt and so does every 
        // symbol.
        HMODULE handle = LoadLibrary (dll_name.c_str());

            BOOST_ASSERT(handle == dll_handle);
            if (!handle) {
                BOOST_PLUGIN_OSSTREAM str;
                str << "Boost.Plugin: Could not open shared library '" 
                    << dll_name << "'";
                
                throw std::logic_error(BOOST_PLUGIN_OSSTREAM_GETSTRING(str));
            }
                        
        // Cast the to right type.
        SymbolType address = 
            (SymbolType)GetProcAddress(dll_handle, symbol_name.c_str());

            if (NULL == address) {
                BOOST_PLUGIN_OSSTREAM str;
                str << "Boost.Plugin: Unable to locate the exported symbol name '" 
                    << symbol_name << "' in the shared library '" 
                    << dll_name << "'";
                    
                throw std::logic_error(BOOST_PLUGIN_OSSTREAM_GETSTRING(str));
            }
            return boost::shared_ptr<PointedType>(address, detail::free_dll(handle));
        }        

    private:
        std::string dll_name;
        HMODULE dll_handle;
    };

///////////////////////////////////////////////////////////////////////////////
}}

#endif
